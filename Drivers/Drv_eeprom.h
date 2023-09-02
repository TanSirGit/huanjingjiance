#ifndef __DRV_EEPROM_H
#define	__DRV_EEPROM_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Drv_IIC.h"
#include "semphr.h"
#include "Globle.h" 

#define E2rom_SlaveAddr                  (0xA0)    //24C02设备地址
#define E2rom_PageSize                   (8)       //24C02页大小

typedef enum
{
  eE2romErr_Sucess = 0, //成功
  eE2romErr_DataPiont,  //数据指针为空
  eE2romErr_WriteOver,  //写数据超范围
  eE2romErr_ReadOver,   //读数据超范围
  eE2romErr_BusErr,     //总线错误
  eE2romErr_WriteCmd,   //设备从地址写命令无应答
  eE2romErr_ReadCmd,    //设备从地址读命令无应答
  eE2romErr_RegAddrHi,  //寄存器高地址无应答
  eE2romErr_RegAddrLo,  //寄存器低地址无应答
  eE2romErr_WriteData,  //写数据无应答
  eE2romErr_Error,      //其他故障
}EnumE2romError;






u8 E2romInit(void);
u32 eeprom_write(u32 Addr, u32 Len, const u8 *pData);
u32 eeprom_read(u32 Addr, u32 Len, u8 *pData);
#endif 



