/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef MOTOR_H
#define MOTOR_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define MOTOR_FREQ			(10*1000) 	// 20kHz
#define MOTOR_OFF			0
#define MOTOR_MIN			1
#define MOTOR_MAX			1000 // 100% in deci-Percent

/*
 * PUBLIC TYPES
 */

typedef struct {
	bool reverse;
	uint32_t duty;
} DriveState_t;

/*
 * PUBLIC FUNCTIONS
 */

void MOTOR_Init (void);
void MOTOR_Deinit (void);
uint8_t MOTOR_Update (DriveState_t *);


/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* MOTOR_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
