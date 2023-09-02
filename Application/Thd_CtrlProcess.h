#ifndef __TASK1_H
#define	__TASK1_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "Globle.h"


extern TaskHandle_t CtrlProcess_Handle;
void CtrlProcess(void* parameter);//信息采集

#endif
