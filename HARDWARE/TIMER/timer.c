#include "timer.h"
#include "led.h"
#include "usart.h"//log
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//通用定时器 驱动代码			   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/07
//版本：V1.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved	
//********************************************************************************
//V1.1 20140306 
//增加TIM1_CH1，PWM输出设置相关内容 
//V1.2 20140307
//增加TIM2输入捕获初始化函数TIM2_Cap_Init及其中断处理
////////////////////////////////////////////////////////////////////////////////// 	  
 
// //定时器3中断服务程序	 
// void TIM3_IRQHandler(void)
// { 		    		  			    
// 	if(TIM3->SR&0X0001)//溢出中断
// 	{
// 		LED1=!LED1;			    				   				     	    	
// 	}				   
// 	TIM3->SR&=~(1<<0);//清除中断标志位 	    
// }
// //通用定时器中断初始化
// //这里时钟选择为APB1的2倍，而APB1为36M
// //arr：自动重装值。
// //psc：时钟预分频数
// //这里使用的是定时器3!
// void TIM3_Int_Init(u16 arr,u16 psc)
// {	NVIC_InitTypeDef NVIC_InitStructure;
// 	RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
//  	TIM3->ARR=arr;  	//设定计数器自动重装值 
// 	TIM3->PSC=psc;  	//预分频器设置
// 	TIM3->DIER|=1<<0;   //允许更新中断				
// 	TIM3->CR1|=0x01;    //使能定时器3
 
//     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
// 	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
// }
// //TIM1_CH1 PWM输出初始化
// //arr：自动重装值
// //psc：时钟预分频数
// void TIM1_PWM_Init(u16 arr,u16 psc)
// {		 					 
// 	//此部分需手动修改IO口设置
// 	RCC->APB2ENR|=1<<11; 	//TIM1时钟使能    
// 	GPIOA->CRH&=0XFFFFFFF0;	//PA8清除之前的设置
// 	GPIOA->CRH|=0X0000000B;	//复用功能输出 
	
// 	TIM1->ARR=arr;			//设定计数器自动重装值 
// 	TIM1->PSC=psc;			//预分频器设置
  
// 	TIM1->CCMR1|=7<<4;  	//CH1 PWM2模式		 
// 	TIM1->CCMR1|=1<<3; 		//CH1预装载使能	 
//  	TIM1->CCER|=1<<0;   	//OC1 输出使能	   
// 	TIM1->BDTR|=1<<15;   	//MOE 主输出使能	   

// 	TIM1->CR1=0x0080;   	//ARPE使能 
// 	TIM1->CR1|=0x01;    	//使能定时器1 										  
// }  
// //定时器2通道1输入捕获配置
// //arr：自动重装值
// //psc：时钟预分频数
// void TIM2_Cap_Init(u16 arr,u16 psc)
// {		NVIC_InitTypeDef NVIC_InitStructure;	 
// 	RCC->APB1ENR|=1<<0;   	//TIM2 时钟使能 
// 	RCC->APB2ENR|=1<<2;    	//使能PORTA时钟  
	 
// 	GPIOA->CRL&=0XFFFFFFF0;	//PA0 清除之前设置  
// 	GPIOA->CRL|=0X00000008;	//PA0 输入   
// 	GPIOA->ODR|=0<<0;		//PA0 下拉
	  
//  	TIM2->ARR=arr;  		//设定计数器自动重装值   
// 	TIM2->PSC=psc;  		//预分频器 

// 	TIM2->CCMR1|=1<<0;		//CC1S=01 	选择输入端 IC1映射到TI1上
//  	TIM2->CCMR1|=1<<4; 		//IC1F=0001 配置输入滤波器 以Fck_int采样，2个事件后有效
//  	TIM2->CCMR1|=0<<10; 	//IC2PS=00 	配置输入分频,不分频 

// 	TIM2->CCER|=0<<1; 		//CC1P=0	上升沿捕获
// 	TIM2->CCER|=1<<0; 		//CC1E=1 	允许捕获计数器的值到捕获寄存器中

// 	TIM2->DIER|=1<<1;   	//允许捕获中断				
// 	TIM2->DIER|=1<<0;   	//允许更新中断	
// 	TIM2->CR1|=0x01;    	//使能定时器2


//     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
// 	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
// }

// //捕获状态
// //[7]:0,没有成功的捕获;1,成功捕获到一次.
// //[6]:0,还没捕获到高电平;1,已经捕获到高电平了.
// //[5:0]:捕获高电平后溢出的次数
// u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
// u16	TIM2CH1_CAPTURE_VAL;	//输入捕获值
// //定时器2中断服务程序	 
// void TIM2_IRQHandler(void)
// { 		    
// 	u16 tsr;
// 	tsr=TIM2->SR;
//  	if((TIM2CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
// 	{
// 		if(tsr&0X01)//溢出
// 		{	    
// 			if(TIM2CH1_CAPTURE_STA&0X40)//已经捕获到高电平了
// 			{
// 				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
// 				{
// 					TIM2CH1_CAPTURE_STA|=0X80;//标记成功捕获了一次
// 					TIM2CH1_CAPTURE_VAL=0XFFFF;
// 				}else TIM2CH1_CAPTURE_STA++;
// 			}	 
// 		}
// 		if(tsr&0x02)//捕获1发生捕获事件
// 		{	
// 			if(TIM2CH1_CAPTURE_STA&0X40)		//捕获到一个下降沿 		
// 			{	  			
// 				TIM2CH1_CAPTURE_STA|=0X80;		//标记成功捕获到一次高电平脉宽
// 			    TIM2CH1_CAPTURE_VAL=TIM2->CCR1;	//获取当前的捕获值.
// 	 			TIM2->CCER&=~(1<<1);			//CC1P=0 设置为上升沿捕获
// 			}else  								//还未开始,第一次捕获上升沿
// 			{ 
// 				TIM2CH1_CAPTURE_VAL=0;
// 				TIM2CH1_CAPTURE_STA=0X40;		//标记捕获到了上升沿
// 				TIM2->CNT=0;					//计数器清空
// 				TIM2->CCER|=1<<1; 				//CC1P=1 设置为下降沿捕获 
// 			}		    
// 		}			     	    					   
//  	}
// 	TIM2->SR=0;//清除中断标志位 	    
// }







//add

//设置TIM4的开关
//sta:0，关闭;1,开启;
void TIM5_Set(u8 sta)
{
	if(sta)
	{
       
		TIM_SetCounter(TIM5,0);//计数器清空
		TIM_Cmd(TIM5, ENABLE);  //使能TIMx	
	}
	else 
		TIM_Cmd(TIM5, DISABLE);//关闭定时器4	   
}


//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM5_Int_Init(u16 arr,u16 psc)//uart
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
	
	//定时器TIM5初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM5中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx					
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );	
}

u8 daojishi_time =35;
u8 mtimer_flag=0;
//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
	u8 number_buffer[10]={0};
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM5更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
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

					//关闭TIM 
	}
}



















