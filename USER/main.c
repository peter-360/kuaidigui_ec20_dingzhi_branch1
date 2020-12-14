#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"//log
#include "lcd.h"
#include "key.h"
#include "usmart.h" 
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "sim900a.h"	
#include "touch.h"	
#include "usart2.h"	
#include "usart3.h"	
#include "usart4.h"	
#include "lcd_u4.h"
 #include "md5.h"
 #include "timer.h"
//ALIENTEK Mini STM32开发板扩展实验14
//ATK-SIM900A GSM模块测试实验-库函数版本
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司  
 
 
void uart0_debug_str(uint8_t* str,uint16_t len)//str8
{
	int i;
    DB_PR("--1str---debug_str:\n");
    for(i=0;i<len;i++)
        DB_PR("%c",str[i]);
    DB_PR("\r\n");
}
 
void uart0_debug_data_h(uint8_t* data,uint16_t len)//hex8
{
	int i;
    DB_PR("---2s----debug_data:\n");
    for(i=0;i<len;i++)
        DB_PR("%02x ",data[i]);
    DB_PR("\n-----2e---------\n");
}
void uart0_debug_data_d(uint8_t* data,uint16_t len)//dec8
{
	int i;
    DB_PR("---3---debug_data:");
    for(i=0;i<len;i++)
        DB_PR("%02d ",data[i]);
    DB_PR("\r\n");
}
 
/*
 * 系统软件复位
 */
void Soft_Reset(void)
{
  __set_FAULTMASK(1);   /* 关闭所有中断*/  
  NVIC_SystemReset();   /* 系统复位 */
}
 

void power_down_reset_ec20()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	delay_ms(800); 
	GPIO_SetBits(GPIOB,GPIO_Pin_8);



	DB_PR("---------ec20 will rst-----------\n");
	/* 4G重启	*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	 
	delay_ms(580); //500
	/* 关闭所有led灯	*/
	GPIO_SetBits(GPIOB,GPIO_Pin_0);	 
	DB_PR("---------ec20 rst  ok--------------\n");	

}

void reset_ec20()
{
	// GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	// delay_ms(800); 
	// GPIO_SetBits(GPIOB,GPIO_Pin_8);



	DB_PR("---------ec20 will rst-----------\n");
	/* 4G重启	*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	 
	delay_ms(580); //500
	/* 关闭所有led灯	*/
	GPIO_SetBits(GPIOB,GPIO_Pin_0);	 
	DB_PR("---------ec20 rst  ok--------------\n");	

}
void at_mode_go()
{
	DB_PR("...at_mode_go...\n");
	delay_ms(1000); //500
	sim900a_send_cmd_tou_data("+++",0,0);//AT
	DB_PR("...a-0-1...\n");
	delay_ms(1000); //500
}


/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}
 //V0_0_2C1  log
 //V0_0_2C2 timer time
#define FIRM_VERSION_EC20 "V0_0_3B"//V0_0_2C1

 int main(void)
 { 
	u8 key,fontok=0;  
	// int i;



	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级  
	uart_init(115200);	 	//串口初始化为9600			 
//	LCD_Init();				//初始化液晶 
	LED_Init();         	//LED初始化	 
//	KEY_Init();				//按键初始化	  													    
 	USART2_Init(115200);	//初始化串口 4g
	USART3_Init(9600);		//初始化串口 lock
	USART4_Init(115200);	//初始化串口 lcd
	DB_PR2("\r\n-0-心甜智能柜_%s\r\n",FIRM_VERSION_EC20);
	//Usart_SendString( UART4,"abcde\n");


	delay_ms(100); //500
	send_cmd_to_lcd_pic(0x0000);

	TIM5_Int_Init(9999,7199);
	TIM5_Set(0);



	// delay_ms(500); //500
	
// chengxu_start_0:
	power_down_reset_ec20();
	// reset_ec20();
	at_mode_go();
	 
//	tp_dev.init();			//触摸屏初始化
// 	mem_init();				//初始化内存池	    
// 	exfuns_init();			//为fatfs相关变量申请内存  
//  	f_mount(fs[0],"0:",1); 	//挂载SD卡 
// 	f_mount(fs[1],"1:",1); 	//挂载FLASH.
//	key=KEY_Scan(0);  
//	if(key==KEY0_PRES)		//强制校准
//	{
//		LCD_Clear(WHITE);	//清屏
//		tp_dev.adjust();  	//屏幕校准  
//		LCD_Clear(WHITE);	//清屏
//	}
//	fontok=font_init();		//检查字库是否OK
//	if(fontok||key==KEY1_PRES)//需要更新字库（字库不存在/KEY1按下）			 
//	{
//		LCD_Clear(WHITE);		   	//清屏
// 		POINT_COLOR=RED;			//设置字体为红色	   	   	  
//		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
//		while(SD_Initialize())		//检测SD卡
//		{
//			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
//			delay_ms(200);
//			LCD_Fill(60,70,200+60,70+16,WHITE);
//			delay_ms(200);		    
//		}								 						    
//		LCD_ShowString(60,70,200,16,16,"SD Card OK");
//		LCD_ShowString(60,90,200,16,16,"Font Updating...");
//		key=update_font(20,110,16);//更新字库
//		while(key)//更新失败		
//		{			 		  
//			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
//			delay_ms(200);
//			LCD_Fill(20,110,200+20,110+16,WHITE);
//			delay_ms(200);		       
//		} 		  
//		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
//		delay_ms(1500);	
//		LCD_Clear(WHITE);//清屏	       
//	}  

	// sim900a_test();
	sim900a_gprs_test();
}


