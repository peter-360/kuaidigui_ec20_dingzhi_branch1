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
//ALIENTEK Mini STM32��������չʵ��14
//ATK-SIM900A GSMģ�����ʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾  
 
 
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
 * ϵͳ�����λ
 */
void Soft_Reset(void)
{
  __set_FAULTMASK(1);   /* �ر������ж�*/  
  NVIC_SystemReset();   /* ϵͳ��λ */
}
 

void power_down_reset_ec20()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	delay_ms(800); 
	GPIO_SetBits(GPIOB,GPIO_Pin_8);



	DB_PR("---------ec20 will rst-----------\n");
	/* 4G����	*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	 
	delay_ms(580); //500
	/* �ر�����led��	*/
	GPIO_SetBits(GPIOB,GPIO_Pin_0);	 
	DB_PR("---------ec20 rst  ok--------------\n");	

}

void reset_ec20()
{
	// GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	// delay_ms(800); 
	// GPIO_SetBits(GPIOB,GPIO_Pin_8);



	DB_PR("---------ec20 will rst-----------\n");
	/* 4G����	*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);	 
	delay_ms(580); //500
	/* �ر�����led��	*/
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


/*****************  ����һ���ֽ� **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}

/****************** ����8λ������ ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* ����һ���ֽ����ݵ�USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* �ȴ�������� */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  �����ַ��� **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
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



	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�  
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ9600			 
//	LCD_Init();				//��ʼ��Һ�� 
	LED_Init();         	//LED��ʼ��	 
//	KEY_Init();				//������ʼ��	  													    
 	USART2_Init(115200);	//��ʼ������ 4g
	USART3_Init(9600);		//��ʼ������ lock
	USART4_Init(115200);	//��ʼ������ lcd
	DB_PR2("\r\n-0-�������ܹ�_%s\r\n",FIRM_VERSION_EC20);
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
	 
//	tp_dev.init();			//��������ʼ��
// 	mem_init();				//��ʼ���ڴ��	    
// 	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
//  	f_mount(fs[0],"0:",1); 	//����SD�� 
// 	f_mount(fs[1],"1:",1); 	//����FLASH.
//	key=KEY_Scan(0);  
//	if(key==KEY0_PRES)		//ǿ��У׼
//	{
//		LCD_Clear(WHITE);	//����
//		tp_dev.adjust();  	//��ĻУ׼  
//		LCD_Clear(WHITE);	//����
//	}
//	fontok=font_init();		//����ֿ��Ƿ�OK
//	if(fontok||key==KEY1_PRES)//��Ҫ�����ֿ⣨�ֿⲻ����/KEY1���£�			 
//	{
//		LCD_Clear(WHITE);		   	//����
// 		POINT_COLOR=RED;			//��������Ϊ��ɫ	   	   	  
//		LCD_ShowString(60,50,200,16,16,"ALIENTEK STM32");
//		while(SD_Initialize())		//���SD��
//		{
//			LCD_ShowString(60,70,200,16,16,"SD Card Failed!");
//			delay_ms(200);
//			LCD_Fill(60,70,200+60,70+16,WHITE);
//			delay_ms(200);		    
//		}								 						    
//		LCD_ShowString(60,70,200,16,16,"SD Card OK");
//		LCD_ShowString(60,90,200,16,16,"Font Updating...");
//		key=update_font(20,110,16);//�����ֿ�
//		while(key)//����ʧ��		
//		{			 		  
//			LCD_ShowString(60,110,200,16,16,"Font Update Failed!");
//			delay_ms(200);
//			LCD_Fill(20,110,200+20,110+16,WHITE);
//			delay_ms(200);		       
//		} 		  
//		LCD_ShowString(60,110,200,16,16,"Font Update Success!");
//		delay_ms(1500);	
//		LCD_Clear(WHITE);//����	       
//	}  

	// sim900a_test();
	sim900a_gprs_test();
}


