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

#define BATT_1S_LOW			3300
#define BATT_1S_HIGH		4200
#define BATT_2S_LOW			6600
#define BATT_2S_HIGH		8400
#define BATT_3S_LOW			9900
#define BATT_3S_HIGH		12600
#define BATT_4S_LOW			13200
#define BATT_4S_HIGH		16800

#define SYSTEM_BATT_WARN	600
#define SYSTEM_BATT_FAULT	300
#define SYSTEM_BATT_HYST	200

//#define SYSTEM_TEMP_WARN	80
//#define	SYSTEM_TEMP_FAULT	100
//#define SYSTEM_TEMP_HYST	5

#define SYSTEM_INPUT_TIMEOUT 50

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
	CORE_Delay(10);
	uint32_t volt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);

	if (volt >= BATT_1S_LOW && volt <= BATT_1S_HIGH)
	{
		battery_fault = BATT_1S_LOW + SYSTEM_BATT_FAULT;
		battery_warn = BATT_1S_LOW + SYSTEM_BATT_WARN;
	}
	else if (volt >= BATT_2S_LOW && volt <= BATT_2S_HIGH)
	{
		battery_fault = BATT_2S_LOW + SYSTEM_BATT_FAULT;
		battery_warn = BATT_2S_LOW + SYSTEM_BATT_WARN;
	}
	else if (volt >= BATT_3S_LOW && volt <= BATT_3S_HIGH)
	{
		battery_fault = BATT_3S_LOW + SYSTEM_BATT_FAULT;
		battery_warn = BATT_3S_LOW + SYSTEM_BATT_WARN;
	}
	else if (volt >= BATT_4S_LOW && volt <= BATT_4S_HIGH)
	{
		battery_fault = BATT_4S_LOW + SYSTEM_BATT_FAULT;
		battery_warn = BATT_4S_LOW + SYSTEM_BATT_WARN;
	}
	else
	{
		battery_fault = BATT_1S_LOW + SYSTEM_BATT_FAULT;
		battery_warn = BATT_1S_LOW + SYSTEM_BATT_WARN;
	}

}

void SYSTEM_Update (void)
{
	uint32_t SystemVolt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL),BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
	uint8_t a = (uint8_t)(SystemVolt/100);
	uint32_t SystemInput = CORE_GetTick() - heartbeatRadio;

	if (status.faultBatt == 0) {
		if (SystemVolt < battery_fault) { status.faultBatt = 1; }
	}
	else { //status.faultBatt == 1
		if (SystemVolt > battery_fault) { status.faultBatt = 0; }
	}

	if (status.warnBatt == 0) {
		if (SystemVolt < battery_warn) { status.warnBatt = 1; }
	}
	else { //status.warnBatt == 1
		if (SystemVolt > battery_warn) { status.warnBatt = 0; }
	}

	if (status.faultInput == 0) {
		if (SystemInput > SYSTEM_INPUT_TIMEOUT) { status.faultInput = 1; }
	}
	else { //status.faultInput == 1
		if (SystemInput < SYSTEM_INPUT_TIMEOUT) { status.faultInput = 0; }
	}

	static uint32_t tick;
	uint32_t now = CORE_GetTick();

	if (status.faultBatt)
	{
		if (FAULT_LED_FLASH <= (now - tick))
		{
			if (GPIO_Read(LED_GRN_GPIO, LED_GRN_PIN))
			{
				GPIO_Reset(LED_GRN_GPIO, LED_GRN_PIN);
				GPIO_Reset(LED_RED_GPIO, LED_RED_PIN);
			}
			else
			{
				GPIO_Set(LED_GRN_GPIO, LED_GRN_PIN);
				GPIO_Set(LED_RED_GPIO, LED_RED_PIN);
			}
			tick = now;
		}
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
		GPIO_Set(LED_RED_GPIO, LED_RED_PIN);
		if (WARNING_LED_FLASH <= (now - tick))
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
