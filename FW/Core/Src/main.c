
#include "main.h"
#include "Board.h"
#include "GPIO.h"
#include "TIM.h"
#include "Radio.h"
#include "System.h"
#include "Motor.h"
#include "Core.h"
#include "UART.h"

int main(void)
{
	CORE_Init();
	SYSTEM_Init();
	RADIO_Init();
	MOTOR_Init();
	while (1)
	{
		SYSTEM_Update();
		MOTOR_Update();
		CORE_Idle();
	}
}
