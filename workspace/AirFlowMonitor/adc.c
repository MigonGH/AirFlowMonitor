/*
 * adc.c
 *
 *  Created on: 11 gru 2016
 *      Author: Mat
 */
#include "avr/io.h"
#include "avr/interrupt.h"
#include "utils.h"
#include "adc.h"
#include "uartDebug.h"
#include "stdlib.h"
#include "avr/pgmspace.h"
/***************************************
 * 				DEFINES
 */
#define ADC_CHANNEL_SELECTION_MASK (0x0F)

/***************************************
 * 				TYPES
 */
typedef struct
{
	bool adcChannelEnabled;
	uint16_t adcReadValue;
}adcConfig_t;

typedef struct
{
	adcChannel_t currentChannel;
	adcConfig_t config[ADC_MAX];
}adcContainer_t;


static adcContainer_t adcContainer =
{
	.currentChannel = ADC0,
	.config = {
		/*ADC0*/ {DISABLED, 0},
		/*ADC1*/ {DISABLED, 0},
		/*ADC2*/ {DISABLED,  0},
		/*ADC3*/ {DISABLED,  0},
		/*ADC4*/ {ENABLED,  0},
		/*ADC5*/ {ENABLED,  0}
	}
};

/***************************************
 * 		FUNCTION PROTOTYPES
 */

static void incrementChannel();


/***************************************
 * 		GLOBAL FUNCTIONS
 */
void initADC()
{
	uint8_t index = ADC0;
	ADCSRA = RESET;
	ADCSRB = RESET;
	ADMUX = RESET;

	ADCSRA |= (1 << ADEN);   // ADEN=1 enable ADC
	ADCSRA &= ~(1 << ADPS0); // set prescaler to 16
	ADCSRA &= ~(1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
	//ADCSRA |= (1 << ADATE);  //Auto trigger enable

    ADCSRA |= (1 << ADIE);   // enable ADC_vect
//	ADCSRB &= ~(1 << ADTS0); // Set trigger source to free running
//	ADCSRB &= ~(1 << ADTS1);
//	ADCSRB &= ~(1 << ADTS2);

	ADMUX |= (1 << REFS0);   // Set Internal 1.1V Voltage Reference with external capacitor at AREF pin
	ADMUX |= (1 << REFS1);

	/* Just look for first enabled channel in adcConfig and set it into ADMUX register */
	for(index = ADC0; index < ADC_MAX ; index++)
	{
		if (adcContainer.config[index].adcChannelEnabled == ENABLED)
		{
			ADMUX = (uint8_t) (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | index;
			adcContainer.currentChannel = index;
			break;
		}
	}
//	if ((ADMUX & ADC_CHANNEL_SELECTION_MASK) == ADC4)
//	{
//		SET_DEBUG0;
//	}
//	ADMUX |= (1 << MUX2);
	ADCSRA |= (1 << ADSC);  // Start conversion
}
#if 0
void setChannel(adcChannel_t channel)
{
	/* Check if channel is supported */
	if ((channel < ADC_MAX) && (channel >= ADC0))
	{
		ADMUX = (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | channel;
		adcContainer.currentChannel = channel;
	}
}
#endif
#if 1
uint16_t getAdcRead(adcChannel_t channel)
{
	uint16_t retVal = 0;
	if ((channel < ADC_MAX) && (channel >= ADC0))
	{
		retVal = adcContainer.config[channel].adcReadValue;
	}
	else
	{
		/* invalid channel request */
		retVal = 0;
	}
	return retVal;
}
#else
uint16_t getAdcRead(adcChannel_t channel)
{
	//ADCSRA |= (1 << ADSC);  // Start conversion
	return ADCW;
}
#endif

static uint16_t debugADCread1 = 0;
static uint16_t debugADCread0 = 0;
void adcCycylic ()
{
	debugADCread0 = getAdcRead(ADC4);
	if ((debugADCread0/100)%2 == 0)
	{
		SET_DEBUG1;
	}
	else
	{
		RESET_DEBUG1;
	}

		debugADCread1 = getAdcRead(ADC5);
		if ((debugADCread1/100)%2 == 0)
		{
			SET_DEBUG0;
		}
		else
		{
			RESET_DEBUG0;
		}
}

/***************************************
 * 		    STATIC FUNCTIONS
 */

static void incrementChannel()
{
	adcChannel_t index = 0;
	/* Switch for next enabled channel */
	for (index = adcContainer.currentChannel + 1 ; index < ADC_MAX ; index++)
	{
		if (adcContainer.config[index].adcChannelEnabled == ENABLED)
		{
			ADMUX = (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | index;
			adcContainer.currentChannel = index;
			return;
		}
	}
	/* Reached the end of config lets start again from the begginig and search though another "half" */
	for (index = ADC0 ; index < adcContainer.currentChannel ; index++)
	{
		if (adcContainer.config[index].adcChannelEnabled == ENABLED)
		{
			ADMUX = (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | index;
			adcContainer.currentChannel = index;
			return;
		}

	}
	//TODO find better way to circle through adc channels
}


/***************************************
 * 				  ISR
 */
ISR(ADC_vect)
{

	debugLog("adcContainer.currentChannel: ", adcContainer.currentChannel);
	debugLog("adcContainer.config[adcContainer.currentChannel].adcReadValue: ", adcContainer.config[adcContainer.currentChannel].adcReadValue);
	debugLog("*****", 0);
	/* Save the reading from currently processed channel into a local container */
	adcContainer.config[adcContainer.currentChannel].adcReadValue = ADCW;
	incrementChannel();
	ADCSRA |= (1 << ADSC);  // Start conversion

}

