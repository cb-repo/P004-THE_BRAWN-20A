/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "Config.h"
#include "EEPROM.h"
#include "Core.h"
#include "string.h"
#include "Motor.h"
#include "GPIO.h"
#include "LED.h"
#include "Radio.h"


/*
 * PRIVATE DEFINITIONS
 */

#define CONFIG_EEPROM_OFFSET 	0

#define CONFIG_SET 				100
#define CONFIG_RESET			(10*1000)
#define CONFIG_TIMEOUT			(10*1000)
#define TRIGGER_TIMEOUT			200
#define SETSUCCESS_TIMEOUT 		(2*1000)
#define RESET_HUMAN_DELAY		(2*1000)
#define RESETSUCCESS_TIMEOUT	(5*1000)

#define RADIO_DET_HYST			200
#define RADIO_DET_P				(RADIO_CENTER + RADIO_DET_HYST)
#define RADIO_DET_N				(RADIO_CENTER - RADIO_DET_HYST)

#define MOTOR_TWITCH_MS			200

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

void CONFIG_Reset(Config_t *);
uint8_t CONFIG_DetRadio(void);
uint8_t CONFIG_DetMode(void);
void CONFIG_Calibrate_IRQ(void);

/*
 * PRIVATE VARIABLES
 */

static bool configFlag;
static bool configSetFlag;
static uint32_t config_tick;

/*
 * PUBLIC FUNCTIONS
 */


/*
 * READS CONFIG FROM EEPROM.
 * IF NO VALID CONFIG IS DETECTED THE CONFIG IS RESTORED TO DEFAULT
 */
void CONFIG_Init(Config_t * config)
{
	EEPROM_Read(CONFIG_EEPROM_OFFSET, &config, sizeof(config));
	if (config->radio < SPWM ||
		config->radio > SBUS ||
		config->locoCh < IP1 ||
		config->locoCh > IP16 ||
		config->reverseMask != true ||
		config->reverseMask != false )
	{
		CONFIG_Reset(config);
	}
	else {
		for (uint8_t i = 0; i < INPUT_NUM_MAX; i++)
		{
			if (config->failsafeMask[i] < RADIO_MIN ||
			    config->failsafeMask[i] > RADIO_MAX)
			{
				CONFIG_Reset(config);
				break;
			}
		}
	}
	GPIO_OnChange(CALIBRATE_GPIO, CALIBRATE_PIN, GPIO_IT_Both, CONFIG_Calibrate_IRQ);
}


bool CONFIG_Set(Config_t * config)
{
	if (configSetFlag)
	{
		RADIO_DetInputType();
		CONFIG_DetMode(&config);
		EEPROM_Write(CONFIG_EEPROM_OFFSET, &systemConfig, sizeof(systemConfig));
		LED_TriPulse();
		CORE_Delay(SETSUCCESS_TIMEOUT);
		return 1;
	}
	return 0;
}


bool CONFIG_CheckForReset(void)
{
	if (configFlag && (CONFIG_RESET <= (CORE_GetTick() - config_tick)))
	{
		CONFIG_Reset();
		return 1;
	}
	return 0;
}


/*
 * PRIVATE FUNCTIONS
 */

/*
 * RESTORES THE CONFIG TO DEFAULT
 */
void CONFIG_Reset(Config_t * config)
{
	Config_t configDefault = {
			.radio = SPWM,
			.locoCh = IP1,
			.reverseMask = false,
	};
	for (uint8_t i = 0; i < INPUT_NUM_MAX; i++)
	{
		configDefault.failsafeMask[i] = RADIO_CENTER;
	}

	LED_TriPulse();
	EEPROM_Write(CONFIG_EEPROM_OFFSET, &configDefault, sizeof(configDefault));
	EEPROM_Read(CONFIG_EEPROM_OFFSET, &config, sizeof(config));
	CORE_Delay(RESET_HUMAN_DELAY);
	LED_TriPulse();
	CORE_Delay(RESETSUCCESS_TIMEOUT);
}

uint8_t CONFIG_DetMode(Config_t * config)
{
	// DETERMINE THE NUMBER OF INPUTS TO CHECK
	switch (config->radio) {
		case SPWM:
			uint8_t ip_num = INPUT_SPWM_NUM;
			break;
		case PPM:
			uint8_t ip_num = INPUT_PPM_NUM;
			break;
		case IBUS:
			uint8_t ip_num = INPUT_IBUS_NUM;
			break;
		case SBUS:
			uint8_t ip_num = INPUT_SBUS_NUM;
			break;
		default:
			uint8_t ip_num = INPUT_SPWM_NUM;
			break;
	}

	// RECORD THE FAILSAFE VALUES
	for (uint8_t ip = 0; ip < ip_num; ip++)
	{
		config->failsafeMask[ip] = radioInput[ip];
	}

	// TWITCH THE DRIVE MOTORS FOR 'MOTOR_TWITCH' [ms]
	DriveState_t stateTwitch = {
			.duty = MOTOR_MAX,
			.reverse = false };
	DriveState_t statsOff = {
				.duty = MOTOR_OFF,
				.reverse = false };
	MOTOR_Update(stateTwitch);
	CORE_Delay(MOTOR_TWITCH_MS);
	MOTOR_Update(statsOff);

	// WAIT FOR USER INPUT FROM RADIO
	uint32_t tick = CORE_GetTick();
	uint32_t now = tick;
	bool trigger = false;
	do
	{
		for (uint8_t ip = 0; ip < ip_num; ip++)
		{
			if (input[ip] >= RADIO_DET_P) {
				config->locoCh = ip;
				config->reverseMask = false;
				trigger = true;
				break;
			} else if (input[ip] <= RADIO_DET_N) {
				config->locoCh = ip;
				config->reverseMask = true;
				trigger = true;
				break;
			}
		}
		CORE_Idle();
		now = CORE_GetTick();
	} while (!trigger && CONFIG_TIMEOUT >= now - tick);
}

/*
 * INTERRUPT ROUTINES
 */


void CONFIG_Calibrate_IRQ (void)
{
	if (GPIO_Read(CALIBRATE_GPIO, CALIBRATE_PIN))
	{
		configFlag = 1;
		config_tick = CORE_GetTick();
	}
	else if (status == BOOT && (CONFIG_SET <= CORE_GetTick() - config_tick))
	{
		configSetFlag = 1;
	}
	else
	{
		configFlag = 0;
	}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
