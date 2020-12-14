#include "delay.h"
#include "usart3.h"
#include "stdarg.h"	 	 
#include "stdio.h"	 	 
#include "string.h"	  
#include "led.h"
#include "usart.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����2��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//���ڷ��ͻ����� 	
__align(8) u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 	//���ͻ���,���USART3_MAX_SEND_LEN�ֽ�
#ifdef USART3_RX_EN   								//���ʹ���˽���   	  
//���ڽ��ջ����� 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//���ջ���,���USART3_MAX_RECV_LEN���ֽ�.


//ͨ���жϽ�������2���ַ�֮���ʱ������10ms�������ǲ���һ������������.
//���2���ַ����ռ������10ms,����Ϊ����1����������.Ҳ���ǳ���10msû�н��յ�
//�κ�����,���ʾ�˴ν������.
//���յ�������״̬
//[15]:0,û�н��յ�����;1,���յ���һ������.
//[14:0]:���յ������ݳ���
u16 USART3_RX_STA=0;   	 
void USART3_IRQHandler(void)
{
	u8 res;	    
if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)//���յ�����
	{	 
		res =USART_ReceiveData(USART3);				 
		if(USART3_RX_STA<USART3_MAX_RECV_LEN)		//�����Խ�������
		{
		  TIM_SetCounter(TIM3,0);//���������      
			if(USART3_RX_STA==0)TIM3_Set(1);	 	//ʹ�ܶ�ʱ��4���ж� 
			USART3_RX_BUF[USART3_RX_STA++]=res;		//��¼���յ���ֵ	 
		}else 
		{
			USART3_RX_STA|=1<<15;					//ǿ�Ʊ�ǽ������
		} 
	}  											 
}   
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������	  
void USART3_Init(u32 bound)
{ 	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

 	USART_DeInit(USART3);  //��λ����1
		 //USART3_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PA2
   
    //USART3_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PA3
	
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  
	USART_Init(USART3, &USART_InitStructure); //��ʼ������	2
  
// 	//����������
//  //	USART3->BRR=(pclk1*1000000)/(bound);// ����������	 
// 	//USART3->CR1|=0X200C;  	//1λֹͣ,��У��λ.
// 	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);  	//ʹ�ܴ���2��DMA����
// 	UART_DMA_Config(DMA1_Channel7,(u32)&USART3->DR,(u32)USART3_TX_BUF);//DMA1ͨ��7,����Ϊ����2,�洢��ΪUSART3_TX_BUF 
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 
	
#ifdef USART3_RX_EN		  	//���ʹ���˽���
	//ʹ�ܽ����ж�
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�   
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	TIM3_Init(99,7199);		//10ms�ж�
	USART3_RX_STA=0;		//����
	TIM3_Set(0);			//�رն�ʱ��4
#endif	 									  	
}
// //����2,printf ����
// //ȷ��һ�η������ݲ�����USART3_MAX_SEND_LEN�ֽ�
// void u3_printf(char* fmt,...)  
// {  
// 	va_list ap;
// 	va_start(ap,fmt);
// 	vsprintf((char*)USART3_TX_BUF,fmt,ap);
// 	va_end(ap);
// 	while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
// 	UART_DMA_Enable(DMA1_Channel7,strlen((const char*)USART3_TX_BUF)); 	//ͨ��dma���ͳ�ȥ
// }
//��ʱ��4�жϷ������		    
void TIM3_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־    
		TIM3_Set(0);			//�ر�TIM3  
	}   
}
//����TIM3�Ŀ���
//sta:0���ر�;1,����;
void TIM3_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM3,0);//���������
		TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx	
	}else TIM_Cmd(TIM3, DISABLE);//�رն�ʱ��4
}
//ͨ�ö�ʱ���жϳ�ʼ��
//����ʼ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��		 
void TIM3_Init(u16 arr,u16 psc)
{	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��//TIM3ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	 	  
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
}


//void debug_uart3_send_datas(uint8_t* str, uint16_t len)
//{
//	DB_PR("----u3 rcv-----\r\n");
//	while(len--)
//	{
//		USART_SendData(USART3, *str++);
//		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
//		str++;
//		//DB_PR("%c",*str++);
//	}
//	DB_PR("-----u3 end----\r\n");
//}

//1
void spear_uart3_send_datas(uint8_t* str, uint8_t len)
{
	while(len--)
	{
		//delay_ms(50);  
		//while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
		USART_SendData(USART3, *str);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET); 
		str++;
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart֧�ֲ��� 
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART4_RX_STA;
//     1,����USART4_RX_STA;
void lock_at_response(u8 mode)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		DB_PR("USART3_RX_BUF=sssssssssssss\n");
		RS485_TX_EN();
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
			DB_PR("USART3_RX_BUF=%s\n",USART3_RX_BUF);	//���͵�����-----to
			DB_PR("USART3_RX_STA=%x",USART3_RX_STA);	
			// spear_uart3_send_datas(USART3_RX_BUF,USART3_RX_STA);
		//Usart_SendByte
		// uart0_debug_data_h(USART3_RX_BUF,USART3_RX_STA);
		if(mode)
		{
			RS485_RX_EN();
			USART3_RX_STA=0;		
		}

		DB_PR("USART3_RX_BUF=eeeeeeeeeeeeeee-lock\n\n");

	} 
}
#endif		 
///////////////////////////////////////USART3 DMA�������ò���//////////////////////////////////	   		    
// //DMA1�ĸ�ͨ������
// //����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
// //�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
// //DMA_CHx:DMAͨ��CHx
// //cpar:�����ַ
// //cmar:�洢����ַ    
// void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar)
// {
// 	DMA_InitTypeDef DMA_InitStructure;
//  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
//   DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
// 	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;  //DMA����ADC����ַ
// 	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  //DMA�ڴ����ַ
// 	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
// 	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
// 	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
// 	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
// 	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
// 	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
// 	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
// 	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
// 	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
// 	DMA_Init(DMA_CHx, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���	
// } 
// //����һ��DMA����
// void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len)
// {
// 	DMA_Cmd(DMA_CHx, DISABLE );  //�ر� ָʾ��ͨ��        
// 	DMA_SetCurrDataCounter(DMA_CHx,len);//DMAͨ����DMA����Ĵ�С	
// 	DMA_Cmd(DMA_CHx, ENABLE);           //����DMA����
// }	   
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////// 									 





















