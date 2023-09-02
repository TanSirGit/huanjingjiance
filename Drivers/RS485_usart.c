#include "RS485_usart.h"

 /********************* 队列句柄 ***********************/
 QueueHandle_t	Device_MCU_QueueHandle=NULL;
 QueueHandle_t	MCU_Device_QueueHandle=NULL;
SemaphoreHandle_t RS485_mutex = NULL;
 /********************* 函数声明 ***********************/

static void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure2;
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* 配置USART为中断源 */
  NVIC_InitStructure2.NVIC_IRQChannel = RS485_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  /* 子优先级 */
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 2;
  /* 使能中断 */
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure2);
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure2;
	USART_InitTypeDef USART_InitStructure2;
	// 打开串口GPIO的时钟
	RS485_USART_GPIO_APBxClkCmd(RS485_USART_GPIO_CLK, ENABLE);
	// 打开串口外设的时钟
	RS485_USART_APBxClkCmd(RS485_USART_CLK, ENABLE);
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_TX_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_USART_TX_GPIO_PORT, &GPIO_InitStructure2);
  	// 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_RX_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RS485_USART_RX_GPIO_PORT, &GPIO_InitStructure2);
	//RS485
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_EN_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RS485_USART_EN_GPIO_PORT, &GPIO_InitStructure2);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure2.USART_BaudRate = RS485_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure2.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure2.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure2.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(RS485_USARTx, &USART_InitStructure2);
	// 串口中断优先级配置
	NVIC_Configuration2();
	// 使能串口接收中断
	USART_ITConfig(RS485_USARTx, USART_IT_RXNE, ENABLE);	
	// 使能串口
	USART_Cmd(RS485_USARTx, ENABLE);

    Device_MCU_QueueHandle = xQueueCreate(Device_MCU_QueueLen,sizeof(char));
    MCU_Device_QueueHandle = xQueueCreate(MCU_Device_QueueLen,sizeof(char));
	RS485_mutex = xSemaphoreCreateMutex();
   
}

/****************************串口中断服务函数*******************************/
void RS485_USART_IRQHandler(void)
{
	u16 CR1,SR;
  	char RXTemp = 0;
	char TXTemp = 0;
	BaseType_t status;

	SR = RS485_USARTx->SR;
	CR1 = RS485_USARTx->CR1;

	//USART_CR1:RXNEIE 接收缓冲区非空中断使能
	//USART_SR: RXNE 读寄存器非空,数据一位一位的从RX转移到移位寄存器，此时RXNE标志位置1，如果我们检测到RXNE为1，就可以从RDR中读数据。
	if((CR1 & USART_CR1_RXNEIE) && (SR & USART_SR_RXNE))
	{
		RXTemp = (u8)RS485_USARTx->DR;
		xQueueSendFromISR(Device_MCU_QueueHandle,&RXTemp,pdFALSE);
	}
	//USART_CR1:TXEIE 发送缓冲区空中断使能，当为1时，TDR为空（TXE = 1）就会产生中断。
	//USART_SR: TXE 发送数据寄存器空，当TDR寄存器中的数据被硬件转移到移位寄存器中时，该位置1；当为0时，表示数据还没有转移到移位寄存器，这时DMA不能再往里传数据了。
	if((CR1 & USART_CR1_TXEIE) && (SR & USART_SR_TXE))
	{
		status = xQueueReceiveFromISR(MCU_Device_QueueHandle,&TXTemp,pdFALSE);
		if(status == pdTRUE)
			RS485_USARTx->DR = TXTemp;
		else
			RS485_USARTx->CR1 &= ~USART_CR1_TXEIE;
	} 
}


static u8 RS485SendStar(void)
{
    RS485_USARTx->CR1 |= USART_CR1_TXEIE;
    return TRUE;
}

u8 RS485SendByte(char ch)
{
	BaseType_t status;
    status = xQueueSend(MCU_Device_QueueHandle,&ch,portMAX_DELAY);
    if(status == pdTRUE)
	{
		RS485SendStar();
    	return TRUE;
	}
	return FALSE;
}

u8 RS485SendData(u8* Buf,u8 Count)
{
    u8 Buflen 	= 0;
	
    if(Buf == NULL)
    return FALSE;

	RS485_TX_ENABLE;
	vTaskDelay(1);

    while (Count)
    {
        xQueueSend(MCU_Device_QueueHandle,&Buf[Buflen],NULL);
        Buflen++;
        Count--;
    }
    RS485SendStar();
	/* 等待发送完成 */
	while(USART_GetFlagStatus(RS485_USARTx,USART_FLAG_TC)==RESET);

	vTaskDelay(1);
	RS485_RX_ENABLE;
	vTaskDelay(1);

    return TRUE;
}





