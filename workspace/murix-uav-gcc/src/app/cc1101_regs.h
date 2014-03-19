/*
 * CC1101_BOARD_REGS.h
 *
 *  Created on: 07/02/2014
 *      Author: mpontes
 */

#ifndef CC1101_BOARD_REGS_H_
#define CC1101_BOARD_REGS_H_

#define CC1101_DUMMY_BYTE                  0xDB

#define CC1101_WRITE_BYTE                  0x00
#define CC1101_WRITE_BURST                 0x40
#define CC1101_READ_BYTE                   0x80
#define CC1101_READ_BURST                  0xC0


/* configuration registers */
#define CC1101_IOCFG2      0x00      /*  IOCFG2   - GDO2 output pin configuration  */
#define CC1101_IOCFG1      0x01      /*  IOCFG1   - GDO1 output pin configuration  */
#define CC1101_IOCFG0      0x02      /*  IOCFG1   - GDO0 output pin configuration  */
#define CC1101_FIFOTHR     0x03      /*  FIFOTHR  - RX FIFO and TX FIFO thresholds */
#define CC1101_SYNC1       0x04      /*  SYNC1    - Sync word, high byte */
#define CC1101_SYNC0       0x05      /*  SYNC0    - Sync word, low byte */
#define CC1101_PKTLEN      0x06      /*  PKTLEN   - Packet length */
#define CC1101_PKTCTRL1    0x07      /*  PKTCTRL1 - Packet automation control */
#define CC1101_PKTCTRL0    0x08      /*  PKTCTRL0 - Packet automation control */
#define CC1101_ADDR        0x09      /*  ADDR     - Device address */
#define CC1101_CHANNR      0x0A      /*  CHANNR   - Channel number */
#define CC1101_FSCTRL1     0x0B      /*  FSCTRL1  - Frequency synthesizer control */
#define CC1101_FSCTRL0     0x0C      /*  FSCTRL0  - Frequency synthesizer control */
#define CC1101_FREQ2       0x0D      /*  FREQ2    - Frequency control word, high byte */
#define CC1101_FREQ1       0x0E      /*  FREQ1    - Frequency control word, middle byte */
#define CC1101_FREQ0       0x0F      /*  FREQ0    - Frequency control word, low byte */
#define CC1101_MDMCFG4     0x10      /*  MDMCFG4  - Modem configuration */
#define CC1101_MDMCFG3     0x11      /*  MDMCFG3  - Modem configuration */
#define CC1101_MDMCFG2     0x12      /*  MDMCFG2  - Modem configuration */
#define CC1101_MDMCFG1     0x13      /*  MDMCFG1  - Modem configuration */
#define CC1101_MDMCFG0     0x14      /*  MDMCFG0  - Modem configuration */
#define CC1101_DEVIATN     0x15      /*  DEVIATN  - Modem deviation setting */
#define CC1101_MCSM2       0x16      /*  MCSM2    - Main Radio Control State Machine configuration */
#define CC1101_MCSM1       0x17      /*  MCSM1    - Main Radio Control State Machine configuration */
#define CC1101_MCSM0       0x18      /*  MCSM0    - Main Radio Control State Machine configuration */
#define CC1101_FOCCFG      0x19      /*  FOCCFG   - Frequency Offset Compensation configuration */
#define CC1101_BSCFG       0x1A      /*  BSCFG    - Bit Synchronization configuration */
#define CC1101_AGCCTRL2    0x1B      /*  AGCCTRL2 - AGC control */
#define CC1101_AGCCTRL1    0x1C      /*  AGCCTRL1 - AGC control */
#define CC1101_AGCCTRL0    0x1D      /*  AGCCTRL0 - AGC control */
#define CC1101_WOREVT1     0x1E      /*  WOREVT1  - High byte Event0 timeout */
#define CC1101_WOREVT0     0x1F      /*  WOREVT0  - Low byte Event0 timeout */
#define CC1101_WORCTRL     0x20      /*  WORCTRL  - Wake On Radio control */
#define CC1101_FREND1      0x21      /*  FREND1   - Front end RX configuration */
#define CC1101_FREND0      0x22      /*  FREDN0   - Front end TX configuration */
#define CC1101_FSCAL3      0x23      /*  FSCAL3   - Frequency synthesizer calibration */
#define CC1101_FSCAL2      0x24      /*  FSCAL2   - Frequency synthesizer calibration */
#define CC1101_FSCAL1      0x25      /*  FSCAL1   - Frequency synthesizer calibration */
#define CC1101_FSCAL0      0x26      /*  FSCAL0   - Frequency synthesizer calibration */
#define CC1101_RCCTRL1     0x27      /*  RCCTRL1  - RC oscillator configuration */
#define CC1101_RCCTRL0     0x28      /*  RCCTRL0  - RC oscillator configuration */
#define CC1101_FSTEST      0x29      /*  FSTEST   - Frequency synthesizer calibration control */
#define CC1101_PTEST       0x2A      /*  PTEST    - Production test */
#define CC1101_AGCTEST     0x2B      /*  AGCTEST  - AGC test */
#define CC1101_TEST2       0x2C      /*  TEST2    - Various test settings */
#define CC1101_TEST1       0x2D      /*  TEST1    - Various test settings */
#define CC1101_TEST0       0x2E      /*  TEST0    - Various test settings */

/* command strobe registers */
#define CC1101_SRES        0x30      /*  SRES    - Reset chip. */
#define CC1101_SFSTXON     0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define CC1101_SXOFF       0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define CC1101_SCAL        0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define CC1101_SRX         0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define CC1101_STX         0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define CC1101_SIDLE       0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define CC1101_SRSVD       0x37      /*  SRVSD   - Reserved.  Do not use. */
#define CC1101_SWOR        0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define CC1101_SPWD        0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define CC1101_SFRX        0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define CC1101_SFTX        0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define CC1101_SWORRST     0x3C      /*  SWORRST - Reset real time clock. */
#define CC1101_SNOP        0x3D      /*  SNOP    - No operation. Returns status byte. */

/* status registers */
#define CC1101_PARTNUM     0x30      /*  PARTNUM    - Chip ID */
#define CC1101_VERSION     0x31      /*  VERSION    - Chip ID */
#define CC1101_FREQEST     0x32      /*  FREQEST    – Frequency Offset Estimate from demodulator */
#define CC1101_LQI         0x33      /*  LQI        – Demodulator estimate for Link Quality */
#define CC1101_RSSI        0x34      /*  RSSI       – Received signal strength indication */
#define CC1101_MARCSTATE   0x35      /*  MARCSTATE  – Main Radio Control State Machine state */
#define CC1101_WORTIME1    0x36      /*  WORTIME1   – High byte of WOR time */
#define CC1101_WORTIME0    0x37      /*  WORTIME0   – Low byte of WOR time */
#define CC1101_PKTSTATUS   0x38      /*  PKTSTATUS  – Current GDOx status and packet status */
#define CC1101_VCO_VC_DAC  0x39      /*  VCO_VC_DAC – Current setting from PLL calibration module */
#define CC1101_TXBYTES     0x3a      /*  TXBYTES    – Underflow and number of bytes */
#define CC1101_RXBYTES     0x3b      /*  RXBYTES    – Overflow and number of bytes */
#define CC1101_RCCTRL1_STATUS 0x3c   /* RCCTRL1_STATUS – Last RC Oscillator Calibration Result */
#define CC1101_RCCTRL0_STATUS 0x3d   /* RCCTRL0_STATUS – Last RC Oscillator Calibration Result */


/* burst write registers */
#define CC1101_PA_TABLE0   0x3E      /*  PA_TABLE0 - PA control settings table */
#define CC1101_TXFIFO      0x3F      /*  TXFIFO  - Transmit FIFO */
#define CC1101_RXFIFO      0x3F      /*  RXFIFO  - Receive FIFO */







//-----------------------------------------------------------------------------
// CC1101 STATE MACHINE
//------------------------------------------------------------------------------
#define CC1101_MARCSTATE_SLEEP            0x00
#define CC1101_MARCSTATE_IDLE             0x01
#define CC1101_MARCSTATE_XOFF             0x02
#define CC1101_MARCSTATE_VCOON_MC         0x03
#define CC1101_MARCSTATE_REGON_MC         0x04
#define CC1101_MARCSTATE_MANCAL           0x05
#define CC1101_MARCSTATE_VCOON            0x06
#define CC1101_MARCSTATE_REGON            0x07
#define CC1101_MARCSTATE_STARTCAL         0x08
#define CC1101_MARCSTATE_BWBOOST          0x09
#define CC1101_MARCSTATE_FS_LOCK          0x0a
#define CC1101_MARCSTATE_IFADCON          0x0b
#define CC1101_MARCSTATE_ENDCAL           0x0c
#define CC1101_MARCSTATE_RX               0x0d
#define CC1101_MARCSTATE_RX_END           0x0e
#define CC1101_MARCSTATE_RX_RST           0x0f
#define CC1101_MARCSTATE_TXRX_SWITCH      0x10
#define CC1101_MARCSTATE_RXFIFO_OVERFLOW  0x11
#define CC1101_MARCSTATE_FSTXON           0x12
#define CC1101_MARCSTATE_TX               0x13
#define CC1101_MARCSTATE_TX_END           0x14
#define CC1101_MARCSTATE_RXTX_SWITCH      0x15
#define CC1101_MARCSTATE_TXFIFO_UNDERFLOW 0x16

//--------------------------------------------
// CC1101 GDO config
//--------------------------------------------


#define CC1101_GDO_RX_ASSERT_WHEN_FILLED_OR_ABOVE_THRESHOLD                 0x00
#define CC1101_GDO_RX_ASSERT_WHEN_FILLED_OR_ABOVE_THRESHOLD_OR_END_PACKET   0x01
#define CC1101_GDO_TX_ASSERT_WHEN_FILLED_OR_ABOVE_THRESHOLD                 0x02
#define CC1101_GDO_TX_ASSERT_WHEN_FULL                                      0x03
#define CC1101_GDO_RX_ASSERT_ON_OVERFLOW                                    0x04
#define CC1101_GDO_TX_ASSERT_ON_UNDERFLOW                                   0x05
#define CC1101_GDO_ASSERT_ON_SYNC_WORD                                      0x06
#define CC1101_GDO_ASSERT_ON_CRC_OK                                         0x07
#define CC1101_GDO_ASSERT_ON_PQI_ABOVE_PQT                                  0x08
#define CC1101_GDO_CCA_ASSERT_WHEN_RSSI_ABOVE_THRESHOLD                     0x09
#define CC1101_GDO_LOCK_DETECTOR_OUTPUT                                     0x0a
#define CC1101_GDO_SERIAL_CLOCK                                             0x0b
#define CC1101_GDO_SERIAL_SYNCHRONOUS_DATA_OUTPUT                           0x0c
#define CC1101_GDO_SERIA_DATA_OUTPUT                                        0x0d
#define CC1101_GDO_CARRIER_SENSE_HIGH_IF_RSSI_ABOVE_THRESHOLD               0x0e
#define CC1101_GDO_CRC_OK                                                   0x0f
#define CC1101_GDO_RX_HARD_DATA_1                                           0x16
#define CC1101_GDO_RX_HARD_DATA_0                                           0x17
#define CC1101_GDO_PA_PD                                                    0x1b
#define CC1101_GDO_LNA_PD                                                   0x1c
#define CC1101_GDO_RX_SYMBOL_TICK                                           0x1d
#define CC1101_GDO_WOR_1                                                    0x24
#define CC1101_GDO_WOR_0                                                    0x25
#define CC1101_GDO_CLK_32K                                                  0x27
#define CC1101_GDO_CHIP_RDYN                                                0x29
#define CC1101_GDO_XOSC_STABLE                                              0x2b
#define CC1101_GDO_GDO0_Z_EN_N                                              0x2d
#define CC1101_GDO_HIGH_IMPEDANCE                                           0x2e
#define CC1101_GDO_HW_0                                                     0x2f
#define CC1101_GDO_CLK_XOSC1                                                0x30
#define CC1101_GDO_CLK_XOSC15                                               0x31
#define CC1101_GDO_CLK_XOSC2                                                0x32
#define CC1101_GDO_CLK_XOSC3                                                0x33
#define CC1101_GDO_CLK_XOSC4                                                0x34
#define CC1101_GDO_CLK_XOSC6                                                0x35
#define CC1101_GDO_CLK_XOSC8                                                0x36
#define CC1101_GDO_CLK_XOSC12                                               0x37
#define CC1101_GDO_CLK_XOSC16                                               0x38
#define CC1101_GDO_CLK_XOSC24                                               0x39
#define CC1101_GDO_CLK_XOSC32                                               0x3a
#define CC1101_GDO_CLK_XOSC48                                               0x3b
#define CC1101_GDO_CLK_XOSC64                                               0x3c
#define CC1101_GDO_CLK_XOSC96                                               0x3d
#define CC1101_GDO_CLK_XOSC128                                              0x3e
#define CC1101_GDO_CLK_XOSC192                                              0x3f




#endif /* CC1101_BOARD_REGS_H_ */
