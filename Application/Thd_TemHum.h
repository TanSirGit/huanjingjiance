#ifndef __TASK1_H
#define	__TASK1_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"

#include "bsp_led.h"
#include "ModBus_Device.h"
#include "temperature.h"

extern TaskHandle_t TemHumProcess_Handle;
void TemHumProcess(void* parameter);//信息采集

#endif
