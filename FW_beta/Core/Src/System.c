/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "System.h"
#include "LED.h"
#include "Radio.h"
#include "Motor.h"

/*
 * PRIVATE DEFINITIONS
 */

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

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void)
{
	status = BOOT;
	// Read config
}

void SYSTEM_Update (void)
{
	if (0) // Check for fault conditions
	{
		status = FAULT;
	}
	else if (0) // Check for fault conditions
	{
		status = WARNING;
	}
	else
	{
		status = RUN;
	}
}

Status_t SYSTEM_Status (void)
{
	return status;
}

DriveState_t SYSTEM_RadioToMotor(uint16_t radio)
{
	DriveState_t drive;
	if (radio < RADIO_CENTER)
	{
		drive.reverse = true;
		drive.duty = ((RADIO_CENTER - radio) * 100) / RADIO_HALFSCALE;
	}
	else if (radio > RADIO_CENTER)
	{
		drive.reverse = false;
		drive.duty = ((radio - RADIO_CENTER) * 100) / RADIO_HALFSCALE;
	}
	else
	{
		drive.reverse = false;
		drive.duty = 0;
	}
	return drive;
}

/*
 * PRIVATE FUNCTIONS
 */

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
