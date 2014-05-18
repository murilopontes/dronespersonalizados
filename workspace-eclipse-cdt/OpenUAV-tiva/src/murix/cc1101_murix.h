/*
 * CC1101_BOARD.h
 *
 *  Created on: 07/02/2014
 *      Author: mpontes
 */

#ifndef CC1101_BOARD_H_
#define CC1101_BOARD_H_

#include <stdint.h>

typedef void (*cc1101_rx_callback_func)(uint8_t* buf,uint8_t buflen);

///////////////////////////////////////////////////////////////////////////////////////
void cc1101_init(uint8_t cs_pin, uint8_t spi_port,uint8_t gdo0_pin,uint8_t mosi_pin,cc1101_rx_callback_func func);
void cc1101_debug_on(void);
void cc1101_debug_off(void);

///////////////////////////////////////////////////////////////////////////////////////
const char* cc1101_reg_name(uint8_t reg);
const char* cc1101_marcstate(uint8_t state);
const char* cc1101_strobe(uint8_t strobe);
///////////////////////////////////////////////////////////////////////////////////////
uint8_t cc1101_command_strobe(uint8_t addr);
uint8_t cc1101_spi_register_single_byte(uint8_t addrByte, uint8_t mode,uint8_t writeValue);
void    cc1101_spi_register_multi_byte(uint8_t addrByte,uint8_t mode, uint8_t * pData, uint8_t len);
///////////////////////////////////////////////////////////////////////////////////////
uint8_t cc1101_register_read(uint8_t addr);
void    cc1101_register_write(uint8_t addr, uint8_t value);
void    cc1101_fifo_rx_read(uint8_t * pData, uint8_t len);
void    cc1101_fifo_tx_write(uint8_t * pData, uint8_t len);
void    cc1101_patable_read(uint8_t * pData, uint8_t len);
void    cc1101_patable_write(uint8_t * pData, uint8_t len);
///////////////////////////////////////////////////////////////////////////////////////
void cc1101_packet_send(uint8_t *text,uint8_t textlen);
void cc1101_packet_recv(void);
void cc1101_rssi_valid_wait(void);
int8_t cc1101_calculate_rssi(uint8_t rawValue);
void cc1101_debug_state(void);

//////////////////////////////////////////////////////////////////////////////////////



#endif /* CC1101_BOARD_H_ */
