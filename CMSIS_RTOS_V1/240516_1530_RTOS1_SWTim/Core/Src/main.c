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
 * Board: Nucleo-WL55JC1
 * CMSIS RTOS 1: Software Timer Usecase Test
 * 2 SW Timers and 2 Threads are created. 
 * Tim 1 expires every 1 seconds(periodic) and Tim 2 expires after 2 seconds and it is a single shot timer
 * Both tim have one CB function
 * Tim1 populates a queue with CB instance count and Tim2 populates the same queue with device status objects
 * Thread 1 unblocks at that queue and when count reaches 10, it stops and delete timer 1
 * Thread 2 only once unblocked by the same queue and then deletes that queue
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
void fnRTOSTask1(void const *argument);
void fnRTOSTask2(void const *argument);

//SW Timer Callback function prototype
void fnRTOS1SWTimerCB(void const *arg);

void fnCMSIS1_Thread_Config_Assert(osThreadId osThreadHandler);

// Configure CMSIS RTOS 1 Task: fnRTOSTask1
osThreadId Task1_ID;
osThreadDef(CMSIS1_Task1, fnRTOSTask1, osPriorityAboveNormal, TASK_INSTANCE_SINGLE, SENDER_TASK_STACK_SIZE);

// Config CMSIS RTOS 1 Task: fnRTOSTask2
osThreadId Task2_ID;
osThreadDef(CMSIS1_Task2, fnRTOSTask2, osPriorityNormal, TASK_INSTANCE_SINGLE, RECIEVER_TASK_STACK_SIZE);

// Create a message queue to pass structure pointer from one ptr to another.
osMessageQDef(Device_State_Queue, DEVICE_STATE_QUEUE_SIZE, pStructDeviceState);
osMessageQId(Device_State_Queue_Id);

//Cofig the SW Timer 1
uint32_t u32Tim1CBParam;
osTimerDef(SWTim1, fnRTOS1SWTimerCB);
osTimerId osTim1ID;

//Cofig the SW Timer 2
uint32_t u32Tim2CBParam;
osTimerDef(SWTim2, fnRTOS1SWTimerCB);
osTimerId osTim2ID;

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

  Task1_ID = osThreadCreate(osThread(CMSIS1_Task1), NULL);
  if (Task1_ID == NULL)
  {
    printf("Task could not be creared sucessfully!!");
    Error_Handler();
  }
  // Create Receiver 1 Thread
  Task2_ID = osThreadCreate(osThread(CMSIS1_Task2), NULL);
  fnCMSIS1_Thread_Config_Assert(Task2_ID);

  // create a message queue to store the device status data and pass from one thread to other
  osThreadId osMessageQueueSendThreadID = NULL;
  Device_State_Queue_Id = osMessageCreate(osMessageQ(Device_State_Queue), osMessageQueueSendThreadID);
  if (Device_State_Queue_Id == NULL)
  {
    printf("Queue could not be created!!\r\n");
    Error_Handler();
  }

  //create the sw timers
  u32Tim1CBParam = 1; //SW Tim1 identifier
  u32Tim2CBParam = 2; //SW tim2 identifier

  osTim1ID = osTimerCreate(osTimer(SWTim1), osTimerPeriodic, (void*)1); 
  osTim2ID = osTimerCreate(osTimer(SWTim2), osTimerOnce, (void*)2); 

  if(osTim1ID == NULL || osTim2ID == NULL)
  {
    printf("SW Timer could not be created, check the software timer serive queue\r\n");
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
void fnRTOSTask1(void const *argument)
{
  uint32_t u32QueueReceiveBlockTime = 500;
  DeviceState_t *pStructRxDeviceState;
  osStatus osTimerCommandStatus; 
  osEvent osQueueReceiveEvent;
  //Start both the timer
  osTimerCommandStatus = osTimerStart(osTim1ID, 500); 
  if(osTimerCommandStatus != osOK)
  {
    printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
    Error_Handler();
  }
  osTimerCommandStatus = osTimerStart(osTim2ID, 2000); 
  if(osTimerCommandStatus != osOK)
  {
    printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
    Error_Handler();
  }
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
    printf("Test CMSIS-RTOS 1: Task1\r\n");
    osQueueReceiveEvent = osMessageGet(Device_State_Queue_Id, osWaitForever);
    if (osQueueReceiveEvent.status == osEventMessage)
    {
      printf("Message Queue received from TimerCB\r\n");
      pStructRxDeviceState = (DeviceState_t *)osQueueReceiveEvent.value.p;
      printf("Tim1 CB instance count: %d\r\n", pStructRxDeviceState->u8CBCount);
      if(pStructRxDeviceState->u8CBCount >= TIM1_CB_THRESHOLD)
      {
        printf("Tim1 cb instances count reaches %d, deleteing the timer\r\n", TIM1_CB_THRESHOLD);
        osTimerCommandStatus = osTimerStop(osTim1ID);
        if(osTimerCommandStatus == osOK)
        {
          osTimerCommandStatus = osTimerDelete(osTim1ID);
          if(osTimerCommandStatus != osOK)
          {
            printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
            Error_Handler();
          }
          else{
            ;
          }
        }
        else{
          printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
          Error_Handler();
        }
      }
    }
  }
}

/* USER CODE END Header_StartDefaultTask */
void fnRTOSTask2(void const *argument)
{
  uint32_t u32QueueReceiveBlockTime = 500;
  DeviceState_t *pStructRxDeviceState;
  osStatus osTimerCommandStatus; 
  osEvent osQueueReceiveEvent;
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    printf("Test CMSIS-RTOS 1: Task 2\r\n");
    osQueueReceiveEvent = osMessageGet(Device_State_Queue_Id, osWaitForever);
    if (osQueueReceiveEvent.status == osEventMessage)
    {
      pStructRxDeviceState = (DeviceState_t *)osQueueReceiveEvent.value.p;
      printf("Device status: %d\r\n", pStructRxDeviceState->u8DeviceStatus);
      if(pStructRxDeviceState->u8DeviceStatus)
      {
        printf("Tim2 expires once and device status is set\r\n");
        osTimerCommandStatus = osTimerStop(osTim2ID);
        if(osTimerCommandStatus == osOK)
        {
          osTimerCommandStatus = osTimerDelete(osTim2ID);
          if(osTimerCommandStatus != osOK)
          {
            printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
            Error_Handler();
          }
          else{
            ;
          }
        }
        else{
          printf("Timer command could not be sent to the Timer Service Queue!\r\n"); 
          Error_Handler();
        }
      }
      
    }
  }
}

void fnRTOS1SWTimerCB(void const *arg)
{
  uint8_t *u8TimIdentity = (uint8_t*)arg;
  DeviceState_t StructRxDeviceState;
  osStatus osMessageSendStatus;
  printf("CB arg: %d\r\n", *u8TimIdentity);
  if(*u8TimIdentity == 1)
  {
    StructRxDeviceState.u8CBCount++;
    osMessageSendStatus = osMessagePut(Device_State_Queue_Id, (uint32_t)&StructRxDeviceState, 0);
    if(osMessageSendStatus != osOK) printf("Queue message could not be sent\r\n");
  }
  else if(*u8TimIdentity == 2)
  {
    StructRxDeviceState.u8DeviceStatus = 1;
    osMessageSendStatus = osMessagePut(Device_State_Queue_Id, (uint32_t)&StructRxDeviceState, 0);
    if(osMessageSendStatus != osOK) printf("Queue message could not be sent\r\n");
  }
  else 
  {
    ;
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

// Test comment 