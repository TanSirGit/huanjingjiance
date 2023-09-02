#ifndef __RELAY_H
#define	__RELAY_H
#include "stm32f10x.h"
// #include "FreeRTOS.h"
// #include "task.h"
#include "string.h"
#include "Globle.h"
#include "ModBus_Device.h"

#define Open            1
#define Close           0

typedef struct
{
    u8 RelayStatus1;
    u8 RelayStatus2;
    u8 RelayStatus3;
    u8 RelayStatus4;
    u32 Tick;
    u8 ErrCnt;
}Relaystruct;
extern Relaystruct Relay;

u8 OpenSingleRelay(u8 num);//��1�ż̵���
u8 CloseSingleRelay(u8 num);//�ر�1�ż̵���
u8 OpenAllRelay(void);//�����м̵���
u8 CloseAllRelay(void);//�ر����м̵���
u8 ReadRelayStatus(void);//��ȡ�̵���״̬
u8 RelayCtrProcess(Relaystruct Data);
#endif 

