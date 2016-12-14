/*
 * adc.h
 *
 *  Created on: 11 gru 2016
 *      Author: Mat
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#define DEBUG_LOGS DISABLED

typedef enum
{
	ADC0 = 0,
	ADC1 = 1,
	ADC2 = 2,
	ADC3 = 3,
	ADC4 = 4,
	ADC5 = 5,
	ADC_MAX_CHANNELS
}adcChannel_t;

void initADC();
void adcCycylic();

uint16_t getAdcRead(adcChannel_t channel);

#endif /* ADC_ADC_H_ */
