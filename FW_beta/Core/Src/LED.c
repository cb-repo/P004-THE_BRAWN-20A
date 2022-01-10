/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "LED.h"
#include "Core.h"
#include "GPIO.h"
#include "System.h"

/*
 * PRIVATE DEFINITIONS
 */

#define PULSE100 		100

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void LED_GreenON (void);
void LED_RedON (void);
void LED_GreenOFF (void);
void LED_RedOFF (void);

/*
 * PRIVATE VARIABLES
 */

/*
 * PUBLIC FUNCTIONS
 */


void LED_Init (void)
{
	GPIO_EnableOutput(LED_GRN_GPIO, LED_GRN_PIN, false);
	GPIO_EnableOutput(LED_RED_GPIO, LED_RED_PIN, false);
}

void LED_Update (void)
{
	// Generate the appropriate status variables
	Status_t s = SYSTEM_Status();
	static Status_t s_prev = BOOT;

	// Generate the appropriate timer variables
	uint32_t now = CORE_GetTick();
	static uint32_t tick = 0;

	// Handle LED updates
	switch (s)
	{
	case BOOT:
		LED_GreenOFF();
		LED_RedOFF();
		break;
	case CONFIG:
		LED_GreenON();
		LED_RedOFF();
		break;
	case RUN:
		LED_GreenOFF();
		LED_RedON();
		break;
	case WARNING:
		// Reset tick timer if first loop
		if (s != s_prev) { tick = now; }
		// Handle blinking
		break;
	case FAULT:
		// Reset tick timer if first loop
		if (s != s_prev) { tick = now; }
		// Handle blinking
		break;
	default:
		break;
	}

	// Update previous state variable
	s_prev = s;
}

void LED_Pulse (void)
{
	LED_RedON();
	LED_GreenON();
	CORE_Delay(PULSE100);
	LED_RedOFF();
	LED_GreenOFF();
}

void LED_TriPulse (void)
{
	LED_Pulse();
	CORE_Delay(PULSE100);
	LED_Pulse();
	CORE_Delay(PULSE100);
	LED_Pulse();
}


/*
 * PRIVATE FUNCTIONS
 */


void LED_GreenON (void)
{
	GPIO_Set(LED_GRN_GPIO, LED_GRN_PIN);
}

void LED_RedON (void)
{
	GPIO_Set(LED_RED_GPIO, LED_RED_PIN);
}

void LED_GreenOFF (void)
{
	GPIO_Reset(LED_GRN_GPIO, LED_GRN_PIN);
}

void LED_RedOFF (void)
{
	GPIO_Reset(LED_RED_GPIO, LED_RED_PIN);
}


/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
