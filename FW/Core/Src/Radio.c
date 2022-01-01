/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Radio.h"
#include "GPIO.h"
#include "Tim.h"
#include "string.h"
#include "UART.h"
#include "IBUS.h"
#include "SBUS.h"
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

void RADIO_InitSPWM (void);
void RADIO_DeinitSPWM (void);
void RADIO_InitPPM (void);
void RADIO_DeinitPPM (void);
bool RADIO_IsValidSPWM(void);
bool RADIO_IsValidPPM(void);

void RADIO_SPWM_IRQ (void);
void RADIO_PPM_IRQ (void);

/*
 * PRIVATE VARIABLES
 */

uint16_t radioInput[INPUT_NUM_MAX];
InputType_t input;

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (InputType_t in)
{
	input = in;
	if (input == PPM) { RADIO_InitPPM(); }
	else if (input == IBUS) { IBUS_Init(RADIO_UART); }
	else if (input == SBUS) { SBUS_Init(RADIO_UART); }
	else { RADIO_InitSPWM(); }
}


void RADIO_Deinit (void)
{
	if (input == PPM) { RADIO_DeinitPPM(); }
	else if (input == IBUS) { IBUS_Deinit(); }
	else if (input== SBUS) { SBUS_Deinit(); }
	else { RADIO_DeinitSPWM(); }
}

void RADIO_Update(void)
{
	switch(input) {
	case UNASSIGNED:
		break; // Do nothing
	case SPWM:
		break; // Handled in interrupts
	case PPM:
		break; // Handled in interrupts
	case IBUS:
		IBUS_Update(radioInput);
		break;
	case SBUS:
		IBUS_Update(radioInput);
		break;
	}
}

uint16_t RADIO_GetValue(uint8_t ip_num)
{
	return radioInput[ip_num];
}

bool RADIO_DetInputType (void)
{
	int8_t in = input;
	CORE_Delay(SPWM_PERIOD);
	switch(in) {
		case SPWM:
			if (RADIO_IsValidSPWM()) { return 1; }
			else { RADIO_DeinitSPWM(); break; }
		case PPM:
			if (RADIO_IsValidPPM()) { return 1; }
			else { RADIO_DeinitPPM(); break; }
		case IBUS:
			if (IBUS_IsValid()) { return 1; }
			else { IBUS_Deinit(); break; }
		case SBUS:
			if (SBUS_IsValid()) { return 1; }
			else { SBUS_Deinit(); break; }
	}


	return 0;
}

/*
 * PRIVATE FUNCTIONS
 */

void RADIO_InitSPWM (void)
{
	memset(radioInput, RADIO_CENTER, sizeof(radioInput));
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

void RADIO_InitPPM (void)
{
	memset(radioInput, RADIO_CENTER, sizeof(radioInput));
	TIM_Init(TIM_RADIO, TIM_RADIO_FREQ, TIM_RADIO_RELOAD);
	TIM_Start(TIM_RADIO);
	GPIO_OnChange(RADIO_GPIO, RADIO_PIN, GPIO_IT_Rising, RADIO_PPM_IRQ);
}

void RADIO_DeinitPPM (void)
{
	TIM_Stop(TIM_RADIO);
	TIM_Deinit(TIM_RADIO);
	GPIO_OnChange(RADIO_GPIO, RADIO_PIN, GPIO_IT_None, NULL);
}

bool RADIO_IsValidSPWM (void)
{
	return 0;
}

bool RADIO_IsValidPPM (void)
{
	return 0;
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
		radioInput[IP1] = now - tick;
	}
}

void RADIO_PPM_IRQ (void)
{
	static uint8_t ch_index = 0;
	static uint32_t tick = 0;
	uint32_t now = TIM_Read(TIM_RADIO);

	if (PPM_RESET_THRES <= (now - tick)) { // Check for PPM schronisation
		ch_index = 0;
	}

	if (ch_index > 0 && ch_index <= INPUT_PPM_NUM)
	{
		radioInput[(ch_index-1)] = now - tick;
	}

	tick = now;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
