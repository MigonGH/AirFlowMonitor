/*
 * uartDebug.h
 *
 *  Created on: 12 gru 2016
 *      Author: Mat
 */

#ifndef UARTDEBUG_H_
#define UARTDEBUG_H_

#include "utils.h"
#include <stdio.h>

void initUSART();
void USART_putlong(uint32_t value, uint8_t radix);
void USART_puts(char *s);
void USART_putc( unsigned char data );

#endif /* UARTDEBUG_H_ */
