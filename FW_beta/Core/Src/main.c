
#include "Core.h"
#include "System.h"
#include "LED.h"
#include "Radio.h"
#include "Motor.h"
#include "Tim.h"

// To Do:
// - Update LED fault/warning blinking

void SystemClock_Config(void);

int main(void)
{
	CORE_Init();
	SYSTEM_Init();
	LED_Init();
	RADIO_Init();
//	MOTOR_Init();
	uint16_t tick = TIM_Read(TIM_RADIO);
	while(1)
	{
		SYSTEM_Update();
//		LED_Update();

//		RADIO_Update();
//		MOTOR_Update(SYSTEM_RadioToMotor(RADIO_GetValue()));

		CORE_Idle();
	}
}

//#include "Board.h"
//#include "Core.h"
//
//ADC_HandleTypeDef hadc;
//
//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_ADC_Init(void);
//
//int main(void)
//{
////	CORE_Init();
//	HAL_Init();
//	SystemClock_Config();
//	MX_GPIO_Init();
//	MX_ADC_Init();
//	HAL_GPIO_WritePin(LED_GRN_GPIO, LED_GRN_PIN, GPIO_PIN_SET);
////  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
//	while (1) { }
//
//
//}
//
//void SystemClock_Config(void)
//{
//  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//
//  /** Configure the main internal regulator output voltage
//  */
//  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
//  /** Initializes the RCC Oscillators according to the specified parameters
//  * in the RCC_OscInitTypeDef structure.
//  */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
//  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
//  RCC_OscInitStruct.MSICalibrationValue = 0;
//  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_5;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Initializes the CPU, AHB and APB buses clocks
//  */
//  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
//
//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }
//}
//
//static void MX_ADC_Init(void)
//{
//
//  /* USER CODE BEGIN ADC_Init 0 */
//
//  /* USER CODE END ADC_Init 0 */
//
//  ADC_ChannelConfTypeDef sConfig = {0};
//
//  /* USER CODE BEGIN ADC_Init 1 */
//
//  /* USER CODE END ADC_Init 1 */
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
//  */
//  hadc.Instance = ADC1;
//  hadc.Init.OversamplingMode = DISABLE;
//  hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV1;
//  hadc.Init.Resolution = ADC_RESOLUTION_12B;
//  hadc.Init.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
//  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
//  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc.Init.ContinuousConvMode = DISABLE;
//  hadc.Init.DiscontinuousConvMode = DISABLE;
//  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc.Init.DMAContinuousRequests = DISABLE;
//  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
//  hadc.Init.LowPowerAutoWait = DISABLE;
//  hadc.Init.LowPowerFrequencyMode = ENABLE;
//  hadc.Init.LowPowerAutoPowerOff = DISABLE;
//  if (HAL_ADC_Init(&hadc) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /** Configure for the selected ADC regular channel to be converted.
//  */
//  sConfig.Channel = ADC_CHANNEL_8;
//  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
//  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
//  {
//    Error_Handler();
//  }
//  /* USER CODE BEGIN ADC_Init 2 */
//
//  /* USER CODE END ADC_Init 2 */
//
//}
//
//static void MX_GPIO_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  /* GPIO Ports Clock Enable */
//  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOA_CLK_ENABLE();
//  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin Output Level */
//  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
//
//  /*Configure GPIO pin : PC14 */
//  GPIO_InitStruct.Pin = GPIO_PIN_14;
//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PC15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PA0 PA1 PA3 */
//  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_3;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PA2 PA4 PA5 PA6
//                           PA7 PA9 PA11 PA13
//                           PA14 */
//  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
//                          |GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13
//                          |GPIO_PIN_14;
//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PB1 PB3 PB4 PB5
//                           PB6 PB7 */
//  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
//                          |GPIO_PIN_6|GPIO_PIN_7;
//  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  /*Configure GPIO pins : PA8 PA15 */
//  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_15;
//  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PA10 */
//  GPIO_InitStruct.Pin = GPIO_PIN_10;
//  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  GPIO_InitStruct.Alternate = GPIO_AF5_TIM2;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//  /*Configure GPIO pin : PA12 */
//  GPIO_InitStruct.Pin = GPIO_PIN_12;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//}
//
//void Error_Handler(void)
//{
//
//  __disable_irq();
//  while (1)
//  {
//  }
//  /* USER CODE END Error_Handler_Debug */
//}
//
//
///*****END OF FILE****/
