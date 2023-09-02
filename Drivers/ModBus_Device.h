#ifndef __MODBUS_H
#define	__MODBUS_H
#include "stm32f10x.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Globle.h"
#include "RS485_usart.h"
#include "usart.h"



u8 ModBusRevHandle(u8* pFrame);
void ModBusSendHandle(u8* pFrame,u8 pFrameLen);
#endif 




