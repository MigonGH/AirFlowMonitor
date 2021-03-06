/*
 * utils.h
 *
 *  Created on: 10 gru 2016
 *      Author: Mat
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdbool.h>
#include <stdint.h>

#define TRUE (!0)
#define FALSE (0)
#define RESET (0)
#define SET (1)

#define DISABLED (0)
#define ENABLED (1)
#define SET_PIN(PORT, PIN)    (PORT |= (1 << PIN))		// lights on
#define RESET_PIN(PORT, PIN)  (PORT &= ~(1 << PIN))	   // lights off
#define TOGGLE_PIN(PORT, PIN) (PORT ^= (1 << PIN))    // toggle

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* DEBUG PINS */
#define SET_DEBUG0   SET_PIN(PORTD,PD2)
#define RESET_DEBUG0 RESET_PIN(PORTD,PD2)
#define TOGGLE_DEBUG0 TOGGLE_PIN(PORTD,PD2)
#define SET_DEBUG1   SET_PIN(PORTD,PD3)
#define RESET_DEBUG1 RESET_PIN(PORTD,PD3)
#define TOGGLE_DEBUG1 TOGGLE_PIN(PORTD,PD3)

//typedef unsigned int uint16;
//typedef signed int int16;

#endif /* UTILS_H_ */
