#include "temperature.h"

TemHumstruct TemHum;
static u8 TemHumWriteAddrcount;

u8 TemHumWriteAddr(void)//д�豸��ַ2
{
    u8 Frame[]={0x01,0x06,0x00,0x64,0x00,0x02};
    u8 ReadBuf[Device_MCU_QueueLen]={0};

    if(TemHumWriteAddrcount == 1)
        return TRUE;      
    TemHumWriteAddrcount = 1;

    ModBusSendHandle(Frame,sizeof(Frame));
   if(ModBusRevHandle(ReadBuf) == TRUE)
    {
        TemHum.ErrCnt = 0;
        return TRUE;   
    }

    TemHum.ErrCnt++;
    return FALSE;
}



u8 TemHumReadAddr(void)//��ȡ�豸��ַ0-FF
{
    u8 Frame[]={0xFF,0x03,0x00,0x64,0x00,0x01};
    u8 ReadBuf[Device_MCU_QueueLen]={0};

    ModBusSendHandle(Frame,sizeof(Frame));
    if(ModBusRevHandle(ReadBuf) == TRUE)
    {
        TemHum.ErrCnt = 0;
        return TRUE;   
    }

    TemHum.ErrCnt++;
    return FALSE;
}

u8 TemHumReadVolue(void)//��ȡ�¶�ʪ��
{
    u8 Frame[]={0x02,0x03,0x00,0x00,0x00,0x02};
    u8 ReadBuf[Device_MCU_QueueLen]={0};

    ModBusSendHandle(Frame,sizeof(Frame));
    if(ModBusRevHandle(ReadBuf) == TRUE)
    {
        TemHum.Tem = (ReadBuf[3]<<8) | ReadBuf[4];
        TemHum.Hum = (ReadBuf[5]<<8) | ReadBuf[6];
        TemHum.ErrCnt = 0;
        return TRUE;   
    }

    TemHum.ErrCnt++;
    return FALSE;
}






