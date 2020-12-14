#include "timer.h"
#include "led.h"
#include "usart.h"//log
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ͨ�ö�ʱ�� ��������			   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/07
//�汾��V1.2
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20140306 
//����TIM1_CH1��PWM�������������� 
//V1.2 20140307
//����TIM2���벶���ʼ������TIM2_Cap_Init�����жϴ���
////////////////////////////////////////////////////////////////////////////////// 	  
 
// //��ʱ��3�жϷ������	 
// void TIM3_IRQHandler(void)
// { 		    		  			    
// 	if(TIM3->SR&0X0001)//����ж�
// 	{
// 		LED1=!LED1;			    				   				     	    	
// 	}				   
// 	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
// }
// //ͨ�ö�ʱ���жϳ�ʼ��
// //����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
// //arr���Զ���װֵ��
// //psc��ʱ��Ԥ��Ƶ��
// //����ʹ�õ��Ƕ�ʱ��3!
// void TIM3_Int_Init(u16 arr,u16 psc)
// {	NVIC_InitTypeDef NVIC_InitStructure;
// 	RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
//  	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
// 	TIM3->PSC=psc;  	//Ԥ��Ƶ������
// 	TIM3->DIER|=1<<0;   //��������ж�				
// 	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
 
//     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
// 	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
// }
// //TIM1_CH1 PWM�����ʼ��
// //arr���Զ���װֵ
// //psc��ʱ��Ԥ��Ƶ��
// void TIM1_PWM_Init(u16 arr,u16 psc)
// {		 					 
// 	//�˲������ֶ��޸�IO������
// 	RCC->APB2ENR|=1<<11; 	//TIM1ʱ��ʹ��    
// 	GPIOA->CRH&=0XFFFFFFF0;	//PA8���֮ǰ������
// 	GPIOA->CRH|=0X0000000B;	//���ù������ 
	
// 	TIM1->ARR=arr;			//�趨�������Զ���װֵ 
// 	TIM1->PSC=psc;			//Ԥ��Ƶ������
  
// 	TIM1->CCMR1|=7<<4;  	//CH1 PWM2ģʽ		 
// 	TIM1->CCMR1|=1<<3; 		//CH1Ԥװ��ʹ��	 
//  	TIM1->CCER|=1<<0;   	//OC1 ���ʹ��	   
// 	TIM1->BDTR|=1<<15;   	//MOE �����ʹ��	   

// 	TIM1->CR1=0x0080;   	//ARPEʹ�� 
// 	TIM1->CR1|=0x01;    	//ʹ�ܶ�ʱ��1 										  
// }  
// //��ʱ��2ͨ��1���벶������
// //arr���Զ���װֵ
// //psc��ʱ��Ԥ��Ƶ��
// void TIM2_Cap_Init(u16 arr,u16 psc)
// {		NVIC_InitTypeDef NVIC_InitStructure;	 
// 	RCC->APB1ENR|=1<<0;   	//TIM2 ʱ��ʹ�� 
// 	RCC->APB2ENR|=1<<2;    	//ʹ��PORTAʱ��  
	 
// 	GPIOA->CRL&=0XFFFFFFF0;	//PA0 ���֮ǰ����  
// 	GPIOA->CRL|=0X00000008;	//PA0 ����   
// 	GPIOA->ODR|=0<<0;		//PA0 ����
	  
//  	TIM2->ARR=arr;  		//�趨�������Զ���װֵ   
// 	TIM2->PSC=psc;  		//Ԥ��Ƶ�� 

// 	TIM2->CCMR1|=1<<0;		//CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
//  	TIM2->CCMR1|=1<<4; 		//IC1F=0001 ���������˲��� ��Fck_int������2���¼�����Ч
//  	TIM2->CCMR1|=0<<10; 	//IC2PS=00 	���������Ƶ,����Ƶ 

// 	TIM2->CCER|=0<<1; 		//CC1P=0	�����ز���
// 	TIM2->CCER|=1<<0; 		//CC1E=1 	�������������ֵ������Ĵ�����

// 	TIM2->DIER|=1<<1;   	//�������ж�				
// 	TIM2->DIER|=1<<0;   	//��������ж�	
// 	TIM2->CR1|=0x01;    	//ʹ�ܶ�ʱ��2


//     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�3
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
// 	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
// }

// //����״̬
// //[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
// //[6]:0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
// //[5:0]:����ߵ�ƽ������Ĵ���
// u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
// u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
// //��ʱ��2�жϷ������	 
// void TIM2_IRQHandler(void)
// { 		    
// 	u16 tsr;
// 	tsr=TIM2->SR;
//  	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
// 	{
// 		if(tsr&0X01)//���
// 		{	    
// 			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
// 			{
// 				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
// 				{
// 					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
// 					TIM2CH1_CAPTURE_VAL=0XFFFF;
// 				}else TIM2CH1_CAPTURE_STA++;
// 			}	 
// 		}
// 		if(tsr&0x02)//����1���������¼�
// 		{	
// 			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
// 			{	  			
// 				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
// 			    TIM2CH1_CAPTURE_VAL=TIM2->CCR1;	//��ȡ��ǰ�Ĳ���ֵ.
// 	 			TIM2->CCER&=~(1<<1);			//CC1P=0 ����Ϊ�����ز���
// 			}else  								//��δ��ʼ,��һ�β���������
// 			{ 
// 				TIM2CH1_CAPTURE_VAL=0;
// 				TIM2CH1_CAPTURE_STA=0X40;		//��ǲ�����������
// 				TIM2->CNT=0;					//���������
// 				TIM2->CCER|=1<<1; 				//CC1P=1 ����Ϊ�½��ز��� 
// 			}		    
// 		}			     	    					   
//  	}
// 	TIM2->SR=0;//����жϱ�־λ 	    
// }







//add

//����TIM4�Ŀ���
//sta:0���ر�;1,����;
void TIM5_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM5,0);//���������
		TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx	
	}
	else 
		TIM_Cmd(TIM5, DISABLE);//�رն�ʱ��4	   
}


//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM5_Int_Init(u16 arr,u16 psc)//uart
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM5��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM5�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx					
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );	
}

u8 daojishi_time =35;
u8 mtimer_flag=0;
//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM5�ж�
{
	u8 number_buffer[10]={0};
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM5�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
//		LED1=!LED1;
		//SEGGER_RTT_printf(0, "TIM5_IRQHandler Uart1_Rx= %x\n",Uart1_Rx);
		//Uart1_Rx =0 ;
		

		daojishi_time--;

		DB_PR("-------daojishi_time=%d--------\n",daojishi_time);
		if(daojishi_time ==34)
		{
			//kaiji jinru yici INT
			TIM5_Set(0);

		}
		else if(daojishi_time <30)
		{

			sprintf((char*)number_buffer, "%d", daojishi_time);
			// DB_PR("-------number_buffer=%s--------\n",number_buffer);

			if(mtimer_flag == 1)
				send_cmd_to_lcd_bl_len(0x1900,number_buffer,10+4);
			else if(mtimer_flag == 2)
                send_cmd_to_lcd_bl_len(0x1950,number_buffer,10+4);
			if(0==daojishi_time)
			{
				DB_PR("-------zhuye--------\n");
				send_cmd_to_lcd_pic(0x0003);
				daojishi_ongo_flag =0;
				daojishi_time=30;
				TIM5_Set(0);
			}

		}




		// switch (daojishi_time)
		// {
		// case 19/* constant-expression */:
		// 	/* code */
		// 	break;
		
		// default:
		// 	break;
		// }

					//�ر�TIM 
	}
}



















