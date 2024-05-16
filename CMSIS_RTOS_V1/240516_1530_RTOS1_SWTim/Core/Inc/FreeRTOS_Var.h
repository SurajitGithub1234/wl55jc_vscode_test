#ifndef _FREERTOS_VAR_H
#define _FREERTOS_VAR_H

#include <stdio.h>
#include <stdint.h>

#define DEVICE_STATE_QUEUE_SIZE     2
#define TIM1_CB_THRESHOLD           12

typedef struct StructDeviceState
{
    uint8_t u8CBCount;
    uint8_t u8DeviceStatus; 
}DeviceState_t;
DeviceState_t *pStructDeviceState;


#endif