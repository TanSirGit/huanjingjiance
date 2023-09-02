#ifndef __GPRS_USART_H
#define	__GPRS_USART_H
#include "stm32f10x.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "Globle.h"
#include "usart.h"


#define     WiFi_MCU_QueueLen         (400)
#define     MCU_WiFi_QueueLen         (256)

/**************************���ڽṹ�����Ϣ����**********************************/
extern QueueHandle_t	WiFi_MCU_QueueHandle;
extern QueueHandle_t	WiFi_MCU_QueueHandle2;

extern QueueHandle_t	MCU_WiFi_QueueHandle;

typedef enum
{
  eWiFiProc_AT = 0,
  eWiFiProc_RST,        //1
  eWiFiProc_CWMODE,     //2
  eWiFiProc_CWDHCP,     //3
  eWiFiProc_CWJAP,      //4
  eWiFiProc_MQTTUSERCFG,//5
  eWiFiProc_MQTTCONN,   //6
  eWiFiProc_MQTTSUB,    //7
  eWiFiProc_Data,       //8
  eWiFiProc_Send,       //9
  eWiFiProc_Idle,       //10
  eWiFiProc_Error,      //11
  eWiFiProc_DisConn,    //12
}EnumWiFiProc;

typedef struct
{
  volatile u8 ErrCnt;
  volatile EnumWiFiProc Status;
  volatile u32 WiFiStatusTick;
}StructWiFiProc;



typedef struct
{
  volatile u8 Sta;//0δʹ��  1�ѷ��� 2��Ӧ��
  volatile u8 Ack;//0δӦ��  1Ӧ��pResp_1   1Ӧ��pResp_2
  char RespExBuf[WiFi_MCU_QueueLen];  //Ӧ�������չBuff
  char *pRespExFlag;//Ӧ�����ݱ�־
  char *pResp_1;    //Ӧ��1
  char *pResp_2;    //Ӧ��2
  volatile u8 ConnSta;       //����״̬
  volatile u8 RecvFlag;      //���ձ�־
//   char *Buffer;
}StructWiFiCmd;





/*ȫ�ֱ���*/
extern StructWiFiCmd WiFiCmd;
extern StructWiFiProc WiFiProc;
/**************************���ں�**********************************/

// ����3-USART3
#define  GPRS_USARTx                   USART3
#define  GPRS_USART_CLK                RCC_APB1Periph_USART3
#define  GPRS_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  GPRS_USART_BAUDRATE           (115201)

// USART GPIO ���ź궨��
#define  GPRS_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  GPRS_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  GPRS_USART_TX_GPIO_PORT       GPIOB   
#define  GPRS_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  GPRS_USART_RX_GPIO_PORT       GPIOB
#define  GPRS_USART_RX_GPIO_PIN        GPIO_Pin_11

#define  GPRS_USART_IRQ                USART3_IRQn
#define  GPRS_USART_IRQHandler         USART3_IRQHandler

void GPIO_Power_On(void);
void USART3_Config(void);

u8 WiFiSend(char ch);
u8 WiFiSendStar(void);
u8 WiFiSendByte(char ch);
u8 WiFiSendArray(char *array, uint16_t num);
u8 WiFiSendCmd(char *pRespEx,char* Cmd);
u8 WiFi_Cmd_RecieveFrame(char* pFrame);
u8 WiFi_Cmd_WaitAck(u32 Ms);
void WiFi_Cmd_Reciece(void);
#endif



