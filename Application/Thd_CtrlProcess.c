#include "Thd_CtrlProcess.h" 

TaskHandle_t CtrlProcess_Handle = NULL;     /* CtrlProcess������ */

void CtrlProcess(void* parameter)//��Ϣ�ɼ�
{  
    while(1)
    {

        vTaskDelay(500);
    }
}
