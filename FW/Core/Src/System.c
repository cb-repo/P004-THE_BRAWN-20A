/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "System.h"
#include "Core.h"
#include "GPIO.h"
#include "Radio.h"
#include "ADC.h"

#include "UART.h"

/*
 * PRIVATE DEFINITIONS
 */

#define FAULT_LED_FLASH 	500
#define WARNING_LED_FLASH	500

// Battery Voltage Limits
#define BATT_1S_LOW			3300
#define BATT_1S_HIGH		4200
#define BATT_2S_LOW			6600
#define BATT_2S_HIGH		8400
#define BATT_3S_LOW			9900
#define BATT_3S_HIGH		12600
#define BATT_4S_LOW			13200
#define BATT_4S_HIGH		16800
// Battery Warning Above Lower Limit
#define BATT_WARNING		200

#define BATT_HYST			100

#define INPUT_TIMEOUT 		50

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

/*
 * PRIVATE VARIABLES
 */

Status_t status;

uint32_t battery_warn;
uint32_t battery_fault;

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void)
{
	GPIO_EnableOutput(LED_GRN_GPIO, LED_GRN_PIN, false);
	GPIO_EnableOutput(LED_RED_GPIO, LED_RED_PIN, false);

	ADC_Init();
	CORE_Delay(10); // Let stabilize before taking reading
	uint32_t volt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);

	if (volt >= BATT_1S_LOW && volt <= BATT_1S_HIGH)
	{
		battery_fault = BATT_1S_LOW;
		battery_warn = BATT_1S_LOW + BATT_WARNING;
	}
	else if (volt >= BATT_2S_LOW && volt <= BATT_2S_HIGH)
	{
		battery_fault = BATT_2S_LOW;
		battery_warn = BATT_2S_LOW + BATT_WARNING;
	}
	else if (volt >= BATT_3S_LOW && volt <= BATT_3S_HIGH)
	{
		battery_fault = BATT_3S_LOW;
		battery_warn = BATT_3S_LOW + BATT_WARNING;
	}
	else if (volt >= BATT_4S_LOW && volt <= BATT_4S_HIGH)
	{
		battery_fault = BATT_4S_LOW;
		battery_warn = BATT_4S_LOW + BATT_WARNING;
	}
	else
	{
		battery_fault = BATT_1S_LOW;
		battery_warn = BATT_1S_LOW + BATT_WARNING;
	}

}

void SYSTEM_Update (void)
{
	uint32_t SystemVolt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);

	if (status.faultBatt == false) {
		if (SystemVolt <= battery_fault) { status.faultBatt = true; }
	}
	else { //status.faultBatt == true
		if (SystemVolt >= (battery_fault + BATT_HYST)) { status.faultBatt = false; }
	}

	if (status.warnBatt == false) {
		if (SystemVolt <= battery_warn) { status.warnBatt = true; }
	}
	else { //status.warnBatt == true
		if (SystemVolt >= (battery_warn + BATT_HYST)) { status.warnBatt = false; }
	}

	if (INPUT_TIMEOUT <= CORE_GetTick() - inputHeartbeat)
	{
		status.faultInput = true;
	}
	else
	{
		status.faultInput = false;
	}


	static uint32_t tick;
	uint32_t now = CORE_GetTick();

	if (status.faultBatt)
	{
		GPIO_Set(LED_GRN_GPIO, LED_GRN_PIN);
		GPIO_Reset(LED_RED_GPIO, LED_RED_PIN);
	}
	else if (status.faultInput)
	{
		GPIO_Reset(LED_RED_GPIO, LED_RED_PIN);
		if (FAULT_LED_FLASH <= (now - tick))
		{
			if (GPIO_Read(LED_GRN_GPIO, LED_GRN_PIN))
			{
				GPIO_Reset(LED_GRN_GPIO, LED_GRN_PIN);
			}
			else
			{
				GPIO_Set(LED_GRN_GPIO, LED_GRN_PIN);
			}
			tick = now;
		}
	}
	else if (status.warnBatt)
	{
		GPIO_Set(LED_GRN_GPIO, LED_GRN_PIN);
		GPIO_Set(LED_RED_GPIO, LED_RED_PIN);
	}
	else
	{
		GPIO_Reset(LED_GRN_GPIO, LED_GRN_PIN);
		GPIO_Set(LED_RED_GPIO, LED_RED_PIN);
	}
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
