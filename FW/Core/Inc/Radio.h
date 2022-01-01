/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef RADIO_H
#define RADIO_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"
#include "Config.h"

/*
 * PUBLIC DEFINITIONS
 */

#define SPWM_PERIOD		20
#define PPM_PERIOD		20

#define PPM_RESET_THRES	3000

#define RADIO_MIN		1000
#define RADIO_CENTER	1500
#define RADIO_MAX		2000
#define RADIO_FULLSCALE	(RADIO_MAX-RADIO_MIN)
#define RADIO_HALFSCALE	(RADIO_MAX-RADIO_CENTER)

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void RADIO_Init (InputType_t);
void RADIO_Deinit (void);
void RADIO_Update(void);
uint16_t RADIO_GetValue(uint8_t);
bool RADIO_DetInputType (void);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* RADIO_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
