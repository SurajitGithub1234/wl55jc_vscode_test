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

//char buffer to print string to ITM Console
char chArrBufITMConsole[MAX_CHARACTER_SIZE_ITM_CONSOLE];

//Timer fot mutex acquire while printing to ITM Console
uint32_t u32ITMMutexTimeout = 500; //For now ITM Console Mutex timeout is set at 500 ms

//externed Mutex ID
extern osMutexId OS2MutexITMConsoleID;

//Global variables for Signal Flags//
uint32_t u32SetFlagFromthread, u32SignalApiReturnFlag, u32SignalFlagBeforeClear, u32SignalFlagForWaiting;
/**
 * @brief : Definition and configuration of threads
 * @details : Thread ID creation which acts as a handler for thread creation and attribute structure whcih hold configuration factors 
 *            like name, priority, stack size, array ptr for static thread alllocation etc.  
 * 
 */
osThreadId_t OS2SynchSender1ID, OS2SynchSender2ID, OS2SynchReceiverID; // Thread ids: structure that holds the handler after individual thread creation
const osThreadAttr_t OS2SynchSender1Attr = {
  .name = "SynchSender1Thread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
const osThreadAttr_t OS2SynchSender2Attr = {
  .name = "SynchSender2Thread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};
const osThreadAttr_t OS2SynchReceiverAttr = {
  .name = "SynchReceiverThread",
  .priority = (osPriority_t) osPriorityAboveNormal1,
  .stack_size = 128 * 4
};

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the fnOS2SynchSender1 thread.
  * @param  argument: Frequency of the LED 1 toggling
  * @details : This function toggles LED 1 (Nucleo Board) with fixed periocity got from the function argument and set a 32bit signal flag
  * @retval None
  */
void fnOS2ThreadSynchSender1(void *arg)
{
  /*1st get the kernel tick freq details*/
  u32KernelTickFrequency = osKernelGetTickFreq();
  u32KernelSystemTimer = osKernelGetSysTimerFreq(); 
  
  sprintf(chArrBufITMConsole, "Kernel tick freq: %ldHz\r\n", u32KernelTickFrequency);
  fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
  sprintf(chArrBufITMConsole, "Kernel system timer freq: %ldHz | ARM's systick freq: %ldHz\r\n", u32KernelSystemTimer, HAL_RCC_GetSysClockFreq());
  fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
  
  uint32_t u32LocTick;
  u32LocTick = osKernelGetTickCount();
  uint32_t u32LEDTogglePeriod = (uint32_t)arg; 

  sprintf(chArrBufITMConsole, "Sender thread 1 toggle period: %ld\r\n", u32LEDTogglePeriod);
  fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
  
  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_15);
    //Set specific bits of the 32bit signal flag which is attached to the synch recieve thread
    u32SetFlagFromthread = 0x0000000F;

    sprintf(chArrBufITMConsole, "Setting flag from Sender 1\r\n");
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);

    u32SignalApiReturnFlag = osThreadFlagsSet(OS2SynchReceiverID, u32SetFlagFromthread);
    
    sprintf(chArrBufITMConsole, "return value of 32 bit flag after receiver thread get unblocked: %lx\r\n", u32SignalApiReturnFlag);
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
    
    u32LocTick += u32LEDTogglePeriod; // delay 2000 ticks (2 seconds) periodically 
    osDelayUntil(u32LocTick);
  }
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the fnOS2SynchSender2 thread.
  * @param  argument: Frequency of the LED 2 toggling
  * @details : This function toggles LED 2 (Nucleo Board) with fixed periocity got from the function argument and set a 32bit signal flag
  * @retval None
  */
void fnOS2ThreadSynchSender2(void *arg)
{
  uint32_t u32LocTick;
  u32LocTick = osKernelGetTickCount();
  uint32_t u32LEDTogglePeriod = (uint32_t)arg;
  sprintf(chArrBufITMConsole, "Sender thread 2 toggle period: %ld\r\n", u32LEDTogglePeriod);
  fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
    //Set specific bits of the 32bit signal flag which is attached to the synch recieve thread
    u32SetFlagFromthread = 0x00000010;

    sprintf(chArrBufITMConsole, "Setting flag from Sender 2\r\n");
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
    
    u32SignalApiReturnFlag = osThreadFlagsSet(OS2SynchReceiverID, u32SetFlagFromthread);

    sprintf(chArrBufITMConsole, "return value of 32 bit flag after receiver thread get unblocked: %lx\r\n", u32SignalApiReturnFlag);
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
    
    u32LocTick += u32LEDTogglePeriod; // delay 2000 ticks (2 seconds) periodically 
    osDelayUntil(u32LocTick);
  }
}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the fnOS2SynchReceiver thread.
  * @param  argument: Frequency of the LED 3 toggling
  * @details : This function toggles LED 3 (Nucleo Board) with fixed periocity got from the function argument and then wait for a specific bits to be set in signal flag
  * @retval None
  */
void fnOS2ThreadSynchReceiver(void *arg)
{
  //As this thread is having highest priority, all the timers should be enabled here only
  fnTimerStart();
  uint32_t u32LocTick;
  u32LocTick = osKernelGetTickCount();
  uint32_t u32LEDTogglePeriod = (uint32_t)arg; 

  sprintf(chArrBufITMConsole, "receiver thread toggle period: %ld\r\n", u32LEDTogglePeriod);
  fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
  
  while(1)
  {
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
    //Got blocked state while waiting for specific bits of 32 but signal flag
    u32SignalFlagForWaiting = RECEIVER_THREAD_SIGNAL_FLAG_PATTERN;
    u32SignalFlagBeforeClear = osThreadFlagsWait(u32SignalFlagForWaiting, osFlagsWaitAll, osWaitForever);

    sprintf(chArrBufITMConsole, "***********Thread gets unblocked**********\r\n");
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
    
    sprintf(chArrBufITMConsole, "Receiver Signal Flag before clear: %lx\r\n", u32SignalFlagBeforeClear);
    fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
    
    u32LocTick += u32LEDTogglePeriod; // delay 2000 ticks (2 seconds) periodically 
    osDelayUntil(u32LocTick);
  }
}
/* USER CODE END Header_StartDefaultTask */

bool fnConfigAndCreateThreads(void)
{
    osThreadFunc_t fnThreadPtr = fnOS2ThreadSynchSender1;
    uint32_t u32ThreadArgument = (uint32_t)SYNCH_SENDER_1_THREAD_TOGGLE_FREQUENCY;
    OS2SynchSender1ID = osThreadNew(fnThreadPtr, (void *)u32ThreadArgument, &OS2SynchSender1Attr);

    fnThreadPtr = fnOS2ThreadSynchSender2;
    u32ThreadArgument = (uint32_t)SYNCH_SENDER_2_THREAD_TOGGLE_FREQUENCY;
    OS2SynchSender2ID = osThreadNew(fnThreadPtr, (void *)u32ThreadArgument, &OS2SynchSender2Attr);

    fnThreadPtr = fnOS2ThreadSynchReceiver;
    u32ThreadArgument = (uint32_t)SYNCH_RECEIVER_THREAD_TOGGLE_FREQUENCY;
    OS2SynchReceiverID = osThreadNew(fnThreadPtr, (void *)u32ThreadArgument, &OS2SynchReceiverAttr);

    
    if(OS2SynchSender1ID == NULL || OS2SynchSender2ID == NULL || OS2SynchReceiverID == NULL)
    {
        return false;
    }
    else
    {
        return true;
    }
}