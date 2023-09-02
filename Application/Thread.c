#include "Thread.h"

static TaskHandle_t MasterThread_Handle = NULL;        /* ���������� */

static void MasterThread(void)
{
  BaseType_t xReturn = TRUE;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���

  /* ����LightProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )LightProcess,  /* ������ں��� */
                        (const char*    )"LightProcess",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&LightProcess_Handle);/* ������ƿ�ָ�� */ 
  if(TRUE == xReturn)
    printf("����LightProcess����ɹ�!\r\n");

  /* ����RelayProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )RelayProcess, /* ������ں��� */
                        (const char*    )"RelayProcess",/* �������� */
                        (uint16_t       )1024,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&RelayProcess_Handle);/* ������ƿ�ָ�� */
  if(TRUE == xReturn)
    printf("����RelayProcess����ɹ�!\r\n");
  
  /* ����TemHumProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )TemHumProcess, /* ������ں��� */
                        (const char*    )"TemHumProcess",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )0,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&TemHumProcess_Handle);/* ������ƿ�ָ�� */
  if(TRUE == xReturn)
    printf("����TemHumProcess����ɹ�!\r\n");

  /* ����WiFIProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )WiFIProcess,  /* ������ں��� */
                        (const char*    )"WiFIProcess",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&WiFIProcess_Handle);/* ������ƿ�ָ�� */ 
  if(TRUE == xReturn)
    printf("����WiFIProcess����ɹ�!\r\n");

  /* ����ESP32SendProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )ESP32SendProcess,  /* ������ں��� */
                        (const char*    )"ESP32SendProcess",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&ESP32SendProcess_Handle);/* ������ƿ�ָ�� */ 
  if(TRUE == xReturn)
    printf("����ESP32SendProcess����ɹ�!\r\n");

  /* ����ESP32ReciveProcess���� */
  xReturn = xTaskCreate((TaskFunction_t )ESP32ReciveProcess,  /* ������ں��� */
                        (const char*    )"ESP32ReciveProcess",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&ESP32ReciveProcess_Handle);/* ������ƿ�ָ�� */ 
  if(TRUE == xReturn)
    printf("����ESP32ReciveProcess����ɹ�!\r\n");

  
  
  vTaskDelete(MasterThread_Handle); //ɾ��MasterThread����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}

void Thread_Init(void)
{
    BaseType_t xReturn = TRUE;
    xReturn = xTaskCreate((TaskFunction_t )MasterThread,  /* ������ں��� */
								(const char*    )"MasterThread",/* �������� */
								(uint16_t       )512,  /* ����ջ��С */
								(void*          )NULL,/* ������ں������� */
								(UBaseType_t    )1, /* ��������ȼ� */
								(TaskHandle_t*  )&MasterThread_Handle);/* ������ƿ�ָ�� */          
		if(TRUE == xReturn)
			vTaskStartScheduler();   /* �������� */
		// else
			// printf("��������ʧ��!\n\n");
}

