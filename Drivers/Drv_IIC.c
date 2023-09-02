/******************************************************************************
文件  : Drv_IIC.c
描述  : IIC驱动
作者  :
修改  : 2023-06-17  谭梓盛    建立
******************************************************************************/
#include "Drv_IIC.h"

static void I2cDelayUs(u32 Us)
{
  u32 i;
  while(Us)
  {
    for(i=0;i<29;i++)
    {}
    Us--;
  }
}

void SetSclOut(void)//SCL信号线设置为开漏输出
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSclIn(void)//SCL信号线设置为输入
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSclOutExt(void)//SCL信号线设置为推挽输出
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaOut(void)//SDA信号线设置为开漏输出
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaIn(void)//SDA信号线设置为输入
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaOutExt(void)//SDA信号线设置为推挽输出
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

/******************************************************************************
函数  : I2cStart
描述  : I2C总线开始信号
参数  : 无
返回  : TRUE - 初始化完成
        FALSE - 初始化有误
******************************************************************************/
u8 I2cStart(void)
{
  u8 i;

  SclHi();
  SdaHi();
  I2cDelay();
  SetSclIn();
  SetSdaIn();
  if(0 == SdaRead() || (0 == SclRead()))//总线有问题
  {
    SetSclOutExt();
    for(i=0; i<10; i++)//10个clk解锁
    {
      SclHi();
      I2cDelay();
      I2cDelay();
      SclLo();
      I2cDelay();
      I2cDelay();
    }
    SetSdaOutExt();//结束
    SdaLo();
    I2cDelay();
    SclHi();
    I2cDelay();
    SdaHi();

    SetSclIn();
    SetSdaIn();
    I2cDelay();
    if(0 == SdaRead() || (0 == SclRead()))
    {
      SetSclOut();
      SetSdaOut();
      return FALSE;
    }
  }
  /*设置为推挽输出，增加驱动力*/
  SetSclOutExt();
  SetSdaOutExt();
  I2cDelay();
  SdaLo();
  I2cDelay();
  SclLo();
  I2cDelay();
  return TRUE;
}

/******************************************************************************
函数  : I2cReStart
描述  : I2C总线重新开始
参数  : 无
返回  : 无
******************************************************************************/
void I2cReStart(void)
{
  SdaHi();
  SclHi();
  I2cDelay();
  SdaLo();
  I2cDelay();
  SclLo();
  I2cDelay();
}

/******************************************************************************
函数  : I2cStart
描述  : I2C总线结束信号
参数  : 无
返回  : 无
******************************************************************************/
void I2cStop(void)
{
  SdaLo();   //PA2(SDA)脚=0；0v
  I2cDelay();
  SclHi();    //PA1(SCL)脚=1；3.3V
  I2cDelay();
  SdaHi();    //PA2(SDA)脚=1；3.3V
  I2cDelay();
  /*设置为开漏输出*/
  SetSclOut();
  SetSdaOut();
}

/******************************************************************************
函数  : I2cSendByte
描述  : I2C总线发送一个字节
参数  : Data - 发送的字节
返回  : TRUE - 成功
        FALSE - 失败
******************************************************************************/
u8 I2cSendByte(u8 Data)
{
  u8 i;
  u8 Bit;
  u8 Rtn;

  Bit = 0x80;
  for(i=0; i<8; i++)
  {
    if(Data & Bit)
      SdaHi();//SDA脚发1
    else
      SdaLo();//SDA脚发0
    I2cDelay();
    SclHi();    //PA1(SCL)脚=1；3.3V
    I2cDelay();
    I2cDelay();
    SclLo();   //PA1(SCL)脚=0；0v
    Bit = Bit >> 1;
    I2cDelay();
  }

  SdaHi();
  SetSdaIn();
  I2cDelay();
  SclHi();
  I2cDelay();
  if(SdaRead())
    Rtn = FALSE;
  else
    Rtn = TRUE;
  I2cDelay();
  SclLo();
  I2cDelay();
  SetSdaOutExt();
  return Rtn;
}

/******************************************************************************
函数  : I2cReadByte
描述  : I2C总线读一个字节
参数  : Ack - 0 发送应答 1 非应答
返回  : 返回读到的数据
******************************************************************************/
u8 I2cReadByte(u8 Ack)
{
  u8 i;
  u8 Data;

  Data = 0;
  SetSdaIn();
  for(i=0; i<8; i++)
  {
    Data = Data << 1;
    I2cDelay();
    SclHi();    //PA1(SCL)脚=1；3.3V
    I2cDelay();
    if(SdaRead())
      Data |= 0x01;
    I2cDelay();
    SclLo();   //PA1(SCL)脚=0；0v
    I2cDelay();
  }
  SetSdaOutExt();

  if(Ack)
    SdaHi();
  else
    SdaLo();
  I2cDelay();
  SclHi();
  I2cDelay();
  I2cDelay();
  SclLo();
  I2cDelay();
  return Data;
}

/******************************************************************************
函数  : I2cDeviceInit
描述  : I2cDevice接口初始化。
参数  : 无
返回  : TRUE - 初始化完成
        FALSE - 初始化有误
******************************************************************************/
u8 I2cDeviceInit(void)
{
  u8 i;

  RCC_APB2PeriphClockCmd(IIC_SCL_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(IIC_SDA_GPIO_CLK, ENABLE);
  SclHi();
  SdaHi();
  SetSdaOut();
  SetSclOut();

  I2cDelay();
  SetSclIn();
  SetSdaIn();
  if(0 == SdaRead() || (0 == SclRead()))
  {//总线有问题
    SetSclOutExt();
    for(i=0; i<10; i++)//9个clk解锁
    {
      SclHi();
      I2cDelay();
      I2cDelay();
      SclLo();
      I2cDelay();
      I2cDelay();
    }
    SetSdaOutExt();//结束
    SdaLo();
    I2cDelay();
    SclHi();
    I2cDelay();
    SdaHi();

    SetSclIn();
    SetSdaIn();
    I2cDelay();
    if(0 == SdaRead() || (0 == SclRead()))
    {
//      SclHi();
//      SdaHi();
      SetSclOut();
      SetSdaOut();
      return FALSE;
    }
  }

//  SclHi();
//  SdaHi();
    SetSclOut();
    SetSdaOut();

    return TRUE;
}













