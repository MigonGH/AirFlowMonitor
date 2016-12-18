/*
 * timer.c
 *
 *  Created on: 17 gru 2016
 *      Author: Mat
 */
#include "avr/io.h"
#include "avr/interrupt.h"
#include "avr/sleep.h"
#include "utils.h"
#include "adc.h"

void initTimer0()
{
    TCNT0 = 0; /* Init counter to 0 */
    /* Setting prescaler to clk/64 for 8MHz - 2,04ms for 255 MAX */
    /* Setting prescaler to clk/1024 for 8MHz - 32.64ms for 255 MAX */
    TCCR0B = _BV(CS00) | _BV(CS02);
    TIMSK0 |= _BV(OCIE0A); /* Enable Compare A Match interrupt flag */
    //IMSK0 |= _BV(TOIE0); /* Overflow interrupt */

    OCR0A = 78; /* ~10ms */
}

#if 0
ISR (TIMER0_COMPA_vect)
{
    TCNT0 = 0; /* reset timer0 counter */
}
#else
ISR(TIMER0_COMPA_vect)
{
    TCNT0 = 0;
    setAdcSleepyConversionFlag();
}
#endif
#if 0
ISR(TIMER0_OVF_vect)
{

    TOGGLE_DEBUG1;
}
#endif
