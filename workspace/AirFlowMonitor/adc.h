/*
 * adc.h
 *
 *  Created on: 11 gru 2016
 *      Author: Mat
 */

#ifndef ADC_ADC_H_
#define ADC_ADC_H_

#define DEBUG_LOGS ENABLED

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

typedef enum
{
    ADC_NO_ERROR = 0,
    ADC_NOT_INITIALIZED = 1,
    ADC_ERROR = 2
}adcChannelState_t;


void initADC();
void adcCycylic();
void setAdcSleepyConversionFlag();

adcChannelState_t getAdcRead(adcChannel_t channel, uint16_t *adcVal);

#endif /* ADC_ADC_H_ */
