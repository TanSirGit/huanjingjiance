#ifndef __THD_RELAY_H
#define	__THD_RELAY_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"

#include "bsp_led.h"
#include "ModBus_Device.h"
#include "Relay.h"

extern TaskHandle_t RelayProcess_Handle;
void RelayProcess(void* parameter);//¼ÌµçÆ÷¿ØÖÆ

#endif
