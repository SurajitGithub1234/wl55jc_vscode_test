#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include "main.h"
#include "cmsis_os.h"
// #include "Middlewares\Third_Party\FreeRTOS\Source\CMSIS_RTOS\cmsis_os.h"
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

// void fnCMSIS1_Thread_Config_Assert(osThreadId osThreadHandler)
// {
//     if (osThreadHandler == NULL)
//     {
//         printf("Thread could not be created due to insuffient heap space during pvPortMalloc\r\n");
//         Error_Handler();
//     }
// }