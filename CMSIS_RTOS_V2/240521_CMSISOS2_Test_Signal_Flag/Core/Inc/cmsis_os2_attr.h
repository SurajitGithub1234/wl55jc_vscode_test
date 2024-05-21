#ifndef CMSIS_OS2_ATTR_H
#define CMSIS_OS2_ATTR_H

#include "cmsis_os2.h"
#include "FreeRTOSConfig.h"
#include <stdbool.h>

/*User PM*/ 
#define MS_TO_TICKS(x)                                  (x * (configTICK_RATE_HZ/ 1000))
#define SYNCH_SENDER_1_THREAD_TOGGLE_FREQUENCY          MS_TO_TICKS(2000)
#define SYNCH_SENDER_2_THREAD_TOGGLE_FREQUENCY          MS_TO_TICKS(3000)
#define SYNCH_RECEIVER_THREAD_TOGGLE_FREQUENCY          MS_TO_TICKS(1000)

#define RECEIVER_THREAD_SIGNAL_FLAG_PATTERN             0x0000001F

/* USER CODE BEGIN PV */
/**
 * @brief : CMSIS OS2 related global variables
 * 
 */
extern osVersion_t osv; 
extern char chArrInfoBuf[100];
extern osKernelState_t KernelState;
extern uint32_t u32KernelTickFrequency, u32KernelSystemTimer;


/* User PFP*/
void Error_Handler(void);
void OS2ConfigAssert(osStatus_t osStatus);
bool fnConfigAndCreateThreads(void);

//thread function prototypes: 
void fnOS2ThreadSynchSender1(void *arg);
void fnOS2ThreadSynchSender1(void *arg);
void fnOS2ThreadSynchReceiver(void *arg);

#endif