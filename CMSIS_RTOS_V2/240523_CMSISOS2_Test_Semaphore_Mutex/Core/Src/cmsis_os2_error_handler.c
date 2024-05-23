#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "cmsis_os2_attr.h"

/*User extern variable*/
extern osMutexId OS2MutexITMConsoleID;
//char buffer to print string to ITM Console
extern char chArrBufITMConsole[MAX_CHARACTER_SIZE_ITM_CONSOLE];

//Timer fot mutex acquire while printing to ITM Console
extern uint32_t u32ITMMutexTimeout; //For now ITM Console Mutex timeout is set at 500 ms

/* User function*/
/**
 * @brief Handles errors by disabling interrupts and entering an infinite loop.
 *
 * This function is called to handle errors that occur during the execution of the program.
 * It disables all interrupts to prevent further processing and enters an infinite loop,
 * effectively halting the program. Users can add their own implementation to report the
 * HAL (Hardware Abstraction Layer) error return state.
 *
 * @note This function does not return.
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


/**
 * @brief Asserts the status of an RTOS operation and prints a corresponding message.
 *
 * This function takes an osStatus_t status code from an RTOS operation and prints
 * a message to the standard output describing the result of the operation.
 *
 * @param osStatus The status code of the RTOS operation. This can be one of the following:
 *                 - osOK: Operation completed successfully.
 *                 - osError: Unspecified RTOS error.
 *                 - osErrorTimeout: Operation not completed successfully within timeout.
 *                 - osErrorResource: Resource not available.
 *                 - osErrorParameter: Parameter error.
 *                 - osErrorNoMemory: System is out of memory.
 *                 - osErrorISR: Not allowed in ISR context.
 *                 - osStatusReserved: Prevents enum down size compiler optimizations.
 */

void OS2ConfigAssert(osStatus_t osStatus)
{
  switch(osStatus)
  {
    case osOK: 
      sprintf(chArrBufITMConsole, "Operation completed successfully\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Operation completed successfully\r\n");
      break;
    case osError:
      sprintf(chArrBufITMConsole, "Unspecified RTOS error: run-time error but no other error message fits.");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Unspecified RTOS error: run-time error but no other error message fits.");
      break;
    case osErrorTimeout:
      sprintf(chArrBufITMConsole, "Operation not completed successfully within timeout\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Operation not completed successfully wthin timeout\r\n");
      break; 
    case osErrorResource: 
      sprintf(chArrBufITMConsole, "Resource not available\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Resource not available\r\n");
      break; 
    case osErrorParameter:
      sprintf(chArrBufITMConsole, "Parameter error\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Parameter error\r\n");
      break;
    case osErrorNoMemory:
      sprintf(chArrBufITMConsole, "System is out of memory: it was impossible to allocate or reserve memory for the operation\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("System is out of memory: it was impossible to allocate or reserve memory for the operation\r\n");
      break;
    case osErrorISR:
      sprintf(chArrBufITMConsole, "Not allowed in ISR context: the function cannot be called from interrupt service routines\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("Not allowed in ISR context: the function cannot be called from interrupt service routines\r\n");
      break; 
    case osStatusReserved: 
      sprintf(chArrBufITMConsole, "prevents enum down size compiler optimizations\r\n");
      fnPrintToITMConsole(OS2MutexITMConsoleID, u32ITMMutexTimeout, chArrBufITMConsole);
      //printf("prevents enum down size compiler optimizations\r\n");
      break; 
    default: 
      break;
  }
}