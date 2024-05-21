#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "cmsis_os2_attr.h"

/* Private variables ---------------------------------------------------------*/
osVersion_t osv; 
char chArrInfoBuf[100];
/**
 * @brief : Definition and configuration of threads
 * @details : Thread ID creation which acts as a handler for thread creation and attribute structure whcih hold configuration factors 
 *            like name, priority, stack size, array ptr for static thread alllocation etc.  
 * 
 */
osThreadId_t OS2SynchSenderID, OS2SynchReceiver1ID; // Thread ids: structure that holds the handler after individual thread creation
const osThreadAttr_t OS2SynchSenderAttr = {
  .name = "SynchSenderThread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
const osThreadAttr_t OS2SynchReceiver1Attr = {
  .name = "SynchReceiver1Thread",
  .priority = (osPriority_t) osPriorityAboveNormal1,
  .stack_size = 128 * 4
};

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the fnOS2SynchSender thread.
  * @param  argument: Frequency of the LED 1 toggling
  * @details : This function toggles LED 1 (Nucleo Board) with fixed periocity got from the function argument and set a 32bit signal flag
  * @retval None
  */
void fnOS2ThreadSynchSender(void *arg)
{
  /*1st get the kernel tick freq details*/
  u32KernelTickFrequency = osKernelGetTickFreq();
  u32KernelSystemTimer = osKernelGetSysTimerFreq(); 
  printf("Kernel tick freq: %ldHz\r\n", u32KernelTickFrequency);
  printf("Kernel system timer freq: %ldHz | ARM's systick freq: %ldHz\r\n", u32KernelSystemTimer, HAL_RCC_GetSysClockFreq());
  uint32_t u32LocTick;
  u32LocTick = osKernelGetTickCount();
  uint32_t u32LEDTogglePeriod = (uint32_t)arg; 
  printf("Sender thread toggle period: %ld\r\n", u32LEDTogglePeriod);
  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
    //Set specific bits of the 32bit signal flag which is attached to the synch recieve thread
    u32LocTick += u32LEDTogglePeriod; // delay 2000 ticks (2 seconds) periodically 
    osDelayUntil(u32LocTick);
  }
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the fnOS2SynchReceiver thread.
  * @param  argument: Frequency of the LED 2 toggling
  * @details : This function toggles LED 2 (Nucleo Board) with fixed periocity got from the function argument and then wait for a specific bit to be set in signal flag
  * @retval None
  */
void fnOS2ThreadSynchReceiver1(void *arg)
{
  uint32_t u32LocTick;
  u32LocTick = osKernelGetTickCount();
  uint32_t u32LEDTogglePeriod = (uint32_t)arg; 
  printf("receiver 1 thread toggle period: %ld\r\n", u32LEDTogglePeriod);
  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    //Set specific bits of the 32bit signal flag which is attached to the synch recieve thread
    u32LocTick += u32LEDTogglePeriod; // delay 2000 ticks (2 seconds) periodically 
    osDelayUntil(u32LocTick);
  }
}
/* USER CODE END Header_StartDefaultTask */

bool fnConfigAndCreateThreads(void)
{
    osThreadFunc_t fnThreadPtr = fnOS2ThreadSynchSender;
    uint32_t u32ThreadArgument = (uint32_t)SYNCH_SENDER_THREAD_TOGGLE_FREQUENCY;
    printf("%ld\r\n", u32ThreadArgument);
    OS2SynchSenderID = osThreadNew(fnThreadPtr, (void *)u32ThreadArgument, &OS2SynchSenderAttr);
    fnThreadPtr = fnOS2ThreadSynchReceiver1;
    u32ThreadArgument = (uint32_t)SYNCH_RECEIVER1_THREAD_TOGGLE_FREQUENCY;
    OS2SynchReceiver1ID = osThreadNew(fnThreadPtr, (void *)u32ThreadArgument, &OS2SynchReceiver1Attr);
    if(OS2SynchSenderID == NULL || OS2SynchReceiver1ID == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}