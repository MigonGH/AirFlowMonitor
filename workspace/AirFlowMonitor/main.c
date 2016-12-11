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
#include "string.h"
#include "utils.h"
#include "main.h"

/* DATA */
static volatile uint16 debugADCread = 0;

/* FUNCTIONS */
static void init()
{
	DDRB = 0xFF; /* DDR.1 output  */
	DDRC = 0x00; /* DDR.0 - input */
	DDRD = 0xFE;
	//EMPTY_INTERRUPT(ADC_vect);
	//initADC();
	//clock_prescale_set(clock_div_1);
	sei();
	initADC();
}
static void initTimer()
{

}

}
static void initADC()
{
	ADCSRA = RESET;
	ADCSRB = RESET;

	ADCSRB &= ~(1 << ADTS0); // Set trigger source to Timer/Counter0 Overflow
	ADCSRB &= ~(1 << ADTS1);
	ADCSRB |= (1 << ADTS2);

	ADMUX |= (1 << REFS0); // Set Internal 1.1V Voltage Reference with external capacitor at AREF pin
	ADMUX |= (1 << REFS1);
	ADMUX &= ~(1 << MUX0); // Set Analog Channel ADC4
	ADMUX &= ~(1 << MUX1);
	ADMUX |=  (1 << MUX2);
	ADMUX &= ~(1 << MUX3);

	ADCSRA |= (1 << ADPS0); // set prescaler to 128
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
    ADCSRA |= (1 << ADIE);  // enable ADC_vect
	ADCSRA |= (1 << ADEN);  // ADEN=1 enable ADC
	ADCSRA |= (1 << ADSC);  // Start convcersion
}

ISR(ADC_vect)
{
	/* ADCL must be read first (freezes both data registers) */
	/* ADCH must be read second (free both data registers)   */
	debugADCread = ADLAR;
	TOGGLE_PIN(PORTC,PC5);

}

int main (int argc, char **argv)
{
	init();
	while (TRUE)
	{
		TOGGLE_DEBUG0;
		_delay_ms(500);
	}
	return 0;
}
