/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> //sprintf print u bon tua prae
#include <string.h> //strlen wat kwam yaw string

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char TxDataBuffer[32] = {0};
char RxDataBuffer[32] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void UARTReceiveAndResponsePolling();
int16_t UARTReceiveIT();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
enum _StateDisplay
{
	StateDisplay_Start = 0,
	StateDisplay_MenuRoot_Print =10,
	StateDisplay_MenuRoot_WaitInput,
	StateDisplay_Menu1_Print =100,
	StateDisplay_Menu1_WaitInput,
	StateDisplay_Menu2_Print =200,
	StateDisplay_Menu2_WaitInput
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  uint8_t STATE_Display = 0;
  uint8_t Sclk[2] = {0};
  int16_t delay = 100;
  int16_t freq = 5;
  int8_t mode = 0;

  {
	  char temp[]="HELLO WORLD\r\n";

	  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	 // UARTReceiveAndResponsePolling(); //tua yang tee mai dee
	  HAL_UART_Receive_IT(&huart2, (uint8_t*)RxDataBuffer, 32);
	  //HAL_UART_Receive(&huart2, (uint8_t*)RxDataBuffer, 32, 10);
	  int16_t inputchar = UARTReceiveIT();

	  /*if(inputchar!=-1) //if = -1 = mee kor moon mhai kaw ma
	  	  {
	  		  sprintf(TxDataBuffer,"ReceiveChar: [%c]\r\n",inputchar);
	  		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  	  }*/
	  switch (STATE_Display)
	  {
	  case StateDisplay_Start:
	        STATE_Display = StateDisplay_MenuRoot_Print;
	        break;
	  case StateDisplay_MenuRoot_Print: //display one time state
	  	  {
	  		  char temp[]="************************\r\n Press 0 to enter LED Control\r\n Press 1 to enter Button status\r\n************************\r\n";
	  		  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
	  	  }
	        STATE_Display = StateDisplay_MenuRoot_WaitInput;
	        break;
	  case StateDisplay_MenuRoot_WaitInput:
		  switch(inputchar)
		  {
		  case -1:
			  //No input
			  break;
		  case '0':
	  	  	  {
	  	  		  char temp[]="You press [0]\r\n";
	  	  		  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
	  	  	  }
	  	  	  STATE_Display = StateDisplay_Menu1_Print;
			  break;
		  case '1':
		  	  {
		  	  	  char temp[]="You press [1]\r\n";
		  	  	  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
		  	  }
		  	  STATE_Display = StateDisplay_Menu2_Print;
			  break;
		  default:
			  if(inputchar!=-1) //if = -1 = mee kor moon mhai kaw ma
			  	  {
			  		  sprintf(TxDataBuffer,"You press [%c]\r\n",inputchar);
			  		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
			  		{
			  			char temp[]="Pls press 0 or 1\r\n";
			  			HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  		}

			  	  }
			  inputchar = -1;
		  	  STATE_Display = StateDisplay_MenuRoot_WaitInput;
		  	  break;
		  }
		  break;

	  case StateDisplay_Menu1_Print:
		  	  {
		  		 char temp[]="************************\r\nWelcome to LED control menu\r\n Press a: Speed up 1 Hz \r\n";
		  	  	 HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
		  	  	 char temp2[] = " Press s: Speed down 1 Hz\r\n Press d: On/Off\r\n Press x: Back\r\n************************\r\n";
		  	  	 HAL_UART_Transmit(&huart2, (uint8_t*)temp2,strlen(temp2),10);
		  	  }
		  	  STATE_Display = StateDisplay_Menu1_WaitInput;
		  	  break;

	  case StateDisplay_Menu1_WaitInput:
			  switch(inputchar)
			  {
			  case -1:
			  	  //No input
				  break;
			  case 'a':
				  if(mode==0)
				  {
				  freq+=1;
				  delay=1000/(freq*2);
		  	  	  {
		  	  		  char temp[]="You press [a]\r\nSpeed Up\r\n";
		  	  		  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
		  	  	  }
		  	  	  {
					  sprintf(TxDataBuffer,"Now frequency is [%d] Hz\r\n",freq);
					  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
		  	  	  }
				  sprintf(TxDataBuffer,"delay [%d]\r\n",delay);
				  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
				  }

		  	  	  STATE_Display = StateDisplay_Menu1_WaitInput;
				  break;
			  case 's':
				  if(mode==0)
				  {
			  	  {
			  	  	  char temp[]="You press [s]\r\nSpeed down\r\n";
			  	  	  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  	  }
				  if(freq!=0)
				  {
					  freq-=1;
					  if(freq!=0)
					  {
						  delay=1000/(freq*2);
					  }
				  }
				  if(freq==0)
				  {
					  char temp[]="Can't decrease more frequency\r\n";
					  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
				  }
				  sprintf(TxDataBuffer,"Now frequency is [%d] Hz\r\n",freq);
				  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
				  sprintf(TxDataBuffer,"delay [%d] \r\n",delay);
				  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
				  }

			  	  STATE_Display = StateDisplay_Menu1_WaitInput;
				  break;
			  case 'd':
			  	  {
			  	  	  char temp[]="You press [d]\r\n";
			  	  	  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  	  }
			  	  if(mode==0)
			  	  {
			  		  mode=1;
			  		{
			  			char temp[]="LED Off\r\n";
			  			HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  		}
			  	  }
			  	  else
			  	  {
			  		  mode=0;
				  		{
				  			char temp[]="LED On\r\n";
				  			HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
				  		}
			  	  }
			  	//add something make led con
			  	  STATE_Display = StateDisplay_Menu1_WaitInput;
				  break;
			  case 'x':
			  	  {
			  	  	  char temp[]="You press [x]\r\n";
			  	  	  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  	  }
			  	//add something make led con
			  	  STATE_Display = StateDisplay_MenuRoot_Print;
				  break;
			  default:
				  if(inputchar!=-1) //if = -1 = mee kor moon mhai kaw ma
				  {
				  	sprintf(TxDataBuffer,"You press [%c]\r\nPls press button in menu\r\n",inputchar);
				  	HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);

				  }
				  inputchar = -1;
				  STATE_Display = StateDisplay_Menu1_WaitInput;
			  	  break;
			  }
			  break;

		  case StateDisplay_Menu2_Print:
	  	  	  {
	  	  		  char temp[]="************************\r\nWelcome to Button status menu\r\n Press x: back\r\n************************\r\n";
	  	  		  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
	  	  	  }
	  	  	  STATE_Display = StateDisplay_Menu2_WaitInput;
	  	  break;

		  case StateDisplay_Menu2_WaitInput:
			  switch(inputchar)
			  {
			  case -1:
				  Sclk[0] = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
				  if(Sclk[0]!=Sclk[1])
				  {
					  if(Sclk[0] == SET)
					  {
						  {
						  	char temp[]="Button status: Unpress\r\n";
						  	HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
						  }
					  }
					  else if(Sclk[0]==RESET)
					  {
						  {
						  	char temp[]="Button status: Press\r\n";
						  	HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
						  }
					  }
				  }
				  else
				  {
					  //Do nothing
				  }
				  Sclk[1]=Sclk[0];
				  STATE_Display = StateDisplay_Menu2_WaitInput;
				  break;
			  case 'x':
			  	  {
			  		  char temp[]="You press [x]\r\n";
			  		  HAL_UART_Transmit(&huart2, (uint8_t*)temp,strlen(temp),10);
			  	  }
			  		//add something make led con
			  	  STATE_Display = StateDisplay_MenuRoot_Print;
			  	  break;
			  default:
			  	if(inputchar!=-1) //if = -1 = mee kor moon mhai kaw ma
			  	{
			  		sprintf(TxDataBuffer,"You press [%c]\r\nPls press button in menu\r\n",inputchar);
			  		HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
			  	}
			  	inputchar = -1;
			  	STATE_Display = StateDisplay_Menu2_WaitInput;
			  	break;
			  }



	  }
	  //int16_t inputchar = UARTReceiveIT();
	  /*if(inputchar!=-1) //if = -1 = mee kor moon mhai kaw ma
	  {
		  sprintf(TxDataBuffer,"ReceiveChar: [%c]\r\n",inputchar);
		  HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
	  }*/



	  //simulate work load
	  if(freq!=0&mode ==0)
	  {
		  HAL_Delay(delay);
		  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	  }
	  else if(mode==1)
	  {
		  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void UARTReceiveAndResponsePolling()
{
	char Receive[32]={0};
	HAL_UART_Receive(&huart2, (uint8_t*) Receive, 32, 1000); //tha arn kob thung 32 tua cha return bab mai roh 1000 ms
	sprintf(TxDataBuffer,"Received:[%s]\r\n",Receive);
	HAL_UART_Transmit(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer), 1000);
}

int16_t UARTReceiveIT()
{
	static uint32_t dataPos = 0;
	int16_t data = -1;
	if (huart2.RxXferSize - huart2.RxXferCount != dataPos) //mhee kor moon mhai khau ma
	{
		data = RxDataBuffer[dataPos];
		dataPos = (dataPos + 1)%huart2.RxXferSize;
	}
	return data;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//sprintf(TxDataBuffer,"Received:[%s]\r\n",RxDataBuffer);
	//HAL_UART_Transmit_IT(&huart2, (uint8_t*)TxDataBuffer, strlen(TxDataBuffer));
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
