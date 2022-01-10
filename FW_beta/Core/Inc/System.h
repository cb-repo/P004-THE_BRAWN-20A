/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SYSTEM_H
#define SYSTEM_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"
#include "Motor.h"

/*
 * PUBLIC DEFINITIONS
 */

// System Operating States
typedef enum {
	BOOT = 			0x00,
	CONFIG =		0x01,
	RUN = 			0x02,
	WARNING = 		0x03,
	FAULT =			0x04,
} Status_t;

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void SYSTEM_Init (void);
void SYSTEM_Update (void);
Status_t SYSTEM_Status (void);
DriveState_t SYSTEM_RadioToMotor (uint16_t);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SYSTEM_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
