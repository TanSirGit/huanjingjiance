#ifndef __DRV_EEPROM_H
#define	__DRV_EEPROM_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Drv_IIC.h"
#include "semphr.h"
#include "Globle.h" 

#define E2rom_SlaveAddr                  (0xA0)    //24C02�豸��ַ
#define E2rom_PageSize                   (8)       //24C02ҳ��С

typedef enum
{
  eE2romErr_Sucess = 0, //�ɹ�
  eE2romErr_DataPiont,  //����ָ��Ϊ��
  eE2romErr_WriteOver,  //д���ݳ���Χ
  eE2romErr_ReadOver,   //�����ݳ���Χ
  eE2romErr_BusErr,     //���ߴ���
  eE2romErr_WriteCmd,   //�豸�ӵ�ַд������Ӧ��
  eE2romErr_ReadCmd,    //�豸�ӵ�ַ��������Ӧ��
  eE2romErr_RegAddrHi,  //�Ĵ����ߵ�ַ��Ӧ��
  eE2romErr_RegAddrLo,  //�Ĵ����͵�ַ��Ӧ��
  eE2romErr_WriteData,  //д������Ӧ��
  eE2romErr_Error,      //��������
}EnumE2romError;






u8 E2romInit(void);
u32 eeprom_write(u32 Addr, u32 Len, const u8 *pData);
u32 eeprom_read(u32 Addr, u32 Len, u8 *pData);
#endif 



