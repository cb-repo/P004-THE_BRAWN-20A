/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef CONFIG_H
#define CONFIG_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define INPUT_SPWM_NUM	1
#define INPUT_PPM_NUM	8
#define INPUT_IBUS_NUM  14
#define INPUT_SBUS_NUM	16
#define INPUT_NUM_MAX	INPUT_SBUS_NUM

/*
 * PUBLIC TYPES
 */

typedef enum {
	UNASSIGNED,
	SPWM,
	PPM,
	IBUS,
	SBUS,
} InputType_t;

typedef enum {
	IP1,  IP2,  IP3,  IP4,
	IP5,  IP6,  IP7,  IP8,
	IP9,  IP10, IP11, IP12,
	IP13, IP14, IP15, IP16,
} InputChannel_t;

typedef struct {
	InputType_t radio;
	InputChannel_t locoCh;
	bool reverseMask;
	uint32_t failsafeMask[INPUT_NUM_MAX];
} Config_t;

/*
 * PUBLIC FUNCTIONS
 */

void CONFIG_Init (Config_t *);
bool CONFIG_Set (Config_t *);
bool CONFIG_CheckForReset (void);

/*
 * EXTERN DECLARATIONS
 */

extern Config_t config;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* CONFIG_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
