/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef SBUS_H
#define SBUS_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

/*
 * PUBLIC DEFINITIONS
 */

#define SBUS_MAX_CH
#define SBUS_BAUD			100000
#define SBUS_INVERT			1
#define SBUS_START_BITS
#define SBUS_DATA_BITS		8
#define SBUS_PARITY			EVEN
#define SBUS_STOP_BITS		2

#define SBUS_BITS_PER_CH	11

#define SBUS_TRANS_LEN		25
#define SBUS_HEAD_INDEX
#define SBUS_HEAD_POS		(SBUS_HEAD_INDEX + 1)
#define SBUS_CMD_INDEX
#define SBUS_CMD_POS		(SBUS_CMD_INDEX + 1)
#define SBUS_CS_INDEX
#define SBUS_DATA_INDEX
#define SBUS_DATA_POS		(SBUS_DATA_INDEX + 1)
#define SBUS_CD_POS			(SBUS_CMD_POS + 1)
#define SBUS_CS_LEN

#define SBUS_HEADER			0x0F
#define	SBUS_CMD_SET
#define SBUS_BYTE_PER_CH

#define SBUS_MIN		1000
#define SBUS_CENTER		1500
#define SBUS_MAX		2000

/*
 * PUBLIC TYPES
 */

/*
 * PUBLIC FUNCTIONS
 */

void SBUS_Init(UART_t);
void SBUS_Deinit(void);
uint8_t SBUS_Update(uint16_t *);
bool SBUS_IsValid(void);

/*
 * EXTERN DECLARATIONS
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#endif /* SBUS_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
