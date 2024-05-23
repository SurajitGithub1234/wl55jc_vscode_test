#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "cmsis_os2_attr.h"

/* Mutex ID */
osMutexId OS2MutexITMConsoleID;

/* Mutex attributes */
const osMutexAttr_t OS2ITMMutexAttr = {
    "MutexITMConsole",
    (osMutexPrioInherit | osMutexRobust)
};

bool fnOs2MutexCreate(void)
{
    OS2MutexITMConsoleID = osMutexNew(NULL);
    if(OS2MutexITMConsoleID != NULL)
    {
        return true;
    }
    return false;
}

void fnPrintToITMConsole(osMutexId osMutexID, uint32_t timeout, char *buffer)
{
    osStatus Os2MutexStatus = osMutexAcquire(osMutexID, timeout);
    if(Os2MutexStatus == osOK)
    {
        printf(buffer);
        Os2MutexStatus = osMutexRelease(osMutexID);
        if(Os2MutexStatus == osOK)
        {
            ;
        }
        else
        {
            ;
        }
    }
    else if(Os2MutexStatus == osErrorParameter) //osMutexID = NULL, which means mutex has not be created yet
    {
        printf(buffer);
    }
    else if(Os2MutexStatus == osErrorTimeout || Os2MutexStatus == osErrorResource || Os2MutexStatus == osErrorISR)
    {
        ;
    }
    else
    {
        ;
    }
}