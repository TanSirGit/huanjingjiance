/******************************************************************************
�ļ�  : Drv_IIC.c
����  : IIC����
����  :
�޸�  : 2023-06-17  ̷��ʢ    ����
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

void SetSclOut(void)//SCL�ź�������Ϊ��©���
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSclIn(void)//SCL�ź�������Ϊ����
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSclOutExt(void)//SCL�ź�������Ϊ�������
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
  GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaOut(void)//SDA�ź�������Ϊ��©���
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaIn(void)//SDA�ź�������Ϊ����
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

void SetSdaOutExt(void)//SDA�ź�������Ϊ�������
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
  GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure);
}

/******************************************************************************
����  : I2cStart
����  : I2C���߿�ʼ�ź�
����  : ��
����  : TRUE - ��ʼ�����
        FALSE - ��ʼ������
******************************************************************************/
u8 I2cStart(void)
{
  u8 i;

  SclHi();
  SdaHi();
  I2cDelay();
  SetSclIn();
  SetSdaIn();
  if(0 == SdaRead() || (0 == SclRead()))//����������
  {
    SetSclOutExt();
    for(i=0; i<10; i++)//10��clk����
    {
      SclHi();
      I2cDelay();
      I2cDelay();
      SclLo();
      I2cDelay();
      I2cDelay();
    }
    SetSdaOutExt();//����
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
  /*����Ϊ�������������������*/
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
����  : I2cReStart
����  : I2C�������¿�ʼ
����  : ��
����  : ��
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
����  : I2cStart
����  : I2C���߽����ź�
����  : ��
����  : ��
******************************************************************************/
void I2cStop(void)
{
  SdaLo();   //PA2(SDA)��=0��0v
  I2cDelay();
  SclHi();    //PA1(SCL)��=1��3.3V
  I2cDelay();
  SdaHi();    //PA2(SDA)��=1��3.3V
  I2cDelay();
  /*����Ϊ��©���*/
  SetSclOut();
  SetSdaOut();
}

/******************************************************************************
����  : I2cSendByte
����  : I2C���߷���һ���ֽ�
����  : Data - ���͵��ֽ�
����  : TRUE - �ɹ�
        FALSE - ʧ��
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
      SdaHi();//SDA�ŷ�1
    else
      SdaLo();//SDA�ŷ�0
    I2cDelay();
    SclHi();    //PA1(SCL)��=1��3.3V
    I2cDelay();
    I2cDelay();
    SclLo();   //PA1(SCL)��=0��0v
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
����  : I2cReadByte
����  : I2C���߶�һ���ֽ�
����  : Ack - 0 ����Ӧ�� 1 ��Ӧ��
����  : ���ض���������
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
    SclHi();    //PA1(SCL)��=1��3.3V
    I2cDelay();
    if(SdaRead())
      Data |= 0x01;
    I2cDelay();
    SclLo();   //PA1(SCL)��=0��0v
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
����  : I2cDeviceInit
����  : I2cDevice�ӿڳ�ʼ����
����  : ��
����  : TRUE - ��ʼ�����
        FALSE - ��ʼ������
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
  {//����������
    SetSclOutExt();
    for(i=0; i<10; i++)//9��clk����
    {
      SclHi();
      I2cDelay();
      I2cDelay();
      SclLo();
      I2cDelay();
      I2cDelay();
    }
    SetSdaOutExt();//����
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













