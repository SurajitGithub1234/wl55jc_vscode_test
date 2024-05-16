/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "main.h"
#include "cmsis_os.h"
#include "FreeRTOS_Var.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define TASK_INSTANCE_SINGLE 1
#define SENDER_TASK_STACK_SIZE (128 * 2)
#define RECIEVER_TASK_STACK_SIZE (128 * 2)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
// osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
// void StartDefaultTask(void const * argument);
void fnRTOSMessageSendTask(void const *argument);
void fnRTOSMessageReceiverTask(void const *argument);
void fnCMSIS1_Thread_Config_Assert(osThreadId osThreadHandler);

// Configure CMSIS RTOS 1 Task: fnRTOSMessageSendTask
osThreadId SenderTaskID;
osThreadDef(CMSIS1_Sender_Task, fnRTOSMessageSendTask, osPriorityNormal, TASK_INSTANCE_SINGLE, SENDER_TASK_STACK_SIZE);

// Config CMSIS RTOS 1 Task: fnRTOSMessageReceiverTask
osThreadId ReceiverTaskID;
osThreadDef(CMSIS1_Receiver_Task, fnRTOSMessageReceiverTask, osPriorityAboveNormal, TASK_INSTANCE_SINGLE, RECIEVER_TASK_STACK_SIZE);

// Create a message queue to pass structure pointer from one ptr to another.
osMessageQDef(EM_Data_Queue, EM_DATA_QUEUE_SIZE, pStructEMData);
osMessageQId(EM_Data_Queue_Id);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  // osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);

  SenderTaskID = osThreadCreate(osThread(CMSIS1_Sender_Task), NULL);
  if (SenderTaskID == NULL)
  {
    printf("Task could not be creared sucessfully!!");
    Error_Handler();
  }

  ReceiverTaskID = osThreadCreate(osThread(CMSIS1_Receiver_Task), NULL);
  fnCMSIS1_Thread_Config_Assert(ReceiverTaskID);

  // create a message queue to store the energy monitor data and pass from one thread to other
  osThreadId osMessageQueueSendThreadID = NULL;
  EM_Data_Queue_Id = osMessageCreate(osMessageQ(EM_Data_Queue), osMessageQueueSendThreadID);
  if (EM_Data_Queue_Id == NULL)
  {
    printf("Queue could not be created!!\r\n");
    Error_Handler();
  }
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK3 | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK3Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pins : T_VCP_RX_Pin T_VCP_RXA2_Pin */
  GPIO_InitStruct.Pin = T_VCP_RX_Pin | T_VCP_RXA2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_15 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
// void StartDefaultTask(void const *argument)
// {
//   /* USER CODE BEGIN 5 */
//   /* Infinite loop */
//   for (;;)
//   {
//     osDelay(1);
//   }
//   /* USER CODE END 5 */
// }

/**
 * @brief  Function implementing the SenderTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void fnRTOSMessageSendTask(void const *argument)
{

  EnergyData_t StructConfigEnergyData;
  StructConfigEnergyData.fKVAh_pha = 111.23;
  StructConfigEnergyData.fKVArh_pha = 222.23;
  StructConfigEnergyData.fKWh_pha = 333.23;

  PowerData_t StructConfigPowerData;
  StructConfigPowerData.fKW_pha = 11.22;
  StructConfigPowerData.fVA_pha = 22.22;
  StructConfigPowerData.fVAr_pha = 33.22;

  PhaseAngle_t StructConfigPhaseAngle;
  StructConfigPhaseAngle.fPhiA = 120.33;

  EMData_t StructConfigEMData;
  StructConfigEMData.fVoltage_pha = 220.10;
  StructConfigEMData.fCurrent_pha = 50.10;
  StructConfigEMData.fPF = 0.50;
  StructConfigEMData.pStructEnergyData = &StructConfigEnergyData;
  StructConfigEMData.pStructPowerData = &StructConfigPowerData;
  StructConfigEMData.pStructPhaseAngleData = &StructConfigPhaseAngle;

  uint32_t u32QueueSendBlockTime = 500;
  osStatus osMessageSendStatus;
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
    printf("Test CMSIS-RTOS 1: Sender Task\r\n");
    printf("Sending EM value struct pointer via Message Queue to rx-thread\r\n");
    printf("Pha_Voltage: %d\r\n", (int)StructConfigEMData.fVoltage_pha);
    osMessageSendStatus = osMessagePut(EM_Data_Queue_Id, (uint32_t)&StructConfigEMData, osWaitForever);
    if (osMessageSendStatus != osOK)
    {
      printf("Message not sent sucessfully!!\r\n");
    }
    osDelay(2000);
  }
}

/* USER CODE END Header_StartDefaultTask */
void fnRTOSMessageReceiverTask(void const *argument)
{
  uint32_t u32MessageQueueRxBlockTime = 500;
  osEvent event;
  EMData_t *pStructRxEMData;
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    printf("Test CMSIS-RTOS 1: Receiver Task\r\n");
    printf("Waiting for the EM data from Sender Task!!\r\n");
    event = osMessageGet(EM_Data_Queue_Id, osWaitForever);
    if (event.status == osEventMessage)
    {
      pStructRxEMData = (EMData_t *)event.value.p;
      printf("PhaseA Voltage: %d\n PhaseA Current: %d\r\n", (int)pStructRxEMData->fVoltage_pha, (int)pStructRxEMData->fCurrent_pha);
    }
  }
}
/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM17 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void fnCMSIS1_Thread_Config_Assert(osThreadId osThreadHandler)
{
  if (osThreadHandler == NULL)
  {
    printf("Thread could not be created due to insuffient heap space during pvPortMalloc\r\n");
    Error_Handler();
  }
}

#ifdef USE_FULL_ASSERT
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
