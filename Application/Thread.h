#ifndef __THREAD_H
#define	__THREAD_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"
#include "Thd_Light.h"
#include "Thd_Relay.h" 
#include "Thd_TemHum.h"
#include "Thd_WIFI.h"
#include "ESP32.h"
#include "usart.h"

void Thread_Init(void);
#endif

