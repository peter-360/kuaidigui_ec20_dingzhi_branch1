#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(4)	// PA8
#define LED1 PDout(5)	// PD2	

void LED_Init(void);//��ʼ��




	/// ����ȷ����ʱ
static void RS485_delay(__IO u32 nCount)
{
	for(; nCount != 0; nCount--);
} 


/*�����շ�����*/
//�������ģʽ,����Ҫ����ʱ�ȴ�485����������
#define RS485_RX_EN()			RS485_delay(1000); GPIO_ResetBits(GPIOB,GPIO_Pin_1);  RS485_delay(1000);
//���뷢��ģʽ,����Ҫ����ʱ�ȴ�485����������
#define RS485_TX_EN()			RS485_delay(1000); GPIO_SetBits(GPIOB,GPIO_Pin_1);  RS485_delay(1000);

		 				    
#endif
