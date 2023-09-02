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

u8 OpenSingleRelay(u8 num);//打开1号继电器
u8 CloseSingleRelay(u8 num);//关闭1号继电器
u8 OpenAllRelay(void);//打开所有继电器
u8 CloseAllRelay(void);//关闭所有继电器
u8 ReadRelayStatus(void);//读取继电器状态
u8 RelayCtrProcess(Relaystruct Data);
#endif 

