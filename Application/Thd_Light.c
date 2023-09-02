#include "Thd_Light.h" 

TaskHandle_t LightProcess_Handle = NULL;  /* LightProcess任务句柄 */
void LightProcess(void* parameter)//指示灯与存储
{	
  u32 SendTick;

  Relay.Tick = SendTick = GetSystemTick();
  while(1)
  {
    LED3_TOGGLE;
    if((GetSystemTick() - SendTick) > 2000)
    {

      printf("温度%d",TemHum.Tem/10);
      printf(".");
      printf("%d℃",TemHum.Tem%10);
      printf("\r\n");
      printf("湿度%d",TemHum.Hum/10);
      printf(".");
      printf("%dRH",TemHum.Hum%10);
      printf("\r\n");

      printf("继电器1:%d",Relay.RelayStatus1);
      printf(",继电器2:%d",Relay.RelayStatus2);
      printf(",继电器3:%d",Relay.RelayStatus3);
      printf(",继电器4:%d",Relay.RelayStatus4);
      printf("\r\n");
      printf("继电器故障:%d",Relay.ErrCnt);
      printf("\r\n");
      
      SendTick = GetSystemTick();
    }
    vTaskDelay(500);
  }
}


