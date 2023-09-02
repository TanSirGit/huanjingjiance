#include "Thd_Light.h" 

TaskHandle_t LightProcess_Handle = NULL;  /* LightProcess������ */
void LightProcess(void* parameter)//ָʾ����洢
{	
  u32 SendTick;

  Relay.Tick = SendTick = GetSystemTick();
  while(1)
  {
    LED3_TOGGLE;
    if((GetSystemTick() - SendTick) > 2000)
    {

      printf("�¶�%d",TemHum.Tem/10);
      printf(".");
      printf("%d��",TemHum.Tem%10);
      printf("\r\n");
      printf("ʪ��%d",TemHum.Hum/10);
      printf(".");
      printf("%dRH",TemHum.Hum%10);
      printf("\r\n");

      printf("�̵���1:%d",Relay.RelayStatus1);
      printf(",�̵���2:%d",Relay.RelayStatus2);
      printf(",�̵���3:%d",Relay.RelayStatus3);
      printf(",�̵���4:%d",Relay.RelayStatus4);
      printf("\r\n");
      printf("�̵�������:%d",Relay.ErrCnt);
      printf("\r\n");
      
      SendTick = GetSystemTick();
    }
    vTaskDelay(500);
  }
}


