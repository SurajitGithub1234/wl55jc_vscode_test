#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "cmsis_os2_attr.h"

//***User PV***************//
//2 SW Timers are created: Oneshot and Periodic
osTimerId_t osOneshotTimId, osPeriodicTimId;
uint8_t u8TimerCBArg;


uint32_t u32SignalFlagSetFromOneshotCB = 0x00000100;
uint32_t u32SignalFlagSetFromPeriodicCB = 0x00000200;

//extern the reciever thread's handler id
extern osThreadId_t OS2SynchReceiverID;
/**
 * @brief : config 2 timers by config the struct: osTimerAttr_t like Time name etc
 * 
 */
osTimerAttr_t osOneshotTimAttr = {
    .name = "OneShotTimer"
};
osTimerAttr_t osPeriodicTimAttr = {
    .name = "PeriodicTimer"
};

/**
 * @brief : create 2 timers by calling osTimerNew()
 * 
 */

bool fnCreateTimer(void)
{
    osTimerFunc_t fnPtrTimCallback = SwTimCommonCallback;
    u8TimerCBArg = (uint8_t)ONESHOT_TIM_NAME_FLAG;
    osOneshotTimId = osTimerNew(fnPtrTimCallback, osTimerOnce, (void *)u8TimerCBArg, &osOneshotTimAttr);

    fnPtrTimCallback = SwTimCommonCallback;
    u8TimerCBArg = (uint8_t)PERIODIC_TIM_NAME_FLAG;
    osPeriodicTimId = osTimerNew(fnPtrTimCallback, osTimerPeriodic, (void *)u8TimerCBArg, &osPeriodicTimAttr);

    if(osOneshotTimId == NULL || osPeriodicTimId == NULL)
    {
        printf("Timers could not be created\r\n");
        return false;
    }
    else
    {
        return true;
    }
}
void fnTimerStart(void)
{
    OS2ConfigAssert(osTimerStart(osOneshotTimId, (uint32_t)ONE_SHOT_TIMER_PERIOD));
    OS2ConfigAssert(osTimerStart(osPeriodicTimId, (uint32_t)PERIODIC_TIMER_PERIOD));
}
void SwTimCommonCallback(void *argument)
{
    uint32_t u32SignalApiReturnFlag = 0;
    u8TimerCBArg = (uint8_t)argument;
    printf("Timer Callback arg: %u\r\n",u8TimerCBArg);
    if(u8TimerCBArg == ONESHOT_TIM_NAME_FLAG)
    {
        printf("Oneshot timer invoked\r\n");
        printf("Signal flag set from Oneshot timer callback\r\n");
        u32SignalApiReturnFlag = osThreadFlagsSet(OS2SynchReceiverID, u32SignalFlagSetFromOneshotCB);
        //If we try to stop Oneshot timer, it will give "Resource not found" error
        // printf("Stoping Oneshot timer\r\n");
        // OS2ConfigAssert(osTimerStop(osOneshotTimId));
        printf("Deleting Oneshot timer\r\n");
        OS2ConfigAssert(osTimerDelete(osOneshotTimId));
    }
    else if(u8TimerCBArg == PERIODIC_TIM_NAME_FLAG)
    {
        printf("Periodic timer invoked\r\n");
        printf("Signal flag set from Periodic timer callback\r\n");
        u32SignalApiReturnFlag = osThreadFlagsSet(OS2SynchReceiverID, u32SignalFlagSetFromPeriodicCB);
        if(osOneshotTimId == NULL) //After POR, after 4 seconds, Oneshot Timer got deleted and after that Periodic timer sets both signal flags for Oneshot and Periodic tim
        {
            u32SignalApiReturnFlag = osThreadFlagsSet(OS2SynchReceiverID, u32SignalFlagSetFromOneshotCB);
        }
    }
}