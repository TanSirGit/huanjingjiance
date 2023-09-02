#include "Thd_CtrlProcess.h" 

TaskHandle_t CtrlProcess_Handle = NULL;     /* CtrlProcess任务句柄 */

void CtrlProcess(void* parameter)//信息采集
{  
    while(1)
    {

        vTaskDelay(500);
    }
}
