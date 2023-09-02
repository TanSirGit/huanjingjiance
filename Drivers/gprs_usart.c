#include "gprs_usart.h"

 /********************* ���о�� ***********************/
 QueueHandle_t	WiFi_MCU_QueueHandle=NULL;
 QueueHandle_t	WiFi_MCU_QueueHandle2=NULL;
 QueueHandle_t	MCU_WiFi_QueueHandle=NULL;

 /*********** ȫ�ֱ������壨��ͷ�ļ����á�extern��������******/
StructWiFiCmd WiFiCmd;
StructWiFiProc WiFiProc;
 /********************* �������� ***********************/
void GPIO_Power_On(void)
{
	GPIO_InitTypeDef GPIO_InitStructurePower;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructurePower.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructurePower.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructurePower.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructurePower);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	vTaskDelay(500);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	vTaskDelay(500);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	vTaskDelay(500);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
static void NVIC_Configuration3(void)
{
  NVIC_InitTypeDef NVIC_InitStructure3;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure3.NVIC_IRQChannel = GPRS_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 2;
  /* �����ȼ� */
  NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж� */
  NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure3);
}

void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure3;
	USART_InitTypeDef USART_InitStructure3;

	// �򿪴���GPIO��ʱ��
	GPRS_USART_GPIO_APBxClkCmd(GPRS_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	GPRS_USART_APBxClkCmd(GPRS_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure3.GPIO_Pin = GPRS_USART_TX_GPIO_PIN;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPRS_USART_TX_GPIO_PORT, &GPIO_InitStructure3);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure3.GPIO_Pin = GPRS_USART_RX_GPIO_PIN;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPRS_USART_RX_GPIO_PORT, &GPIO_InitStructure3);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure3.USART_BaudRate = GPRS_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure3.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure3.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure3.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure3.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure3.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(GPRS_USARTx, &USART_InitStructure3);
	
	// �����ж����ȼ�����
	NVIC_Configuration3();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(GPRS_USARTx, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(GPRS_USARTx, ENABLE);	  
	
	WiFi_MCU_QueueHandle = xQueueCreate(WiFi_MCU_QueueLen,sizeof(char));
	WiFi_MCU_QueueHandle2 = xQueueCreate(WiFi_MCU_QueueLen,sizeof(char));
	MCU_WiFi_QueueHandle = xQueueCreate(MCU_WiFi_QueueLen,sizeof(char)); 
}



//////////////////////////////////////////////////////////////////////////////////

// �����жϷ�����
void GPRS_USART_IRQHandler(void)
{
  	char RXTemp = 0;
	char TXTemp = 0;
	BaseType_t status;

	u16 CR1,SR;
	SR = GPRS_USARTx->SR;
	CR1 = GPRS_USARTx->CR1;

	// if(USART_GetITStatus(GPRS_USARTx,USART_IT_RXNE)!=RESET)
	// {		
	// 	RXTemp = USART_ReceiveData(GPRS_USARTx);
    
    // 	xQueueSendFromISR(WiFi_MCU_QueueHandle,&RXTemp,NULL);
	// 	xQueueSendFromISR(WiFi_MCU_QueueHandle2,&RXTemp,NULL);	
	// 	USART_ClearFlag(GPRS_USARTx,USART_IT_RXNE);
	// }
	
	// if(USART_GetITStatus(GPRS_USARTx,USART_IT_TXE)!=RESET)
	// {	
	// 	status = xQueueReceiveFromISR(MCU_WiFi_QueueHandle,&TXTemp,pdFALSE);	
	// 	// if(uxQueueMessagesWaitingFromISR(MCU_WiFi_QueueHandle) != 0)
	// 	if(status == pdTRUE)
	// 	{
	// 		// xQueueReceiveFromISR(MCU_WiFi_QueueHandle,&TXTemp,NULL);

	// 		USART_SendData(GPRS_USARTx,TXTemp);
	// 	}
	// 	else
	// 	{
	// 		USART_ITConfig(GPRS_USARTx, USART_IT_TXE, DISABLE);	
	// 	}
	// }

	/*USART_CR1:RXNEIE ���ջ������ǿ��ж�ʹ��*/
	/*USART_SR: RXNE ���Ĵ����ǿ�,����һλһλ�Ĵ�RXת�Ƶ���λ�Ĵ�������ʱRXNE��־λ��1��������Ǽ�⵽RXNEΪ1���Ϳ��Դ�RDR�ж����ݡ�*/
	if((CR1 & USART_CR1_RXNEIE) && (SR & USART_SR_RXNE))
	{
		RXTemp = (u8)GPRS_USARTx->DR;
		xQueueSendFromISR(WiFi_MCU_QueueHandle,&RXTemp,pdFALSE);
		xQueueSendFromISR(WiFi_MCU_QueueHandle2,&RXTemp,pdFALSE);	
	}
	/*USART_CR1:TXEIE ���ͻ��������ж�ʹ�ܣ���Ϊ1ʱ��TDRΪ�գ�TXE = 1���ͻ�����жϡ�*/
	/*USART_SR: TXE �������ݼĴ����գ���TDR�Ĵ����е����ݱ�Ӳ��ת�Ƶ���λ�Ĵ�����ʱ����λ��1����Ϊ0ʱ����ʾ���ݻ�û��ת�Ƶ���λ�Ĵ�������ʱDMA���������ﴫ�����ˡ�*/
	if((CR1 & USART_CR1_TXEIE) && (SR & USART_SR_TXE))
	{
		status = xQueueReceiveFromISR(MCU_WiFi_QueueHandle,&TXTemp,pdFALSE);
		if(status == pdTRUE)
			GPRS_USARTx->DR = TXTemp;
		else
			GPRS_USARTx->CR1 &= ~USART_CR1_TXEIE;
	} 
}

u8 WiFiSend(char ch)
{
    xQueueSend(MCU_WiFi_QueueHandle,&ch,NULL);
    return TRUE;
}

u8 WiFiSendStar(void)
{
    // USART_ITConfig(GPRS_USARTx, USART_IT_TXE, ENABLE);
	GPRS_USARTx->CR1 |= USART_CR1_TXEIE;
    return TRUE;
}

u8 WiFiSendByte(char ch)
{
	BaseType_t status;
    status = xQueueSend(MCU_WiFi_QueueHandle,&ch,portMAX_DELAY);
    if(status == pdTRUE)
	{
		WiFiSendStar();
    	return TRUE;
	}
	return FALSE;
}

u8 WiFiSendArray(char *array, uint16_t num)
{
  	uint8_t i;
	for(i=0; i<num; i++)
	{
		/* ����һ���ֽ����ݵ����� */
		xQueueSend(MCU_WiFi_QueueHandle,&array[i],NULL);
	}
	WiFiSendStar();
    return TRUE;
}

u8 WiFiSendCmd(char *pRespEx,char* Cmd)
{
	char Buf[MCU_WiFi_QueueLen];
    u8 Datalen 	= 0;
    int Count 	= 0;
	WiFiCmd.Sta = 0;
  	WiFiCmd.Ack = 0;
	WiFiCmd.pRespExFlag = pRespEx;
	
    if(Cmd == NULL)
    return FALSE;
	
	Count = snprintf(Buf,MCU_WiFi_QueueLen,(const char*)Cmd);

    while (Count)
    {
        xQueueSend(MCU_WiFi_QueueHandle,&Buf[Datalen],NULL);
		// WiFiSendByte(Buf[Datalen]);
        Datalen++;
        Count--;
    }
    WiFiSendStar();
	WiFiCmd.Sta = 1;//0δʹ��  1�ѷ��� 2��Ӧ��
    return TRUE;
}

typedef enum
{
  eWiFiRespSta_0D = 0,
  eWiFiRespSta_0A,
  eWiFiRespSta_C,
}EnumWiFiResp;
u8 WiFi_Cmd_RecieveFrame(char* pFrame)
{
	char 		DataTemp = 0;
	u16 		FrameLen = 0;
	UBaseType_t RecvDataFlag = 0;
	EnumWiFiResp RecvStatus = eWiFiRespSta_0D;

	if(pFrame == NULL)
	return FALSE;

	RecvDataFlag = xQueueReceive(WiFi_MCU_QueueHandle,&DataTemp,23); //����WiFiģ����Ϣ�����ж��Ƿ�������portMAX_DELAY
	while((RecvDataFlag == pdTRUE) && (FrameLen<(WiFi_MCU_QueueLen-1)))
	{
	// while((FrameLen<(WiFi_MCU_QueueLen-1)) && (RecvDataFlag != 0))
	// {
    //     xQueueReceive(WiFi_MCU_QueueHandle,&DataTemp,100);

		pFrame[FrameLen] = DataTemp;
		FrameLen++;
		pFrame[FrameLen] = 0;
		switch(RecvStatus)
		{	
			case eWiFiRespSta_0D:
				if(0x0D == DataTemp)//���յ�0x0DΪ��ʼ
					RecvStatus = eWiFiRespSta_0A;
				else
					RecvStatus = eWiFiRespSta_0D;
				break;
			case eWiFiRespSta_0A:
				if(0x0A == DataTemp)//���յ�0x0AΪ����
				{
					pFrame[FrameLen] = 0;
					if(FrameLen > 2)//�����ݷ���
						return FrameLen;
					else			//�����ݣ��������㣬��������
					{
						// RecvStatus = eWiFiRespSta_0D;
						// FrameLen = 0;
						RecvStatus = eWiFiRespSta_C;
					}
				}
				else if(0x0D == DataTemp)
					RecvStatus = eWiFiRespSta_0A;
				else
					RecvStatus = eWiFiRespSta_0D;
				break;
			case eWiFiRespSta_C:
				if('>' == DataTemp)
					return FrameLen;
				else if(0x0D == DataTemp)
					RecvStatus = eWiFiRespSta_0A;
				else
					RecvStatus = eWiFiRespSta_0D;
				break;
			default:
				if(0x0D == DataTemp)//���յ�0x0D
					RecvStatus = eWiFiRespSta_0A;
				else
					RecvStatus = eWiFiRespSta_0D;
				FrameLen = 0;
				pFrame[FrameLen] = DataTemp;
				FrameLen++;
				pFrame[FrameLen] = 0;
				break;
		}
		// RecvDataFlag = uxQueueMessagesWaiting(WiFi_MCU_QueueHandle);
		RecvDataFlag = xQueueReceive(WiFi_MCU_QueueHandle,&DataTemp,5);
	}
	pFrame[FrameLen]= 0;
	return FrameLen;
}

u8 WiFi_Cmd_WaitAck(u32 Ms)
{
	u32 Cnt = Ms/13 + 1;
	do
	{
		vTaskDelay(13);
		if(2 == WiFiCmd.Sta)
		return WiFiCmd.Ack;
		Cnt--;
	}while(Cnt);

	return 0xFF;//��ʱ
}

void WiFi_Cmd_Reciece(void)
{
	char Frame[WiFi_MCU_QueueLen]={0};
	char *pStr;
	u16 Len;

	Len = WiFi_Cmd_RecieveFrame(Frame);
	if(0 != Len)//MCU���յ���Ϣ
	{
		
		if(1 == WiFiCmd.Sta)//MCU�ѷ���ATָ��
		{	
			if(NULL != WiFiCmd.pRespExFlag)
			{
				pStr = strstr(Frame, WiFiCmd.pRespExFlag);//����pRespExFlag����
				if(pStr)
				{
					memset(WiFiCmd.RespExBuf,0,WiFi_MCU_QueueLen);
					memcpy(WiFiCmd.RespExBuf, pStr, Len);
					// WiFiCmd.RespExBuf[Len-1] = 0;
					pStr = strstr(WiFiCmd.pRespExFlag, "ERROR");
					if(pStr==NULL)//��ERROR
					{
						return;
					}
				}
			}
				
			if(NULL == WiFiCmd.pRespExFlag)
			{
				memset(WiFiCmd.RespExBuf,0,WiFi_MCU_QueueLen);
				memcpy(WiFiCmd.RespExBuf, Frame, Len);
				// WiFiCmd.RespExBuf[Len-1] = 0;
			}

			// Usart_SendArray(DEBUG_USARTx,Frame,Len);
			// printf("%d\r\n",Len);

			if(NULL != WiFiCmd.pResp_1)
			{
				pStr = strstr(Frame, WiFiCmd.pResp_1);
				if(pStr)
				{
					WiFiCmd.Ack = 1;
					WiFiCmd.Sta = 2;
					return;
				}
			}
			if(NULL != WiFiCmd.pResp_2)
			{
				pStr = strstr(Frame, WiFiCmd.pResp_2);
				if(pStr)
				{
					WiFiCmd.Ack = 2;
					WiFiCmd.Sta = 2;
					return;
				}
			}
		}
  	}
}


