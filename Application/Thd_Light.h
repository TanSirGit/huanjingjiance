#ifndef __TASK4_H
#define	__TASK4_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"

#include "bsp_led.h"
#include "temperature.h"
#include "Relay.h"
#include "usart.h"

extern TaskHandle_t LightProcess_Handle;
void LightProcess(void* parameter);//÷∏ æµ∆”Î¥Ê¥¢

#endif
