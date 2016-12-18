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
#include "stdlib.h"
#include "avr/pgmspace.h"
#include "avr/sleep.h"
#include <util/atomic.h>
#if (ENABLED == DEBUG_LOGS)
#include "uartDebug.h"
#endif


/***************************************
 * 				DEFINES
 **************************************/
#define ADC_CHANNEL_SELECTION_MASK (0x0F)

#define CURRENT_CHANNEL     (adcContainer.currentChannel)
#define _ADC(channel)       (adcContainer.config[channel].adcReadValue)
#define _ADC_MAX(channel)   (adcContainer.config[channel].adcMaxValue)
#define _ADC_MIN(channel)   (adcContainer.config[channel].adcMinValue)
#define _ADC_STATE(channel) (adcContainer.config[channel].adcChannelState)

/***************************************
* 				TYPES
***************************************/
typedef struct
{
	bool adcChannelEnabled;
	uint16_t adcReadValue;
	adcChannelState_t adcChannelState;
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

#if (ENABLED == DEBUG_LOGS)
static adcContainer_t adcContainer =
{
    .currentChannel = ADC0,
    .config = { /*  CONFIG     ADC         STATE          MAX MIN    */
        /*ADC0*/ {  DISABLED,   0,   ADC_NOT_INITIALIZED,  0,  1024   },
        /*ADC1*/ {  DISABLED,   0,   ADC_NOT_INITIALIZED,  0,  1024   },
        /*ADC2*/ {  DISABLED,   0,   ADC_NOT_INITIALIZED,  0,  1024   },
        /*ADC3*/ {  DISABLED,   0,   ADC_NOT_INITIALIZED,  0,  1024   },
        /*ADC4*/ {  DISABLED,   0,   ADC_NOT_INITIALIZED,  0,  1024   },
        /*ADC5*/ {  ENABLED,    0,   ADC_NOT_INITIALIZED,  0,  1024   }
    }
};
#else
static adcContainer_t adcContainer =
{
    .currentChannel = ADC0,
    .config = { /*  CONFIG      ADC    STATE                */
        /*ADC0*/ {  DISABLED,   0,     ADC_NOT_INITIALIZED  },
        /*ADC1*/ {  DISABLED,   0,     ADC_NOT_INITIALIZED  },
        /*ADC2*/ {  ENABLED,    0,     ADC_NOT_INITIALIZED  },
        /*ADC3*/ {  ENABLED,    0,     ADC_NOT_INITIALIZED  },
        /*ADC4*/ {  ENABLED,    0,     ADC_NOT_INITIALIZED  },
        /*ADC5*/ {  ENABLED,    0,     ADC_NOT_INITIALIZED  }
    }
};
#endif

static bool timeToAdcSleep = FALSE;
/***************************************
* 		FUNCTION PROTOTYPES
***************************************/

/***************************************
* 		GLOBAL FUNCTIONS
***************************************/
void initADC()
{
	ADCSRA = RESET;
	ADCSRB = RESET;
	ADMUX = RESET;

	ADCSRA |= (1 << ADEN);     // ADEN=1 enable ADC
	ADCSRA |= (1 << ADPS0);    // set prescaler to 16
	ADCSRA |= (1 << ADPS1);
	ADCSRA |= (1 << ADPS2);
	//ADCSRA |= (1 << ADATE);  //Auto trigger enable, dont need it in adc noise reduction mode tho

    ADCSRA |= (1 << ADIE);     // enable ADC_vect
	//ADCSRB |= (1 << ADTS0);    // Set trigger source Compare Match A, dont need it in adc noise reduction mode tho
	//ADCSRB |= (1 << ADTS1);

	ADMUX |= (1 << REFS0);     // Set Internal 1.1V Voltage Reference with external capacitor at AREF pin
	ADMUX |= (1 << REFS1);

	/* Just look for first enabled channel in adcConfig and set it into ADMUX register */
	for(uint8_t index = ADC0; index < ADC_MAX_CHANNELS ; index++)
	{
		if (adcContainer.config[index].adcChannelEnabled == ENABLED)
		{
			ADMUX = (uint8_t) (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | index;
			adcContainer.currentChannel = index;
			break;
		}
	}
	ADCSRA |= (1 << ADSC);  // Start conversion, dont need it in adc noise reduction mode tho
}

adcChannelState_t getAdcRead(adcChannel_t channel, uint16_t *adcVal)
{
    adcChannelState_t retVal = ADC_ERROR;
	if ((channel < ADC_MAX_CHANNELS) && (channel >= ADC0))
	{
	    *adcVal = _ADC(channel);
	    retVal = _ADC_STATE(channel);
	}
	else
	{
		/* invalid channel request */
		retVal = ADC_ERROR;
	}
	return retVal;
}

void setAdcSleepyConversionFlag()
{
    timeToAdcSleep = TRUE;
}

void adcCycylic ()
{
    if (TRUE == timeToAdcSleep)
    {
        timeToAdcSleep = FALSE;
        set_sleep_mode (SLEEP_MODE_ADC);
        /* To avoid the MCU entering the sleep mode unless it is the programmer’s purpose,
       it is recommended to write the Sleep Enable (SE) bit to one just before the execution of the SLEEP
       instruction and to clear it immediately after waking up */
        sleep_mode();
        /* Enter Sleep Mode To Trigger ADC Measurement
           CPU Will Wake Up From ADC Interrupt */
    }
}

/***************************************
* 		    STATIC FUNCTIONS
***************************************/
static void incrementCurrentChannel()
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
	/* Reached the end of config and didnt find anything. Lets start again from the begginig and search though another "half" */
	for (index = ADC0 ; index < adcContainer.currentChannel ; index++)
	{
		if (adcContainer.config[index].adcChannelEnabled == ENABLED)
		{
			ADMUX = (ADMUX & (~ADC_CHANNEL_SELECTION_MASK)) | index;
			adcContainer.currentChannel = index;
			return;
		}
	}
}
#if (ENABLED == DEBUG_LOGS)
void updateDebugScreen (adcChannel_t channel)
{
	if (channel > ADC0 && channel < ADC_MAX_CHANNELS)
	{
	    /* magic char set to throw around cursor in console */
	    printf("\033[%d;3H", channel);
	    printf("ADC%01d: %04d", channel, _ADC(channel));

	    printf("\033[%d;20H", channel);
	    printf("MIN: %04d", _ADC_MIN(channel));

	    printf("\033[%d;35H", channel);
	    printf("MAX: %04d", _ADC_MAX(channel));

	    printf("\033[%d;50H", channel);
	    printf("Delta: %04d", _ADC_MAX(channel) - _ADC_MIN(channel));
	}
}
#endif

/***************************************
* 				  ISR
***************************************/
static uint16_t averageSum = 0;
static uint8_t index = 0;
ISR(ADC_vect)
{
    /* Collect average of every 16 samples */
    if (index++ < 16)
    {
        averageSum += ADCW;
    }
    else
    {
        /* Save the reading from currently processed channel into a local container */
        _ADC(CURRENT_CHANNEL) = (averageSum >> 4);
        _ADC_STATE(CURRENT_CHANNEL) = ADC_NO_ERROR;
    #if (ENABLED == DEBUG_LOGS)
        _ADC_MAX(CURRENT_CHANNEL) = MAX(_ADC(CURRENT_CHANNEL), _ADC_MAX(CURRENT_CHANNEL));
        _ADC_MIN(CURRENT_CHANNEL) = MIN(_ADC(CURRENT_CHANNEL), _ADC_MIN(CURRENT_CHANNEL));
        updateDebugScreen(CURRENT_CHANNEL);
    #endif
        incrementCurrentChannel();
        index = 1;
        averageSum = ADCW;
    }
}
