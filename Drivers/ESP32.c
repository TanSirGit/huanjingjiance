#include "ESP32.h"

NetStructre Net;
volatile EnumWiFiSelfCtrl WiFiSelfCtrl;
volatile EnumWiFiSendSelfCtrl WiFiSendSelfCtrl;

void WiFi_Cmd_AT(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,"AT\r\n");
    Ack = WiFi_Cmd_WaitAck(1000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_RST;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_RST(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "WIFI CONNECTED";
	WiFiCmd.pResp_2 = "ERROR";

    WiFiSendCmd(NULL,"AT+RST\r\n");
    Ack = WiFi_Cmd_WaitAck(5000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_CWMODE;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_CWMODE(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,"AT+CWMODE=1\r\n");
    Ack = WiFi_Cmd_WaitAck(1000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_CWDHCP;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_CWDHCP(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,"AT+CWDHCP=1,1\r\n");
    Ack = WiFi_Cmd_WaitAck(1000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_CWJAP;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_CWJAP(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,ESP32_WIFI_INFO);
    Ack = WiFi_Cmd_WaitAck(3000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_MQTTUSERCFG;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_MQTTUSERCFG(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,ESP32_MQTTUSERCFG);
    Ack = WiFi_Cmd_WaitAck(2000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_MQTTCONN;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_MQTTCONN(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,ESP32_MQTTCONN);
    Ack = WiFi_Cmd_WaitAck(2000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_MQTTSUB;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}
void WiFi_Cmd_MQTTSUB(void)
{ 
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    WiFiCmd.pResp_1 = "OK";
	WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,ESP32_MQTTSUB);
    Ack = WiFi_Cmd_WaitAck(2000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Idle;
        printf("MQTTSUB_OK\r\n");
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
    
}
void WiFi_Cmd_Data(void)
{
    u8 Ack = 0;
    
    WiFiCmd.pResp_1 = "OK";
    WiFiCmd.pResp_2 = "ERROR";
    
    WiFiSendCmd(NULL,Net.PUB_BUF);   
    Ack = WiFi_Cmd_WaitAck(3000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        
    }
    else if(2 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        if(eSelfCtrl_None == WiFiSelfCtrl)
            WiFiSelfCtrl = eSelfCtrl_Disconn;
        WiFiSendSelfCtrl = eSendCtrl_SendErr;
        WiFiProc.Status = eWiFiProc_Idle;
        return;
    }
        WiFiSendSelfCtrl = eSendCtrl_SendDone;
        WiFiProc.Status = eWiFiProc_Idle;
}
void WiFi_Cmd_Send(void)
{ 
    u8 Ack = 0;
    u8 len = 0;
    char Buff[256] ={0};
    WiFiCmd.pResp_1 = ">";
    WiFiCmd.pResp_2 = "ERROR";
    len = sprintf(Net.PUB_BUF,"{\"id\": \"123\",\"version\": \"1.0.0\",\"params\":{\"Humidity\":%d,\"temperature\":%d,\"PowerSwitch_1\":%d,\"PowerSwitch_2\":%d,\"PowerSwitch_3\":%d,\"PowerSwitch_4\":%d},\"method\": \"thing.event.property.post\"}\r\n",
    Net.PUBData.Hum,Net.PUBData.Tem,Net.PUBData.Switch1?1:0,Net.PUBData.Switch2?1:0,Net.PUBData.Switch3?1:0,Net.PUBData.Switch4?1:0);//发布的JSON格式

    sprintf(Buff,"AT+MQTTPUBRAW=0,\"/sys/ipsjoPXIs2G/ESP32/thing/event/property/post\",%d,0,0\r\n",len);
    WiFiSendCmd(NULL,Buff);
    Ack = WiFi_Cmd_WaitAck(1000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFi_Cmd_Data();
    }
    else
    {
        vTaskDelay(1500);
        WiFi_Cmd_Data();
    }  
}
void WiFi_Idle(void)
{
    WiFiCmd.ConnSta = 1;
    switch(WiFiSelfCtrl)
    {
        case eSelfCtrl_None:
        switch(WiFiSendSelfCtrl)
        {
            case eSendCtrl_None:
            WiFiProc.WiFiStatusTick = 0;
            WiFiProc.ErrCnt = 0;
            WiFiProc.Status = eWiFiProc_Idle;
            break; 
            case eSendCtrl_SendReq:
            WiFiProc.WiFiStatusTick = 0;
            WiFiProc.ErrCnt = 0;
            WiFiProc.Status = eWiFiProc_Send;
            break;
            default:
            /*
            if(0 == WiFiProc.WiFiStatusTick)
                WiFiProc.WiFiStatusTick = GetSystemTick();
            if((0 != WiFiProc.WiFiStatusTick) && ((GetSystemTick() - WiFiProc.WiFiStatusTick) > 10000))
            {
                WiFiProc.WiFiStatusTick = 0;
                WiFiProc.ErrCnt = 0;
                WiFiProc.Status = eWiFiProc_MQTTSUB;
            }
            */
            WiFiProc.WiFiStatusTick = 0;
            WiFiProc.ErrCnt = 0;
            WiFiProc.Status = eWiFiProc_Idle;
            break;      
        }
        break;
        case eSelfCtrl_Disconn:
        WiFiSelfCtrl = eSelfCtrl_None;
        WiFiProc.WiFiStatusTick = 0;
        WiFiProc.ErrCnt = 0;
        WiFiCmd.ConnSta = 0;    //进入第一层接收
        WiFiProc.Status = eWiFiProc_DisConn;
        break;
        case eSelfCtrl_Error:
        WiFiSelfCtrl = eSelfCtrl_None;
        WiFiProc.WiFiStatusTick = 0;
        WiFiProc.ErrCnt = 0;
        WiFiCmd.ConnSta = 0;    //进入第一层接收
        WiFiProc.Status = eWiFiProc_AT;
        break;
    }
    
}

void WiFi_Error(void)
{
    WiFi_Cmd_RST();
    vTaskDelay(3000);
    WiFiProc.Status = eWiFiProc_AT;
    WiFiCmd.ConnSta = 0;
}

void WiFi_Disconn(void)
{
    u8 Ack = 0;

    WiFiCmd.ConnSta = 0;
    
    WiFiSendCmd(NULL,"AT+CWQAP");
    Ack = WiFi_Cmd_WaitAck(1000);
    if(1 == Ack)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_CWJAP;
    }
    else
        WiFiProc.ErrCnt++;
    
    if(WiFiProc.ErrCnt > 3)
    {
        WiFiProc.ErrCnt = 0;
        WiFiProc.Status = eWiFiProc_Error;
    }
}

/************************************ESP32发送***************************************/
static pWiFiProcHandle ESP32ProcHandle[] =
{
  WiFi_Cmd_AT,
  WiFi_Cmd_RST,
  WiFi_Cmd_CWMODE,
  WiFi_Cmd_CWDHCP,
  WiFi_Cmd_CWJAP,
  WiFi_Cmd_MQTTUSERCFG,
  WiFi_Cmd_MQTTCONN,
  WiFi_Cmd_MQTTSUB,
  WiFi_Cmd_Data,
  WiFi_Cmd_Send,
  WiFi_Idle,
  WiFi_Error,
  WiFi_Disconn,
};
void ESP32SendHandle(void)
{
  if(WiFiProc.Status < (sizeof(ESP32ProcHandle)/sizeof(pWiFiProcHandle)))
    ESP32ProcHandle[WiFiProc.Status]();
  else
    WiFiProc.Status = eWiFiProc_AT;
}

/************************************ESP32接收***************************************/


void ESP32RecieceHandle(void)
{
    WiFi_Cmd_Reciece();
}

/***************************************************************************/
typedef enum
{
  eWiFiRespSta_p = 0,
  eWiFiRespSta_a,
  eWiFiRespSta_r,
  eWiFiRespSta_aa,
  eWiFiRespSta_m,
  eWiFiRespSta_s,
  eWiFiRespStaRecv0x30,
  eWiFiRespStaRecv0x22,
  eWiFiRespStaRecv0x7D,
  
}EnumNetResp;
u8 NetFrameRecv(char* pFrame)
{
    char        Head[6]={'m','e','t','h','o','d'};
    char 		DataTemp = 0;
	u16 		FrameLen = 0;
    int         TickWaitTime = 5;
	UBaseType_t RecvDataFlag = 0;
	EnumNetResp RecvStatus = eWiFiRespSta_p;

	if(pFrame == NULL)
	return FALSE;

	RecvDataFlag = xQueueReceive(WiFi_MCU_QueueHandle2,&DataTemp,23); //接收WiFi模块信息，且判断是否有数据
	while((RecvDataFlag == pdTRUE) && (FrameLen<(WiFi_MCU_QueueLen-1)))
	{		
		pFrame[FrameLen] = DataTemp;
		FrameLen++;
		pFrame[FrameLen] = 0;
		switch(RecvStatus)
		{
			case eWiFiRespSta_p:
				if(Head[0] == DataTemp)        //接收到0x0D为开始
                {
                    RecvStatus = eWiFiRespSta_a;
                }
                else
                {
                    RecvStatus = eWiFiRespSta_p;
                }	
			break;
			case eWiFiRespSta_a:
				if(Head[1] == DataTemp)
					RecvStatus = eWiFiRespSta_r;
                else if(Head[0] == DataTemp)
                    RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
			break;
            case eWiFiRespSta_r:
				if(Head[2] == DataTemp)
					RecvStatus = eWiFiRespSta_aa;
                else if(Head[0] == DataTemp)
                    RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
			break;
            case eWiFiRespSta_aa:
				if(Head[3] == DataTemp)
					RecvStatus = eWiFiRespSta_m;
                else if(Head[0] == DataTemp)
                    RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
			break;
            case eWiFiRespSta_m:
				if(Head[4] == DataTemp)
					RecvStatus = eWiFiRespSta_s;
                else if(Head[0] == DataTemp)
                    RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
			break;
            case eWiFiRespSta_s:
				if(Head[5] == DataTemp)
                {
                    FrameLen = 0;
				    TickWaitTime = 1000;
                    RecvStatus = eWiFiRespStaRecv0x30;
                }	
                else if(Head[0] == DataTemp)
                    RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
			break;
            case eWiFiRespStaRecv0x30:
                if('0' == DataTemp)
					RecvStatus = eWiFiRespStaRecv0x22;
				else
					RecvStatus = eWiFiRespStaRecv0x30;
			break;
            case eWiFiRespStaRecv0x22:
				if('\"' == DataTemp)
					RecvStatus = eWiFiRespStaRecv0x7D;
                else if('0' == DataTemp)
                    RecvStatus = eWiFiRespStaRecv0x22;
				else
					RecvStatus = eWiFiRespStaRecv0x30;
			break; 
            case eWiFiRespStaRecv0x7D:
				if('}' == DataTemp)
				{
                    return FrameLen;
                }
                else if('0' == DataTemp)
                    RecvStatus = eWiFiRespStaRecv0x22;
				else
					RecvStatus = eWiFiRespStaRecv0x30;
			break; 
			default:
				if(Head[0] == DataTemp)//接收到0x0D
					RecvStatus = eWiFiRespSta_a;
				else
					RecvStatus = eWiFiRespSta_p;
				FrameLen = 0;
				pFrame[FrameLen] = DataTemp;
				FrameLen++;
				pFrame[FrameLen] = 0;
			break;
		}
		RecvDataFlag = xQueueReceive(WiFi_MCU_QueueHandle2,&DataTemp,TickWaitTime);
	}
	pFrame[FrameLen]= 0;
	return FrameLen;
}
signed char NetStringCmp(char*Buff , char* String)
{
    u16     len;
    char*   pFirst;
    u8      hundredDigit = 0;
    u8      TensDigit = 0;
    u8      SingleDigit = 0;
    if((Buff==NULL) || (String==NULL))
    return -1;

    pFirst = strstr(Buff, String);
    if(pFirst != NULL)
    {
        len = strlen(String);
        if (('0' <= pFirst[len+2]) && (pFirst[len+2] <= '9'))//从左往右第3位数
        {
            hundredDigit = pFirst[len] - 48;
            TensDigit = pFirst[len+1] - 48;
            SingleDigit = pFirst[len+2] - 48;
        }
        else if(('0' <= pFirst[len+1]) && (pFirst[len+1] <= '9'))//从左往右第2位数
        {
            TensDigit = pFirst[len] - 48;
            SingleDigit = pFirst[len+1] - 48;
        }
        else
        {
            SingleDigit = pFirst[len] - 48;
        }
        return hundredDigit = hundredDigit*100 + TensDigit*10 + SingleDigit;
    }
    return -1;
}
u8 NetFraccmeProcess(PublishData* pData)
{
    char*   pFirst;
    signed char      Humidity        = 0;
    signed char      temperature     = 0;
    signed char      PowerSwitch_1   = 0;
    signed char      PowerSwitch_2   = 0;
    signed char      PowerSwitch_3   = 0;
    signed char      PowerSwitch_4   = 0;
    char    Frame[WiFi_MCU_QueueLen]={0};
    u16     FrameLen = 0;

    FrameLen = NetFrameRecv(Frame);
    if(FrameLen != 0)
    {	
        pFirst = strstr(Frame, "\"params\"");//第一层识别
        if(pFirst != NULL)
        {
            // temperature   = NetStringCmp(pFirst,"\"temperature\":");
            // if(temperature >= 0)
            // printf("temperature:%d\r\n",temperature);
            // Humidity      = NetStringCmp(pFirst,"\"Humidity\":");
            // if(Humidity >= 0)
            // printf("Humidity:%d\r\n",Humidity);
            PowerSwitch_1 = NetStringCmp(pFirst,"\"PowerSwitch_1\":");
            if(PowerSwitch_1 >= 0)
            pData->Switch1 = PowerSwitch_1;
            // printf("PowerSwitch_1:%d\r\n",PowerSwitch_1);

            PowerSwitch_2 = NetStringCmp(pFirst,"\"PowerSwitch_2\":");
            if(PowerSwitch_2 >= 0)
            pData->Switch2 = PowerSwitch_2;
            // printf("PowerSwitch_2:%d\r\n",PowerSwitch_2);

            PowerSwitch_3 = NetStringCmp(pFirst,"\"PowerSwitch_3\":");
            if(PowerSwitch_3 >= 0)
            pData->Switch3 = PowerSwitch_3;
            // printf("PowerSwitch_3:%d\r\n",PowerSwitch_3);

            PowerSwitch_4 = NetStringCmp(pFirst,"\"PowerSwitch_4\":");
            if(PowerSwitch_4 >= 0)
            pData->Switch4 = PowerSwitch_4;
            // printf("PowerSwitch_4:%d\r\n",PowerSwitch_4);
            
            // memset(pFrame,0,WiFi_MCU_QueueLen);
            // memcpy(pFrame, pFirst, FrameLen);
            // Usart_SendArray(DEBUG_USARTx,Frame,FrameLen);
             
            return TRUE;
        }
    }
    return FALSE;
}
void NetEvent(void)
{

}
void NetRecvProcess(PublishData* Data)
{
    NetFraccmeProcess(Data);

    
}

/***************************************************************************/
u8 NetInit(void)
{
    USART3_Config();
    return TRUE;
}

u8 NetAvail(void)
{
  if((eSelfCtrl_None == WiFiSelfCtrl) && (WiFiProc.Status >= eWiFiProc_Idle))
    return TRUE;
  return FALSE;
}

int NetTCPSend(void)
{
    int Ret = 0;
    u32 SendTick;

    if(FALSE == NetAvail())
    return -1;
    
    WiFiSendSelfCtrl = eSendCtrl_SendReq;
    SendTick = GetSystemTick();
    while((GetSystemTick() - SendTick) < 10000)
    {
        vTaskDelay(100);
        if(FALSE == NetAvail())
        break;
        if((eSendCtrl_SendDone == WiFiSendSelfCtrl) || (eSendCtrl_SendErr == WiFiSendSelfCtrl))
        break;
    }

    if(eSendCtrl_SendDone == WiFiSendSelfCtrl)
        Ret = 0;
    else
        Ret = -4;

    WiFiSendSelfCtrl = eSendCtrl_None;

    return Ret;
}

void NetTcpClose(void)
{
  WiFiSelfCtrl = eSelfCtrl_Disconn;
}


void NetHeartBeatProcess(PublishData pData)
{
    Net.PUBData.Hum = pData.Hum;
    Net.PUBData.Tem = pData.Tem;
    Net.PUBData.Switch1 = pData.Switch1;
    Net.PUBData.Switch2 = pData.Switch2;
    Net.PUBData.Switch3 = pData.Switch3;
    Net.PUBData.Switch4 = pData.Switch4;
    NetTCPSend();
}


