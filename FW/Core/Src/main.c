/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// TO DO:
//	- Implement input fault timeout

#include "STM32X.h"
#include "Core.h"
#include "Config.h"
#include "LED.h"
#include "Motor.h"
#include "ADC.h"
#include "Radio.h"


/*
 * PRIVATE DEFINITIONS
 */


// System Timeouts, in [ms]
#define BOOT_TIMEOUT			(2*1000)
#define CONFIG_SET_TIMEOUT		100
#define CONFIG_RESET_TIMEOUT	(10*1000)

// System battery levels, in [V]
#define SYSTEM_BATT_WARN		3500
#define SYSTEM_BATT_FAULT		3300
#define SYSTEM_BATT_HYST		100

// System input timeout, in [ms]
#define SYSTEM_INPUT_FAULT 		50


/*
 * PRIVATE TYPES
 */


// State-machine operating states
typedef enum {
	BOOT = 			0x00, // Init system parameters
	CONFIG =		0x01, // Config run parameter
	RUN = 			0x02, // Normal operation
	WARNING = 		0x03, // Warning Flag is set
	FAULT =			0x04 // Fault Flag is set
} Status_t;

// System warning types
typedef struct {
	uint8_t batt;
	uint8_t temp; // Note: temp is legacy warning from THE_BRAINS
} WarningStates_t;

// System fault types
typedef struct {
	uint8_t input;
	uint8_t batt;
	uint8_t temp; // Note: temp is legacy fault from THE_BRIANS
} FaultStates_t;


/*
 * PRIVATE PROTOTYPES
 */


void SYSTEM_ClockConfig(void);
void SYSTEM_CheckForFault(Status_t *, WarningStates_t *, FaultStates_t *);
void SYSTEM_Update(Status_t *, DriveState_t *);


/*
 * PRIVATE VARIABLES
 */

Status_t systemStatus = BOOT;
Config_t systemConfig;
DriveState_t statusDrive = {
		.reverse = 0,
		.duty = 0, };
WarningStates_t statusWarning = {
		.batt = 0,
		.temp = 0, };
FaultStates_t statusFault = {
		.batt = 0,
		.temp = 0,
		.input = 0, };

/*
 * PUBLIC FUNCTIONS
 */

int main (void)
{
//	CORE_Init();
	SYSTEM_ClockConfig();
	uint32_t bootTick = CORE_GetTick();

	// INITIALISE STATE LED
	LED_Init();

	// CHECK FOR ANY FAULTS ON BOOT
	ADC_Init();
	SYSTEM_CheckForFault(&systemStatus, &statusWarning, &statusFault);
	while (systemStatus == FAULT)
	{
		LED_Update(&systemStatus);
		CORE_Idle();
		SYSTEM_CheckForFault(&systemStatus, &statusWarning, &statusFault);
	}
	LED_Update(&systemStatus);


	// CONTINUE BOOT SEQUENCE
//	CONFIG_Init(&systemConfig); 		//  <<------------------------------ Need to implement CONFIG
	MOTOR_Init();
	InputType_t temp_input = SPWM;
	RADIO_Init(temp_input);

	// WAIT FOR CONFIG TIMEOUT
	while (BOOT_TIMEOUT >= CORE_GetTick() - bootTick)
	{
//		if (CONFIG_Set(&systemConfig)) { break; }
		CORE_Idle();
	}
	systemStatus = RUN;

	while (1)
	{
//		SYSTEM_CheckForFault(systemStatus, statusWarning, statusFault);
//		SYSTEM_Update(systemStatus, statusDrive);
//		MOTOR_Update(statusDrive);
//		LED_Update();
		CORE_Idle();
	}
}

/*
 * PRIVATE FUNCTIONS
 */

void SYSTEM_ClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLLMUL_4;
  RCC_OscInitStruct.PLL.PLLDIV = RCC_PLLDIV_2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_LPUART1
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Lpuart1ClockSelection = RCC_LPUART1CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
}

/*
 * CHECKS FOR, AND CLEARS, ANY WARNING/FAULT CONDITION
 */
void SYSTEM_CheckForFault(Status_t * status, WarningStates_t * warning, FaultStates_t * fault)
{
	int32_t SystemVolt = AIN_AinToDivider(ADC_Read(BATTERY_CHANNEL), BATTERY_DET_RLOW, BATTERY_DET_RHIGH);
//	int32_t SystemTemp = 0; //ADC_ReadDieTemp();
	int32_t SystemInput = 0; // Need to update... time between radio comms

	if (!warning->batt) {
		if (SystemVolt < SYSTEM_BATT_WARN) { warning->batt = 1; }
	}
	else {
		if (SystemVolt > SYSTEM_BATT_WARN + SYSTEM_BATT_HYST) { warning->batt = 0; }
	}

	if (!fault->batt) {
		if (SystemVolt < SYSTEM_BATT_FAULT) { fault->batt = 1; }
	}
	else {
		if (SystemVolt > SYSTEM_BATT_FAULT + SYSTEM_BATT_HYST) { fault->batt = 0; }
	}

//	if (!warning->temp) {
//		if (SystemTemp > SYSTEM_TEMP_WARN) { warning->temp = 1; }
//	}
//	else {
//		if (SystemTemp < SYSTEM_TEMP_WARN - SYSTEM_TEMP_HYST) { warning->temp = 0; }
//	}
//
//	if (!fault->temp) {
//		if (SystemTemp > SYSTEM_TEMP_FAULT) { fault->temp = 1; }
//	}
//	else {
//		if (SystemTemp < SYSTEM_TEMP_FAULT - SYSTEM_TEMP_HYST) { fault->temp = 0; }
//	}

	if (!fault->input) {
		if (SystemInput > SYSTEM_INPUT_TIMEOUT) { fault->input = 1; }
	}
	else {
		if (SystemInput < SYSTEM_INPUT_TIMEOUT) { fault->input = 0; }
	}

	if (fault->batt || fault->temp || fault->input) {
		*status = FAULT;
	} else if (warning->batt || warning->temp) {
		*status = WARNING;
	} else {
		*status = RUN;
		CONFIG_CheckForReset();
	}
}

void SYSTEM_Update(Status_t * status, DriveState_t * drive)
{
//	if (status == FAULT)
//	{
//		drive->duty = MOTOR_OFF;
//		drive->reverse = false;
//		break;
//	}
}

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
