#include "TASK_Safe.h"
#include "SYSConfig.h"

void Safe_Task(void const *argument)
{
    vTaskDelay(SAFE_TASK_INIT_TIME);
    while (1)
    {
        vTaskDelay(SAFE_CONTROL_TIME_MS);
    }
    
}