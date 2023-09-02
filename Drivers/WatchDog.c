#include "WatchDog.h"


void WdgInit(void)
{
  u32 ErrorTime;
/* Wait till LSI is ready */
  ErrorTime = 0x00800000;
  RCC_LSICmd(ENABLE);
  while((RESET == RCC_GetFlagStatus(RCC_FLAG_LSIRDY)) && ErrorTime)
  {
    ErrorTime--;
  }

  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
  IWDG_SetPrescaler(IWDG_Prescaler_128);
  IWDG_SetReload(0xC35);//����10s���Ź�
  IWDG_ReloadCounter();
  IWDG_Enable();
}//ʵ��ʱ��(ms)=(prer*lrl)/40ms

void FeedDog(void)
{
  IWDG_ReloadCounter();
}
















