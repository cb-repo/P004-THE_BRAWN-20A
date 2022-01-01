/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "IBUS.h"
#include "UART.h"

/*
 * PRIVATE DEFINITIONS
 */

/*
 * PRIVATE TYPES
 */

/*
 * PRIVATE PROTOTYPES
 */

uint16_t IBUS_CalculateCS(uint8_t *);
bool IBUS_VerifyCS(uint8_t *);

/*
 * PRIVATE VARIABLES
 */

static UART_t uart;

/*
 * PUBLIC FUNCTIONS
 */

void IBUS_Init(UART_t u)
{
	uart = u;
	UART_Mode_t mode = UART_Mode_Default;
	UART_Init(&uart, IBUS_BAUD, mode);
}

void IBUS_Deinit(void)
{
	UART_Deinit(&uart);
}

/* CHECKS FOR IBUS TRANSMISSION AND HANDLES READ WHEN REQUIRED
 * Returns:
 * 		- 0 : Transmission success
 * 		- 1 : No transmission present
 * 		- 2 : Transmission detected, waiting for full transmission
 * 		- 3 : Incorrect transmission header
 * 		- 4 : Incorrect transmission command
 * 		- 5 : Incorrect transmission checksum
 */
uint8_t IBUS_Update(uint16_t * data)
{
	uint32_t count = UART_ReadCount(&uart);
	if (count >= IBUS_HEAD_POS)
	{
		if (count >= IBUS_TRANS_LEN)
		{
			uint8_t temp[IBUS_TRANS_LEN] = {0};
			UART_Read(&uart, temp, IBUS_TRANS_LEN);
			UART_ReadFlush(&uart);

			if (temp[IBUS_HEAD_INDEX] == IBUS_HEADER)
			{
				if (temp[IBUS_CMD_INDEX] == IBUS_CMD_SET)
				{
					if (IBUS_VerifyCS(temp))
					{
						for (uint8_t i = 0; i < IBUS_MAX_CH; i++)
						{
							data[i] = (temp[(i*IBUS_BYTE_PER_CH)+IBUS_DATA_INDEX+1] << 8) | (temp[(i*IBUS_BYTE_PER_CH)+IBUS_DATA_INDEX]);
						}
						return 0;
					}
					return 5;
				}
				return 4;
			}
			return 3;
		}
		return 2;
	}
	return 1;
}

bool IBUS_IsValid(void)
{
	return 0;
}

/*
 * PRIVATE FUNCTIONS
 */

uint16_t IBUS_CalculateCS(uint8_t * data)
{
	uint16_t checksum = 0xFFFF;
	for(uint8_t i = 0; i < IBUS_CS_INDEX; i++)
	{
		checksum -= data[i];
	}
	return checksum;
}

bool IBUS_VerifyCS(uint8_t * data)
{
	uint16_t checksum = (data[IBUS_CS_INDEX+1] << 8) | (data[IBUS_CS_INDEX]);
	return (checksum == IBUS_CalculateCS(data));
}

/*
 * INTERRUPT ROUTINES
 */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
