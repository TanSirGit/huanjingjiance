#include "Thread.h"

static TaskHandle_t MasterThread_Handle = NULL;        /* 创建任务句柄 */

static void MasterThread(void)
{
  BaseType_t xReturn = TRUE;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区

  /* 创建LightProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )LightProcess,  /* 任务入口函数 */
                        (const char*    )"LightProcess",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )3, /* 任务的优先级 */
                        (TaskHandle_t*  )&LightProcess_Handle);/* 任务控制块指针 */ 
  if(TRUE == xReturn)
    printf("创建LightProcess任务成功!\r\n");

  /* 创建RelayProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )RelayProcess, /* 任务入口函数 */
                        (const char*    )"RelayProcess",/* 任务名字 */
                        (uint16_t       )1024,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&RelayProcess_Handle);/* 任务控制块指针 */
  if(TRUE == xReturn)
    printf("创建RelayProcess任务成功!\r\n");
  
  /* 创建TemHumProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )TemHumProcess, /* 任务入口函数 */
                        (const char*    )"TemHumProcess",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )0,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&TemHumProcess_Handle);/* 任务控制块指针 */
  if(TRUE == xReturn)
    printf("创建TemHumProcess任务成功!\r\n");

  /* 创建WiFIProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )WiFIProcess,  /* 任务入口函数 */
                        (const char*    )"WiFIProcess",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )3, /* 任务的优先级 */
                        (TaskHandle_t*  )&WiFIProcess_Handle);/* 任务控制块指针 */ 
  if(TRUE == xReturn)
    printf("创建WiFIProcess任务成功!\r\n");

  /* 创建ESP32SendProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )ESP32SendProcess,  /* 任务入口函数 */
                        (const char*    )"ESP32SendProcess",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )3, /* 任务的优先级 */
                        (TaskHandle_t*  )&ESP32SendProcess_Handle);/* 任务控制块指针 */ 
  if(TRUE == xReturn)
    printf("创建ESP32SendProcess任务成功!\r\n");

  /* 创建ESP32ReciveProcess任务 */
  xReturn = xTaskCreate((TaskFunction_t )ESP32ReciveProcess,  /* 任务入口函数 */
                        (const char*    )"ESP32ReciveProcess",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )3, /* 任务的优先级 */
                        (TaskHandle_t*  )&ESP32ReciveProcess_Handle);/* 任务控制块指针 */ 
  if(TRUE == xReturn)
    printf("创建ESP32ReciveProcess任务成功!\r\n");

  
  
  vTaskDelete(MasterThread_Handle); //删除MasterThread任务
  
  taskEXIT_CRITICAL();            //退出临界区
}

void Thread_Init(void)
{
    BaseType_t xReturn = TRUE;
    xReturn = xTaskCreate((TaskFunction_t )MasterThread,  /* 任务入口函数 */
								(const char*    )"MasterThread",/* 任务名字 */
								(uint16_t       )512,  /* 任务栈大小 */
								(void*          )NULL,/* 任务入口函数参数 */
								(UBaseType_t    )1, /* 任务的优先级 */
								(TaskHandle_t*  )&MasterThread_Handle);/* 任务控制块指针 */          
		if(TRUE == xReturn)
			vTaskStartScheduler();   /* 开启调度 */
		// else
			// printf("开启调度失败!\n\n");
}

