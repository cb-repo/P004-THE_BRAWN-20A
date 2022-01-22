/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "Tim.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

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

volatile uint16_t input = RADIO_CENTER;
volatile uint32_t heartbeatRadio;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	heartbeatRadio = CORE_GetTick();
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
	static uint16_t tick;
	heartbeatRadio = CORE_GetTick();
	if (GPIO_Read(RADIO_GPIO, RADIO_PIN))
	{
		tick = now;
	}
	else
	{
		input = now - tick;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
