/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef MOTOR_H
#define MOTOR_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define MOTOR_PERIOD_MS		10

#define MOTOR_OFF			0
#define MOTOR_MIN			MOTOR_OFF
#define MOTOR_MAX			TIM_MOTOR_RELOAD

#define MOTOR_OFF_ERROR			50
#define MOTOR_OFF_HYST			10
#define MOTOR_MAX_ERROR			10

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void MOTOR_Init (void);
void MOTOR_Update (void);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* MOTOR_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
