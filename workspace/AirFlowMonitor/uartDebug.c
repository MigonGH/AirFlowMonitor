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
#include <stdio.h>

#define BAUD 19200
#include "util/setbaud.h"

static void USART_cleanScreen();
static void USARTS_putc(char c, FILE *stream);
static void USART_putc( unsigned char data );


FILE USART_output = FDEV_SETUP_STREAM(USARTS_putc, NULL, _FDEV_SETUP_WRITE);

/***************************************
* 		    STATIC FUNCTIONS
***************************************/
static void USARTS_putc(char c, FILE *stream)
{
    if (c == '\n')
    {
        USARTS_putc('\r', stream);
    }
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1 << UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = c;
}
/***************************************
* 		GLOBAL FUNCTIONS
***************************************/
void initUSART()
{
    /* Redirect stdout to USART register */    stdout = &USART_output;

    /*Set baud rate */
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    /* Enable transmitter */
    UCSR0B |= _BV(TXEN0);

    /* Set frame format: 8data, 1 stop */
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
    USART_cleanScreen();
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

static void USART_cleanScreen()
{
	USART_putc(12);
}

static void USART_putc( unsigned char data )
{
    /* Wait for empty transmit buffer */
    while ( !( UCSR0A & (1 << UDRE0)) );
    /* Put data into buffer, sends the data */
    UDR0 = data;
}
