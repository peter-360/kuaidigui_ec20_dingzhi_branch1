#ifndef __USART4_H
#define __USART4_H	 
#include "sys.h"  
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

#define USART4_MAX_RECV_LEN		1024				//�����ջ����ֽ���
#define USART4_MAX_SEND_LEN		1024				//����ͻ����ֽ���
#define USART4_RX_EN 			1					//0,������;1,����.

extern u8  USART4_RX_BUF[USART4_MAX_RECV_LEN]; 		//���ջ���,���USART4_MAX_RECV_LEN�ֽ�
extern u8  USART4_TX_BUF[USART4_MAX_SEND_LEN]; 		//���ͻ���,���USART4_MAX_SEND_LEN�ֽ�
extern u16 USART4_RX_STA;   						//��������״̬

void USART4_Init(u32 bound);				//����2��ʼ�� 
void TIM2_Set(u8 sta);
void TIM2_Init(u16 arr,u16 psc);
void UART_DMA_Config(DMA_Channel_TypeDef*DMA_CHx,u32 cpar,u32 cmar);
void UART_DMA_Enable(DMA_Channel_TypeDef*DMA_CHx,u16 len);
void u4_printf(char* fmt, ...);


void uart4_send_datas(uint8_t* str, uint8_t len);
#endif













