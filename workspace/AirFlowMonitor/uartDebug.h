/*
 * uartDebug.h
 *
 *  Created on: 12 gru 2016
 *      Author: Mat
 */

#ifndef UARTDEBUG_H_
#define UARTDEBUG_H_
#include "utils.h"

#define UART_BAUD 19200
#define __UBRR ((F_CPU+UART_BAUD*8UL)/(16L*UART_BAUD)-1)

void USART_init(uint16_t UBRR);
void USART_putc( unsigned char data );
void USART_puts(char *s);
void USART_putlong(uint32_t value, uint8_t radix);
void USART_cleanScreen();

#endif /* UARTDEBUG_H_ */
