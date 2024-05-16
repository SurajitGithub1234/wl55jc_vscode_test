Test Project: 
Board: Nucleo-WL55JC1
OS: CMSIS-RTOS-V1
Kernel Objects tested: 
Thread Creation, 
Thread Signaling 
Message Queue
Software Timer

**Issues faced in porting CMSIS-OS-v1 in STM32CubeMX: 
1. Signaling: 
    * Basic 32 bit thread signaling works fine.
    * But the API: osEvent osSignalWait (int32_t signals, uint32_t millisec) should unblock when mentioned bit in the 32bit signal value is set. 
    So this signaling object can be used to unblock a thread when one or many threads reach their respective preconfigured condition state and can unblock master thread. 
    But osSignalWait can be unblocked if any 1 bit in the 32 bit signal value is set.
    * osSignalClear API does not work with CubeMx: https://community.st.com/t5/stm32-mcus-embedded-software/cmsis-rtos-feature-ossignal-incorrectly-implemented-on-freertos/td-p/419021
2. Message Queue: 
    * Normal message queue works fine
    * The api: osMessageQId osMessageCreate (const osMessageQDef_t *queue_def, osThreadId thread_id) suggest that a specific message queue can be solely linked with a specific Thread ID. But this API gets unblocked if any thread send message in the queue using: osMessageQPut
3. Software Timer: 
    * The API: osTimerId osTimerCreate (const osTimerDef_t *timer_def, os_timer_type type, void *argument) suggest that argument can be passed to the timer callback function at the time of timer creation. But the CB function does not register any such parameter.
    * So multiple Timer can not be integrated with a common callback function (in FreeRTOS, TimerID is there)
    * https://github.com/ARM-software/CMSIS-FreeRTOS/issues/9
