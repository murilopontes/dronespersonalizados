/*
 * CC1101_BOARD.cpp
 *
 *  Created on: 07/02/2014
 *      Author: mpontes
 */


#include <Energia.h>
#include <SPI.h>

#include "cc1101.h"
#include "cc1101_regs.h"



//////////////////////////////////////////////////////////////////////////////////
/* bit value */
#ifndef BV
#define BV(n)      (1 << (n))
#endif

/*
 *
 * RXFIFO_OVERFLOW Issue — Radio stays in RX state instead of entering RXFIFO_OVERFLOW state

Workaround(s) In applications where the packets are short enough to fit in the RX FIFO and one wants
to wait for the whole packet to be received before starting to read the RX FIFO, for
variable packet length mode (PKTCTRL0.LENGTH_CONFIG=1) the PKTLEN register
should be set to 61 to make sure the whole packet including status bytes are 64 bytes or
less (length byte (61) + 61 payload bytes + 2 status bytes = 64 bytes) or PKTLEN <= 62 if
fixed packet length mode is used (PKTCTRL0.LENGTH_CONFIG=0). In application
where the packets do not fit in the RX FIFO, one must start reading the RX FIFO before
it reaches its limit (64 bytes).

 */
#define CC1101_MAX_FIFO_ERRATA 61


//////////////////////////////////////////////////////////////////////////////////
static uint8_t _debug=0;
static uint8_t _cs_pin=0;
static uint8_t _spi_port=0;
static uint8_t _gdo0_pin=0;
static cc1101_rx_callback_func _func;


static const int RSSI_OFFSET=74;   /* no units */
static const int CCA_RETRIES=4;
static const int MAX_FRAME_SIZE=        65;
static const int BACKOFF_PERIOD_USECS=  250;
static const int MAX_HOPS=              4;
static const int RADIO_OSC_FREQ=        26000000;
static const int PHY_PREAMBLE_SYNC_BYTES=   8;
static const int RANDOM_OFFSET         =67;
static const int RANDOM_MULTIPLIER     =109;
static uint8_t _RndSeed = 0;
static uint16_t sBackoffHelper = 0;
static uint8_t cc1101_tx_buf[64];
static uint8_t cc1101_rx_buf[64];


void cc1101_debug_on(void){
	_debug=1;
}
void cc1101_debug_off(void){
	_debug=0;
}




//////////////////////////////////////////////////////////////////////////////////



void cc1101_cca_help_init(){

	/* Platform constant used to calculate worst-case for an application acknowledgment delay.
                                   processing time on peer
                                   |   round trip
                                   |   |      max number of replays
                                   |   |      |             number of backoff opportunities
                                   |   |      |             |         average number of backoffs
                                   |   |      |             |         |                                    */
	int PLATFORM_FACTOR_CONSTANT= (2 + 2*(MAX_HOPS*(CCA_RETRIES*(8*BACKOFF_PERIOD_USECS)/1000)));

	uint32_t dataRate, bits;
	uint16_t exponent, mantissa;
	uint16_t sReplyDelayScalar = 0;

	/* mantissa is in MDMCFG3 */
	mantissa = 256 + cc1101_register_read(CC1101_MDMCFG3);

	/* exponent is lower nibble of MDMCFG4. */
	exponent = 28 - (cc1101_register_read(CC1101_MDMCFG4) & 0x0F);

	/* we can now get data rate */
	dataRate = mantissa * (RADIO_OSC_FREQ>>exponent);

	bits = ((uint32_t)((PHY_PREAMBLE_SYNC_BYTES + MAX_FRAME_SIZE)*8))*10000;

	/* processing on the peer + the Tx/Rx time plus more */
	sReplyDelayScalar = PLATFORM_FACTOR_CONSTANT + (((bits/dataRate)+5)/10);

	/* This helper value is used to scale the backoffs during CCA. At very
	 * low data rates we need to backoff longer to prevent continual sampling
	 * of valid frames which take longer to send at lower rates. Use the scalar
	 * we just calculated divided by 32. With the backoff algorithm backing
	 * off up to 16 periods this will result in waiting up to about 1/2 the total
	 * scalar value. For high data rates this does not contribute at all. Value
	 * is in microseconds.
	 */
	sBackoffHelper = BACKOFF_PERIOD_USECS + (sReplyDelayScalar>>5)*1000;

	////////////////////////////////////////////////////////////////////////////////

	//
	cc1101_command_strobe( CC1101_SRX );

	/* use most random bit of rssi to populate the random seed */
	{
		uint8_t i;
		for(i=0; i<16; i++)
		{
			_RndSeed = (_RndSeed << 1) | (cc1101_register_read(CC1101_RSSI) & 0x01);
		}
	}

	/* Force the seed to be non-zero by setting one bit, just in case... */
	_RndSeed |= 0x0080;

	//
	cc1101_command_strobe( CC1101_SIDLE );

	//////////////////////////////////////////////////////////////////////////////
}



uint8_t cc1101_RandomByte(void)
{


	_RndSeed = (_RndSeed*RANDOM_MULTIPLIER) + RANDOM_OFFSET;

	return _RndSeed;
}



void cc1101_apply_smartrf_433mhz_600()
{
	/*
SMART RF TEMPLATE
cc1101_register_write(0x@AH@,0x@VH@);//@RN@:@Rd@
	 */

	/* Sync word qualifier mode = 30/32 sync word bits detected */
	/* CRC autoflush = false */
	/* Channel spacing = 199.951172 */
	/* Data format = Normal mode */
	/* Data rate = 0.599742 */
	/* RX filter BW = 58.035714 */
	/* PA ramping = false */
	/* Preamble count = 4 */
	/* Whitening = false */
	/* Address config = No address check */
	/* Carrier frequency = 432.999817 */
	/* Device address = 0 */
	/* TX power = 10 */
	/* Manchester enable = false */
	/* CRC enable = true */
	/* Deviation = 14.282227 */
	/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
	/* Packet length = 255 */
	/* Modulation format = GFSK */
	/* Base frequency = 432.999817 */
	/* Modulated = true */
	/* Channel number = 0 */
	/***************************************************************
	 *  SmartRF Studio(tm) Export
	 *
	 *  Radio register settings specifed with C-code
	 *  compatible #define statements.
	 *
	 *  RF device: CC1101
	 *
	 ***************************************************************/

	cc1101_register_write(0x0002,0x06);//IOCFG0:GDO0 Output Pin Configuration
	cc1101_register_write(0x0003,0x47);//FIFOTHR:RX FIFO and TX FIFO Thresholds
	cc1101_register_write(0x0008,0x05);//PKTCTRL0:Packet Automation Control
	cc1101_register_write(0x000B,0x06);//FSCTRL1:Frequency Synthesizer Control
	cc1101_register_write(0x000D,0x10);//FREQ2:Frequency Control Word, High Byte
	cc1101_register_write(0x000E,0xA7);//FREQ1:Frequency Control Word, Middle Byte
	cc1101_register_write(0x000F,0x62);//FREQ0:Frequency Control Word, Low Byte
	cc1101_register_write(0x0010,0xF4);//MDMCFG4:Modem Configuration
	cc1101_register_write(0x0011,0x83);//MDMCFG3:Modem Configuration
	cc1101_register_write(0x0012,0x13);//MDMCFG2:Modem Configuration
	cc1101_register_write(0x0015,0x31);//DEVIATN:Modem Deviation Setting
	cc1101_register_write(0x0018,0x18);//MCSM0:Main Radio Control State Machine Configuration
	cc1101_register_write(0x0019,0x16);//FOCCFG:Frequency Offset Compensation Configuration
	cc1101_register_write(0x0020,0xFB);//WORCTRL:Wake On Radio Control
	cc1101_register_write(0x0023,0xE9);//FSCAL3:Frequency Synthesizer Calibration
	cc1101_register_write(0x0024,0x2A);//FSCAL2:Frequency Synthesizer Calibration
	cc1101_register_write(0x0025,0x00);//FSCAL1:Frequency Synthesizer Calibration
	cc1101_register_write(0x0026,0x1F);//FSCAL0:Frequency Synthesizer Calibration
	cc1101_register_write(0x002C,0x81);//TEST2:Various Test Settings
	cc1101_register_write(0x002D,0x35);//TEST1:Various Test Settings
	cc1101_register_write(0x002E,0x09);//TEST0:Various Test Settings


}

void cc1101_apply_smartrf_433mhz_1200()
{
	/*
SMART RF TEMPLATE
cc1101_register_write(0x@AH@,0x@VH@);//@RN@:@Rd@
	 */

	/* Sync word qualifier mode = 30/32 sync word bits detected */
	/* CRC autoflush = false */
	/* Channel spacing = 199.951172 */
	/* Data format = Normal mode */
	/* Data rate = 1.19948 */
	/* RX filter BW = 58.035714 */
	/* PA ramping = false */
	/* Preamble count = 4 */
	/* Whitening = false */
	/* Address config = No address check */
	/* Carrier frequency = 432.999817 */
	/* Device address = 0 */
	/* TX power = 10 */
	/* Manchester enable = false */
	/* CRC enable = true */
	/* Deviation = 5.157471 */
	/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
	/* Packet length = 255 */
	/* Modulation format = GFSK */
	/* Base frequency = 432.999817 */
	/* Modulated = true */
	/* Channel number = 0 */
	/***************************************************************
	 *  SmartRF Studio(tm) Export
	 *
	 *  Radio register settings specifed with C-code
	 *  compatible #define statements.
	 *
	 *  RF device: CC1101
	 *
	 ***************************************************************/

	cc1101_register_write(0x0002,0x06);//IOCFG0:GDO0 Output Pin Configuration
	cc1101_register_write(0x0003,0x47);//FIFOTHR:RX FIFO and TX FIFO Thresholds
	cc1101_register_write(0x0008,0x05);//PKTCTRL0:Packet Automation Control
	cc1101_register_write(0x000B,0x06);//FSCTRL1:Frequency Synthesizer Control
	cc1101_register_write(0x000D,0x10);//FREQ2:Frequency Control Word, High Byte
	cc1101_register_write(0x000E,0xA7);//FREQ1:Frequency Control Word, Middle Byte
	cc1101_register_write(0x000F,0x62);//FREQ0:Frequency Control Word, Low Byte
	cc1101_register_write(0x0010,0xF5);//MDMCFG4:Modem Configuration
	cc1101_register_write(0x0011,0x83);//MDMCFG3:Modem Configuration
	cc1101_register_write(0x0012,0x13);//MDMCFG2:Modem Configuration
	cc1101_register_write(0x0015,0x15);//DEVIATN:Modem Deviation Setting
	cc1101_register_write(0x0018,0x18);//MCSM0:Main Radio Control State Machine Configuration
	cc1101_register_write(0x0019,0x16);//FOCCFG:Frequency Offset Compensation Configuration
	cc1101_register_write(0x0020,0xFB);//WORCTRL:Wake On Radio Control
	cc1101_register_write(0x0023,0xE9);//FSCAL3:Frequency Synthesizer Calibration
	cc1101_register_write(0x0024,0x2A);//FSCAL2:Frequency Synthesizer Calibration
	cc1101_register_write(0x0025,0x00);//FSCAL1:Frequency Synthesizer Calibration
	cc1101_register_write(0x0026,0x1F);//FSCAL0:Frequency Synthesizer Calibration
	cc1101_register_write(0x002C,0x81);//TEST2:Various Test Settings
	cc1101_register_write(0x002D,0x35);//TEST1:Various Test Settings
	cc1101_register_write(0x002E,0x09);//TEST0:Various Test Settings



}

void cc1101_apply_smartrf_433mhz_19200()
{
	/*
SMART RF TEMPLATE
cc1101_register_write(0x@AH@,0x@VH@);//@RN@:@Rd@
	 */

	/* Sync word qualifier mode = 30/32 sync word bits detected */
	/* CRC autoflush = false */
	/* Channel spacing = 199.951172 */
	/* Data format = Normal mode */
	/* Data rate = 19.1917 */
	/* RX filter BW = 101.562500 */
	/* PA ramping = false */
	/* Preamble count = 4 */
	/* Whitening = false */
	/* Address config = No address check */
	/* Carrier frequency = 432.999817 */
	/* Device address = 0 */
	/* TX power = 10 */
	/* Manchester enable = false */
	/* CRC enable = true */
	/* Deviation = 19.042969 */
	/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
	/* Packet length = 255 */
	/* Modulation format = GFSK */
	/* Base frequency = 432.999817 */
	/* Modulated = true */
	/* Channel number = 0 */
	/***************************************************************
	 *  SmartRF Studio(tm) Export
	 *
	 *  Radio register settings specifed with C-code
	 *  compatible #define statements.
	 *
	 *  RF device: CC1101
	 *
	 ***************************************************************/

	cc1101_register_write(0x0002,0x06);//IOCFG0:GDO0 Output Pin Configuration
	cc1101_register_write(0x0003,0x47);//FIFOTHR:RX FIFO and TX FIFO Thresholds
	cc1101_register_write(0x0008,0x05);//PKTCTRL0:Packet Automation Control
	cc1101_register_write(0x000B,0x06);//FSCTRL1:Frequency Synthesizer Control
	cc1101_register_write(0x000D,0x10);//FREQ2:Frequency Control Word, High Byte
	cc1101_register_write(0x000E,0xA7);//FREQ1:Frequency Control Word, Middle Byte
	cc1101_register_write(0x000F,0x62);//FREQ0:Frequency Control Word, Low Byte
	cc1101_register_write(0x0010,0xC9);//MDMCFG4:Modem Configuration
	cc1101_register_write(0x0011,0x83);//MDMCFG3:Modem Configuration
	cc1101_register_write(0x0012,0x13);//MDMCFG2:Modem Configuration
	cc1101_register_write(0x0015,0x34);//DEVIATN:Modem Deviation Setting
	cc1101_register_write(0x0018,0x18);//MCSM0:Main Radio Control State Machine Configuration
	cc1101_register_write(0x0019,0x16);//FOCCFG:Frequency Offset Compensation Configuration
	cc1101_register_write(0x001B,0x43);//AGCCTRL2:AGC Control
	cc1101_register_write(0x0020,0xFB);//WORCTRL:Wake On Radio Control
	cc1101_register_write(0x0023,0xE9);//FSCAL3:Frequency Synthesizer Calibration
	cc1101_register_write(0x0024,0x2A);//FSCAL2:Frequency Synthesizer Calibration
	cc1101_register_write(0x0025,0x00);//FSCAL1:Frequency Synthesizer Calibration
	cc1101_register_write(0x0026,0x1F);//FSCAL0:Frequency Synthesizer Calibration
	cc1101_register_write(0x002C,0x81);//TEST2:Various Test Settings
	cc1101_register_write(0x002D,0x35);//TEST1:Various Test Settings
	cc1101_register_write(0x002E,0x09);//TEST0:Various Test Settings


}



void cc1101_apply_smartrf_433mhz_38400()
{
	/*
SMART RF TEMPLATE
cc1101_register_write(0x@AH@,0x@VH@);//@RN@:@Rd@
	 */

	/* Sync word qualifier mode = 30/32 sync word bits detected */
	/* CRC autoflush = false */
	/* Channel spacing = 199.951172 */
	/* Data format = Normal mode */
	/* Data rate = 38.3835 */
	/* RX filter BW = 101.562500 */
	/* PA ramping = false */
	/* Preamble count = 4 */
	/* Whitening = false */
	/* Address config = No address check */
	/* Carrier frequency = 432.999817 */
	/* Device address = 0 */
	/* TX power = 0 */
	/* Manchester enable = false */
	/* CRC enable = true */
	/* Deviation = 20.629883 */
	/* Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word */
	/* Packet length = 255 */
	/* Modulation format = GFSK */
	/* Base frequency = 432.999817 */
	/* Modulated = true */
	/* Channel number = 0 */
	/***************************************************************
	 *  SmartRF Studio(tm) Export
	 *
	 *  Radio register settings specifed with C-code
	 *  compatible #define statements.
	 *
	 *  RF device: CC1101
	 *
	 ***************************************************************/

	cc1101_register_write(0x0002,0x06);//IOCFG0:GDO0 Output Pin Configuration
	cc1101_register_write(0x0003,0x47);//FIFOTHR:RX FIFO and TX FIFO Thresholds
	cc1101_register_write(0x0008,0x05);//PKTCTRL0:Packet Automation Control
	cc1101_register_write(0x000B,0x06);//FSCTRL1:Frequency Synthesizer Control
	cc1101_register_write(0x000D,0x10);//FREQ2:Frequency Control Word, High Byte
	cc1101_register_write(0x000E,0xA7);//FREQ1:Frequency Control Word, Middle Byte
	cc1101_register_write(0x000F,0x62);//FREQ0:Frequency Control Word, Low Byte
	cc1101_register_write(0x0010,0xCA);//MDMCFG4:Modem Configuration
	cc1101_register_write(0x0011,0x83);//MDMCFG3:Modem Configuration
	cc1101_register_write(0x0012,0x13);//MDMCFG2:Modem Configuration
	cc1101_register_write(0x0015,0x35);//DEVIATN:Modem Deviation Setting
	cc1101_register_write(0x0018,0x18);//MCSM0:Main Radio Control State Machine Configuration
	cc1101_register_write(0x0019,0x16);//FOCCFG:Frequency Offset Compensation Configuration
	cc1101_register_write(0x001B,0x43);//AGCCTRL2:AGC Control
	cc1101_register_write(0x0020,0xFB);//WORCTRL:Wake On Radio Control
	cc1101_register_write(0x0023,0xE9);//FSCAL3:Frequency Synthesizer Calibration
	cc1101_register_write(0x0024,0x2A);//FSCAL2:Frequency Synthesizer Calibration
	cc1101_register_write(0x0025,0x00);//FSCAL1:Frequency Synthesizer Calibration
	cc1101_register_write(0x0026,0x1F);//FSCAL0:Frequency Synthesizer Calibration
	cc1101_register_write(0x002C,0x81);//TEST2:Various Test Settings
	cc1101_register_write(0x002D,0x35);//TEST1:Various Test Settings
	cc1101_register_write(0x002E,0x09);//TEST0:Various Test Settings

}





void cc1101_init(uint8_t cs_pin, uint8_t spi_port,uint8_t gdo0_pin,cc1101_rx_callback_func func)
{
	Serial.println("cc1101_init");
	_cs_pin=cs_pin;
	_spi_port=spi_port;
	_gdo0_pin=gdo0_pin;
	_func=func;

	//arduino stuff
	SPI.begin();
	SPI.setModule(_spi_port);

	//
	pinMode(_cs_pin, OUTPUT);



	//reset radio
	SPI.transfer(CC1101_SRES);
	//wait wake up
	delay(5);

	cc1101_command_strobe( CC1101_SIDLE );
	while (cc1101_command_strobe( CC1101_SNOP ) & 0xF0) ; //if chipstatus not ready wait


	if(_debug){
		//test SPI
		cc1101_register_write( CC1101_PKTLEN, CC1101_MAX_FIFO_ERRATA );
		if(cc1101_register_read( CC1101_PKTLEN ) != CC1101_MAX_FIFO_ERRATA ){
			Serial.println("CC1101 SPI write \033[1;31mERROR!\033[m");
		} else {
			Serial.println("CC1101 SPI write \033[1;32mOK!\033[m");
		}
		//test radio version
		cc1101_register_read( CC1101_PARTNUM);
		cc1101_register_read( CC1101_VERSION);
	}

	//cc1101_apply_smartrf_433mhz_600();
	//cc1101_apply_smartrf_433mhz_19200();
	cc1101_apply_smartrf_433mhz_38400();

	///////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	/* Set logical channel */
	/* make sure radio is off before changing channels */
	cc1101_register_write( CC1101_CHANNR, 0 );
	////////////////////////////////////////////////////////////////////////////
	/* Set RF Output power level */
	uint8_t patable[]={0xc0,0xc0,0xc0,0xc0, 0xc0,0xc0,0xc0,0xc0};
	//uint8_t patable[]={0x50,0x50,0x50,0x50, 0x50,0x50,0x50,0x50};
	cc1101_patable_write(patable,8);
	////////////////////////////////////////////////////////////////////////////

	/* Main Radio Control State Machine control configuration:
	 *
	 * - Auto Calibrate - when going from IDLE to RX/TX
	 * - PO_TIMEOUT is extracted from SmartRF setting.
	 * - XOSC is OFF in Sleep state.
	 *
	 *
	 */

#define MRFI_SETTING_MCSM0      (0x10 | (cc1101_register_read(CC1101_MCSM0) & (BV(2)|BV(3))))
	cc1101_register_write( CC1101_MCSM0, MRFI_SETTING_MCSM0 );

	/* Main Radio Control State Machine control configuration:
	 * - Remain RX state after RX (corrupt RX buffer)
	 * - Go to IDLE after TX
	 * - RSSI below threshold and NOT receiving.
	 */
	//#define MRFI_SETTING_MCSM1      0x3C

	/* Main Radio Control State Machine control configuration:
	 * - Go to IDLE after TX (safety)
	 * - Go to IDLE after RX (safety)
	 * - RSSI below threshold and NOT receiving (try avoid collisions)
	 * */

#define MRFI_SETTING_MCSM1      0x30
	cc1101_register_write( CC1101_MCSM1, MRFI_SETTING_MCSM1 );

	/*
	 *  Packet Length - Setting for maximum allowed packet length.
	 *  The PKTLEN setting does not include the length field but maximum frame size does.
	 *  Subtract length field size from maximum frame size to get value for PKTLEN.
	 */
	cc1101_register_write( CC1101_PKTLEN, CC1101_MAX_FIFO_ERRATA );

	/* Packet automation control - Original value except WHITE_DATA is extracted from SmartRF setting. */
#define MRFI_SETTING_PKTCTRL0   (0x05 | (cc1101_register_read(CC1101_PKTCTRL0) & BV(6)))
	cc1101_register_write( CC1101_PKTCTRL0, MRFI_SETTING_PKTCTRL0 );

	/* Packet automation control - base value is power up value whick has
	 * APPEND_STATUS enabled;
	 * no CRC autoflush */
	/*
	 *  Note!  Automatic CRC check is not, and must not, be enabled.  This feature
	 *  flushes the *entire* receive FIFO when CRC fails.  If this feature is
	 *  enabled it is possible to be reading from the FIFO and have a second
	 *  receive occur that fails CRC and automatically flushes the receive FIFO.
	 *  This could cause reads from an empty receive FIFO which puts the radio
	 *  into an undefined state.
	 */
#define PKTCTRL1_BASE_VALUE         BV(2)
#define PKTCTRL1_ADDR_FILTER_OFF    PKTCTRL1_BASE_VALUE
#define PKTCTRL1_ADDR_FILTER_ON     (PKTCTRL1_BASE_VALUE | (BV(0)|BV(1)))
	/* disable hardware filtering on the radio */
	//register_write( CC1101_PKTCTRL1, PKTCTRL1_ADDR_FILTER_ON );
	cc1101_register_write( CC1101_PKTCTRL1, PKTCTRL1_ADDR_FILTER_OFF );
	/*
	 *  Set the hardware address register.  The hardware address filtering only recognizes
	 *  a single byte but this does provide at least some automatic hardware filtering.
	 */
	cc1101_register_write( CC1101_ADDR, 0xbd );

	/* FIFO threshold - this register has fields that need to be configured for the CC1101 */
#define MRFI_SETTING_FIFOTHR    (0x07 | (cc1101_register_read(CC1101_FIFOTHR) & (BV(4)|BV(5)|BV(6))))
	cc1101_register_write( CC1101_FIFOTHR, MRFI_SETTING_FIFOTHR );

	/////////////////////////////////////////////////////////////////////////////

	cc1101_cca_help_init();


	/////////////////////////////////////////////////////////////////////////////
	/* GDO0 interrupt */
	cc1101_register_write(CC1101_IOCFG0, CC1101_GDO_ASSERT_ON_SYNC_WORD);

	/*
	 * It is recommended to employ an interrupt driven solution since high rate SPI polling
       reduces the RX sensitivity.
	 */
	attachInterrupt(_gdo0_pin, cc1101_packet_recv,FALLING);
	/////////////////////////////////////////////////////////////////////////////

	//put radio in RX mode
	cc1101_command_strobe( CC1101_SRX );
}



////////////////////////////////////////////////////////
const char* cc1101_reg_name(uint8_t addr)
{
	switch(addr){


	case 0x00: return "IOCFG2  (GDO2 output pin configuration)"; break;
	case 0x01: return "IOCFG1  (GDO1 output pin configuration)"; break;
	case 0x02: return "IOCFG1  (GDO0 output pin configuration)"; break;
	case 0x03: return "FIFOTHR (RX FIFO and TX FIFO thresholds)"; break;
	case 0x04: return "SYNC1   (Sync word, high byte)"; break;
	case 0x05: return "SYNC0   (Sync word, low byte)"; break;
	case 0x06: return "PKTLEN  (Packet length)"; break;
	case 0x07: return "PKTCTRL1(Packet automation control)"; break;
	case 0x08: return "PKTCTRL0(Packet automation control)"; break;
	case 0x09: return "ADDR    (Device address)"; break;
	case 0x0A: return "CHANNR  (Channel number)"; break;
	case 0x0B: return "FSCTRL1 (Frequency synthesizer control)"; break;
	case 0x0C: return "FSCTRL0 (Frequency synthesizer control)"; break;
	case 0x0D: return "FREQ2   (Frequency control word, high byte)"; break;
	case 0x0E: return "FREQ1   (Frequency control word, middle byte)"; break;
	case 0x0F: return "FREQ0   (Frequency control word, low byte)"; break;
	case 0x10: return "MDMCFG4 (Modem configuration)"; break;
	case 0x11: return "MDMCFG3 (Modem configuration)"; break;
	case 0x12: return "MDMCFG2 (Modem configuration)"; break;
	case 0x13: return "MDMCFG1 (Modem configuration)"; break;
	case 0x14: return "MDMCFG0 (Modem configuration)"; break;
	case 0x15: return "DEVIATN (Modem deviation setting)"; break;
	case 0x16: return "MCSM2   (Main Radio Control State Machine configuration)"; break;
	case 0x17: return "MCSM1   (Main Radio Control State Machine configuration)"; break;
	case 0x18: return "MCSM0   (Main Radio Control State Machine configuration)"; break;
	case 0x19: return "FOCCFG  (Frequency Offset Compensation configuration)"; break;
	case 0x1A: return "BSCFG   (Bit Synchronization configuration)"; break;
	case 0x1B: return "AGCCTRL2(AGC control)"; break;
	case 0x1C: return "AGCCTRL1(AGC control)"; break;
	case 0x1D: return "AGCCTRL0(AGC control)"; break;
	case 0x1E: return "WOREVT1 (High byte Event0 timeout)"; break;
	case 0x1F: return "WOREVT0 (Low byte Event0 timeout)"; break;
	case 0x20: return "WORCTRL (Wake On Radio control)"; break;
	case 0x21: return "FREND1  (Front end RX configuration)"; break;
	case 0x22: return "FREDN0  (Front end TX configuration)"; break;
	case 0x23: return "FSCAL3  (Frequency synthesizer calibration)"; break;
	case 0x24: return "FSCAL2  (Frequency synthesizer calibration)"; break;
	case 0x25: return "FSCAL1  (Frequency synthesizer calibration)"; break;
	case 0x26: return "FSCAL0  (Frequency synthesizer calibration)"; break;
	case 0x27: return "RCCTRL1 (RC oscillator configuration)"; break;
	case 0x28: return "RCCTRL0 (RC oscillator configuration)"; break;
	case 0x29: return "FSTEST  (Frequency synthesizer calibration control)"; break;
	case 0x2A: return "PTEST   (Production test)"; break;
	case 0x2B: return "AGCTEST (AGC test)"; break;
	case 0x2C: return "TEST2   (Various test settings)"; break;
	case 0x2D: return "TEST1   (Various test settings)"; break;
	case 0x2E: return "TEST0   (Various test settings)"; break;
	case 0x30: return "PARTNUM (Chip ID)"; break;
	case 0x31: return "VERSION (Chip ID)"; break;
	case 0x32: return "FREQEST (Frequency Offset Estimate from demodulator)"; break;
	case 0x33: return "LQI (Demodulator estimate for Link Quality)"; break;
	case 0x34: return "RSSI (Received signal strength indication)"; break;
	case 0x35: return "MARCSTATE (Main Radio Control State Machine state)"; break;
	case 0x36: return "WORTIME1 (High byte of WOR time)"; break;
	case 0x37: return "WORTIME0 (Low byte of WOR time)"; break;
	case 0x38: return "PKTSTATUS (Current GDOx status and packet status)"; break;
	case 0x39: return "VCO_VC_DAC (Current setting from PLL calibration module)"; break;
	case 0x3A: return "TXBYTES (Underflow and number of bytes)"; break;
	case 0x3B: return "RXBYTES (Overflow and number of bytes)"; break;
	case 0x3C: return "RCCTRL1_STATUS (Last RC Oscillator Calibration Result)"; break;
	case 0x3D: return "RCCTRL0_STATUS (Last RC Oscillator Calibration Result)"; break;
	case 0x3E: return "PA_TABLE (PA control settings table)"; break;
	case 0x3F: return "TXFIFO (Transmit FIFO)/RXFIFO (Receive FIFO)"; break;


	}
	return "\033[1;31mUNKNOWN-REGISTER\033[m";
}
const char* cc1101_marcstate(uint8_t state)
{
	switch (state)
	{


	case CC1101_MARCSTATE_SLEEP:return "SLEEP (SLEEP)";	break;
	case CC1101_MARCSTATE_IDLE:	return "IDLE (IDLE)";break;
	case CC1101_MARCSTATE_XOFF:	return "XOFF (XOFF)";break;
	case CC1101_MARCSTATE_VCOON_MC:	return "VCOON_MC (MANCAL)";	break;
	case CC1101_MARCSTATE_REGON_MC:	return "REGON_MC (MANCAL)";	break;
	case CC1101_MARCSTATE_MANCAL:return "MANCAL (MANCAL)";break;
	case CC1101_MARCSTATE_VCOON:return "VCOON (FS_WAKEUP)";	break;
	case CC1101_MARCSTATE_REGON:return "REGON (FS_WAKEUP)";	break;
	case CC1101_MARCSTATE_STARTCAL:return "STARTCAL (CALIBRATE)";break;
	case CC1101_MARCSTATE_BWBOOST:return "BWBOOST (SETTLING)";break;
	case CC1101_MARCSTATE_FS_LOCK:return "FS_LOCK (SETTLING)";break;
	case CC1101_MARCSTATE_IFADCON:return "IFADCON (SETTLING)";break;
	case CC1101_MARCSTATE_ENDCAL:return "ENDCAL (CALIBRATE)";break;
	case CC1101_MARCSTATE_RX:return "RX (RX)";	break;
	case CC1101_MARCSTATE_RX_END:return "RX_END (RX)";	break;
	case CC1101_MARCSTATE_RX_RST:return "RX_RST (RX)";break;
	case CC1101_MARCSTATE_TXRX_SWITCH:	return "TXRX_SWITCH (TXRX_SETTLING)";	break;
	case CC1101_MARCSTATE_RXFIFO_OVERFLOW:	return "RXFIFO_OVERFLOW (RXFIFO_OVERFLOW)";		break;
	case CC1101_MARCSTATE_FSTXON:	return "FSTXON (FSTXON)";	break;
	case CC1101_MARCSTATE_TX:return "TX (TX)";break;
	case CC1101_MARCSTATE_TX_END:return "TX_END (TX)";break;
	case CC1101_MARCSTATE_RXTX_SWITCH:	return "RXTX_SWITCH (RXTX_SETTLING)";break;
	case CC1101_MARCSTATE_TXFIFO_UNDERFLOW:	return "TXFIFO_UNDERFLOW (TXFIFO_UNDERFLOW)";	break;

	}
	return "\033[1;31mUNKNOWN-STATE\033[m" ;
}
const char* cc1101_strobe(uint8_t addr)
{
	switch(addr){


	case 0x30: return "SRES   (Reset chip)"; break;
	case 0x31: return "SFSTXON(Enable and calibrate frequency synthesizer)"; break;
	case 0x32: return "SXOFF  (Turn off crystal oscillator)"; break;
	case 0x33: return "SCAL   (Calibrate frequency synthesizer and turn it off)"; break;
	case 0x34: return "SRX    (Enable RX. Perform calibration if enabled)"; break;
	case 0x35: return "STX    (Enable TX. If in RX state, only enable TX if CCA passes)"; break;
	case 0x36: return "SIDLE  (Exit RX / TX, turn off frequency synthesizer)"; break;
	case 0x37: return "SRSVD  (Reserved.  Do not use)"; break;
	case 0x38: return "SWOR   (Start automatic RX polling sequence (Wake-on-Radio)"; break;
	case 0x39: return "SPWD   (Enter power down mode when CSn goes high)"; break;
	case 0x3A: return "SFRX   (Flush the RX FIFO buffer)"; break;
	case 0x3B: return "SFTX   (Flush the TX FIFO buffer)"; break;
	case 0x3C: return "SWORRST(Reset real time clock)"; break;
	case 0x3D: return "SNOP   (No operation. Returns status byte)"; break;

	}
	return "\033[1;31mUNKNOWN-STROBE\033[m";
}
////////////////////////////////////////////////////////
uint8_t cc1101_command_strobe(uint8_t addr)
{
	//noInterrupts();

	uint8_t chipstatus;
	if(_debug){
		Serial.print("CC1101 strobe=");
		Serial.println(cc1101_strobe(addr));
	}

	digitalWrite(_cs_pin,1);
	digitalWrite(_cs_pin,0);
	chipstatus = SPI.transfer(addr);
	digitalWrite(_cs_pin,1);

	//interrupts();

	return chipstatus;
}
uint8_t cc1101_spi_register_single_byte(uint8_t addrByte,uint8_t mode, uint8_t writeValue)
{
	//noInterrupts();

	uint8_t readValue;
	//
	digitalWrite(_cs_pin,1);
	digitalWrite(_cs_pin,0);
	//
	SPI.transfer(addrByte | mode);
	readValue = SPI.transfer(writeValue);
	//
	digitalWrite(_cs_pin,1);

	if(_debug){
		Serial.print("CC1101 reg=");
		Serial.print(cc1101_reg_name(addrByte));
		Serial.print(" addr=");
		Serial.print(addrByte,HEX);
		Serial.print(" write=");
		Serial.print(writeValue,HEX);
		Serial.print(" read=");
		Serial.println(readValue,HEX);
	}

	//interrupts();

	return readValue;
}
void cc1101_spi_register_multi_byte(uint8_t addrByte,uint8_t mode, uint8_t * pData, uint8_t len)
{

	//noInterrupts();

	digitalWrite(_cs_pin,1);
	digitalWrite(_cs_pin,0);
	SPI.transfer(addrByte | mode);
	for(uint8_t idx=0;idx<len;idx++){
		pData[idx] = SPI.transfer(pData[idx]);
	}
	digitalWrite(_cs_pin,1);

	if(_debug){
		Serial.print("CC1101 reg=");
		Serial.print(cc1101_reg_name(addrByte));
		Serial.print(" addr=");
		Serial.println(addrByte,HEX);
	}

	//interrupts();
}
/////////////////////////////////////////////////////////////
uint8_t cc1101_register_read(uint8_t addr)
{
	return( cc1101_spi_register_single_byte(addr, CC1101_READ_BURST, CC1101_DUMMY_BYTE) );
}
void cc1101_register_write(uint8_t addr, uint8_t value)
{
	cc1101_spi_register_single_byte(addr,CC1101_WRITE_BYTE, value);
}
void cc1101_fifo_rx_read(uint8_t * pData, uint8_t len)
{
	cc1101_spi_register_multi_byte(CC1101_RXFIFO,CC1101_READ_BURST , pData, len);
}
void cc1101_fifo_tx_write(uint8_t * pData, uint8_t len)
{
	cc1101_spi_register_multi_byte(CC1101_TXFIFO, CC1101_WRITE_BURST, pData, len);
}
void cc1101_patable_read(uint8_t * pData, uint8_t len)
{
	cc1101_spi_register_multi_byte(CC1101_PA_TABLE0, CC1101_READ_BURST, pData, len);
}
void cc1101_patable_write(uint8_t * pData, uint8_t len)
{
	cc1101_spi_register_multi_byte(CC1101_PA_TABLE0, CC1101_WRITE_BURST, pData, len);
}
///////////////////////////////////////////////////////////////////////////////////



/* There is no bit in h/w to tell if RSSI in the register is valid or not.
 * The hardware needs to be in RX state for a certain amount of time before
 * a valid RSSI value is calculated and placed in the register. This min
 * wait time is defined by MRFI_BOARD_RSSI_VALID_DELAY_US. We don't need to
 * add such delay every time RSSI value is needed. If the Carier Sense signal
 * is high or CCA signal is high, we know that the RSSI value must be valid.
 * We use that knowledge to reduce our wait time. We break down the delay loop
 * in multiple chunks and during each iteration, check for the CS and CCA
 * signal. If either of these signals is high, we return immediately. Else,
 * we wait for the max delay specified.
 */
void cc1101_rssi_valid_wait(void)
{
#define MRFI_RSSI_VALID_DELAY_US    1300
#define MRFI_PKTSTATUS_CCA BV(4)
#define MRFI_PKTSTATUS_CS  BV(6)

	int16_t delay = MRFI_RSSI_VALID_DELAY_US;
	do
	{
		if(cc1101_register_read(CC1101_PKTSTATUS) & (MRFI_PKTSTATUS_CCA | MRFI_PKTSTATUS_CS))
		{
			break;
		}
		delayMicroseconds(64); /* sleep */
		delay -= 64;
	}while(delay > 0);

}




void cc1101_packet_send(uint8_t *text,uint8_t textlen)
{
	/*disable receive interrupts */
	detachInterrupt(_gdo0_pin);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(_debug){
		Serial.print("CC1101 base_backoff=");
		Serial.println(sBackoffHelper);
	}

	/* turn off radio */
	cc1101_command_strobe( CC1101_SIDLE );
	while (cc1101_command_strobe( CC1101_SNOP ) & 0xF0) ; //chipstatus not ready wait

	/* flush the receive FIFO of any residual data */
	cc1101_command_strobe( CC1101_SFRX );


	/* Format packet */

	//clean buffer
	memset(cc1101_tx_buf,0,sizeof(cc1101_tx_buf));

	if(textlen>CC1101_MAX_FIFO_ERRATA){
		textlen=CC1101_MAX_FIFO_ERRATA;
	}
	//
	cc1101_tx_buf[0]=textlen;
	memcpy(&cc1101_tx_buf[1],text,textlen);

	uint8_t txBytes=textlen+1;

	if(_debug){
		//packet length
		Serial.print("CC1101 txlen=");
		Serial.print(txBytes);
		Serial.print(" txbuf=");
		for(uint8_t i=0;i<txBytes;i++){
			Serial.print(cc1101_tx_buf[i],HEX);
			if(i!=txBytes-1) Serial.print("|");
		}
		Serial.println("");
	}

	/* Write packet to transmit FIFO */
	cc1101_fifo_tx_write(cc1101_tx_buf,txBytes);

	/* For CCA algorithm, we need to know the transition from the RX state to
	 * the TX state. There is no need for SYNC signal in this logic. So we
	 * can re-configure the GDO_0 output from the radio to be PA_PD signal
	 * instead of the SYNC signal.
	 * Since both SYNC and PA_PD are used as falling edge interrupts, we
	 * don't need to reconfigure the MCU input.
	 */
	/* GDO0 interrupt */

	/* PA_PD signal stays LOW while in TX state and goes back to HIGH when the radio transitions to RX state. */
	cc1101_register_write(CC1101_IOCFG0, CC1101_GDO_PA_PD);

	/* set number of CCA retries */
	uint8_t ccaRetries = CCA_RETRIES;
	bool cca_end_ok=true;

	for (;;){
		if(_debug){
			Serial.print("CC1101 ------------ TX ccaRetries=");
			Serial.print(ccaRetries);
			Serial.println("---------------");
		}

		/* put Radio in RX mode */
		cc1101_command_strobe( CC1101_SRX );

		/* wait for the rssi to be valid. */
		cc1101_rssi_valid_wait();

		/* send strobe to initiate transmit */
		cc1101_command_strobe( CC1101_STX );


		/////////////////////////////////////////////////////////////
		//////////////// TIME CRITICAL START ////////////////////////
		/////////////////////////////////////////////////////////////


		/* poll GDO0 pin state */

		/* Delay long enough for the PA_PD signal to indicate a
		 * successful transmit. This is the 250 XOSC periods
		 * (9.6 us for a 26 MHz crystal) See section 19.6 of 2500 datasheet.
		 * Found out that we need a delay of at least 20 us on CC2500 and
		 * 25 us on CC1100 to see the PA_PD signal change.
		 */

		unsigned long papd_t_start=micros();
		unsigned long papd_t_stop=micros();
		unsigned long papd_t_change=0;
		unsigned long txend_t_start=micros();
		unsigned long txend_t_stop=micros();
		unsigned long txend_t_change=0;

		bool cca_ok=false;
		uint32_t pa_pd_high=0;
		uint32_t pa_pd_low=0;
		for(;;){

			if(digitalRead(_gdo0_pin)){
				pa_pd_high++;
			}
			else {
				pa_pd_low++;
			}

			if(pa_pd_low>0){

				//CCA ok to enter (down->TX start)
				cca_ok=true;
				papd_t_stop=micros();
				papd_t_change=papd_t_stop-papd_t_start;
				txend_t_start=micros();
				while (digitalRead(_gdo0_pin)==0 ){
					txend_t_stop=micros();
					//using abs fix a bug with negative time, TODO verify this.
					txend_t_change=abs((double)txend_t_stop-txend_t_start);
					// average time = 975836 microseconds at 600bps  (1,02 fps)
					// average time = 30497 microseconds at 19200bps (32,79 fps)
					// average time = 15247 microseconds at 38400bps (65,58 fps)
					if(txend_t_change>1000000){
						if(_debug){
							Serial.println("CC1101 \033[1;31mERROR\033[m txend wait timeout");
						}
						break;
					}
				}
				break;
			}

			//exit by timeout and not pin change after 25us
			if(micros()-papd_t_start > 35){
				//CCA fail to enter (stay high-> still in RX)
				break;
			}

		}

		/////////////////////////////////////////////////////////////
		//////////////// TIME CRITICAL STOP ////////////////////////
		/////////////////////////////////////////////////////////////

		if (cca_ok)
		{
			/* PA_PD signal stays LOW while in TX state and goes back to HIGH when the radio transitions to RX state. */
			/* wait for transmit to complete */

			if(_debug){
				Serial.print("CC1101 PA_PD tchange=");
				Serial.print(papd_t_change);
				Serial.println(" microseconds");
				Serial.print("CC1101 \033[1;32mTX CCA send ok\033[m wait_end=");
				Serial.print(txend_t_change);
				Serial.println(" microseconds");
			}

			/* transmit done, break */
			break;
		}
		else
		{

			if(_debug){
				Serial.print("CC1101 CCA pa_pd_high=");
				Serial.print(pa_pd_high);
				Serial.print(" pa_pd_low=");
				Serial.print(pa_pd_low);
				Serial.println("");
			}

			/* Turn off radio and save some power during backoff */
			cc1101_command_strobe( CC1101_SIDLE );
			while (cc1101_command_strobe( CC1101_SNOP ) & 0xF0) ; //if chipstatus not ready wait

			/* flush the receive FIFO of any residual data */
			cc1101_command_strobe( CC1101_SFRX );

			/* Retry ? */
			if (ccaRetries > 0)
			{
				/* calculate random value for backoffs - 1 to 16 */
				uint8_t backoffs = (cc1101_RandomByte() & 0x0F) + 1;
				/* delay for randomly computed number of backoff periods */
				for (uint8_t i=0; i<backoffs; i++)
				{
					delayMicroseconds( sBackoffHelper );
				}
				if(_debug){
					Serial.println("CC1101 \033[1;33mTX CCA backoff\033[m");
				}
				/* decrement CCA retries before loop continues */
				ccaRetries--;
			}
			else
			{
				/* No CCA retries are left, abort */
				cca_end_ok=false;
				break;
			}
		}


	}

	/* Restore GDO_0 to be SYNC signal */
	cc1101_register_write(CC1101_IOCFG0, CC1101_GDO_ASSERT_ON_SYNC_WORD);


	////////////////////////////////////////////////////////////////
	if(!cca_end_ok){
		if(_debug){
			Serial.println("CC1101 ------------ TX FORCE ---------------");
		}
		/* Issue the TX strobe. */
		cc1101_command_strobe( CC1101_STX );

		/* Wait for transmit to complete */
		unsigned long txend_t_start=micros();
		unsigned long txend_t_stop=micros();
		unsigned long txend_t_change=0;
		txend_t_start=micros();
		while (digitalRead(_gdo0_pin)==0 ){
			txend_t_stop=micros();
			txend_t_change=txend_t_stop-txend_t_start;
			// average time = 975836 microseconds at 600bps  (1,02 fps)
			// average time = 30497 microseconds at 19200bps (32,79 fps)
			// average time = 15247 microseconds at 38400bps (65,58 fps)
			if(txend_t_change>1000000){
				if(_debug){
					Serial.println("CC1101 \033[1;31mERROR\033[m txend wait timeout");
				}
				break;
			}
		}

		if(_debug){
			Serial.print("CC1101 TX gdo0 change=");
			Serial.print(txend_t_change);
			Serial.println(" microseconds");
			Serial.println("CC1101 \033[1;31mTX send force\033[m");
		}

	}
	////////////////////////////////////////////////////////////////

	/* clean TX fifo for next packet*/
	cc1101_command_strobe( CC1101_SFTX );

	/* flush the receive FIFO of any residual data */
	cc1101_command_strobe( CC1101_SFRX );
	/* send strobe to enter receive mode */
	cc1101_command_strobe( CC1101_SRX );

	/* enable receive interrupts */
	attachInterrupt(_gdo0_pin, cc1101_packet_recv,FALLING);
}

int8_t cc1101_calculate_rssi(uint8_t rawValue)
{
	int16_t rssi;



	/* The raw value is in 2's complement and in half db steps. Convert it to
	 * decimal taking into account the offset value.
	 */
	if(rawValue >= 128)
	{
		rssi = (int16_t)(rawValue - 256)/2 - RSSI_OFFSET;
	}
	else
	{
		rssi = (rawValue/2) - RSSI_OFFSET;
	}

	/* Restrict this value to least value can be held in an 8 bit signed int */
	if(rssi < -128)
	{
		rssi = -128;
	}

	return rssi;
}



void cc1101_packet_recv(void)
{
	//noInterrupts();


	if(_debug){
		Serial.println("\033[1;33mcc1101_packet_recv\033[m");
	}
	uint8_t rxBytes;

	/*
	 *  Read the RXBYTES register from the radio.
	 *  Bit description of RXBYTES register:
	 *    bit 7     - RXFIFO_OVERFLOW, set if receive overflow occurred
	 *    bits 6:0  - NUM_BYTES, number of bytes in receive FIFO
	 *
	 *  Due a chip bug, the RXBYTES register must read the same value twice
	 *  in a row to guarantee an accurate value.
	 */
	{
		uint8_t rxBytesVerify;
		rxBytesVerify = cc1101_register_read(CC1101_RXBYTES );
		do
		{
			rxBytes = rxBytesVerify;
			rxBytesVerify = cc1101_register_read(CC1101_RXBYTES );
		}
		while (rxBytes != rxBytesVerify);
	}

	//
	if(rxBytes>64){

		if(_debug){
			Serial.println("CC1101 \033[1;31mERROR\033[m RXBYTES>64");
		}

		/* send strobe to enter receive mode */
		cc1101_command_strobe( CC1101_SIDLE );
		/* flush the receive FIFO of any residual data */
		cc1101_command_strobe( CC1101_SFRX );
		/* send strobe to enter receive mode */
		cc1101_command_strobe( CC1101_SRX );

		//goto rx_end;
		return;
	}

	if(rxBytes<4){

		if(_debug){
			Serial.println("CC1101 \033[1;31mERROR\033[m RXBYTES<4");
		}

		/* send strobe to enter receive mode */
		cc1101_command_strobe( CC1101_SIDLE );
		/* flush the receive FIFO of any residual data */
		cc1101_command_strobe( CC1101_SFRX );
		/* send strobe to enter receive mode */
		cc1101_command_strobe( CC1101_SRX );

		//goto rx_end;
		return;
	}

	//copy to mcu
	cc1101_fifo_rx_read(cc1101_rx_buf,rxBytes);

	if(_debug){
		Serial.print("CC1101 RX bytes=");
		Serial.print(rxBytes);
		Serial.print(" buf=");
		for(uint8_t i=0;i<rxBytes;i++){
			Serial.print(cc1101_rx_buf[i],HEX);
			if(i!=rxBytes-1) Serial.print("|");
		}
	}

	uint8_t rssi_raw = cc1101_rx_buf[rxBytes-2];
	uint8_t crc_lqi_raw = cc1101_rx_buf[rxBytes-1];

	uint8_t crc_ok = crc_lqi_raw & BV(7);
	uint8_t lqi= crc_lqi_raw & 0x7f;
	int8_t rssi_dbm = cc1101_calculate_rssi(rssi_raw);

	if(_debug){

		if(rxBytes & BV(7)){
			Serial.print(" \033[1;31mRX-OVERFLOW\033[m");
		}

		if(crc_ok ){
			Serial.print(" \033[1;32mCRC-OK\033[m");
		} else {
			Serial.print(" \033[1;31mCRC-ERROR\033[m");
		}

		Serial.print(" RSSI=");
		Serial.print(rssi_dbm);
		Serial.print(" LQI=");
		Serial.print(lqi);
		Serial.println("");
	}



	if(crc_ok && _func!=NULL){
		_func(cc1101_rx_buf,rxBytes);
	}

	/* send strobe to enter receive mode */
	cc1101_command_strobe( CC1101_SIDLE );
	/* flush the receive FIFO of any residual data */
	cc1101_command_strobe( CC1101_SFRX );
	/* send strobe to enter receive mode */
	cc1101_command_strobe( CC1101_SRX );

}

void cc1101_debug_state(void){

	uint8_t reg;

	//
	reg=cc1101_register_read(CC1101_PARTNUM);
	Serial.print("CC1101_PARTNUM=");
	Serial.println(reg,HEX);

	//
	reg=cc1101_register_read(CC1101_VERSION);
	Serial.print("CC1101_VERSION=");
	Serial.println(reg,HEX);

	//
	reg=cc1101_register_read(CC1101_FREQEST);
	Serial.print("CC1101_FREQEST=");
	Serial.println(reg,HEX);

	//
	reg=cc1101_register_read(CC1101_LQI);
	Serial.print("CC1101_LQI crc_ok=");
	Serial.println(reg & 0x80,HEX);
	Serial.print("CC1101_LQI lqi_est=");
	Serial.println(reg & 0x7f,HEX);

	//
	reg=cc1101_register_read(CC1101_RSSI);
	Serial.print("CC1101_RSSI rssi_dbm=");
	Serial.println(cc1101_calculate_rssi(reg));

	//
	reg=cc1101_register_read(CC1101_MARCSTATE);
	Serial.print("CC1101_MARCSTATE marcstate=");
	Serial.println(cc1101_marcstate(reg));

	//
	reg=cc1101_register_read(CC1101_WORTIME1);
	Serial.print("CC1101_WORTIME1=");
	Serial.println(reg,HEX);

	//
	reg=cc1101_register_read(CC1101_WORTIME0);
	Serial.print("CC1101_WORTIME0=");
	Serial.println(reg,HEX);


	reg=cc1101_register_read(CC1101_PKTSTATUS);

	Serial.print("CC1101_PKTSTATUS CRC_OK=");
	Serial.println(reg & BV(7),HEX);

	Serial.print("CC1101_PKTSTATUS CS=");
	Serial.println(reg & BV(6),HEX);

	Serial.print("CC1101_PKTSTATUS PQT_REACHED=");
	Serial.println(reg & BV(5),HEX);

	Serial.print("CC1101_PKTSTATUS CCA=");
	Serial.println(reg & BV(4),HEX);

	Serial.print("CC1101_PKTSTATUS SFD=");
	Serial.println(reg & BV(3),HEX);

	Serial.print("CC1101_PKTSTATUS GDO2=");
	Serial.println(reg & BV(2),HEX);

	Serial.print("CC1101_PKTSTATUS GDO0=");
	Serial.println(reg & BV(0),HEX);

	reg=cc1101_register_read(CC1101_VCO_VC_DAC);
	Serial.print("CC1101_VCO_VC_DAC=");
	Serial.println(reg,HEX);


	reg=cc1101_register_read(CC1101_TXBYTES);
	Serial.print("CC1101_TXBYTES tx-underflow=");
	Serial.println(reg & 0x80,HEX);
	Serial.print("CC1101_TXBYTES tx-bytes=");
	Serial.println(reg & 0x7f,HEX);


	reg=cc1101_register_read(CC1101_RXBYTES);
	Serial.print("CC1101_RXBYTES rx-overflow=");
	Serial.println(reg & 0x80,HEX);
	Serial.print("CC1101_RXBYTES rx-bytes=");
	Serial.println(reg & 0x7f,HEX);

	reg=cc1101_register_read(CC1101_RCCTRL1_STATUS);
	Serial.print("CC1101_RCCTRL1_STATUS=");
	Serial.println(reg,HEX);

	reg=cc1101_register_read(CC1101_RCCTRL0_STATUS);
	Serial.print("CC1101_RCCTRL0_STATUS=");
	Serial.println(reg,HEX);




}



