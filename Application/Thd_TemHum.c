#include "Thd_TemHum.h" 

TaskHandle_t TemHumProcess_Handle = NULL;     /* TemHumProcess������ */

void TemHumProcess(void* parameter)//��Ϣ�ɼ�
{  
    while(1)
    {
        LED1_TOGGLE;    
        TemHumReadVolue();
        vTaskDelay(500);
    }
}
