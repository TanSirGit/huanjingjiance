#ifndef __DRV_IIC_H
#define	__DRV_IIC_H
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Globle.h" 

#define I2cDelay()  I2cDelayUs(5)

#define SclHi()     GPIO_SetBits(IIC_SCL_GPIO_PORT, IIC_SCL_PIN)
#define SclLo()     GPIO_ResetBits(IIC_SCL_GPIO_PORT, IIC_SCL_PIN)
#define SclRead()   GPIO_ReadInputDataBit(IIC_SCL_GPIO_PORT, IIC_SCL_PIN)

#define SdaHi()     GPIO_SetBits(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)
#define SdaLo()     GPIO_ResetBits(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)
#define SdaRead()   GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT, IIC_SDA_PIN)


#define IIC_SCL_GPIO_CLK               RCC_APB2Periph_GPIOB //时钟
#define IIC_SCL_GPIO_PORT              GPIOB                //端口
#define IIC_SCL_PIN                    GPIO_Pin_6          //引脚

#define IIC_SDA_GPIO_CLK               RCC_APB2Periph_GPIOB //时钟
#define IIC_SDA_GPIO_PORT              GPIOB                //端口
#define IIC_SDA_PIN                    GPIO_Pin_7          //引脚


void SetSclOut(void);
void SetSclIn(void);
void SetSclOutExt(void);
void SetSdaOut(void);
void SetSdaIn(void);
void SetSdaOutExt(void);
u8 I2cStart(void);
void I2cReStart(void);
void I2cStop(void);
u8 I2cSendByte(u8 Data);
u8 I2cReadByte(u8 Ack);
u8 I2cDeviceInit(void);
#endif 

