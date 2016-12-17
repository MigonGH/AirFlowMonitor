/*
 * timer.c
 *
 *  Created on: 17 gru 2016
 *      Author: Mat
 */
#include "avr/io.h"
#include "avr/interrupt.h"

void initTimer0()
{
    TCNT0 = 0; /* Init counter to 0 */
    /* Setting prescaler to clk/64 for 8MHz - 2,04ms for 255 MAX */
    TCCR0B = _BV(CS00) | _BV(CS01);
    TIMSK0 |= _BV(OCIE0A); /* Enable Compare A Match interrupt flag */
    OCR0A = 250;
}

ISR(SIG_OUTPUT_COMPARE0A)
{
    TCNT0 = 0; /* reset timer0 counter */
}
