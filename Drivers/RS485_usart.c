#include "RS485_usart.h"

 /********************* ���о�� ***********************/
 QueueHandle_t	Device_MCU_QueueHandle=NULL;
 QueueHandle_t	MCU_Device_QueueHandle=NULL;
SemaphoreHandle_t RS485_mutex = NULL;
 /********************* �������� ***********************/

static void NVIC_Configuration2(void)
{
  NVIC_InitTypeDef NVIC_InitStructure2;
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure2.NVIC_IRQChannel = RS485_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 2;
  /* �����ȼ� */
  NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж� */
  NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure2);
}

void USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure2;
	USART_InitTypeDef USART_InitStructure2;
	// �򿪴���GPIO��ʱ��
	RS485_USART_GPIO_APBxClkCmd(RS485_USART_GPIO_CLK, ENABLE);
	// �򿪴��������ʱ��
	RS485_USART_APBxClkCmd(RS485_USART_CLK, ENABLE);
	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_TX_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_USART_TX_GPIO_PORT, &GPIO_InitStructure2);
  	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_RX_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(RS485_USART_RX_GPIO_PORT, &GPIO_InitStructure2);
	//RS485
	GPIO_InitStructure2.GPIO_Pin = RS485_USART_EN_GPIO_PIN;
	GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(RS485_USART_EN_GPIO_PORT, &GPIO_InitStructure2);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure2.USART_BaudRate = RS485_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure2.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure2.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure2.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure2.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure2.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(RS485_USARTx, &USART_InitStructure2);
	// �����ж����ȼ�����
	NVIC_Configuration2();
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(RS485_USARTx, USART_IT_RXNE, ENABLE);	
	// ʹ�ܴ���
	USART_Cmd(RS485_USARTx, ENABLE);

    Device_MCU_QueueHandle = xQueueCreate(Device_MCU_QueueLen,sizeof(char));
    MCU_Device_QueueHandle = xQueueCreate(MCU_Device_QueueLen,sizeof(char));
	RS485_mutex = xSemaphoreCreateMutex();
   
}

/****************************�����жϷ�����*******************************/
void RS485_USART_IRQHandler(void)
{
	u16 CR1,SR;
  	char RXTemp = 0;
	char TXTemp = 0;
	BaseType_t status;

	SR = RS485_USARTx->SR;
	CR1 = RS485_USARTx->CR1;

	//USART_CR1:RXNEIE ���ջ������ǿ��ж�ʹ��
	//USART_SR: RXNE ���Ĵ����ǿ�,����һλһλ�Ĵ�RXת�Ƶ���λ�Ĵ�������ʱRXNE��־λ��1��������Ǽ�⵽RXNEΪ1���Ϳ��Դ�RDR�ж����ݡ�
	if((CR1 & USART_CR1_RXNEIE) && (SR & USART_SR_RXNE))
	{
		RXTemp = (u8)RS485_USARTx->DR;
		xQueueSendFromISR(Device_MCU_QueueHandle,&RXTemp,pdFALSE);
	}
	//USART_CR1:TXEIE ���ͻ��������ж�ʹ�ܣ���Ϊ1ʱ��TDRΪ�գ�TXE = 1���ͻ�����жϡ�
	//USART_SR: TXE �������ݼĴ����գ���TDR�Ĵ����е����ݱ�Ӳ��ת�Ƶ���λ�Ĵ�����ʱ����λ��1����Ϊ0ʱ����ʾ���ݻ�û��ת�Ƶ���λ�Ĵ�������ʱDMA���������ﴫ�����ˡ�
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
	/* �ȴ�������� */
	while(USART_GetFlagStatus(RS485_USARTx,USART_FLAG_TC)==RESET);

	vTaskDelay(1);
	RS485_RX_ENABLE;
	vTaskDelay(1);

    return TRUE;
}





