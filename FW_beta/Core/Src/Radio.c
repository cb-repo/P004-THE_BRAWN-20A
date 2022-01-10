/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "Tim.h"
#include "LED.h"

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void RADIO_InitSPWM (void);
void RADIO_DeinitSPWM (void);
void RADIO_SPWM_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

uint16_t input;


/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (void)
{
	RADIO_InitSPWM();
}


void RADIO_Deinit (void)
{
	RADIO_DeinitSPWM();
}

void RADIO_Update (void)
{
	if (input > RADIO_CENTER)
	{
		LED_GreenON();
		LED_RedOFF();
	}
	else if (input < RADIO_CENTER)
	{
		LED_GreenOFF();
		LED_RedON();
	}
	else if (input == RADIO_CENTER)
	{
		LED_GreenON();
		LED_RedON();
	}
	else
	{
		LED_GreenOFF();
		LED_RedOFF();
	}
	// Do nothing
}

uint16_t RADIO_GetValue (void)
{
	return input;
}

/*
 * PRIVATE FUNCTIONS
 */

void RADIO_InitSPWM (void)
{
	input = RADIO_CENTER;
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
	GPIO_OnChange(RADIO_GPIO, RADIO_PIN, GPIO_IT_Both, RADIO_SPWM_IRQ);
}

void RADIO_DeinitSPWM (void)
{
	TIM_Stop(TIM_RADIO);
	TIM_Deinit(TIM_RADIO);
	GPIO_OnChange(RADIO_GPIO, RADIO_PIN, GPIO_IT_None, NULL);
}

/*
 * INTERRUPT ROUTINES
 */

void RADIO_SPWM_IRQ (void)
{
	uint32_t now = TIM_Read(TIM_RADIO);
	static uint32_t tick = 0;
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
