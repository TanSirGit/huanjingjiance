#ifndef __THD_WIFI_H
#define	__THD_WIFI_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"

#include "bsp_led.h"
#include "ESP32.h"
#include "usart.h"
#include "Thd_TemHum.h" 
#include "Thd_Relay.h" 

extern TaskHandle_t WiFIProcess_Handle;
extern TaskHandle_t ESP32SendProcess_Handle;
extern TaskHandle_t ESP32ReciveProcess_Handle;
extern PublishData NetCmdData;

void WiFIProcess(void* parameter);//后台通信
void ESP32SendProcess(void);
void ESP32ReciveProcess(void);
#endif
