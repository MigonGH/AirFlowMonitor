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
 **************************************/
#define ADC_CHANNEL_SELECTION_MASK (0x0F)

#define ADC_READ (adcContainer.config[adcContainer.currentChannel].adcReadValue)
#define ADC_MAX  (adcContainer.config[adcContainer.currentChannel].adcMaxValue)
#define ADC_MIN  (adcContainer.config[adcContainer.currentChannel].adcMinValue)

/***************************************
* 				TYPES
***************************************/
typedef struct
{
	bool adcChannelEnabled;
	uint16_t adcReadValue;
#if (ENABLED == DEBUG_LOGS)
	uint16_t adcMaxValue;
	uint16_t adcMinValue;
#endif
}adcConfig_t;

typedef struct
{
	adcChannel_t currentChannel;
	adcConfig_t  config[ADC_MAX_CHANNELS];
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
***************************************/
#if (ENABLED == DEBUG_LOGS)
static void incrementChannel();
static void initDebugScreen();
#endif

/***************************************
* 		GLOBAL FUNCTIONS
***************************************/
void initADC()
{
	uint8_t index = ADC0;
	ADCSRA = RESET;
	ADCSRB = RESET;
	ADMUX = RESET;

	ADCSRA |= (1 << ADEN);   // ADEN=1 enable ADC
	ADCSRA |= (1 << ADPS0); // set prescaler to 16
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
	//ADCSRA |= (1 << ADATE);  //Auto trigger enable

    ADCSRA |= (1 << ADIE);   // enable ADC_vect
//	ADCSRB &= ~(1 << ADTS0); // Set trigger source to free running
//	ADCSRB &= ~(1 << ADTS1);
//	ADCSRB &= ~(1 << ADTS2);

	ADMUX |= (1 << REFS0);   // Set Internal 1.1V Voltage Reference with external capacitor at AREF pin
	ADMUX |= (1 << REFS1);

	/* Just look for first enabled channel in adcConfig and set it into ADMUX register */
	for(index = ADC0; index < ADC_MAX_CHANNELS ; index++)
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
#if (ENABLED == DEBUG_LOGS)
	initDebugScreen();
#endif
}

uint16_t getAdcRead(adcChannel_t channel)
{
	uint16_t retVal = 0;
	if ((channel < ADC_MAX_CHANNELS) && (channel >= ADC0))
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

void adcCycylic ()
{
	/* Nothing to do? */
}

/***************************************
* 		    STATIC FUNCTIONS
***************************************/
static void incrementChannel()
{
	adcChannel_t index = 0;
	/* Switch for next enabled channel */
	for (index = adcContainer.currentChannel + 1 ; index < ADC_MAX_CHANNELS ; index++)
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
#if (ENABLED == DEBUG_LOGS)
static void initDebugScreen()
{
	int eachEnabledChannel = 0;
	for ( ; eachEnabledChannel < ADC_MAX_CHANNELS ; eachEnabledChannel++)
	{
		if (ENABLED == adcContainer.config[eachEnabledChannel].adcChannelEnabled)
		{
			adcContainer.config[eachEnabledChannel].adcMinValue = 1024; //TODO need to disapear
			USART_puts("adcReadValue:   \r\n");
		}
	}
}
void updateDebugScreen (adcChannel_t channel)
{
	if (channel > ADC0 && channel < ADC_MAX_CHANNELS)
	{
		char temp[20];
		itoa(channel, temp, 10);
		{
			USART_puts("\033[");
			USART_puts(temp);
			USART_puts(";3H");
			USART_puts(" ADC: ");
			USART_putlong(ADC_READ, 10);
			USART_puts("\033[");
			USART_puts(temp);
			USART_puts(";20H");
			USART_puts(" Min: ");
			USART_putlong(ADC_MIN, 10);
			USART_puts("\033[");
			USART_puts(temp);
			USART_puts(";35H");
			USART_puts(" Max: ");
			USART_putlong(ADC_MAX, 10);
			USART_puts("\033[");
			USART_puts(temp);
			USART_puts(";50H");
			USART_puts("  Delta: ");
			USART_putlong(ADC_MAX - ADC_MIN, 10);
		}
	}
}
#endif

/***************************************
* 				  ISR
***************************************/
ISR(ADC_vect)
{
#if (ENABLED == DEBUG_LOGS)
	ADC_MAX = MAX(ADC_READ, ADC_MAX);
	ADC_MIN = MIN(ADC_READ, ADC_MIN);
	updateDebugScreen(adcContainer.currentChannel);
#endif

	/* Save the reading from currently processed channel into a local container */
	ADC_READ = ADCW;
	incrementChannel();
	ADCSRA |= (1 << ADSC);  // Start conversion
}

