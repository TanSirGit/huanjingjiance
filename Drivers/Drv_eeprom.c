/******************************************************************************
文件  : Drv_eeprom.c
描述  : AT24C02驱动
作者  :
修改  : 2023-06-17  谭梓盛    建立
******************************************************************************/
#include "Drv_eeprom.h"

u8 E2romError = 0;
SemaphoreHandle_t IIC_mutex = NULL;

/******************************************************************************
函数  : E2romInit
描述  : E2rom接口初始化。
参数  : 无
返回  : TRUE - 初始化完成
        FALSE - 初始化有误
******************************************************************************/
u8 E2romInit(void)
{
  if(FALSE == I2cDeviceInit())
    return FALSE;

  IIC_mutex = xSemaphoreCreateMutex();

  if (IIC_mutex == NULL )
    return FALSE;
    
  return TRUE;
}

/******************************************************************************
函数  : E2romWritePage
描述  : AT24C02页写数据
参数  : Addr - 开始地址
        pData - 要写数据的指针
        Len - 要写数据长度
返回  : 参考 EnumE2romError
******************************************************************************/
EnumE2romError E2romWritePage(const u8 *pData, u16 Addr, u16 Len)
{
  EnumE2romError Err;
  u16 i;

  if(NULL == pData)
    return eE2romErr_DataPiont;
  if(Len > (0xFFFF - Addr))
    return eE2romErr_WriteOver;

  if(0 == Len)
    return eE2romErr_Sucess;

  if(FALSE == I2cStart())
  {
    E2romError = 1;
    Err = eE2romErr_BusErr;
    goto __E2romWritReturn__;
  }

  Err = eE2romErr_Sucess;
  if(FALSE == I2cSendByte(E2rom_SlaveAddr))
  {
    E2romError = 2;
    Err = eE2romErr_WriteCmd;
    goto __E2romWritReturn__;
  }
  if(FALSE == I2cSendByte((u8)Addr))
  {
    Err = eE2romErr_RegAddrHi;
    goto __E2romWritReturn__;
  }
//   if(FALSE == I2cSendByte((u8)(Addr>>8)))//24c512使用
//   {
//     Err = eE2romErr_RegAddrHi;
//     goto __E2romWritReturn__;
//   }
//   if(FALSE == I2cSendByte((u8)(Addr&0xFF)))
//   {
//     Err = eE2romErr_RegAddrLo;
//     goto __E2romWritReturn__;
//   }

  for(i=0; i<Len; i++)
  {
    if(FALSE == I2cSendByte(pData[i]))
    {
      Err = eE2romErr_WriteData;
      goto __E2romWritReturn__;
    }
  }

__E2romWritReturn__:
  I2cStop();

  if(eE2romErr_Sucess == Err)
    E2romError = 0;

  return Err;
}
 
/******************************************************************************
函数  : eeprom_write
描述  : AT24C02写数据
参数  : Addr - 开始地址
        pData - 要写数据的指针
        Len - 要写数据长度
返回  : 参考 EnumE2romError
******************************************************************************/
u32 eeprom_write(u32 Addr, u32 Len, const u8 *pData)
{
	u32 Write_Addr = 0;
	u16 NumOfPage = 0;
	u16 NumOfSingle = 0;
	u32 count = 0;
	EnumE2romError Err = eE2romErr_Sucess;

	if(NULL == IIC_mutex)
		return 0;

	if(pdFALSE == xSemaphoreTake(IIC_mutex,5000*portTICK_RATE_MS))//5秒
		return 0;

	Write_Addr = Addr % E2rom_PageSize;

	if(0 != Write_Addr)
	{/*数据长度非页对齐*/
		count = E2rom_PageSize - Write_Addr;
		if(Len <= count)//数据全在一页内
		{
			Err = E2romWritePage(pData, Addr, Len);
			goto __E2RomWrReturn__;
		}

		vTaskDelay(25);
		Err = E2romWritePage(pData, Addr, count);
		if(eE2romErr_Sucess != Err)
			goto __E2RomWrReturn__;

		Addr += count;
		pData += count;
		Len -= count;
	}

	NumOfPage =  Len / E2rom_PageSize;
	NumOfSingle = Len % E2rom_PageSize;

	while(NumOfPage)
	{
		vTaskDelay(25);
		Err = E2romWritePage(pData, Addr, E2rom_PageSize);
		if(eE2romErr_Sucess != Err)
			goto __E2RomWrReturn__;
		Addr +=	E2rom_PageSize;
		pData += E2rom_PageSize;
		NumOfPage--;
	}
	if(NumOfSingle != 0)
	{
		vTaskDelay(25);
		Err = E2romWritePage(pData, Addr, NumOfSingle);
	}

__E2RomWrReturn__:
	xSemaphoreGive(IIC_mutex);

	if(Err == eE2romErr_Sucess)
		return Len;
	else
		return 0;
}


/******************************************************************************
函数  : eeprom_read
描述  : AT24C02读数据
参数  : Addr - 开始地址
        pData - 输出数据的指针
        Len - 要读数据长度
返回  : 参考 EnumE2romError
******************************************************************************/
u32 eeprom_read(u32 Addr, u32 Len, u8 *pData)
{
  EnumE2romError Err;
  u32 i;
	
	vTaskDelay(25);

  if(NULL == IIC_mutex)
    return 0;

  if(NULL == pData)
    return 0;
 // if(Len > (0xFFFF - Addr))
   // return 0;

  if(0 == Len)
    return 0;

  if(pdFALSE == xSemaphoreTake(IIC_mutex,5000*portTICK_RATE_MS))//5秒
    return 0;

  if(FALSE == I2cStart())
  {
    E2romError = 1;
    Err = eE2romErr_BusErr;
    goto __E2romReadReturn__;
  }

  Err = eE2romErr_Sucess;
  if(FALSE == I2cSendByte(E2rom_SlaveAddr))
  {
    E2romError = 2;
    Err = eE2romErr_WriteCmd;
    goto __E2romReadReturn__;
  }
  if(FALSE == I2cSendByte((u8)Addr))
  {
    Err = eE2romErr_RegAddrHi;
    goto __E2romReadReturn__;
  }
//   if(FALSE == I2cSendByte((u8)(Addr>>8)))//24c512使用
//   {
//     Err = eE2romErr_RegAddrHi;
//     goto __E2romReadReturn__;
//   }
//   if(FALSE == I2cSendByte((u8)(Addr&0xFF)))
//   {
//     Err = eE2romErr_RegAddrLo;
//     goto __E2romReadReturn__;
//   }

  I2cReStart();
  if(FALSE == I2cSendByte(E2rom_SlaveAddr|0x01))
  {
    Err = eE2romErr_ReadCmd;
    goto __E2romReadReturn__;
  }

  for(i=0; i<(Len-1); i++)
  {
    pData[i] = I2cReadByte(0);
  }

  pData[i] = I2cReadByte(1);

__E2romReadReturn__:
  I2cStop();
  xSemaphoreGive(IIC_mutex);
  if(eE2romErr_Sucess == Err)
    E2romError = 0;

	if(Err == eE2romErr_Sucess)
 		 return Len;
	else
		return 0;
}















