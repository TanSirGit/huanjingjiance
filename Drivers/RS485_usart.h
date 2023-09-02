#ifndef __RS485_H
#define	__RS485_H
#include "stm32f10x.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "Globle.h"
#include "usart.h"


#define     Device_MCU_QueueLen         (24)
#define     MCU_Device_QueueLen         (12)

/**************************串口结构体和消息队列**********************************/
extern QueueHandle_t	Device_MCU_QueueHandle;
extern QueueHandle_t	MCU_Device_QueueHandle;
extern SemaphoreHandle_t RS485_mutex;
// 串口2-USART2
#define  RS485_USARTx                   USART2
#define  RS485_USART_CLK                RCC_APB1Periph_USART2
#define  RS485_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  RS485_USART_BAUDRATE           (9600U)

// USART GPIO 引脚宏定义
#define  RS485_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC)
#define  RS485_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  RS485_USART_TX_GPIO_PORT       GPIOA  
#define  RS485_USART_TX_GPIO_PIN        GPIO_Pin_2
#define  RS485_USART_RX_GPIO_PORT       GPIOA
#define  RS485_USART_RX_GPIO_PIN        GPIO_Pin_3

#define  RS485_USART_IRQ                USART2_IRQn
#define  RS485_USART_IRQHandler         USART2_IRQHandler

//RS485使能引脚 
#define  RS485_USART_EN_GPIO_PORT       GPIOC
#define  RS485_USART_EN_GPIO_PIN        GPIO_Pin_2

#define RS485_RX_ENABLE 		 	 GPIO_ResetBits(RS485_USART_EN_GPIO_PORT,RS485_USART_EN_GPIO_PIN);
#define RS485_TX_ENABLE			 	 GPIO_SetBits(RS485_USART_EN_GPIO_PORT,RS485_USART_EN_GPIO_PIN);



void USART2_Config(void);
u8 RS485SendData(u8* Buf,u8 Count);



#endif 




