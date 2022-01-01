/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Motor.h"
#include "GPIO.h"
#include "TIM.h"
#include "Core.h"

/*
 * PRIVATE DEFINITIONS
 */

#define MOTOR_BRAKE			1

#define OFF_DELAY_PFET_US	100 // OFF in 50us
#define ON_DELAY_PFET_US	1 // ON in 10ns
#define OFF_DELAY_NFET_US	100 // OFF in 50us
#define ON_DELAY_NFET_US	1 // ON in 100ns

#define OFF_DELAY_PFET_MS	1
#define ON_DELAY_PFET_MS	1
#define OFF_DELAY_NFET_MS	1
#define ON_DELAY_NFET_MS	1


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

	TIM_Init(TIM_MOTOR, TIM_MOTOR_FREQ, TIM_MOTOR_RELOAD);
	TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
	TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
	TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_OFF);
	TIM_Start(TIM_MOTOR);
}

void MOTOR_Deinit (void)
{
	TIM_Stop(TIM_MOTOR);

	GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
	GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
	GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
	GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
	CORE_Delay(OFF_DELAY_PFET_MS);
	GPIO_Deinit(FET_N1_GPIO, FET_N1_PIN);
	GPIO_Deinit(FET_N2_GPIO, FET_N2_PIN);
	GPIO_Deinit(FET_P1_GPIO, FET_P1_PIN);
	GPIO_Deinit(FET_P2_GPIO, FET_P2_PIN);

	TIM_Deinit(TIM_MOTOR);
}

/*
 * UPDATE THE MOTOR SPEED
 * Input:
 * 		- ip.duty : Motor duty-cycle in deci-percent (0-1000).
 * 		- ip.reverse : True if motor is driving forward, else false.
 * Return:
 * 		- 0 : The motor speed is up-to-date.
 * 		- 1 : Failed to update the motor speed.
 */
uint8_t MOTOR_Update (DriveState_t * ip)
{
	// INIT DRIVE STATUS VARIABLES
	static bool r_prev = false;
	static uint32_t d_prev = 0;

	// CLAMP THE INPUT DUTY CYCLE
	if (ip->duty > MOTOR_MAX) {
		ip->duty = MOTOR_MAX;
	}
	else if (ip->duty < MOTOR_MIN) {
		ip->duty = MOTOR_MIN;
	}

	// CHECK IF INPUTS HAVE CHANGED
	if (d_prev == ip->duty && r_prev == ip->reverse) {
		return 0;
	}

	// CHECK FOR CHANGE OF DIRECTION AND UPDATE THE MOTOR SPEED
	if (ip->duty == MOTOR_OFF) // check if motor is OFF
	{
		if (MOTOR_BRAKE)
		{
			TIM_Stop(TIM_MOTOR);
			GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
			GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
			CORE_Delay(OFF_DELAY_PFET_MS);
			GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
			GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
			TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_TimerBlankIRQ);
			TIM_OnReload(TIM_MOTOR, MOTOR_TimerBlankIRQ);
			TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_OFF);
			TIM_Start(TIM_MOTOR);
		}
		else // MOTOR COAST
		{
			TIM_Stop(TIM_MOTOR);
			GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
			GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
			GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
			GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
			CORE_Delay(OFF_DELAY_PFET_MS);
			TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_TimerBlankIRQ);
			TIM_OnReload(TIM_MOTOR, MOTOR_TimerBlankIRQ);
			TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_OFF);
			TIM_Start(TIM_MOTOR);
		}
	}
	else if (r_prev == ip->reverse && d_prev != MOTOR_OFF) // check is only updating dity cycle
	{
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, ip->duty);
	}
	else // motor control is changing run states
	{
		TIM_Stop(TIM_MOTOR);
		GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
		GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
		GPIO_Reset(FET_P1_GPIO, FET_P1_PIN);
		GPIO_Reset(FET_P2_GPIO, FET_P2_PIN);
		CORE_Delay(OFF_DELAY_PFET_MS);
		if (ip->reverse)
		{
			GPIO_Set(FET_P2_GPIO, FET_P2_PIN);
			TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N1TimerPulseISR);
			TIM_OnReload(TIM_MOTOR, MOTOR_N1TimerReloadISR);
		}
		else // !ip->reverse
		{
			GPIO_Set(FET_P1_GPIO, FET_P1_PIN);
			TIM_OnPulse(TIM_MOTOR, TIM_MOTOR_CH, MOTOR_N2TimerPulseISR);
			TIM_OnReload(TIM_MOTOR, MOTOR_N2TimerReloadISR);
		}
		TIM_SetPulse(TIM_MOTOR, TIM_MOTOR_CH, ip->duty);
		TIM_Start(TIM_MOTOR);
	}

	// UPDATE STATUS VARIABLES
	d_prev = ip->duty;
	r_prev = ip->reverse;

	return 0;
}

/*
 * PRIVATE FUNCTIONS
 */


/*
 * INTERRUPT ROUTINES
 */

void MOTOR_N2TimerPulseISR (void)
{
	GPIO_Set(FET_N2_GPIO, FET_N2_PIN);
}

void MOTOR_N2TimerReloadISR (void)
{
	GPIO_Reset(FET_N2_GPIO, FET_N2_PIN);
}

void MOTOR_N1TimerPulseISR (void)
{
	GPIO_Set(FET_N1_GPIO, FET_N1_PIN);
}

void MOTOR_N1TimerReloadISR (void)
{
	GPIO_Reset(FET_N1_GPIO, FET_N1_PIN);
}

void MOTOR_TimerBlankIRQ (void)
{
	// Intentionally left blank
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
