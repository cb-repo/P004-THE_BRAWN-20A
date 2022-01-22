/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
#include "GPIO.h"
#include "TIM.h"
#include "Core.h"
#include "Radio.h"

/*
 * PRIVATE DEFINITIONS
 */

#define MOTOR_BRAKE		true

/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */

void MOTOR_N1TimerPulseISR (void);
void MOTOR_N1TimerReloadISR (void);
void MOTOR_N2TimerPulseISR (void);
void MOTOR_N2TimerReloadISR (void);
void MOTOR_TimerBlankIRQ (void);

/*
 * PRIVATE VARIABLES
 */


/*
 * PUBLIC FUNCTIONS
 */

void MOTOR_Init (void)
{
	GPIO_EnableOutput(FET_P1_GPIO, FET_P1_PIN, GPIO_PIN_RESET);
	GPIO_EnableOutput(FET_P2_GPIO, FET_P2_PIN, GPIO_PIN_RESET);
	GPIO_EnableOutput(FET_N1_GPIO, FET_N1_PIN, GPIO_PIN_RESET);
	GPIO_EnableOutput(FET_N2_GPIO, FET_N2_PIN, GPIO_PIN_RESET);
}

/*
 * UPDATE THE MOTOR SPEED
 */
void MOTOR_Update (void)
{
	// INIT DRIVE STATUS VARIABLES
	static bool r_prev = false;
	static uint32_t m_prev = MOTOR_OFF;
	bool reverse = false;
	int32_t motor = (input - RADIO_CENTER);

	if (motor < MOTOR_OFF)
	{
		motor = -motor;
		reverse = true;
	}


	if (m_prev == MOTOR_OFF && motor < (MOTOR_OFF_ERROR + MOTOR_OFF_HYST))
	{
		motor = MOTOR_OFF;
	}
	else if (m_prev != MOTOR_OFF && motor < MOTOR_OFF_ERROR)
	{
		motor = MOTOR_OFF;
	}
	else if (motor > (MOTOR_MAX - MOTOR_MAX_ERROR))
	{
		motor = MOTOR_MAX;
	}



	if (motor == m_prev && reverse == r_prev)
	{
		return;
	}

	if (motor == MOTOR_OFF)
	{
		TIM_Deinit(TIM_MOTOR);
		CORE_Delay(1);
		GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
		GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
		GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
		GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
		CORE_Delay(1);
		if (MOTOR_BRAKE)
		{
			GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
			GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
			CORE_Delay(1);
		}
	}
	else if (reverse)
	{
		if (motor == MOTOR_MAX)
		{
			if (r_prev == reverse)
			{
				TIM_Deinit(TIM_MOTOR);
				CORE_Delay(1);
				GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
				GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
				GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
				GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
				CORE_Delay(1);
			}
			else
			{
				TIM_Deinit(TIM_MOTOR);
				CORE_Delay(1);
				GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
				GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
				GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
				GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
				CORE_Delay(1);
				GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
				GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
				CORE_Delay(1);
			}
		}
		else if (m_prev == MOTOR_OFF)
		{
			GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
			GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
			GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
			GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
			CORE_Delay(1);
			GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
			CORE_Delay(1);
			TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
			TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N1TimerPulseISR);
			TIM_OnReload(TIM_MOTOR, MOTOR_N1TimerReloadISR);
			TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
			TIM_Start(TIM_MOTOR);
		}
		else if (m_prev == MOTOR_MAX)
		{
			if (r_prev == reverse)
			{
				GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
				GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
				GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
				CORE_Delay(1);
				TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
				TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N1TimerPulseISR);
				TIM_OnReload(TIM_MOTOR, MOTOR_N1TimerReloadISR);
				TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
				TIM_Start(TIM_MOTOR);
			}
			else
			{
				GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
				GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
				GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
				GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
				CORE_Delay(1);
				GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
				CORE_Delay(1);
				TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
				TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N1TimerPulseISR);
				TIM_OnReload(TIM_MOTOR, MOTOR_N1TimerReloadISR);
				TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
				TIM_Start(TIM_MOTOR);
			}
		}
		else
		{
			TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
		}
	}
	else // !reverse
	{
		if (motor == MOTOR_MAX)
			{
				if (r_prev == reverse)
				{
					TIM_Deinit(TIM_MOTOR);
					CORE_Delay(1);
					GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
					GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
					GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
					GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
					CORE_Delay(1);
				}
				else
				{
					TIM_Deinit(TIM_MOTOR);
					CORE_Delay(1);
					GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
					GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
					GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
					GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
					CORE_Delay(1);
					GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
					GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
					CORE_Delay(1);
				}
			}
			else if (m_prev == MOTOR_OFF)
			{
				GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
				GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
				GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
				GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
				CORE_Delay(1);
				GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
				CORE_Delay(1);
				TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
				TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
				TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
				TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
				TIM_Start(TIM_MOTOR);
			}
			else if (m_prev == MOTOR_MAX)
			{
				if (r_prev == reverse)
				{
					GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
					GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
					GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
					CORE_Delay(1);
					TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
					TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
					TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
					TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
					TIM_Start(TIM_MOTOR);
				}
				else
				{
					GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
					GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
					GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
					GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
					CORE_Delay(1);
					GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
					CORE_Delay(1);
					TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
					TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
					TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
					TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
					TIM_Start(TIM_MOTOR);
				}
			}
			else
			{
				TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, motor);
			}
	}

	// UPDATE STATUS VARIABLES
	m_prev = motor;
	r_prev = reverse;
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

void MOTOR_N2TimerPulseISR (void)
{
	GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
}

void MOTOR_N2TimerReloadISR (void)
{
	GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
}

void MOTOR_N1TimerPulseISR (void)
{
	GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
}

void MOTOR_N1TimerReloadISR (void)
{
	GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
}

void MOTOR_TimerBlankIRQ (void)
{
	// Intentionally left blank
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
