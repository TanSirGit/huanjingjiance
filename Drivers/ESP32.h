#ifndef __ESP32_H_
#define	__ESP32_H_
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "gprs_usart.h"
#include "Globle.h"
#include "usart.h"

#define ESP32_WIFI_INFO		    (char*)("AT+CWJAP=\"1144_5g\",\"..@13726165241\"\r\n")
#define ESP32_MQTTUSERCFG		(char*)("AT+MQTTUSERCFG=0,1,\"ipsjoPXIs2G.ESP32|securemode=2\\,signmethod=hmacsha256\\,timestamp=1684928653588|\",\"ESP32&ipsjoPXIs2G\",\"2029cda511a1b0d3267ecea15588e34e85f71c0d9f8c21e7d493a657667d1b13\",0,0,\"\"\r\n")
#define ESP32_MQTTCONN		    (char*)("AT+MQTTCONN=0,\"iot-06z00apic20hxm8.mqtt.iothub.aliyuncs.com\",1883,1\r\n")
#define ESP32_MQTTSUB		    (char*)("AT+MQTTSUB=0,\"/ipsjoPXIs2G/ESP32/user/get\",1\r\n")

typedef struct
{
  signed short Hum;
  u16 Tem;
  u8 Switch1;
  u8 Switch2;
  u8 Switch3;
  u8 Switch4;
}PublishData;
typedef struct
{
  char PUB_BUF[MCU_WiFi_QueueLen];
  char Net_ReBuf[WiFi_MCU_QueueLen];
  PublishData PUBData;
}NetStructre;

typedef enum
{
  eSelfCtrl_None = 0,
  eSelfCtrl_Disconn,
  eSelfCtrl_Error,
}EnumWiFiSelfCtrl;
typedef enum
{
  eSendCtrl_None = 0,
  eSendCtrl_SendReq,
  eSendCtrl_SendDone,
  eSendCtrl_SendErr,
}EnumWiFiSendSelfCtrl;


typedef void(*pWiFiProcHandle)(void);

void WiFi_Cmd_AT(void);
void WiFi_Cmd_MQTTUSERCFG(void);

void ESP32SendHandle(void);
void ESP32RecieceHandle(void);

u8 NetFraccmeProcess(PublishData* pData);
void NetRecvProcess(PublishData* pData);

u8 NetInit(void);
u8 NetAvail(void);
int NetTCPSend(void);
void NetHeartBeatProcess(PublishData pData);
#endif
