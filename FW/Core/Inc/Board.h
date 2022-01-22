/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "STM32X.h"

#define STM32L0

// ADC CONFIG
#define ADC_VREF	      	3300

#define CLK_PLL_DIV 		2

// TIM CONFIG
#define TIM_USE_IRQS
// 		Motor O/P
#define TIM2_ENABLE
#define TIM_MOTOR 			TIM_2
#define TIM_MOTOR_AF2		GPIO_AF2_TIM2
#define TIM_MOTOR_AF5		GPIO_AF5_TIM2
#define TIM_MOTOR_CH		0
#define TIM_MOTOR_RELOAD	500
#define TIM_MOTOR_FREQ		(((TIM_MOTOR_RELOAD + 1) * 1000) / MOTOR_PERIOD_MS) // (MOTOR_FREQ*(TIM_MOTOR_RELOAD+1))
// 		Radio I/P
#define TIM21_ENABLE
#define TIM_RADIO 			TIM_21
#define TIM_RADIO_RELOAD 	0xffff
#define TIM_RADIO_FREQ		1000000


// UART CONFIG
// 		USART1_Tx - Pin30 (PA9)
//		USART1_Rx - Pin31	(PA10)
#define UART2_GPIO			GPIOB
#define UART2_PINS			(GPIO_PIN_6 | GPIO_PIN_7)
#define UART2_AF		    GPIO_AF0_USART2
#define DEBUG_UART			UART_2
#define DEBUG_BAUD			115200
#define DEBUG_MODE			UART_Mode_Default
#define UART_BFR_SIZE     	16


// IRQ CONFIG
#define GPIO_USE_IRQS
#define GPIO_IRQ8_ENABLE
#define GPIO_IRQ15_ENABLE
// 		Calibrate I/P - Pin18 (PA8)
#define CALIBRATE_GPIO		GPIOA
#define	CALIBRATE_PIN		GPIO_PIN_8
// 		Radio I/P - Pin25 (PA15) USART2_Rx
#define RADIO_GPIO			GPIOA
#define RADIO_PIN			GPIO_PIN_15


// DC-MOTOR H-BRIDGE CONFIG
/*
 * VSS --------------
 *  	|			|
 *     	P1			P2
 *     	|			|
 *     	|-- MOTOR --|
 *     	|			|
 *     	N1			N2
 *     	|	`		|
 * VCC --------------
 *
 * Note: P1 and P2 are P-FET, controlled through 2x Additional N-FET on the gate
 */
//		N1-FET O/P - Pin6 (PA0) TIM2_CH1
#define FET_N1_GPIO			GPIOA
#define FET_N1_PIN			GPIO_PIN_0
//		N2-FET O/P - Pin20 (PA10) TIM2 CH3
#define FET_N2_GPIO			GPIOA
#define FET_N2_PIN			GPIO_PIN_10
//		P1-FET O/P - Pin7 (PA1) TIM2_CH2
#define FET_P1_GPIO			GPIOA
#define FET_P1_PIN			GPIO_PIN_1
//		P2-FET O/P - Pin9 (PA3) TIM2 CH4
#define FET_P2_GPIO			GPIOA
#define FET_P2_PIN			GPIO_PIN_3

// IO CONFIG
// 		Battery Detect - Pin14 (PB0)
#define BATTERY_GPIO		GPIOB
#define	BATTERY_PIN			GPIO_PIN_0
#define BATTERY_CHANNEL		ADC_CHANNEL_8
#define BATTERY_DET_RLOW	1800
#define BATTERY_DET_RHIGH	10000
// 		Green LED - Pin3 (PC15)
#define LED_GRN_GPIO		GPIOC
#define LED_GRN_PIN			GPIO_PIN_15
// 		Red LED - Pin22 (PA12)
#define LED_RED_GPIO		GPIOA
#define LED_RED_PIN			GPIO_PIN_12


#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
