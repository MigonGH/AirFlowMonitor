/*
 * uartDebug.c
 *
 *  Created on: 12 gru 2016
 *      Author: Mat
 */
#include "uartDebug.h"
#include "avr/io.h"
#include "utils.h"
#include "stdlib.h"


void USART_init(uint16_t UBRR)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(UBRR>>8);
	UBRR0L = (unsigned char)UBRR;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop */
}

void USART_putc( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1 << UDRE0)) );
/* Put data into buffer, sends the data */
UDR0 = data;
}

void USART_puts(char *s)
{
	while (*s) USART_putc (*s++);
}

void USART_putlong(uint32_t value, uint8_t radix)
{
	char buf[20];
	itoa(value, buf, radix);
	USART_puts(buf);
}

void USART_cleanScreen()
{
	USART_putc(12);
}
