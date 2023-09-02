#include "Thd_WIFI.h" 

TaskHandle_t WiFIProcess_Handle = NULL; /* WiFICommunicate_Task任务句柄 */
TaskHandle_t ESP32SendProcess_Handle = NULL;
TaskHandle_t ESP32ReciveProcess_Handle = NULL;
PublishData NetCmdData;
void WiFIProcess(void* parameter)//后台通信
{	
    // u32 SendTick;
    PublishData ReadyPubData;
    
    
    // SendTick = GetSystemTick();

    while(1)
    {
        ReadyPubData.Hum = TemHum.Hum;
        ReadyPubData.Tem = TemHum.Tem;
        ReadyPubData.Switch1 = Relay.RelayStatus1;
        ReadyPubData.Switch2 = Relay.RelayStatus2;
        ReadyPubData.Switch3 = Relay.RelayStatus3;
        ReadyPubData.Switch4 = Relay.RelayStatus4;
        // if((GetSystemTick() - SendTick) > 8000)
        // {
            NetHeartBeatProcess(ReadyPubData);
            // SendTick = GetSystemTick();
        // }
        NetRecvProcess(&NetCmdData);
        vTaskDelay(100);
    }
}

void ESP32SendProcess(void)
{
    WiFiProc.Status = eWiFiProc_AT;
    GPIO_Power_On();
    while(1)
    {
        ESP32SendHandle();
        vTaskDelay(37);
    }
}

void ESP32ReciveProcess(void)
{
    while(1)
    {
        ESP32RecieceHandle();
        vTaskDelay(20);
    }
}





