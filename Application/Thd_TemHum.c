#include "Thd_TemHum.h" 

TaskHandle_t TemHumProcess_Handle = NULL;     /* TemHumProcess任务句柄 */

void TemHumProcess(void* parameter)//信息采集
{  
    while(1)
    {
        LED1_TOGGLE;    
        TemHumReadVolue();
        vTaskDelay(500);
    }
}
