/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
#include "GPIO.h"
#include "TIM.h"
#include "Core.h"
#include "Radio.h"
#include "System.h"

/*
 * PRIVATE DEFINITIONS
 */

#define MOTOR_BRAKING		true

/*
 * PRIVATE TYPES
 */


/*
 * PRIVATE PROTOTYPES
 */

void MOTOR_TurnOFF(void);
void MOTOR_UpdatePWM(int32_t);
void MOTOR_DirectionChanged(int32_t, bool);

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
	// PREVIOUS LOOP VARIABLES
	static bool rvs_p = false;
	static bool fwd_p = false;
	static bool stop_p = true;
	static int32_t duty_p = MOTOR_OFF;

	// CURRENT LOOP VARIABLES
	bool rvs = false;
	bool fwd = false;
	bool stop = true;
	bool direction_changed = false;
	int32_t duty = (int32_t)input;
	bool speed_changed = false;

	// CHECK FOR OVERRIDE CONDITIONS
	if (duty == 0 || status.faultInput == true || status.faultBatt == true)
	{
		duty = RADIO_CENTER;
	}

	// TRUNCATE RADIO INPUT TO LIMITS
	if (duty > RADIO_MAX)
	{
		duty = RADIO_MAX;
	}
	else if (duty < RADIO_MIN)
	{
		duty = RADIO_MIN;
	}

	// PROCESS INPUT TO MOTOR PWM
	if (duty >= (RADIO_CENTER - RADIO_HYST) && duty <= (RADIO_CENTER + RADIO_HYST))
	{
		duty = MOTOR_OFF;
		stop = true;
		fwd = false;
		rvs = false;
	}
	else if (duty < (RADIO_CENTER - RADIO_HYST))
	{
		duty = RADIO_CENTER - duty;
		stop = false;
		rvs = true;
		fwd = false;
	}
	else
	{
		duty = duty - RADIO_CENTER;
		stop = false;
		rvs = false;
		fwd = true;
	}


	// SET THE DIRECTION CHANGE FLAG
	if (rvs == rvs_p && fwd == fwd_p)
	{
		direction_changed = false;
	}
	else
	{
		direction_changed = true;
	}

	// SET THE SPEED CHANGE FLAG
	if (duty == duty_p)
	{
		speed_changed = false;
	}
	else
	{
		speed_changed = true;
	}

	// UPDATE THE MOTOR
	if (speed_changed || direction_changed)
	{
		if (duty == MOTOR_OFF)
		{
			MOTOR_TurnOFF();
		}
		else if (speed_changed && !direction_changed)
		{
			MOTOR_UpdatePWM(duty);
		}
		else //(direction_changed)
		{
			MOTOR_DirectionChanged(duty, fwd);
		}
	}

	// UPDATE STATUS VARIABLES
	duty_p = duty;
	fwd_p = fwd;
	rvs_p = rvs;
	stop_p = stop;
}

/*
 * PRIVATE FUNCTIONS
 */

void MOTOR_TurnOFF(void)
{
	TIM_Stop(TIM_MOTOR);
	TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_TimerBlankIRQ);
	TIM_OnReload(TIM_MOTOR, MOTOR_TimerBlankIRQ);
	TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, 0);
	TIM_Deinit(TIM_MOTOR);

	GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
	GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
	GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
	GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
	CORE_Delay(10);

	if (MOTOR_BRAKING)
	{
		GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
		GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
		CORE_Delay(5);
	}
}

void MOTOR_UpdatePWM(int32_t d)
{
	TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, d);
}

void MOTOR_DirectionChanged(int32_t d, bool fwd)
{
	if (fwd) // going forward
	{
		TIM_Stop(TIM_MOTOR);
		TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_TimerBlankIRQ);
		TIM_OnReload(TIM_MOTOR, MOTOR_TimerBlankIRQ);
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, 0);
		TIM_Deinit(TIM_MOTOR);

		GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
		GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
		GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
		GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
		CORE_Delay(10);
		GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
		CORE_Delay(5);

		TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
		TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
		TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, d);
		TIM_Start(TIM_MOTOR);
	}
	else // going reverse
	{
		TIM_Stop(TIM_MOTOR);
		TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_TimerBlankIRQ);
		TIM_OnReload(TIM_MOTOR, MOTOR_TimerBlankIRQ);
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, 0);
		TIM_Deinit(TIM_MOTOR);

		GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
		GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
		GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
		GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
		CORE_Delay(10);
		GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
		CORE_Delay(5);

		TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
		TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N1TimerPulseISR);
		TIM_OnReload(TIM_MOTOR, MOTOR_N1TimerReloadISR);
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, d);
		TIM_Start(TIM_MOTOR);
	}
}

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
