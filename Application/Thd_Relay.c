#include "Thd_Relay.h" 
#include "Thd_WIFI.h"

TaskHandle_t RelayProcess_Handle = NULL;        /* RelayProcess任务句柄 */

void RelayProcess(void* parameter)//继电器控制
{	
    u32 Tick;
    Relaystruct RelayCtr;

    Tick = GetSystemTick();
    while(1)
    {
        LED2_TOGGLE;
        RelayCtr.RelayStatus1 = NetCmdData.Switch1;
        RelayCtr.RelayStatus2 = NetCmdData.Switch2;
        RelayCtr.RelayStatus3 = NetCmdData.Switch3;
        RelayCtr.RelayStatus4 = NetCmdData.Switch4;
        if((GetSystemTick() - Tick) > 1000)
        {
            vTaskDelay(50);
            ReadRelayStatus();
            vTaskDelay(50);
            Tick = GetSystemTick();
        }       
        RelayCtrProcess(RelayCtr);
        vTaskDelay(100);
    }
}
