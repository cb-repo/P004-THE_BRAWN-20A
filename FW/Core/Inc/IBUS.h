/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef IBUS_H
#define IBUS_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define IBUS_MAX_CH			14
#define IBUS_BAUD			115200
#define IBUS_START_BITS		1
#define IBUS_DATA_BITS		8
#define IBUS_PARITY			NONE
#define IBUS_STOP_BITS		1

#define IBUS_TRANS_LEN		32
#define IBUS_HEAD_INDEX		0
#define IBUS_HEAD_POS		(IBUS_HEAD_INDEX + 1)
#define IBUS_CMD_INDEX		1
#define IBUS_CMD_POS		(IBUS_CMD_INDEX + 1)
#define IBUS_CS_INDEX		30
#define IBUS_DATA_INDEX 	2
#define IBUS_DATA_POS		(IBUS_DATA_INDEX + 1)
#define IBUS_CD_POS			(IBUS_CMD_POS + 1)
#define IBUS_CS_LEN			2

#define IBUS_HEADER			0x20
#define	IBUS_CMD_SET		0x40	// Command to update servo/motor speed
#define IBUS_BYTE_PER_CH	2

#define IBUS_MIN		1000
#define IBUS_CENTER		1500
#define IBUS_MAX		2000

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void IBUS_Init(UART_t);
void IBUS_Deinit(void);
uint8_t IBUS_Update(uint16_t *);
bool IBUS_IsValid(void);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* IBUS_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
