/*
 * main.c
 *
 *  Created on: 10 gru 2016
 *      Author: Mat
 */
#include "avr/io.h"
#include "avr/pgmspace.h"
#include "avr/interrupt.h"
//#include <avr/power.h>
#include "util/delay.h"
#include "uartDebug.h"
#include "string.h"
#include "utils.h"
#include "adc.h"
#include <stdio.h>

/* DATA */

/* FUNCTIONS */
static void init()
{
	/* PORT CONFIGURATION */
	DDRB = 0xFF; /* DDR.1 output  */
	DDRC = 0x00; /* DDR.0 - input */
	DDRD = 0xFE;

	//clock_prescale_set(clock_div_1);
    _delay_ms(1000);
	initADC();
	initUSART();
	sei();



}
//static void initTimer()
//{
//	//TCCR0 |= (1 << CS00); // Set up time with no prescaling
//	TCNT0 = 0; //Init data
//}



int main (int argc, char **argv)
{
	init();

	while (TRUE)
	{
		adcCycylic();
		_delay_ms(200); //delay is affected by sei()
	}
	return 0;
}
