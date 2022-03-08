/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "Tim.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */
#define RADIO_THRESH		500

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void RADIO_SPWM_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

volatile uint16_t input;
volatile uint32_t inputHeartbeat;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	GPIO_EnableInput(RADIO_GPIO, RADIO_PIN, GPIO_Pull_None);
	GPIO_OnChange(RADIO_GPIO, RADIO_PIN, GPIO_IT_Both, RADIO_SPWM_IRQ);
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

void RADIO_SPWM_IRQ (void)
{
	uint16_t now = TIM_Read(TIM_RADIO);
	uint16_t pulse = 0;
	static uint16_t tick;

	if (GPIO_Read(RADIO_GPIO, RADIO_PIN))
	{
		tick = now;
	}
	else
	{
		pulse = now - tick;
//		input = pulse;
//		inputHeartbeat = CORE_GetTick();

		// Check pulse is valid
		if (pulse <= (RADIO_MAX + RADIO_THRESH) && pulse >= (RADIO_MIN - RADIO_THRESH))
		{
			input = pulse;
			inputHeartbeat = CORE_GetTick();
		}
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
