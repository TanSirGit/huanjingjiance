#include "Relay.h"

Relaystruct Relay;

u8 OpenSingleRelay(u8 num)//打开1号继电器
{
    u8 Frame[]={0x01, 0x05, 0x00, 0x00, 0xFF, 0x00};
    u8 CmpFrame[]={0x01, 0x05, 0x00, 0x00, 0xFF, 0x00};
    u8 ReadBuf[Device_MCU_QueueLen]={0};
    u32 RevTick;

    if(num<1 || num>4)
    return FALSE;

    Frame[3] = num-1;
    CmpFrame[3] = num-1;
    
    ModBusSendHandle(Frame,sizeof(Frame));
    RevTick = GetSystemTick();
    while ((GetSystemTick()-RevTick) < 50)
    {
        if(ModBusRevHandle(ReadBuf) == TRUE)
        {
            if(memcmp(CmpFrame,ReadBuf,sizeof(CmpFrame)) == 0)
            {
                Relay.ErrCnt = 0;
                if(num == 1)
                Relay.RelayStatus1 = Open;
                else if(num == 2)
                Relay.RelayStatus2 = Open;
                else if(num == 3)
                Relay.RelayStatus3 = Open;
                else if(num == 4)
                Relay.RelayStatus4 = Open;
                return TRUE; 
            }
        }
    }
    Relay.ErrCnt++;
    return FALSE;
}

u8 CloseSingleRelay(u8 num)//关闭1号继电器
{
    u8 Frame[]={0x01, 0x05, 0x00, 0x00, 0x00, 0x00};
    u8 CmpFrame[]={0x01, 0x05, 0x00, 0x00, 0x00, 0x00};
    u8 ReadBuf[Device_MCU_QueueLen]={0};
    u32 RevTick;

    if(num<1 || num>4)
    return FALSE;

    Frame[3] = num-1;
    CmpFrame[3] = num-1;

    ModBusSendHandle(Frame,sizeof(Frame));
    RevTick = GetSystemTick();
    while ((GetSystemTick()-RevTick) < 50)
    {
        if(ModBusRevHandle(ReadBuf) == TRUE)
        {
            if(memcmp(CmpFrame,ReadBuf,sizeof(CmpFrame)) == 0)
            {
                Relay.ErrCnt = 0;
                if(num == 1)
                Relay.RelayStatus1 = Close;
                else if(num == 2)
                Relay.RelayStatus2 = Close;
                else if(num == 3)
                Relay.RelayStatus3 = Close;
                else if(num == 4)
                Relay.RelayStatus4 = Close;
                return TRUE; 
            }
        }
    }
    Relay.ErrCnt++;
    return FALSE;
}

u8 OpenAllRelay(void)//打开所有继电器
{
    u8 Frame[]={0x01,0x0F,0x00,0x00,0x00,0x08,0x01,0xFF};
    u8 CmpFrame[]={0x01,0x0F,0x00,0x00,0x00,0x08};
    u8 ReadBuf[Device_MCU_QueueLen]={0};
    u32 RevTick;

    ModBusSendHandle(Frame,sizeof(Frame));
    RevTick = GetSystemTick();
    while ((GetSystemTick()-RevTick) < 50)
    {
        if(ModBusRevHandle(ReadBuf) == TRUE)
        {
            if(memcmp(CmpFrame,ReadBuf,sizeof(CmpFrame)) == 0)
            {
                Relay.ErrCnt = 0;
                Relay.RelayStatus1 = Open;
                Relay.RelayStatus2 = Open;
                Relay.RelayStatus3 = Open;
                Relay.RelayStatus4 = Open;
                return TRUE; 
            }
        }
    }   
    Relay.ErrCnt++;
    return FALSE;
}

u8 CloseAllRelay(void)//关闭所有继电器
{
    u8 Frame[]={0x01,0x0F,0x00,0x00,0x00,0x08,0x01,0x00};
    u8 CmpFrame[]={0x01,0x0F,0x00,0x00,0x00,0x08};
    u8 ReadBuf[Device_MCU_QueueLen]={0};
    u32 RevTick;

    ModBusSendHandle(Frame,sizeof(Frame));
    RevTick = GetSystemTick();
    while ((GetSystemTick()-RevTick) < 50)
    {
        if(ModBusRevHandle(ReadBuf) == TRUE)
        {
            if(memcmp(CmpFrame,ReadBuf,sizeof(CmpFrame)) == 0)
            {
                Relay.ErrCnt = 0;
                Relay.RelayStatus1 = Close;
                Relay.RelayStatus2 = Close;
                Relay.RelayStatus3 = Close;
                Relay.RelayStatus4 = Close;
                return TRUE; 
            }
        }
    }
    Relay.ErrCnt++;
    return FALSE;
}

u8 ReadRelayStatus(void)//读取继电器状态 
{
    u8 Frame[]={0x01,0x01,0x00,0x00,0x00,0x08};
    u8 CmpFrame[]={0x01,0x01,0x01};
    u8 ReadBuf[Device_MCU_QueueLen]={0};
    u32 RevTick;

    ModBusSendHandle(Frame,sizeof(Frame));
    RevTick = GetSystemTick();
    while ((GetSystemTick()-RevTick) < 50)
    {
        if(ModBusRevHandle(ReadBuf) == TRUE)
        {
            if(memcmp(CmpFrame,ReadBuf,sizeof(CmpFrame)) == 0)
            {
                Relay.ErrCnt = 0;
                if((ReadBuf[3]&0x01) == 0x01)
                    Relay.RelayStatus1 = Open;
                else
                    Relay.RelayStatus1 = Close;
                if((ReadBuf[3]&0x02) == 0x02)
                    Relay.RelayStatus2 = Open;
                else
                    Relay.RelayStatus2 = Close;
                if((ReadBuf[3]&0x04) == 0x04)
                    Relay.RelayStatus3 = Open;
                else
                    Relay.RelayStatus3 = Close;
                if((ReadBuf[3]&0x08) == 0x08)
                    Relay.RelayStatus4 = Open;
                else
                    Relay.RelayStatus4 = Close;
                return TRUE; 
            }
        }
    }  
    Relay.ErrCnt++;
    return FALSE;
}

u8 RelayCtrProcess(Relaystruct Data)
{
    if(Data.RelayStatus1 != Relay.RelayStatus1)
    {
        if(Data.RelayStatus1 == 1)
        {
            OpenSingleRelay(1);
        }           
        else
        {
            CloseSingleRelay(1);
        }           
    }
    if(Data.RelayStatus2 != Relay.RelayStatus2)
    {
        if(Data.RelayStatus2 == 1)
        {
            OpenSingleRelay(2);
        }           
        else
        {
            CloseSingleRelay(2);
        }            
    }
    if(Data.RelayStatus3 != Relay.RelayStatus3)
    {
        if(Data.RelayStatus3 == 1)
        {
            OpenSingleRelay(3);
        }   
        else
        {
            CloseSingleRelay(3);
        }   
    }
    if(Data.RelayStatus4 != Relay.RelayStatus4)
    {
        if(Data.RelayStatus4 == 1)
        {
            OpenSingleRelay(4);
        }        
        else
        {
            CloseSingleRelay(4);
        }           
    }
    return TRUE; 
}











