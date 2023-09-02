#ifndef __TEMPERATURE_H
#define	__TEMPERATURE_H
#include "stm32f10x.h"
#include "string.h"
#include "ModBus_Device.h"

typedef struct
{
    signed short Tem;
    u16 Hum;
    u8 ErrCnt;
}TemHumstruct;

extern TemHumstruct TemHum;





u8 TemHumWriteAddr(void);
u8 TemHumReadAddr(void);
u8 TemHumReadVolue(void);
#endif 

