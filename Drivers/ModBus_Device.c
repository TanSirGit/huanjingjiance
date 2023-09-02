#include "ModBus_Device.h"



void ModBusSendFrame(u8* pFrame,u8 pFrameLen)
{
    u8 Frame[MCU_Device_QueueLen]={0};
    u8 FrameLen = 0;
    u16 CRCVolue;

    if(pdFALSE == xSemaphoreTake(RS485_mutex,5000*portTICK_RATE_MS))//5秒,开启互斥量
    {
        xSemaphoreGive(RS485_mutex);//释放互斥量
        return;
    }
		

    CRCVolue = CRC16(pFrame,pFrameLen);
    if(pFrameLen <= (sizeof(Frame)-2))
    {
        memcpy(Frame,pFrame,pFrameLen);
        Frame[pFrameLen++] = (CRCVolue & 0xFF);
        Frame[pFrameLen++] = ((CRCVolue>>8) & 0xFF);
        FrameLen = pFrameLen;
        RS485SendData(Frame,FrameLen);
    }
    memset(Frame,0,sizeof(Frame));
}

u8 ModBusRevFrame(u8* pFrame)
{
    u8 		DataTemp = 0;
    u8 Buffer[Device_MCU_QueueLen]={0};
    u16 BufPos = 0;
	u16 		FrameLen = 0;
    u16         CrcCheck;
    u16         RevCRC;
	UBaseType_t RecvDataFlag = pdFALSE;

    if(pFrame == NULL)
    {
        xSemaphoreGive(RS485_mutex);//释放互斥量
        return FALSE;
    }
	
    RecvDataFlag = xQueueReceive(Device_MCU_QueueHandle,&DataTemp,10); //接收WiFi模块信息，且判断是否有数据portMAX_DELAY
	while((RecvDataFlag == pdTRUE) && (BufPos < sizeof(Buffer)))
	{
		Buffer[BufPos] = DataTemp;
		BufPos++;
		if(BufPos >= sizeof(Buffer))
            break;
        RecvDataFlag = xQueueReceive(Device_MCU_QueueHandle,&DataTemp,5);
    }
    if(BufPos > 2)
    {
        FrameLen = BufPos-2;
        CrcCheck = CRC16(Buffer,BufPos-2);
        RevCRC  |= Buffer[BufPos-2];
        RevCRC  |= Buffer[BufPos-1] << 8;
        if(CrcCheck == RevCRC)
        {
            memcpy(pFrame,Buffer,FrameLen);
            memset(Buffer,0,sizeof(Buffer));
            xSemaphoreGive(RS485_mutex);//释放互斥量
            return  TRUE;
        }
    }
    xSemaphoreGive(RS485_mutex);//释放互斥量
    return FALSE;
}

u8 ModBusRevHandle(u8* pFrame)
{
    return ModBusRevFrame(pFrame);
}
void ModBusSendHandle(u8* pFrame,u8 pFrameLen)
{
    ModBusSendFrame(pFrame,pFrameLen); 
}






















