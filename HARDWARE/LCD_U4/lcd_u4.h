#ifndef __LCD_U4_H
#define __LCD_U4_H	 
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口2驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
#define TX4_PIC_LEN 10//12 tupian
#define TX4_BL_LEN 8//12 tupian

//#define _DEBUG_ 1
//#if _DEBUG_
//#define DB_PR(...) printf(__VA_ARGS__)
//#else
//#define DB_PR(...) 
//#endif

#include "timer.h"

extern char regst_key[];

extern u8 daojishi_ongo_flag;
extern u8 qujianma_wait_tcp_flag;
extern int  heart_beart_idx;

void lcd_at_response(u8 mode);


void HexToStr(char *pbDest,char *pbSrc,int nlen);
void StrToHex(char *pbDest, char *pbSrc, int nLen);


char *itoa(int value, char *string, int radix);
char *ltoa(long value, char *string, int radix);

//void  ltoa(long num, char* str, int radix);
//void  itoa(int num, char* str, int radix);

extern void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
 void uart0_debug_str(uint8_t* str,uint16_t len);//str8
 
void uart0_debug_data_h(uint8_t* data,uint16_t len);//hex8

void uart0_debug_data_d(uint8_t* data,uint16_t len);//dec8


void send_cmd_to_lcd_pic(uint16_t temp);
void send_cmd_to_lcd_bl_len(uint16_t opCode, uint8_t* buff_temp,uint16_t data_len);
void send_cmd_to_lcd_bl(uint16_t opCode, uint16_t temp);


u8 sim900a_send_cmd_tou_data(u8 *cmd,u8 *ack,u16 waittime);
#endif













