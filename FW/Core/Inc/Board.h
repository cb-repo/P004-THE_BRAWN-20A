/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#ifndef BOARD_H
#define BOARD_H
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#define STM32L0

// ADC CONFIG
#define ADC_VREF	      	3300

// TIM CONFIG
#define TIM_USE_IRQS
// 		Motor O/P
#define TIM2_ENABLE
#define TIM_MOTOR 			TIM_2
#define TIM_MOTOR_AF2		GPIO_AF2_TIM2
#define TIM_MOTOR_AF5		GPIO_AF5_TIM2
#define TIM_MOTOR_CH		0
#define TIM_MOTOR_RELOAD 	1000
#define TIM_MOTOR_FREQ		(MOTOR_FREQ*(TIM_MOTOR_RELOAD+1))
// 		Radio I/P
#define TIM21_ENABLE
#define TIM_RADIO 			TIM_21
#define TIM_RADIO_RELOAD 	0xffff
#define TIM_RADIO_FREQ		1000000


// UART CONFIG
// 		USART1_Tx - Pin30 (PA9)
//		USART1_Rx - Pin31	(PA10)
#define RADIO_UART			USART2
#define RADIO_GPIO			GPIOA
#define RADIO_PINS			(GPIO_PIN_9 | GPIO_PIN_10)
#define RADIO_AF		    GPIO_AF4_USART2
#define UART_BFR_SIZE     	128


// IRQ CONFIG
#define GPIO_USE_IRQS
#define USE_EXTI_3
#define USE_EXTI_8
#define USE_EXTI_9
#define USE_EXTI_10
#define USE_EXTI_15
// 		Calibrate I/P - Pin13 (PA3)
#define CALIBRATE_GPIO		GPIOA
#define	CALIBRATE_PIN		GPIO_PIN_3
// 		RadioS1 - Pin31 (PA10)
#define RADIO_GPIO			GPIOA
#define RADIO_PIN			GPIO_PIN_10


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
//		N1-FET O/P - PinX (PA1)
#define FET_N1_GPIO			GPIOA
#define FET_N1_PIN			GPIO_PIN_1
//		N2-FET O/P - PinX (PA1)
#define FET_N2_GPIO			GPIOA
#define FET_N2_PIN			GPIO_PIN_1
//		P1-FET O/P - PinX (PA1)
#define FET_P1_GPIO			GPIOA
#define FET_P1_PIN			GPIO_PIN_1
//		P2-FET O/P - PinX (PA1)
#define FET_P2_GPIO			GPIOA
#define FET_P2_PIN			GPIO_PIN_1

// IO CONFIG
// 		Battery Detect - Pin14 (PA4)
#define BATTERY_GPIO		GPIOA
#define	BATTERY_PIN			GPIO_PIN_4
#define BATTERY_CHANNEL		ADC_CHANNEL_4
#define BATTERY_DET_RLOW	1800
#define BATTERY_DET_RHIGH	10000
// 		Green LED - Pin3 (PC14)
#define LED_GRN_GPIO		GPIOC
#define LED_GRN_PIN			GPIO_PIN_14
// 		Red LED - Pin10 (PA0)
#define LED_RED_GPIO		GPIOA
#define LED_RED_PIN			GPIO_PIN_0


#endif /* BOARD_H */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
