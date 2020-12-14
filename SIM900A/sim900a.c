#include "sim900a.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   	 
#include "key.h"	 	 	 	 	 
#include "lcd.h" 	  
#include "flash.h" 	 
#include "touch.h" 	 
#include "malloc.h"
#include "string.h"    
#include "text.h"		
#include "usart2.h" 
#include "usart3.h" 
#include "usart4.h" 
#include "lcd_u4.h"
#include "ff.h"

#include "cJSON.h"
#include "md5.h"
#include "iwdg.h"

#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-SIM900A GSM/GPRSģ������	  
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved	
//********************************************************************************
//��
//////////////////////////////////////////////////////////////////////////////////	
 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//usmart֧�ֲ��� 
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART2_RX_STA;
//     1,����USART2_RX_STA;

void delay_xs(u16 xms)
{	 	
	int i=0;
	for(i=0; i<xms; i++)
	{
   		delay_ms(1000);
		// DB_PR("-----------\n");
	}
	DB_PR("-----delay_xs =%d------\n",xms);

}



u8 UTF8toUnicode(u8 *ch, u16 *_unicode)
{
    u8 *p = NULL ,n = 0;
    u16 e = 0; 
    p = ch;
    if(1)//p == NULL
    {
            if(*p >= 0xfc)
            {
                    /*6:<11111100>*/
                    e  = (p[0] & 0x01) << 30;
                    e |= (p[1] & 0x3f) << 24;
                    e |= (p[2] & 0x3f) << 18;
                    e |= (p[3] & 0x3f) << 12;
                    e |= (p[4] & 0x3f) << 6;
                    e |= (p[5] & 0x3f);
                    n = 6;
            }
            else if(*p >= 0xf8) 
            {
                    /*5:<11111000>*/
                    e = (p[0] & 0x03) << 24;
                    e |= (p[1] & 0x3f) << 18;
                    e |= (p[2] & 0x3f) << 12;
                    e |= (p[3] & 0x3f) << 6;
                    e |= (p[4] & 0x3f);
                    n = 5;
            }
            else if(*p >= 0xf0)
            {
                    /*4:<11110000>*/
                    e = (p[0] & 0x07) << 18;
                    e |= (p[1] & 0x3f) << 12;
                    e |= (p[2] & 0x3f) << 6;
                    e |= (p[3] & 0x3f);
                    n = 4;
            }
            else if(*p >= 0xe0)
            {
                    /*3:<11100000>*/
                    e = (p[0] & 0x0f) << 12;
                    e |= (p[1] & 0x3f) << 6;
                    e |= (p[2] & 0x3f);
                    n = 3;
            }
            else if(*p >= 0xc0) 
            {
                    /*2:<11000000>*/
                    e = (p[0] & 0x1f) << 6;
                    e |= (p[1] & 0x3f);
                    n = 2;
            }
            else
            {
                    e = p[0];           
                    n = 1;
            }
            *_unicode = e;
    }
    return n;
}













u16 cjson_to_struct_info_qrcode(char *text)//kaiji
{
	u8 reg_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson����

	cJSON * item2 = NULL;//cjson����
	
	cJSON * item3 = NULL;//cjson����
	uint8_t buff_t[256]={0};
	int i=0;
	u16 index_m=0;


	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //��ȡ��Чjson
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);

	DB_PR("\n----2----text=\n%s\n",text);


    root = cJSON_Parse(text);     
    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
		reg_status =1;//add

        DB_PR("%s\n", "�и�ʽ�ķ�ʽ��ӡJson:");           
        // DB_PR("%s\n\n", cJSON_Print(root));
        // DB_PR("%s\n", "�޸�ʽ��ʽ��ӡjson��");
        // DB_PR("%s\n\n", cJSON_PrintUnformatted(root));

		//---------------------
		DB_PR("\n%s\n", "--1--һ��һ���Ļ�ȡ------------ ��ֵ��:");
		DB_PR("%s\n", "��ȡresult�µ�cjson����:");
		item = cJSON_GetObjectItem(root, "result");//
		DB_PR("--1--%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
		DB_PR("--2--%s\n", item->valuestring);

		// DB_PR("%s\n", cJSON_Print(item));




		memset(buff_t,0,256);
		memcpy(buff_t, item->valuestring,strlen( item->valuestring));
		index_m = strlen(buff_t);
		DB_PR("--index_m=%d--\n", index_m);
		buff_t[index_m]=0xff;
		buff_t[index_m+1]=0xff;
		
		uart0_debug_data_h(buff_t,256);

		send_cmd_to_lcd_bl_len(0x2000,(uint8_t*)buff_t,128+4);//gekou 33 +3





		// send_cmd_to_lcd_pic(0x0003);
    
    }

    cJSON_Delete(root);
    return reg_status;

}

char regst_key[60]={0};

u16 cjson_to_struct_info_register(char *text)
{
	u8 reg_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson����

	cJSON * item2 = NULL;//cjson����
	
	cJSON * item3 = NULL;//cjson����
	int i=0;


	u32 reg_active_code =0;
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[256]={0};
	char* buff_t3="b5";
	int my_bl_ts=0;
	//��Ƭ������
	// uint8_t buff_t3[256]={0xb5 ,0xa5 ,0xc6 ,0xac ,0xbb ,0xfa ,0xb2 ,0xe2 ,0xca ,0xd4 ,0x32};
	
	int company_id=0;
	char* url_t="https://iot.xintiangui.com/web_wechat/download_app?cid=";//guding



	char regst_key_post[300];
	u16 index_m=0;

	// const char needle[10] = "\r\n";
	// char *ret;
	
	// //�豸ע��ɹ�,�뼤�����
	// char* str_tmp="{\"status\":0,\"msg\":\"abc\",\"result\":{\"active_code\":87993541}}";
	
	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //��ȡ��Чjson
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');
    // char *index=strstr(text,"{\"post_data\":{");
    // bzero(text, sizeof(text));
    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);
	// 	memset(text,0,89);
	// 	memcpy(text,index,89);
	// 	text[89]='\0';

	// DB_PR("---strlen(text)= %d\n", strlen(text));
	// DB_PR("---89= %02x\n", text[89]);
	// DB_PR("---90= %02x\n", text[90]);
 
	// for(i=89;i<strlen(text);i++)
	// {
	// 	text[i]=0;
	// }


//    ret = strstr(text, needle);
	DB_PR("\n----2----text=\n%s\n",text);










	// DB_PR("str_tmp=%s",str_tmp);


	root = cJSON_Parse(text);     
	if (!root) 
	{
		DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
	}
	else
	{
		DB_PR("%s\n", "�и�ʽ�ķ�ʽ��ӡJson:");           
		// DB_PR("%s\n\n", cJSON_Print(root));
		// DB_PR("%s\n", "�޸�ʽ��ʽ��ӡjson��");
		// DB_PR("%s\n\n", cJSON_PrintUnformatted(root));




		DB_PR("%s\n", "��ȡstatus�µ�cjson����");
		item = cJSON_GetObjectItem(root, "status");
		// DB_PR("%s\n", cJSON_Print(item));
		DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
		DB_PR("%d\n", item->valueint);
		reg_status = item->valueint;
		DB_PR("reg_status=%d\n", reg_status);





		if(0== reg_status)
		{
			//---------------------
			DB_PR("\n%s\n", "--1--һ��һ���Ļ�ȡ------------ ��ֵ��:");
			DB_PR("%s\n", "��ȡresult�µ�cjson����:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "��ȡactive_code�µ�cjson����");
			item = cJSON_GetObjectItem(item, "active_code");
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("--1--%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("--2--%u\n", item->valueint);
			reg_active_code= item->valueint;
			DB_PR("reg_active_code=%u\n", reg_active_code);

			// reg_active_code = 12345608;//-------------
			// reg_active_code = 0x12345078;//-------------
			ltoa(reg_active_code,(char*)buff_t,10);
			uart0_debug_data_h(buff_t,strlen(buff_t));
			send_cmd_to_lcd_bl_len(0x1010,(uint8_t*)buff_t,32+4);//gekou 33 +3

			send_cmd_to_lcd_pic(0x0002);



			//��ȡhttp����------------
		}
		else if(2== reg_status)
		{
			//-----------company------------
			DB_PR("%s\n", "��ȡresult�µ�cjson����:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "��ȡ company �µ�cjson����");
			item2 = cJSON_GetObjectItem(item, "company");

			DB_PR("%s\n", "��ȡ service_tel �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "service_tel");
			DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%s\n", item->valuestring);

			memset(buff_t,0,256);
			memcpy(buff_t,item->valuestring,8* sizeof(item->valuestring));
			DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			send_cmd_to_lcd_bl_len(0x1150,(uint8_t*)buff_t,32+4);//gekou 33 +3




			DB_PR("%s\n", "��ȡ register_key �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "register_key");
			DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%s\n", item->valuestring);

			memset(buff_t,0,256);
			memcpy(regst_key,item->valuestring,strlen((item->valuestring)));//8* sizeof(item->valuestring)
			DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			DB_PR("strlen((item->valuestring)=%d\n", strlen((item->valuestring)));
			DB_PR("regst_key=%s\n", regst_key);//-----------------------------------------------
			// send_cmd_to_lcd_bl_len(0x1150,(uint8_t*)buff_t,32+4);//gekou 33 +3









			// //----------todo utf8 gbk-----------
			// DB_PR("%s\n", "��ȡ property �µ�cjson����");
			// item = cJSON_GetObjectItem(item2, "property");
			// DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			// DB_PR("%s\n", item->valuestring);

			// memset(buff_t,0,256);
			// memcpy(buff_t,item->valuestring,8* sizeof(item->valuestring));
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			// uart0_debug_data_h(buff_t,strlen((char*)buff_t));

			// //---------------------
			// DB_PR("%s\n", "��ȡ tag �µ�cjson����");
			// item = cJSON_GetObjectItem(item2, "tag");
			// DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			// DB_PR("%s\n", item->valuestring);

			// uart0_debug_data_h(item->valuestring,strlen((char*)(item->valuestring)));

			// // memset(buff_t,0,256);
			// memcpy(buff_t+8* sizeof(item->valuestring),item->valuestring,8* sizeof(item->valuestring));
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));



			//---------------------

			//---------------------
			DB_PR("%s\n", "��ȡ tag �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "company_name_gbk");
			DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%s\n", item->valuestring);

			uart0_debug_data_h(item->valuestring,strlen((char*)(item->valuestring)));

			memset(buff_t,0,256);
			memset(buff_t2,0,256);
			memcpy(buff_t ,item->valuestring,strlen((char*)(item->valuestring)));
			DB_PR("------sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));
			DB_PR("------strlen((char*)(item->valuestring)=%d\n", strlen((char*)(item->valuestring)));
			uart0_debug_data_h(buff_t,strlen((char*)(buff_t)));
			for (i = 0; i <strlen(buff_t)/3+strlen(buff_t)%3 -1; i++)//-----to
			{
				StrToHex(buff_t2+i,(char*)(buff_t+i*3),1);
				// buff_t2[i] = atoi((const char*)buff_t+i*3);		
				DB_PR("buff_t2[%d]=%02x\n",i, buff_t2[i]);			
			}

			send_cmd_to_lcd_bl_len(0x1020,(uint8_t*)buff_t2,32*7+4);//gekou 33 +3




			// utf8_to_gbk(tmp->valuestring, gbk);--------------------








			//-----------company------------
			DB_PR("%s\n", "��ȡresult�µ�cjson����:");
			item = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "��ȡ company �µ�cjson����");
			item2 = cJSON_GetObjectItem(item, "company");

			DB_PR("%s\n", "��ȡ company_id �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "company_id");
			DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%d\n", item->valueint);
			company_id = item->valueint;

			
			memset(buff_t,0,256);
			sprintf(buff_t,"%s%d",url_t,company_id);

			index_m = strlen(buff_t);
			DB_PR("--index_m=%d--\n", index_m);
			buff_t[index_m]=0xff;
			buff_t[index_m+1]=0xff;

			
			uart0_debug_data_h(buff_t,256);

			// //-----------ads------------
			// DB_PR("%s\n", "��ȡresult�µ�cjson����:");
			// item = cJSON_GetObjectItem(root, "result");//
			// // DB_PR("%s\n", cJSON_Print(item));

			// DB_PR("%s\n", "��ȡ ads �µ�cjson����");
			// item2 = cJSON_GetObjectItem(item, "ads");

			// item3 = cJSON_GetArrayItem(item2, 0);



			// DB_PR("%s\n", "��ȡ url �µ�cjson����");
			// item = cJSON_GetObjectItem(item3, "url");
			// DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			// DB_PR("%s\n", item->valuestring);

			// memset(buff_t,0,256);
			// memcpy(buff_t,item->valuestring,strlen(item->valuestring));
			// // sprintf(buff_t,"%sffff",buff_t);
			// buff_t[strlen(item->valuestring)]=0xff;
			// buff_t[strlen(item->valuestring)+1]=0xff;
			// DB_PR("sizeof(item->valuestring)=%d\n", sizeof(item->valuestring));

			send_cmd_to_lcd_bl_len(0x3000,(uint8_t*)buff_t,128+4);//gekou 33 +3






//-----------------------------------------------------------
			// delay_ms(1000); //500
			// USART2_RX_STA=0;
			// DB_PR("...a-9-1...\n");
			//52
			sim900a_send_cmd("AT+QHTTPURL=52,80","CONNECT",800);// != GSM_TRUE) return GSM_FALSE;//"OK"
			DB_PR("...a-9...\n");


			
			// sim900a_send_cmd_tou_data("http://express.admin.modoubox.com/web_wechat/deliver/qrcode","OK",800);
			sim900a_send_cmd_tou_data("https://iot.xintiangui.com/web_wechat/deliver/qrcode","OK",800);
			DB_PR("...a-10...\n");


			// //USART2_RX_STA =0;
			// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
			// DB_PR("...a-11-1...\n");



			// delay_ms(1000); //500
			for(i=0;i<3;i++)
			{
				// delay_ms(100); //500
				//USART2_RX_STA =0;  86
				DB_PR("-------i=%d---------\n",i);
				memset(regst_key_post,0,sizeof(regst_key_post));
				sprintf(regst_key_post,"AT+QHTTPPOST=%d,80,80",46+strlen(regst_key));
				//sim900a_send_cmd(regst_key_post,"CONNECT",500);// != GSM_TRUE) return GSM_FALSE;//"OK"
				if(0==sim900a_send_cmd(regst_key_post,"CONNECT",1000))
				{
					DB_PR("...a-10-1...\n");
				}
				else
				{
					DB_PR("...a-10-2 err...\n");
					continue;
				}
				
				
				DB_PR("...a-11...\n");

				//delay_ms(1000); //500




				memset(regst_key_post,0,sizeof(regst_key_post));
				//Content-Type:application/x-www-form-urlencoded\r\n
				//Content-Type=application/x-www-form-urlencoded
				sprintf(regst_key_post,"from=cabinet&register_key=%s&type=qrcode_content",regst_key);//

				// sprintf(regst_key_post,"from=cabinet&register_key=%s&type=qrcode_content",regst_key);
				// DB_PR("strlen(regst_key_post)=%d\n",strlen(regst_key_post));
				uart0_debug_str(regst_key_post,strlen(regst_key_post));
				// uart0_debug_data_h(regst_key_post,strlen(regst_key_post));
				//sim900a_send_cmd(regst_key_post,"OK",1500);
				// sim900a_send_cmd("from=cabinet&register_key=register:7c772404a1fda38b4f0a42b8f013ae2&type=qrcode_content","OK",12000);
				if(0==sim900a_send_cmd_tou_data(regst_key_post,"+QHTTPPOST:",900))
				{
					DB_PR("...a-11-1...\n");

				}
				else
				{
					DB_PR("...a-11-2  err...\n");
					continue;
				}
				DB_PR("...a-12...\n");

				
				//delay_ms(1000); //500
				//delay_ms(1000); //500
				// delay_xs(30);

				//reg_status3 = sim_at_response_https(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
				if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD",900))// != GSM_TRUE) return GSM_FALSE;//"OK"
				{ 
					DB_PR("...a-13...\n");
					if(USART2_RX_STA&0X8000)		//���յ�һ��������
					{ 
						USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
						DB_PR("%s",USART2_RX_BUF);	//���͵�����

						if(1==cjson_to_struct_info_qrcode((char*)USART2_RX_BUF))
						{
							DB_PR("...a-13-2...\n");
							break;
						}
						// USART2_RX_STA=0;
					}
					USART2_RX_STA=0;


					// cJSON_Delete(root);
					// return reg_status;
				} 
				else
				{
					DB_PR("...a-13-2 err...\n");
					continue;
				}

				// send_cmd_to_lcd_pic(0x0003);//---------------
			}

		}


	}

	cJSON_Delete(root);
	return reg_status;

}







// u16 sim_at_response_https(u8 mode)
// {
// 	u16 reg_status2=0x000f;
// 	if(USART2_RX_STA&0X8000)		//���յ�һ��������
// 	{ 
// 		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
// 		DB_PR("%s",USART2_RX_BUF);	//���͵�����


// 		reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
		
// 		//cjson_dbg();

// 		if(mode)
// 			USART2_RX_STA=0;
// 	} 
// 	return reg_status2;
// }




int captcha_id=0;


u16 cjson_to_struct_info_overtime_pay(char *text)
{
	u8 my_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson����

	cJSON * item2 = NULL;//cjson����
	
	cJSON * item3 = NULL;//cjson����
	int i=0;


	u32 reg_active_code =0;
	uint8_t buff_t[256]={0};
	//uint8_t buff_t2[256]={0};

	char* buff_t3="b5";
	int my_bl_ts=0;
	//��Ƭ������
	// uint8_t buff_t3[256]={0xb5 ,0xa5 ,0xc6 ,0xac ,0xbb ,0xfa ,0xb2 ,0xe2 ,0xca ,0xd4 ,0x32};
	
	int company_id=0;
	char* url_t="https://iot.xintiangui.com/web_wechat/download_app?cid=";

	char qhttp_post_req[150]={0};

	char regst_key_post[300];
	u16 index_m=0;

	int fee_money=0;

	// const char needle[10] = "\r\n";
	// char *ret;
	
	// //�豸ע��ɹ�,�뼤�����
	// char* str_tmp="{\"status\":0,\"msg\":\"abc\",\"result\":{\"active_code\":87993541}}";
	
	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //��ȡ��Чjson
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');
    // char *index=strstr(text,"{\"post_data\":{");
    // bzero(text, sizeof(text));
    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);
	// 	memset(text,0,89);
	// 	memcpy(text,index,89);
	// 	text[89]='\0';

	// DB_PR("---strlen(text)= %d\n", strlen(text));
	// DB_PR("---89= %02x\n", text[89]);
	// DB_PR("---90= %02x\n", text[90]);
 
	// for(i=89;i<strlen(text);i++)
	// {
	// 	text[i]=0;
	// }


//    ret = strstr(text, needle);
	DB_PR("\n----2----text=\n%s\n",text);










	// DB_PR("str_tmp=%s",str_tmp);


    root = cJSON_Parse(text);     
    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
        DB_PR("%s\n", "�и�ʽ�ķ�ʽ��ӡJson:");           
        // DB_PR("%s\n\n", cJSON_Print(root));
        // DB_PR("%s\n", "�޸�ʽ��ʽ��ӡjson��");
        // DB_PR("%s\n\n", cJSON_PrintUnformatted(root));





		DB_PR("%s\n", "��ȡmoney fee �µ�cjson����");
		item = cJSON_GetObjectItem(root, "status");
		DB_PR("-----------%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
		DB_PR("%d\n", item->valueint);
		my_status = item->valueint;//status ==0

		//////if(my_status ==0)
		{
			//-----------company------------
			DB_PR("%s\n", "��ȡresult�µ�cjson����:");
			item2 = cJSON_GetObjectItem(root, "result");//
			// DB_PR("%s\n", cJSON_Print(item));

			DB_PR("%s\n", "��ȡ qrcode �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "qrcode");
			DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%s\n", item->valuestring);
			


			memset(buff_t,0,256);
			strcpy(buff_t,item->valuestring);
			// sprintf(buff_t,"%s%d",url_t,company_id);

			index_m = strlen(buff_t);
			DB_PR("--index_m=%d--\n", index_m);
			buff_t[index_m]=0xff;
			buff_t[index_m+1]=0xff;

			
			uart0_debug_data_h(buff_t,256);


			send_cmd_to_lcd_bl_len(0x4000,(uint8_t*)buff_t,128+4);//gekou 33 +3






			DB_PR("%s\n", "��ȡmoney fee �µ�cjson����");
			item = cJSON_GetObjectItem(item2, "fee");
			DB_PR("-----------%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
			DB_PR("%d\n", item->valueint);
			fee_money = item->valueint;


			memset(buff_t,0,256);
			// itoa((int)(fee_money),(char*)(buff_t) ,10);

			DB_PR(" ((double)fee_money)/100=%f\n", ((double)fee_money)/100);
			sprintf(buff_t,"%.2f", ((double)fee_money)/100 ); 

			send_cmd_to_lcd_bl_len(0x1960,(uint8_t*)buff_t,32+4);//gekou 33 +3

			

			memset(buff_t,0,256);
			mtimer_flag =2;
			daojishi_time=30;
			TIM5_Set(1);//------------------
			sprintf((char*)buff_t, "%d", daojishi_time);
			DB_PR("-------daojishi_time  buff_t=%s--------\n",buff_t);
			send_cmd_to_lcd_bl_len(0x1950,buff_t,10+4);


			send_cmd_to_lcd_pic(0x0007);//-------chaoshsi yemian--------
			daojishi_ongo_flag =1;
			DB_PR("----1-7---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
		}


    }



    cJSON_Delete(root);
    return my_status;

}











int  heart_beart_idx =0;





u16 cjson_to_struct_info_tcp_rcv_overtime_pay_success(char *text)
{
	u8 ret_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson???��
	char regst_key_post[300];
//	cJSON * item2 = NULL;//cjson???��
//	cJSON * item3 = NULL;//cjson???��
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[40]={0};
	int i=0;
	u16 index_m=0;
	int16_t guimen_gk_temp =0;
	
	int size=0;

	char qhttp_post_req[150]={0};

	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //???????��json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);

	DB_PR("\n----2----text=\n%s\n",text);


    root = cJSON_Parse(text);     
    DB_PR("\n----3----\n");

    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {

		//---------------------
		DB_PR("%s\n", "��ȡtype�µ�cjson����");
		item = cJSON_GetObjectItem(root, "type");//
		DB_PR("--1--%s:", item->string);   //??????cjson???��???��???????????????��??????
		DB_PR("--2--%s\n", item->valuestring);
		// reg_status = item->valueint;
		// DB_PR("%s\n", cJSON_Print(item));
		




		daojishi_ongo_flag =0;
		DB_PR("----1-8c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
		// if(NULL!=strstr(item->valuestring, "stc:overtime_pay_success"))
		if(0==strcmp("stc:overtime_pay_success",item->valuestring))
		{
			send_cmd_to_lcd_pic(0x0008);
			// daojishi_time=2;
			// TIM5_Set(1);

			ret_status =1;
			DB_PR("...stc:overtime_pay_success...\n");
		}
		else
		{
			DB_PR("...stc:overtime_pay_    fail...\n");
		}
		


		printf("ret_status=%d\n",ret_status);
		
		
            //  uart0_debug_data_h(buff_t,256);
            // send_cmd_to_lcd_bl_len(0x2000,(uint8_t*)buff_t,128+4);//gekou 33 +3

    }



    cJSON_Delete(root);
    return ret_status;

}



u16 cjson_to_struct_info_tcp_rcv(char *text)
{
	u8 reg_status=0x000f;
	char *index;
	cJSON * root = NULL;
	cJSON * item = NULL;//cjson???��
	char regst_key_post[300];
//	cJSON * item2 = NULL;//cjson???��
//	cJSON * item3 = NULL;//cjson???��
	uint8_t buff_t[256]={0};
	uint8_t buff_t2[40]={0};
	int i=0;
	u16 index_m=0;
	int16_t guimen_gk_temp =0;
	
	int size=0;

	char qhttp_post_req[150]={0};

	char* temp_cjson=NULL;

	int cjson_len=0;
	u16 ret_value1=0;

	
    if( text == NULL)
    {
        DB_PR("\n----1 err----text=\n%s\n",text);
        return 0xffff;
    }
    // cJSON *root,*psub;

    // cJSON *arrayItem;

    //???????��json
    DB_PR("\n----1----text=\n%s\n",text);
    index=strchr(text,'{');

    if(NULL == index)
    {
        DB_PR("------NULL----4444----------\n");
        return 0xffff;
    }
    strcpy(text,index);

	DB_PR("\n----2----text=\n%s\n",text);







    root = cJSON_Parse(text);     


    DB_PR("\n----3----\n");

    if (!root) 
    {
        DB_PR("Error before: [%s]\n",cJSON_GetErrorPtr());
    }
    else
    {
			IWDG_Feed();
			DB_PR2("\n----TCP JSON IS ON----\n");
			temp_cjson =cJSON_PrintUnformatted(root);
			cjson_len = strlen(temp_cjson);
			// if(temp_cjson)
			{
				DB_PR("create js string is %s\n",temp_cjson);
				free(temp_cjson);
			}
			DB_PR("%s\n", "�޸�ʽ��ʽ��ӡjson��");
			DB_PR("---------\n%s\n------------\n\n",temp_cjson );
			DB_PR("---------cjson_len= %d---------\n\n",cjson_len );
			DB_PR("strlen(text)= %d \n\n",strlen(text) );

			//{"type":"stc:heartbeat","time":1606975970061}
			if(strlen(text)==cjson_len)
			{
				DB_PR("\n----1-1==   always----\n");
			}
			else//>
			{
				DB_PR("\n----1-2!=----\n");
				// cJSON_Delete(root);
				// cjson_to_struct_info_tcp_rcv(text);
				cjson_to_struct_info_tcp_rcv(text+cjson_len);
				
				DB_PR("\n----2----strlen(text+cjson_len)=%d\n",strlen(text+cjson_len));
				DB_PR("\n----2----text+cjson_len=\n%s\n",text+cjson_len);
			}
			
			
			//---------------------
			DB_PR("%s\n", "��ȡtype�µ�cjson����");
			item = cJSON_GetObjectItem(root, "type");//
			DB_PR("--1--%s:", item->string);   //??????cjson???��???��???????????????��??????
			DB_PR("--2--%s\n", item->valuestring);
			// reg_status = item->valueint;
			// DB_PR("%s\n", cJSON_Print(item));
			

			if(0==strcmp("stc:restart",item->valuestring))
			{
				//---------------------
				DB_PR("----------tcp will restart---------\n");   
				Soft_Reset();//
			}
			// else if(0==strcmp("stc:overtime_pay_success",item->valuestring))
			// {
			// 	daojishi_ongo_flag =0;
			// 	send_cmd_to_lcd_pic(0x0008);
			// 	daojishi_time=2;
			// 	TIM5_Set(1);

			// 	DB_PR("...stc:overtime_pay_success...\n");
			// }
			else if(0==strcmp("stc:opendoor",item->valuestring))
			{
				//---------------------
				DB_PR("----------tcp opendoor---------\n");   
				DB_PR("\n%s\n", "--2--һ��һ���Ļ�ȡ door_number ��ֵ��:");




				DB_PR("----2-1---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
				if(1== daojishi_ongo_flag)
				{
					for(i=0;i<30;i++)
					{
						DB_PR("------i=%d----\n",i);   
						delay_ms(100); //500
						if(USART2_RX_STA&0X8000)		//���յ�һ��������
						{
							// DB_PR("--------USART2_RX_BUF=sssssssssssss\n-------");
							USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
							DB_PR("--------timeout dbg1--------4G_UART_RCV=---------------------\r\n");
							// uart0_debug_data_h(data_rx_t,len_rx_t);
							DB_PR("%s",USART2_RX_BUF);	//���͵�����


							if(1==cjson_to_struct_info_tcp_rcv_overtime_pay_success((char*)USART2_RX_BUF))
							{
								daojishi_ongo_flag =0;
								DB_PR("----1-9c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
								DB_PR("--------timeout dbg2---------------------\r\n");
								// USART2_RX_STA=0;
								break;
							}
							else
							{
								DB_PR("--------timeout dbg3 err---------------------\r\n");
							}
							

							DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");
						} 

					}
					
					DB_PR("----my dbg-----i=%d---------\n",i);   
					if(i==30)//û���յ�֧���ɹ�
					{
						DB_PR("----my dbg3 timeout-----i=%d---------\n",i);   
						// daojishi_ongo_flag =0;
						DB_PR("----1-10c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
						// daojishi_time=30;
						// TIM5_Set(1);		

						// return //------------	
					}
					delay_ms(500); //500

					
				}
				
				IWDG_Feed();
				heart_beart_idx++;
				DB_PR2("-2-heart_beart_idx=%d\r\n",heart_beart_idx);


				DB_PR("%s\n", "��ȡ door_number �µ�cjson����");
				item = cJSON_GetObjectItem(root, "door_number");
				// DB_PR("%s\n", cJSON_Print(item));
				DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
				DB_PR("%d\n", item->valueint);
				guimen_gk_temp = item->valueint;
				DB_PR("---open lock-----guimen_gk_temp=%d\n", guimen_gk_temp);


				item = cJSON_GetObjectItem(root, "order_ary");
				// DB_PR("%s\n", cJSON_Print(item));
				item = cJSON_GetObjectItem(item, "data");
				DB_PR("%s\n", cJSON_Print(item));
				size = cJSON_GetArraySize(item);
				DB_PR("--------size=%d-----------\n",size);
				// fprintf(stdout, "key: %s:", "value2");

				for (i = 0; i < size; ++i) {
					cJSON* tmp = cJSON_GetArrayItem(item, i);
					buff_t[i] = tmp->valueint;
					// fprintf(stdout, " %f,", tmp->valuedouble);
					DB_PR("buff_t[%d]=%02x\n",i, buff_t[i]);
				}


				

				RS485_TX_EN();
				DB_PR("buff_t=");
				uart0_debug_data_h(buff_t, size);
				// uart_write_bytes(UART_NUM_LOCK, (const char *) tx_Buffer2, 11);
				Usart_SendArray( USART3,buff_t, size);//open door-------------------------------
				RS485_RX_EN();

				DB_PR("\n----------lock open-----------\n");  
				// buff_t2[0] = guimen_gk_temp/100 +0x30;
				// buff_t2[1] = guimen_gk_temp%100/100 +0x30;
				// buff_t2[2] = guimen_gk_temp%10 +0x30;



				printf("-----daojishi_time=%d-----\n",daojishi_time);
				DB_PR("----2-2---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
				if(0==daojishi_ongo_flag) 
				{
					qujianma_wait_tcp_flag =0;
					DB_PR("\n----------no daojishi yemian-----------\n");  
					itoa((int)(guimen_gk_temp),(char*)(buff_t2) ,10);
					// send_cmd_to_lcd_bl(0x1650,buff_t2);
					send_cmd_to_lcd_bl_len(0x1650,(uint8_t*)buff_t2,32+4);
					send_cmd_to_lcd_pic(0x0006); //kaimen ok

					daojishi_ongo_flag =0;
					DB_PR("----1-11c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
					daojishi_time=5;
					TIM5_Set(1);
				}
				// else
				// {
				// 	DB_PR("\n----------zhengzai daojishi-----------\n");  
				// }
				


				
			}
			else if(0==strcmp("stc:overtime_pay",item->valuestring))
			{
				send_cmd_to_lcd_pic(0x000a); //------------------
				daojishi_ongo_flag =0;
				DB_PR("----1-12c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
                daojishi_time=30;
                TIM5_Set(0);


				DB_PR("%s\n", "��ȡ captcha_id �µ�cjson����");
				item = cJSON_GetObjectItem(root, "captcha_id");
				// DB_PR("%s\n", cJSON_Print(item));
				DB_PR("%s:", item->string);   //��һ��cjson����Ľṹ������������Ա����˼
				DB_PR("%d\n", item->valueint);
				captcha_id = item->valueint;
				DB_PR("-----captcha_id=%d\n", captcha_id);




                //--------http----------------
				//-----------add  panduan qujianma pingbi?  todo
                DB_PR("...a-0-0...\n");
                // delay_ms(1000); //500
                delay_ms(1000); //500
                sim900a_send_cmd_tou_data("+++",0,0);//AT  
                // sim900a_send_cmd("+++","OK",3000);//AT
                DB_PR("...a-0-1...\n");
                delay_ms(1000); //500
                // delay_ms(1000); //500

				heart_beart_idx++;
				DB_PR2("-3-heart_beart_idx=%d\r\n",heart_beart_idx);
				for(i=0;i<2;i++)
				{
					IWDG_Feed();
					DB_PR("-------i=%d---------\n",i);
					//----------------------------
					sim900a_send_cmd("AT+QHTTPURL=70,80","CONNECT",600);// != GSM_TRUE) return GSM_FALSE;//"OK"
					DB_PR("...a-9...\n");


					//2-4
					sim900a_send_cmd_tou_data("http://xintian.modoubox.com/api_cabinet/Deliverorder/getOvertimeQrcode","OK",600);
					DB_PR("...a-10...\n");

					IWDG_Feed();

					//USART2_RX_STA =0;  86
					// memset(regst_key_post,0,sizeof(regst_key_post));
					memset(regst_key_post,0,sizeof(regst_key_post));
					sprintf(regst_key_post,"captcha_id=%d&register_key=%s",captcha_id,regst_key);//
					uart0_debug_str(regst_key_post,strlen(regst_key_post));

					DB_PR("strlen(regst_key_post)=%d\n",strlen(regst_key_post));

					// //USART2_RX_STA =0;
					// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
					// DB_PR("...a-11-1...\n");





					// delay_ms(100); //500


					sprintf(qhttp_post_req,"AT+QHTTPPOST=%d,80,80",strlen(regst_key_post));
					// sim900a_send_cmd(qhttp_post_req,"CONNECT",15000);// != GSM_TRUE) return GSM_FALSE;//"OK"
					if(0==sim900a_send_cmd(qhttp_post_req,"CONNECT",800))
					{
						DB_PR("...a-10-1...\n");
					}
					else
					{
						DB_PR("...a-10-2 err...\n");
						continue;
					}
					// sim900a_send_cmd("AT+QHTTPPOST=99,80,80","CONNECT",125000);
					DB_PR("...a-11...\n");

					// #define POST_DATA_OPENDOOR "code=12345678&type=get_by_code&from=code-user&register_key=register:7c772404a1fda38b4f0a42b8f013ae2"
					uart0_debug_data_h(regst_key_post,strlen(regst_key_post));
					if(0==sim900a_send_cmd_tou_data(regst_key_post,"+QHTTPPOST:",500))
					{
						DB_PR("...a-11-1...\n");
						// if(NULL!=strstr(USART2_RX_BUF,"+QHTTPPOST:"))

					}
					else
					{
						DB_PR("...a-11-2  err...\n");
						continue;
					}
					// sim900a_send_cmd(POST_DATA_OPENDOOR,"OK",1000);
					
					DB_PR("...a-12...\n");

					IWDG_Feed();
					delay_ms(50);
					// delay_ms(1000); //500
					// delay_ms(500); //500
					// delay_xs(30);

					//reg_status3 = sim_at_response_https(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
					// if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD",500))
					if(0==sim900a_send_cmd("AT+QHTTPREAD=80","OK",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
					{ 
						delay_ms(100);
						DB_PR("...a-13...\n");
						// if(USART2_RX_STA&0X8000)		//���յ�һ��������
						{ 
							USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
							DB_PR("%s",USART2_RX_BUF);	//���͵�����

							//send_cmd_to_lcd_pic(0x0007);
							ret_value1=cjson_to_struct_info_overtime_pay((char*)USART2_RX_BUF);
							if((0x000f!=ret_value1)&&(0xffff!=ret_value1))//
							{
								qujianma_wait_tcp_flag =0;
								DB_PR("...a-13-1-1 ok...\n");
								break;
							}
							else
							{
								DB_PR("...a-13-2-2 err...\n");
							}
							
							// USART2_RX_STA=0;
						}
						// cJSON_Delete(root);
						// return reg_status;
						// break;
					} 
					else
					{
						DB_PR("...a-13-2 err...\n");
						continue;
					}
				}

				DB_PR("----zhifu timeout?---i=%d---------\n",i);
				if(i==2)
				{
					DB_PR("...b-http timeout...\n");
					send_cmd_to_lcd_pic(0x0001);
					delay_ms(1000); 
					send_cmd_to_lcd_pic(0x0003);
				}
				else
				{
					DB_PR("...b-ok...\n");
				}




                // delay_ms(1000); //500
                // sim900a_send_cmd("AT+QISWTMD=0,2\r\n","OK",2000);
                sim900a_send_cmd("AT+QISWTMD=0,2",0,0);
				USART2_RX_STA=0;
			}
			else if(0==strcmp("stc:heartbeat",item->valuestring))
			{
				//---------------------
				DB_PR("----------stc:heartbeat---------\n");   
				heart_beart_idx++;
				DB_PR2("heart_beart_idx=%d\r\n",heart_beart_idx);
			}
			else
			{
				DB_PR("------tcp other-------\n");
			}
			
			


			
			

		
            //  uart0_debug_data_h(buff_t,256);
            // send_cmd_to_lcd_bl_len(0x2000,(uint8_t*)buff_t,128+4);//gekou 33 +3

    }



    cJSON_Delete(root);
    return reg_status;

}

void sim_at_response(u8 mode)
{
	int reg_status2=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		DB_PR("USART2_RX_BUF=sssssssssssss\n");
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		DB_PR("--4G_UART_RCV=--\r\n");
        // uart0_debug_data_h(data_rx_t,len_rx_t);
		DB_PR("TCP RCV---\n%s\n----",USART2_RX_BUF);	//���͵�����

		USART2_RX_STA =0;
		cjson_to_struct_info_tcp_rcv((char*)USART2_RX_BUF);



		// reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
		
		//cjson_dbg();

		// if(mode)
		// 	USART2_RX_STA=0;

		DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");
	} 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A �������(���Ų��ԡ����Ų��ԡ�GPRS����)���ô���

//sim900a���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* sim900a_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}




//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim900a_send_cmd_tou_data(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s",cmd);//��������
		DB_PR("send to 4G data=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


//��sim900a��������
//cmd:���͵������ַ���(����Ҫ��ӻس���),��cmd<0XFF��ʱ��,��������(���緢��0X1A),���ڵ�ʱ�����ַ���.
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 sim900a_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	if((u32)cmd<=0XFF)
	{
		while(DMA1_Channel7->CNDTR!=0);	//�ȴ�ͨ��7�������   
		USART2->DR=(u32)cmd;
	}
	else 
	{
		u2_printf("%s\r\n",cmd);//��������		
		DB_PR("sendto 4G cmd=\n%s\n-------\r\n",cmd);
	}

	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(sim900a_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
			delay_ms(10);
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��1���ַ�ת��Ϊ16��������
//chr:�ַ�,0~9/A~F/a~F
//����ֵ:chr��Ӧ��16������ֵ
u8 sim900a_chr2hex(u8 chr)
{
	if(chr>='0'&&chr<='9')return chr-'0';
	if(chr>='A'&&chr<='F')return (chr-'A'+10);
	if(chr>='a'&&chr<='f')return (chr-'a'+10); 
	return 0;
}
//��1��16��������ת��Ϊ�ַ�
//hex:16��������,0~15;
//����ֵ:�ַ�
u8 sim900a_hex2chr(u8 hex)
{
	if(hex<=9)return hex+'0';
	if(hex>=10&&hex<=15)return (hex-10+'A'); 
	return '0';
}
//unicode gbk ת������
//src:�����ַ���
//dst:���(uni2gbkʱΪgbk����,gbk2uniʱ,Ϊunicode�ַ���)
//mode:0,unicode��gbkת��;
//     1,gbk��unicodeת��;
void sim900a_unigbk_exchange(u8 *src,u8 *dst,u8 mode)
{
	u16 temp; 
	u8 buf[2];
	if(mode)//gbk 2 unicode
	{
		while(*src!=0)
		{
			if(*src<0X81)	//�Ǻ���
			{
				temp=(u16)ff_convert((WCHAR)*src,1);
				src++;
			}else 			//����,ռ2���ֽ�
			{
				buf[1]=*src++;
				buf[0]=*src++; 
				temp=(u16)ff_convert((WCHAR)*(u16*)buf,1); 
			}
			*dst++=sim900a_hex2chr((temp>>12)&0X0F);
			*dst++=sim900a_hex2chr((temp>>8)&0X0F);
			*dst++=sim900a_hex2chr((temp>>4)&0X0F);
			*dst++=sim900a_hex2chr(temp&0X0F);
		}
	}else	//unicode 2 gbk
	{ 
		while(*src!=0)
		{
			buf[1]=sim900a_chr2hex(*src++)*16;
			buf[1]+=sim900a_chr2hex(*src++);
			buf[0]=sim900a_chr2hex(*src++)*16;
			buf[0]+=sim900a_chr2hex(*src++);
 			temp=(u16)ff_convert((WCHAR)*(u16*)buf,0);
			if(temp<0X80){*dst=temp;dst++;}
			else {*(u16*)dst=swap16(temp);dst+=2;}
		} 
	}
	*dst=0;//��ӽ�����
} 
//�������
const u8* kbd_tbl1[13]={"1","2","3","4","5","6","7","8","9","*","0","#","DEL"};
const u8* kbd_tbl2[13]={"1","2","3","4","5","6","7","8","9",".","0","#","DEL"};
u8** kbd_tbl;
u8* kbd_fn_tbl[2];
//���ؼ��̽��棨�ߴ�Ϊ240*140��
//x,y:������ʼ���꣨320*240�ֱ��ʵ�ʱ��x����Ϊ0��
void sim900a_load_keyboard(u16 x,u16 y,u8 **kbtbl)
{
	u16 i;
	POINT_COLOR=RED;
	kbd_tbl=kbtbl;
	LCD_Fill(x,y,x+240,y+140,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+140);						   
	LCD_DrawRectangle(x+80,y,x+160,y+140);	 
	LCD_DrawRectangle(x,y+28,x+240,y+56);
	LCD_DrawRectangle(x,y+84,x+240,y+112);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i<13)Show_Str_Mid(x+(i%3)*80,y+6+28*(i/3),(u8*)kbd_tbl[i],16,80);
		else Show_Str_Mid(x+(i%3)*80,y+6+28*(i/3),kbd_fn_tbl[i-13],16,80); 
	}  		 					   
}
//����״̬����
//x,y:��������
//key:��ֵ��0~8��
//sta:״̬��0���ɿ���1�����£�
void sim900a_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>15)return;
	if(sta)LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,GREEN);
	else LCD_Fill(x+j*80+1,y+i*28+1,x+j*80+78,y+i*28+26,WHITE); 
	if(j&&(i>3))Show_Str_Mid(x+j*80,y+6+28*i,(u8*)kbd_fn_tbl[keyx-13],16,80);
	else Show_Str_Mid(x+j*80,y+6+28*i,(u8*)kbd_tbl[keyx],16,80);		 		 
}
//�õ�������������
//x,y:��������
//����ֵ��������ֵ��1~15��Ч��0,��Ч��
u8 sim900a_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ������£�1~15��1~15�Ű�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*28+28)&&tp_dev.y[0]>(y+i*28))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					sim900a_key_staset(x,y,key_x-1,0);
					key_x=key;
					sim900a_key_staset(x,y,key_x-1,1);
				}
				break;
			}
		}  
	}else if(key_x) 
	{
		sim900a_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//���Ų��Բ��ִ���

//sim900a���Ų���
//���ڲ���绰�ͽ����绰
//����ֵ:0,����
//    ����,�������
u8 sim900a_call_test(void)
{
	u8 key;
	u16 lenx;
	u8 callbuf[20]; 
	u8 pohnenumlen=0;	//���볤��,���15���� 
	u8 *p,*p1,*p2;
	u8 oldmode=0;
	u8 cmode=0;	//ģʽ
				//0:�ȴ�����
				//1:������
	            //2:ͨ����
				//3:���յ����� 
	LCD_Clear(WHITE);
	if(sim900a_send_cmd("AT+CLIP=1","OK",200))return 1;	//����������ʾ  
	if(sim900a_send_cmd("AT+COLP=1","OK",200))return 2;	//���ñ��к�����ʾ
 	p1=mymalloc(20);								//����20ֱ�����ڴ�ź���
	if(p1==NULL)return 2;	
	POINT_COLOR=RED;
	Show_Str_Mid(0,30,"ATK-SIM900A ���Ų���",16,240);				    	 
	Show_Str(40,70,200,16,"�벦��:",16,0); 
	kbd_fn_tbl[0]="����";
	kbd_fn_tbl[1]="����"; 
	sim900a_load_keyboard(0,180,(u8**)kbd_tbl1);
	POINT_COLOR=BLUE; 
	while(1)
	{
		delay_ms(10);
		if(USART2_RX_STA&0X8000)		//���յ�����
		{
			sim_at_response(0);
			if(cmode==1||cmode==2)
			{
				if(cmode==1)if(sim900a_check_cmd("+COLP:"))cmode=2;	//���ųɹ�
				if(sim900a_check_cmd("NO CARRIER"))cmode=0;	//����ʧ��
				if(sim900a_check_cmd("NO ANSWER"))cmode=0;	//����ʧ��
				if(sim900a_check_cmd("ERROR"))cmode=0;		//����ʧ��
			}
			if(sim900a_check_cmd("+CLIP:"))//���յ�����
			{
				cmode=3;
				p=sim900a_check_cmd("+CLIP:");
				p+=8;
				p2=(u8*)strstr((const char *)p,"\"");
				p2[0]=0;//��ӽ����� 
				strcpy((char*)p1,(char*)p);
			}
			USART2_RX_STA=0;
		}
		key=sim900a_get_keynum(0,180);
		if(key)
		{ 
			if(key<13)
			{
				if(cmode==0&&pohnenumlen<15)
				{ 
					callbuf[pohnenumlen++]=kbd_tbl[key-1][0];
					u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0]); 
				}else if(cmode==2)//ͨ����
				{ 
					u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0]);
					delay_ms(100);
					u2_printf("AT+VTS=%c\r\n",kbd_tbl[key-1][0]); 
					LCD_ShowChar(40+56,90,kbd_tbl[key-1][0],16,0);
				}
			}else
			{
				if(key==13)if(pohnenumlen&&cmode==0)pohnenumlen--;//ɾ��
				if(key==14)//ִ�в���
				{
					if(cmode==0)//����ģʽ
					{
						callbuf[pohnenumlen]=0;			//����������� 
						u2_printf("ATD%s;\r\n",callbuf);//����
						delay_ms(10);		        	//�ȴ�10ms  
						cmode=1;						//������ģʽ
					}else 
					{
						sim900a_send_cmd("ATH","OK",100);//�һ�
						sim900a_send_cmd("ATH","OK",100);//�һ�
						cmode=0;
					}
				}
				if(key==15)
				{
					if(cmode==3)//���յ�����
					{
						sim900a_send_cmd("ATA","OK",200);//����Ӧ��ָ��
						Show_Str(40+56,70,200,16,callbuf,16,0);
						cmode=2;
					}else
					{ 
						sim900a_send_cmd("ATH",0,0);//������û����ͨ��,������ͨ��
						break;//�˳�ѭ��
					}
				}
			} 
			if(cmode==0)//ֻ���ڵȴ�����ģʽ��Ч
			{
				callbuf[pohnenumlen]=0; 
				LCD_Fill(40+56,70,239,70+16,WHITE);
				Show_Str(40+56,70,200,16,callbuf,16,0);  	
			}				
		}
		if(oldmode!=cmode)//ģʽ�仯��
		{
			switch(cmode)
			{
				case 0: 
					kbd_fn_tbl[0]="����";
					kbd_fn_tbl[1]="����"; 
					POINT_COLOR=RED;
					Show_Str(40,70,200,16,"�벦��:",16,0);  
					LCD_Fill(40+56,70,239,70+16,WHITE);
					if(pohnenumlen)
					{
						POINT_COLOR=BLUE;
						Show_Str(40+56,70,200,16,callbuf,16,0);
					}
					break;
				case 1:
					POINT_COLOR=RED;
					Show_Str(40,70,200,16,"������:",16,0); 
					pohnenumlen=0;
				case 2:
					POINT_COLOR=RED;
					if(cmode==2)Show_Str(40,70,200,16,"ͨ����:",16,0); 
					kbd_fn_tbl[0]="�Ҷ�";
					kbd_fn_tbl[1]="����"; 	
					break;
				case 3:
					POINT_COLOR=RED;
					Show_Str(40,70,200,16,"������:",16,0); 
					POINT_COLOR=BLUE;
					Show_Str(40+56,70,200,16,p1,16,0); 
					kbd_fn_tbl[0]="�Ҷ�";
					kbd_fn_tbl[1]="����"; 
					break;				
			}
			if(cmode==2)Show_Str(40,90,200,16,"DTMF��:",16,0);	//ͨ����,����ͨ����������DTMF��
			else LCD_Fill(40,90,120,90+16,WHITE);
			sim900a_load_keyboard(0,180,(u8**)kbd_tbl1);		//��ʾ���� 
			oldmode=cmode; 
		}
		if((lenx%50)==0)LED0=!LED0; 	    				 
		lenx++;	 
	} 
	myfree(p1);
	return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//���Ų��Բ��ִ���

//SIM900A�����Ų���
void sim900a_sms_read_test(void)
{ 
	u8 *p,*p1,*p2;
	u8 timex=0;
	u8 msgindex[3];
	u8 msglen=0;
	u8 msgmaxnum=0;		//�����������
	u8 key=0;
	u8 smsreadsta=0;	//�Ƿ��ڶ�����ʾ״̬
	p=mymalloc(200);//����200���ֽڵ��ڴ�
	LCD_Clear(WHITE); 
	POINT_COLOR=RED;
	Show_Str_Mid(0,30,"ATK-SIM900A �����Ų���",16,240);				    	 
	Show_Str(30,50,200,16,"��ȡ:     ����Ϣ:",16,0); 	
	kbd_fn_tbl[0]="��ȡ";
	kbd_fn_tbl[1]="����"; 
	sim900a_load_keyboard(0,180,(u8**)kbd_tbl1);//��ʾ����  
	while(1)
	{
		key=sim900a_get_keynum(0,180);
		if(key)
		{  
			if(smsreadsta)
			{
				LCD_Fill(30,75,239,179,WHITE);//�����ʾ�Ķ�������
				smsreadsta=0;
			}
			if(key<10||key==11)
			{
				if(msglen<2)
				{ 
					msgindex[msglen++]=kbd_tbl[key-1][0];
					u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0]); 
				} 
				if(msglen==2)
				{
					key=(msgindex[0]-'0')*10+msgindex[1]-'0';
					if(key>msgmaxnum)
					{
						msgindex[0]=msgmaxnum/10+'0';
						msgindex[1]=msgmaxnum%10+'0';					
					}
				} 
			}else
			{
				if(key==13)if(msglen)msglen--;//ɾ��  
				if(key==14&&msglen)//ִ�ж�ȡ����
				{ 
					LCD_Fill(30,75,239,179,WHITE);//���֮ǰ����ʾ	   	 
					sprintf((char*)p,"AT+CMGR=%s",msgindex);
					if(sim900a_send_cmd(p,"+CMGR:",200)==0)//��ȡ����
					{
						POINT_COLOR=RED;
						Show_Str(30,75,200,12,"״̬:",12,0);
						Show_Str(30+75,75,200,12,"����:",12,0);
						Show_Str(30,90,200,12,"����ʱ��:",12,0);
						Show_Str(30,105,200,12,"����:",12,0);
						POINT_COLOR=BLUE;
						if(strstr((const char*)(USART2_RX_BUF),"UNREAD")==0)Show_Str(30+30,75,200,12,"�Ѷ�",12,0);
						else Show_Str(30+30,75,200,12,"δ��",12,0);
						p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
						p2=(u8*)strstr((const char*)(p1+2),"\"");
						p2[0]=0;//���������
						sim900a_unigbk_exchange(p1+2,p,0);			//��unicode�ַ�ת��Ϊgbk�� 
						Show_Str(30+75+30,75,200,12,p,12,0);		//��ʾ�绰����
						p1=(u8*)strstr((const char*)(p2+1),"/");
						p2=(u8*)strstr((const char*)(p1),"+");
						p2[0]=0;//���������
						Show_Str(30+54,90,200,12,p1-2,12,0);		//��ʾ����ʱ��  
						p1=(u8*)strstr((const char*)(p2+1),"\r");	//Ѱ�һس���
						sim900a_unigbk_exchange(p1+2,p,0);			//��unicode�ַ�ת��Ϊgbk��
						Show_Str(30+30,105,180,75,p,12,0);			//��ʾ��������
						smsreadsta=1;								//�������ʾ�������� 
					}else
					{
						Show_Str(30,75,200,12,"�޶�������!!!����!!",12,0);
						delay_ms(1000);
						LCD_Fill(30,75,239,75+12,WHITE);//�����ʾ
					}	  
					USART2_RX_STA=0;
				}
				if(key==15)break;
			} 
			msgindex[msglen]=0; 
			LCD_Fill(30+40,50,86,50+16,WHITE);
			Show_Str(30+40,50,86,16,msgindex,16,0);  	
		}
		if(timex==0)		//2.5�����Ҹ���һ��
		{
			if(sim900a_send_cmd("AT+CPMS?","+CPMS:",200)==0)	//��ѯ��ѡ��Ϣ�洢��
			{ 
				p1=(u8*)strstr((const char*)(USART2_RX_BUF),","); 
				p2=(u8*)strstr((const char*)(p1+1),",");
				p2[0]='/'; 
				if(p2[3]==',')//С��64K SIM�������洢��ʮ������
				{
					msgmaxnum=(p2[1]-'0')*10+p2[2]-'0';		//��ȡ���洢��������
					p2[3]=0;
				}else //�����64K SIM�������ܴ洢100�����ϵ���Ϣ
				{
					msgmaxnum=(p2[1]-'0')*100+(p2[2]-'0')*10+p2[3]-'0';//��ȡ���洢��������
					p2[4]=0;
				}
				sprintf((char*)p,"%s",p1+1);
				Show_Str(30+17*8,50,200,16,p,16,0);
				USART2_RX_STA=0;		
			}
		}	
		if((timex%20)==0)LED0=!LED0;//200ms��˸ 
		timex++;
		delay_ms(10);
		if(USART2_RX_STA&0X8000)sim_at_response(1);//����GSMģ����յ������� 
	}
	myfree(p); 
}
//���Զ��ŷ�������(70����[UCS2��ʱ��,1���ַ�/���ֶ���1����])
const u8* sim900a_test_msg="���ã�����һ�����Զ��ţ���ATK-SIM900A GSMģ�鷢�ͣ�ģ�鹺���ַ:http://eboard.taobao.com��лл֧�֣�";
//SIM900A�����Ų��� 
void sim900a_sms_send_test(void)
{
	u8 *p,*p1,*p2;
	u8 phonebuf[20]; 		//���뻺��
	u8 pohnenumlen=0;		//���볤��,���15���� 
	u8 timex=0;
	u8 key=0;
	u8 smssendsta=0;		//���ŷ���״̬,0,�ȴ�����;1,����ʧ��;2,���ͳɹ� 
	p=mymalloc(100);	//����100���ֽڵ��ڴ�,���ڴ�ŵ绰�����unicode�ַ���
	p1=mymalloc(300);	//����300���ֽڵ��ڴ�,���ڴ�Ŷ��ŵ�unicode�ַ���
	p2=mymalloc(100);	//����100���ֽڵ��ڴ� ��ţ�AT+CMGS=p1 
	LCD_Clear(WHITE);  
	POINT_COLOR=RED;
	Show_Str_Mid(0,30,"ATK-SIM900A �����Ų���",16,240);				    	 
	Show_Str(30,50,200,16,"���͸�:",16,0); 	 
	Show_Str(30,70,200,16,"״̬:",16,0);
	Show_Str(30,90,200,16,"����:",16,0);  
	POINT_COLOR=BLUE;
	Show_Str(30+40,70,170,90,"�ȴ�����",16,0);//��ʾ״̬	
	Show_Str(30+40,90,170,90,(u8*)sim900a_test_msg,16,0);//��ʾ��������		
	kbd_fn_tbl[0]="����";
	kbd_fn_tbl[1]="����"; 
	sim900a_load_keyboard(0,180,(u8**)kbd_tbl1);//��ʾ���� 
	while(1)
	{
		key=sim900a_get_keynum(0,180);
		if(key)
		{   
			if(smssendsta)
			{
				smssendsta=0;
				Show_Str(30+40,70,170,90,"�ȴ�����",16,0);//��ʾ״̬	
			}
			if(key<10||key==11)
			{
				if(pohnenumlen<15)
				{ 
					phonebuf[pohnenumlen++]=kbd_tbl[key-1][0];
					u2_printf("AT+CLDTMF=2,\"%c\"\r\n",kbd_tbl[key-1][0]); 
				}
			}else
			{
				if(key==13)if(pohnenumlen)pohnenumlen--;//ɾ��  
				if(key==14&&pohnenumlen)				//ִ�з��Ͷ���
				{  
					Show_Str(30+40,70,170,90,"���ڷ���",16,0);			//��ʾ���ڷ���		
					smssendsta=1;		 
					sim900a_unigbk_exchange(phonebuf,p,1);				//���绰����ת��Ϊunicode�ַ���
					sim900a_unigbk_exchange((u8*)sim900a_test_msg,p1,1);//����������ת��Ϊunicode�ַ���.
					sprintf((char*)p2,"AT+CMGS=\"%s\"",p); 
					if(sim900a_send_cmd(p2,">",200)==0)					//���Ͷ�������+�绰����
					{ 		 				 													 
						u2_printf("%s",p1);		 						//���Ͷ������ݵ�GSMģ�� 
 						if(sim900a_send_cmd((u8*)0X1A,"+CMGS:",1000)==0)smssendsta=2;//���ͽ�����,�ȴ��������(��ȴ�10����,��Ϊ���ų��˵Ļ�,�ȴ�ʱ��᳤һЩ)
					}  
					if(smssendsta==1)Show_Str(30+40,70,170,90,"����ʧ��",16,0);	//��ʾ״̬
					else Show_Str(30+40,70,170,90,"���ͳɹ�",16,0);				//��ʾ״̬	
					USART2_RX_STA=0;
				}
				if(key==15)break;
			} 
			phonebuf[pohnenumlen]=0; 
			LCD_Fill(30+54,50,239,50+16,WHITE);
			Show_Str(30+54,50,156,16,phonebuf,16,0);  	
		}
		if((timex%20)==0)LED0=!LED0;//200ms��˸ 
		timex++;
		delay_ms(10);
		if(USART2_RX_STA&0X8000)sim_at_response(1);//����GSMģ����յ������� 
	}
	myfree(p);
	myfree(p1);
	myfree(p2); 
} 
//sms����������
void sim900a_sms_ui(u16 x,u16 y)
{ 
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	Show_Str_Mid(0,y,"ATK-SIM900A ���Ų���",16,240);  
	Show_Str(x,y+40,200,16,"��ѡ��:",16,0); 				    	 
	Show_Str(x,y+60,200,16,"KEY0:�����Ų���",16,0); 				    	 
	Show_Str(x,y+80,200,16,"KEY1:�����Ų���",16,0);				    	 
	Show_Str(x,y+100,200,16,"WK_UP:�����ϼ��˵�",16,0);
}
//sim900a���Ų���
//���ڶ����Ż��߷�����
//����ֵ:0,����
//    ����,�������
u8 sim900a_sms_test(void)
{
	u8 key;
	u8 timex=0;
	if(sim900a_send_cmd("AT+CMGF=1","OK",200))return 1;			//�����ı�ģʽ 
	if(sim900a_send_cmd("AT+CSCS=\"UCS2\"","OK",200))return 2;	//����TE�ַ���ΪUCS2 
	if(sim900a_send_cmd("AT+CSMP=17,0,2,25","OK",200))return 3;	//���ö���Ϣ�ı�ģʽ���� 
	sim900a_sms_ui(40,30);
	while(1)
	{
		key=KEY_Scan(0);
		if(key==KEY0_PRES)
		{ 
			sim900a_sms_read_test();
			sim900a_sms_ui(40,30);
			timex=0;
		}else if(key==KEY1_PRES)
		{ 
			sim900a_sms_send_test();
			sim900a_sms_ui(40,30);
			timex=0;			
		}else if(key==3)break;
		timex++;
		if(timex==20)
		{
			timex=0;
			LED0=!LED0;
		}
		delay_ms(10);
		sim_at_response(1);										//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
	} 
	sim900a_send_cmd("AT+CSCS=\"GSM\"","OK",200);				//����Ĭ�ϵ�GSM 7λȱʡ�ַ���
	return 0;
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//GPRS���Բ��ִ���

const u8 *modetbl[2]={"TCP","UDP"};//����ģʽ
//tcp/udp����
//����������,��ά������
//mode:0:TCP����;1,UDP����)
//ipaddr:ip��ַ
//port:�˿� 
void sim900a_tcpudp_test(u8 mode,u8* ipaddr,u8* port)
{ 
	u8 *p,*p1,*p2,*p3;
	u8 key;
	u16 timex=0;
	u8 count=0;
	const u8* cnttbl[3]={"��������","���ӳɹ�","���ӹر�"};
	u8 connectsta=0;			//0,��������;1,���ӳɹ�;2,���ӹر�; 
	u8 hbeaterrcnt=0;			//�������������,����5�������ź���Ӧ��,����������
	u8 oldsta=0XFF;
	p=mymalloc(100);		//����100�ֽ��ڴ�
	p1=mymalloc(100);	//����100�ֽ��ڴ�
	LCD_Clear(WHITE);  
	POINT_COLOR=RED; 
	if(mode)Show_Str_Mid(0,30,"ATK-SIM900A UDP���Ӳ���",16,240);
	else Show_Str_Mid(0,30,"ATK-SIM900A TCP���Ӳ���",16,240); 
	Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0); 	
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipaddr,port);
	Show_Str(30,65,200,12,p,12,0);			//��ʾIP��ַ�Ͷ˿�	
	Show_Str(30,80,200,12,"״̬:",12,0); 	//����״̬
	Show_Str(30,100,200,12,"��������:",12,0); 	//����״̬
	Show_Str(30,115,200,12,"��������:",12,0);	//�˿ڹ̶�Ϊ8086
	POINT_COLOR=BLUE;
	USART2_RX_STA=0;
	sprintf((char*)p,"AT+CIPSTART=\"%s\",\"%s\",\"%s\"",modetbl[mode],ipaddr,port);
	

	
	if(sim900a_send_cmd(p,"OK",500))return;		//��������
	while(1)
	{ 
		key=KEY_Scan(0);
		if(key==WKUP_PRES)//�˳�����		 
		{  
			sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//�ر�����
			sim900a_send_cmd("AT+CIPSHUT","SHUT OK",500);		//�ر��ƶ����� 
			break;											 
		}else if(key==KEY0_PRES&(hbeaterrcnt==0))				//��������(��������ʱ����)
		{
			Show_Str(30+30,80,200,12,"���ݷ�����...",12,0); 		//��ʾ���ݷ�����
			if(sim900a_send_cmd("AT+CIPSEND",">",500)==0)		//��������
			{ 
 				DB_PR("CIPSEND DATA:%s\r\n",p1);	 			//�������ݴ�ӡ������
				u2_printf("%s\r\n",p1);
				delay_ms(10);
				if(sim900a_send_cmd((u8*)0X1A,"SEND OK",1000)==0)Show_Str(30+30,80,200,12,"���ݷ��ͳɹ�!",12,0);//��ȴ�10s
				else Show_Str(30+30,80,200,12,"���ݷ���ʧ��!",12,0);
				delay_ms(500); 
			}else sim900a_send_cmd((u8*)0X1B,0,0);	//ESC,ȡ������ 
			oldsta=0XFF;	
		}
		if((timex%20)==0)
		{
			LED0=!LED0;
			count++;	
			if(connectsta==2||hbeaterrcnt>8)//�����ж���,��������8������û����ȷ���ͳɹ�,����������
			{
				sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",500);	//�ر�����
				sim900a_send_cmd("AT+CIPSHUT","SHUT OK",500);		//�ر��ƶ����� 
				sim900a_send_cmd(p,"OK",500);						//������������
				connectsta=0;	
 				hbeaterrcnt=0;
			}
			sprintf((char*)p1,"ATK-SIM900A %s���� %d  ",modetbl[mode],count);
			Show_Str(30+54,100,200,12,p1,12,0); 
		}
		if(connectsta==0&&(timex%200)==0)//���ӻ�û������ʱ��,ÿ2���ѯһ��CIPSTATUS.
		{
			sim900a_send_cmd("AT+CIPSTATUS","OK",500);	//��ѯ����״̬
			if(strstr((const char*)USART2_RX_BUF,"CLOSED"))connectsta=2;
			if(strstr((const char*)USART2_RX_BUF,"CONNECT OK"))connectsta=1;
		}
		if(connectsta==1&&timex>=600)//����������ʱ��,ÿ6�뷢��һ������
		{
			timex=0;
			if(sim900a_send_cmd("AT+CIPSEND",">",200)==0)//��������
			{
				sim900a_send_cmd((u8*)0X00,0,0);	//��������:0X00  
				delay_ms(20);						//�������ʱ
				sim900a_send_cmd((u8*)0X1A,0,0);	//CTRL+Z,�������ݷ���,����һ�δ���	
			}else sim900a_send_cmd((u8*)0X1B,0,0);	//ESC,ȡ������ 		
				
			hbeaterrcnt++; 
			DB_PR("hbeaterrcnt:%d\r\n",hbeaterrcnt);//������Դ���
		} 
		delay_ms(10);
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{ 
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;	//��ӽ����� 
			DB_PR("%s",USART2_RX_BUF);				//���͵�����  
			if(hbeaterrcnt)							//��Ҫ�������Ӧ��
			{
				if(strstr((const char*)USART2_RX_BUF,"SEND OK"))hbeaterrcnt=0;//��������
			}				
			p2=(u8*)strstr((const char*)USART2_RX_BUF,"+IPD");
			if(p2)//���յ�TCP/UDP����
			{
				p3=(u8*)strstr((const char*)p2,",");
				p2=(u8*)strstr((const char*)p2,":");
				p2[0]=0;//���������
				sprintf((char*)p1,"�յ�%s�ֽ�,��������",p3+1);//���յ����ֽ���
				LCD_Fill(30+54,115,239,130,WHITE);
				POINT_COLOR=BRED;
				Show_Str(30+54,115,156,12,p1,12,0); //��ʾ���յ������ݳ���
				POINT_COLOR=BLUE;
				LCD_Fill(30,130,210,319,WHITE);
				Show_Str(30,130,180,190,p2+1,12,0); //��ʾ���յ������� 
			}
			USART2_RX_STA=0;
		}
		if(oldsta!=connectsta)
		{
			oldsta=connectsta;
			LCD_Fill(30+30,80,239,80+12,WHITE);
			Show_Str(30+30,80,200,12,(u8*)cnttbl[connectsta],12,0); //����״̬
		} 
		timex++; 
	} 
	myfree(p);
	myfree(p1);
}
//gprs����������
void sim900a_gprs_ui(void)
{
	LCD_Clear(WHITE);  
	POINT_COLOR=RED;
	Show_Str_Mid(0,30,"ATK-SIM900A GPRSͨ�Ų���",16,240);	 
	Show_Str(30,50,200,16,"WK_UP:���ӷ�ʽ�л�",16,0); 	 	
	Show_Str(30,90,200,16,"���ӷ�ʽ:",16,0); 	//���ӷ�ʽͨ��WK_UP����(TCP/UDP)
	Show_Str(30,110,200,16,"IP��ַ:",16,0);		//IP��ַ���Լ�������
	Show_Str(30,130,200,16,"�˿�:",16,0);		//�˿ڹ̶�Ϊ8086
	kbd_fn_tbl[0]="����";
	kbd_fn_tbl[1]="����"; 
	sim900a_load_keyboard(0,180,(u8**)kbd_tbl2);//��ʾ���� 
} 




typedef enum{
    GSM_TRUE,
    GSM_FALSE,
    
}gsm_res_e;








// int str_test(char *pcBuf, char *pcRes)
// {
// 	char *pcBegin = NULL;
// 	char *pcEnd = NULL;
 
// 	pcBegin = strstr(pcBuf, "\n");
// 	pcEnd = strstr(pcBuf, "\r");
 
// 	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
// 	{
// 		DB_PR("Mail name not found!\n");
// 	}
// 	else
// 	{
// 		pcBegin += strlen(":");
// 		memcpy(pcRes, pcBegin, pcEnd-pcBegin);
// 	}
 
// 	return SUCCESS;
// }

//sim900a GPRS����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������


// void uart0_debug_data_h2(uint8_t* data,uint16_t len)//hex8
// {
// 	int i;
//     DB_PR("---2----debug_data:");
//     for(i=0;i<len;i++)
//         DB_PR("date[%d]=%02x \n",i,data[i]);
//     DB_PR("\r\n");
// }



// void tcp_http_init()
// {
// 	DB_PR("\r\n-------------1-tcp rst------- \r\n");
// 	//TCP 1
// 	while(1)
// 	{
// 		delay_ms(1000); //500
// 		if(0==sim900a_send_cmd("AT","OK", 100) )//!= GSM_TRUE) return GSM_FALSE;
// 		{
// 			DB_PR("...a0-1...\n");
// 			break;
// 		}
// 		else
// 		{
// 			DB_PR("...a0-2 wait...\n");		
// 		}
// 	}
// 	DB_PR("\r\n-------------2-tcp rst------- \r\n");




// 	//GSM_CLEAN_RX();  SIM READY?    2s
// 	if(0==sim900a_send_cmd("AT+CPIN?","+CPIN: READY", 500) )//!= GSM_TRUE) return GSM_FALSE;
// 	{
// 		DB_PR("...a1-1...\n");
// 	}
// 	else
// 	{
// 		send_cmd_to_lcd_pic(0x0001);
// 		DB_PR("...a1-2 err...\n");		
// 		// return;
// 		delay_ms(1000); //500
// 		delay_ms(1000); //500
// 		delay_ms(1000); //500
// 		Soft_Reset();
// 	}

	




// 	// //GSM_CLEAN_RX();
// 	// sim900a_send_cmd("AT+CSQ","+CSQ:", 150);// != GSM_TRUE) return GSM_FALSE;
// 	// DB_PR("...a2...\n");
	
// 	//
// 	//GSM_CLEAN_RX();+CREG: 0,1   todo
// 	if(0==sim900a_send_cmd("AT+CREG?","OK", 200))// != GSM_TRUE) return GSM_FALSE;
// 	{
// 		DB_PR("...a2-1...\n");
// 	}
// 	else
// 	{
// 		send_cmd_to_lcd_pic(0x0001);
// 		DB_PR("...a2-2 err...\n");		
// 		// return;
// 		Soft_Reset();
// 	}
	
// 	//GSM_CLEAN_RX();+CGREG: 0,1
// 	if(0==sim900a_send_cmd("AT+CGREG?","OK", 150))// != GSM_TRUE) return GSM_FALSE;
// 	{
// 		DB_PR("...a3-1...\n");
// 	}
// 	else
// 	{
// 		send_cmd_to_lcd_pic(0x0001);
// 		DB_PR("...a3-2 err...\n");		
// 		// return;
// 	}




// //----------------http-----------------------
	
// 	sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1","OK",200);
// 	// sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-1...\n");
	
// 	// sim900a_send_cmd("AT+QIACT?\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	// DB_PR("...a-2...\n");

// 	sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1","OK", 200);
// 	//sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\","" ,"" ,1\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-3...\n");


// 	sim900a_send_cmd("AT+QIACT=1","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-4...\n");
	
// 	sim900a_send_cmd("AT+QIACT?","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-4-2...\n");


// 	//------------------
// 	if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"express_tcp.xintiangui.com\"","OK",300))
// 	{
// 		DB_PR("...a4-1...\n");
// 	}
// 	else
// 	{
// 		// send_cmd_to_lcd_pic(0x0001);
// 		DB_PR("...a4-2 err...\n");		
// 		// return;
// 	}




// 	sim900a_send_cmd("AT+QHTTPCFG=\"sslctxid\",1","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-5...\n");
	
// 	//1
// 	sim900a_send_cmd("AT+QSSLCFG=\"sslversion\",1,4","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-6...\n");

// 	//0x0005
// 	sim900a_send_cmd("AT+QSSLCFG=\"ciphersuite\",1,0xFFFF","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-7...\n");

// 	//0
// 	sim900a_send_cmd("AT+QSSLCFG=\"seclevel\",1,0","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-8...\n");










// 	//TCP 2
// 	//CMNET CMNET
// 	//GSM_CLEAN_RX();
// 	sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1","OK", 200);// != GSM_TRUE) return GSM_FALSE;
// 	DB_PR("...1...\n");
	
// 	//GSM_CLEAN_RX();
// 	sim900a_send_cmd("AT+QIACT=1","OK", 1000);// != GSM_TRUE) return GSM_FALSE;
// 	DB_PR("...2...\n");
	
	
	
	
// 	USART2_RX_STA =0;
// 	//GSM_CLEAN_RX();register:53988b31ffdb2e7db9c9429b84f0f84  register:6ef6d30c49a08134897c6f2cc297063
// 	// if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"103.46.128.21\",12310,0,2\r\n","CONNECT", 1000)==0)//192.168.10.111
// 	// express_tcp.xintiangui.com
// 	if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"39.98.243.128\",8091,0,2","CONNECT", 1000)==0)
// 	{
// 		DB_PR("----conn-----\r\n");
// 		send_cmd_to_lcd_pic(0x0003);//---------------kaiji
// 		daojishi_ongo_flag =0;
// 		delay_ms(500); //500
// 			// delay_ms(1000); //500
// 			// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
		
// 		sim900a_send_cmd_tou_data(regst_key,0,0) ;
// 		// sim900a_send_cmd("register:43981c0ecf4dfadb2d9cc878c874ab8",0,0) ;
// 		// sim900a_send_cmd("touchuan",0,0) ;
// 			// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);

// 	}

// 	//	//����Ƿ�������
// 	//	while(gsm_cmd_check("CONNECT") != GSM_TRUE)//OK
// 	//	{		
// 	//		if(++testConnect >200)//��ȴ�20��   150s----------
// 	//		{
// 	//			return GSM_FALSE;
// 	//		}
// 	//		GSM_DELAY(100); 		
// 	//	}


// 	delay_ms(500); //500
// 	// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
// 	sim900a_send_cmd_tou_data("cabinet_heartbeat",0,0);	
// 	// sim900a_send_cmd("123",0,0);	
// 	// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);
	



// }





char IMEI_cRes[100] = {0};	
char deviceid_decrypt_c[33];
u8 sim900a_gprs_test(void)
{
	unsigned char deviceid_decrypt[16]={0};
	char deviceid_decrypt_c2[50]={0};
	const u8 *port="8086";	//�˿ڹ̶�Ϊ8086,����ĵ���8086�˿ڱ���������ռ�õ�ʱ��,���޸�Ϊ�������ж˿�
	u8 mode=0;				//0,TCP����;1,UDP����
	u8 key;
	u8 timex=0; 
	long timex_t=0; 
	long timex_t2=0; 
	u16 timex_t3=0; 
	u16 timex_t4=0; 
	// u8 ipbuf[16]; 		//IP����
	u8 iplen=0;			//IP���� 

	char tcp_ip[20]={0};
	char tcp_ip2[16]={0};
	char at_tcp_ip[64]={0};
	char *ptr =NULL;
	const char needle[10] = "AT+CGSN\r\r\n";
	char *ret;


	int i;
	// unsigned char IMEI_cRes[] = "admin";//21232f297a57a5a743894a0e4a801fc3  


	MD5_CTX md5;

	u16 reg_status3=0x000f;

//	sim900a_gprs_ui();	//����������
//	Show_Str(30+72,90,200,16,(u8*)modetbl[mode],16,0);	//��ʾ���ӷ�ʽ	
//	Show_Str(30+40,130,200,16,(u8*)port,16,0);			//��ʾ�˿� 	






// 	//GSM_CLEAN_RX();
// //	if(gsm_cmd("AT+CIPCLOSE=1\r","OK",200) != GSM_TRUE)
// 	sim900a_send_cmd("AT+QICLOSE=0\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-1...\n");

// 	//GSM_CLEAN_RX();
// 	sim900a_send_cmd("AT+QIDEACT=1\r\n","OK",400) ;//!= GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a0...\n");
	DB_PR2("\r\n-1-�������ܹ�\r\n");

chengxu_start_1:
	while(1)
	{
		delay_ms(1000); //500
		if(0==sim900a_send_cmd("AT","OK", 100) )//!= GSM_TRUE) return GSM_FALSE;
		{
			DB_PR("...a0-1...\n");
			break;
		}
		else
		{
			DB_PR("...a0-2 wait...\n");		
		}
	}

	
	DB_PR2("\r\n-2-�������ܹ�\r\n");



///////////////////////////////////////////////////////////////////


chengxu_start_2:
	//GSM_CLEAN_RX();  SIM READY?    2s
	if(0==sim900a_send_cmd("AT+CPIN?","+CPIN: READY", 200) )//!= GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a1-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a1-2 err...\n");		
		delay_xs(1); 
		Soft_Reset();
	}

	

	delay_xs(4); 
	DB_PR("\r\n-2-1-�������ܹ�\r\n");


	// //GSM_CLEAN_RX();
	// sim900a_send_cmd("AT+CSQ\r\n","+CSQ:", 150);// != GSM_TRUE) return GSM_FALSE;
	// DB_PR("...a2...\n");
	
	//
	//GSM_CLEAN_RX();+CREG: 0,1   todo
	delay_ms(100);
	if(0==sim900a_send_cmd("AT+CREG?","OK", 200))// != GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a2-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a2-2 err...\n");		
		Soft_Reset();
	}
	
	delay_ms(100);
	//GSM_CLEAN_RX();+CGREG: 0,1
	if(0==sim900a_send_cmd("AT+CGREG?","OK", 150))// != GSM_TRUE) return GSM_FALSE;
	{
		DB_PR("...a3-1...\n");
	}
	else
	{
		send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a3-2 err...\n");		
	}
	
	



//----------------http-----------------------
	
	delay_ms(50);
	sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1","OK",200);
	// sim900a_send_cmd("AT+QHTTPCFG=\"contextid\",1\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-1...\n");
	
	delay_ms(50);
	sim900a_send_cmd("AT+QIACT?","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-2...\n");
	
	delay_ms(50);
	sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1","OK", 200);
	//sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\","" ,"" ,1\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-3...\n");


	delay_ms(200);
	if(0==sim900a_send_cmd("AT+QIACT=1","OK",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
	{
		DB_PR("--1--\n%s\n-----\n",USART2_RX_BUF);
		DB_PR2("...a-4-1-1...\n");
	}
	else
	{
		delay_ms(2000);
		DB_PR2("...a-4-1-2 e...\n");
//		DB_PR2("--2--\n%s\n-----\n",USART2_RX_BUF);
//		if(0==sim900a_send_cmd("AT+QIDEACT=1","OK",400) )
//		{
//			DB_PR2("...a-4-1-2a...\n\n\n\n");
//			goto chengxu_start_2;

//		}
//		else
//		{
//			DB_PR2("...a-4-1-2b err...\n\n\n\n\n\n\n");
//			Soft_Reset();
//		}

	}
	








	

	
	sim900a_send_cmd("AT+QIACT?","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-4-2-1...\n");



	// if(0==sim900a_send_cmd("AT+QPING=1,\"iot.dev.modoubox.com\"","+QPING:",500))
	// {
	// 	delay_ms(1000); //500
	// 	delay_ms(1000); //500
	// 	DB_PR("...b-1...\n");
	// 	DB_PR("%s",USART2_RX_BUF);	//���͵�����
	// }
	// else
	// {
	// 	DB_PR("...b-2 err...\n");
	// }
	



// +QIURC: "dnsgip",0,1,600

// +QIURC: "dnsgip","39.98.243.128"

	//---------287e9v3367.zicp.vip---------
	// AT+QIDNSGIP=1,"www.baidu.com" iot.dev.modoubox.com   express_tcp.xintiangui.com
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"www.baidu.com\"","+QIURC:",2000))//300
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"287e9v3367.zicp.vip\"","+QIURC:",800))
	// if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"iot.dev.modoubox.com\"","+QIURC:",800))
	if(0==sim900a_send_cmd("AT+QIDNSGIP=1,\"express.tcp.xintiangui.com\"","+QIURC:",800))
	{
		DB_PR("...a4-3-1...\n");
		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{ 
			// DB_PR("USART2_RX_BUF=sssssssssssss\n");
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
			DB_PR("--4G_UART_RCV=--\r\n");
			// uart0_debug_data_h(data_rx_t,len_rx_t);
			DB_PR("%s",USART2_RX_BUF);	//���͵�����


			ptr = strrchr(USART2_RX_BUF, ',');
			if (ptr)
				DB_PR("The character ',' is at position: %s\n", ptr);
			else
				DB_PR("The character was not found\n");
			sprintf(tcp_ip,"%s",ptr+1);//39.98.243.128"

			memcpy(tcp_ip2,tcp_ip,strlen(tcp_ip)-2);
			DB_PR("tcp_ip2 =%s\n", tcp_ip2);

			uart0_debug_data_h(tcp_ip2,strlen(tcp_ip2));


			// ret_string_ip = strstr(haystack, needle);
			// cjson_to_struct_info_tcp_rcv((char*)USART2_RX_BUF);
			// DB_PR("tcp ip=\n%s\n");
			// reg_status2 = cjson_to_struct_info_register((char*)USART2_RX_BUF);
			//cjson_dbg();

			// if(mode)
			USART2_RX_STA=0;

			DB_PR("eeeeeeeeeeeee-4G\n\n");
		} 
	}
	else
	{
		// send_cmd_to_lcd_pic(0x0001);
		DB_PR("...a4-3-2 err...\n");		
		goto chengxu_start_2;
	}









	sim900a_send_cmd("AT+QHTTPCFG=\"sslctxid\",1","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-5...\n");
	
	//1
	sim900a_send_cmd("AT+QSSLCFG=\"sslversion\",1,4","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-6...\n");

	//0x0005
	sim900a_send_cmd("AT+QSSLCFG=\"ciphersuite\",1,0xFFFF","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-7...\n");

	//0
	sim900a_send_cmd("AT+QSSLCFG=\"seclevel\",1,0","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	DB_PR("...a-8...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"cacert\",1,\"RAM:cacert.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-1...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"clientcert\",1,\"RAM:clientcert.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-2...\n");

	// sim900a_send_cmd("AT+QSSLCFG=\"clientkey\",1,\"RAM:clientkey.pem\"\r\n","OK",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
	// DB_PR("...a-8-3...\n");








// 	sim900a_send_cmd("AT+QHTTPURL=49,80\r\n","CONNECT",200);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-9...\n");

// 	sim900a_send_cmd("https://cabinet.u-xuan.com/api/control_app/status","OK",200);
// 	DB_PR("...a-10...\n");

// 	sim900a_send_cmd("AT+QHTTPPOST=73,80,80\r\n","CONNECT",8000);// != GSM_TRUE) return GSM_FALSE;//"OK"
// 	DB_PR("...a-11...\n");

// #define http_upload_data     "device_id=00e5c9c6e22e66e2d32c22ef2cdb2a41&sim_iccid=898604641919c0808317"
//		sim900a_send_cmd("device_id=1417862573f5bbb40f299df52bc6e70e","OK",12000);
//
// 	sim900a_send_cmd(http_upload_data,"OK",200);
	// DB_PR("...a-12...\n");







chengxu_start_3:

	DB_PR("...a-12...\n");
	USART2_RX_STA =0;
	//imei
	if(sim900a_send_cmd("AT+CGSN","OK",500)==0)//��ѯ��Ʒ���к�
	{ 
		delay_ms(100);
		// p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");//���һس�
		// p1[0]=0;//��������� 
		// sprintf((char*)p,"���к�:%s",USART2_RX_BUF+2);
 		DB_PR("---USART2_RX_BUF+2=%s\n", USART2_RX_BUF+2); 

		if(USART2_RX_STA&0X8000)		//���յ�һ��������
		{ 
			// DB_PR("USART2_RX_BUF=sssssssssssss\n");
			USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
			DB_PR("--4G_UART_RCV=--\r\n");
			// uart0_debug_data_h(data_rx_t,len_rx_t);
			DB_PR("---%s----\n",USART2_RX_BUF);	//���͵�����
			DB_PR("---USART2_RX_STA&0x7FFF = %d----\n",USART2_RX_STA&0x7FFF); //33
        	uart0_debug_data_h(USART2_RX_BUF,USART2_RX_STA&0x7FFF);

			// USART2_RX_STA=0;
			// DB_PR("USART2_RX_BUF=eeeeeeeeeeeee-4G\n\n");

			if((USART2_RX_STA&0x7FFF) !=33)
			{
				DB_PR("--IMEI chongxinhuoqu 1=--\r\n");
				goto chengxu_start_3;
			}



			
			ret = strstr(USART2_RX_BUF, needle);
			DB_PR("���ַ����ǣ� ----%s---\n", ret);
			if(ret == NULL)
			{
				DB_PR("--IMEI chongxinhuoqu 2=--\r\n");
				goto chengxu_start_3;
			}


			memset(IMEI_cRes,0,sizeof(IMEI_cRes));
			// uart0_debug_data_h2(USART2_RX_BUF,strlen(USART2_RX_BUF));
			// str_test(USART2_RX_BUF+2, IMEI_cRes);
			// memcpy(IMEI_cRes,USART2_RX_BUF+10,15);

			memcpy(IMEI_cRes,ret+10,15);
			DB_PR2("---111----%s------\n", IMEI_cRes); 




			MD5Init(&md5);
			MD5Update(&md5, IMEI_cRes, strlen((char *)IMEI_cRes));
			MD5Final(&md5, deviceid_decrypt);

			// //Md5���ܺ��32λ���
			// DB_PR("����ǰ:%s\n���ܺ�16λ:", IMEI_cRes);
			// for (i = 4; i<12; i++)//8*2
			// {
			// 	DB_PR("%02x", deviceid_decrypt[i]);  
			// }

			//Md5���ܺ��32λ���
			DB_PR("\n����ǰ:%s\n���ܺ�32λ:", IMEI_cRes);

			memset(deviceid_decrypt_c,0,sizeof(deviceid_decrypt_c));
			for (i = 0; i<16; i++)
			{
				DB_PR("%02x", deviceid_decrypt[i]); 
				// ltoa((int)(deviceid_decrypt[i]),(char*)(deviceid_decrypt_c+2*i) ,16);
				
				itoa((int)(deviceid_decrypt[i]/16),(char*)(deviceid_decrypt_c+i*2) ,16);
				itoa((int)(deviceid_decrypt[i]%16),(char*)(deviceid_decrypt_c+i*2+1) ,16);
				
			}
			DB_PR("\n-----------deviceid_decrypt_c=%s\n",deviceid_decrypt_c);
			deviceid_decrypt_c[32]=0;
			// uart0_debug_data_h2(deviceid_decrypt_c,32);

		} 
		else
		{
			DB_PR("...a-12-2 err...\n");
		}
		
		// Show_Str(x,y+150,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	else
	{
		DB_PR("...a-12-3 err...\n");
		DB_PR("---USART2_RX_BUF=%s---\n", USART2_RX_BUF); 
		DB_PR("--IMEI chongxinhuoqu=--\r\n");
		goto chengxu_start_3;
	}
	
	// if(sim900a_send_cmd("AT+CNUM","+CNUM",500)==0)			//��ѯ��������
	// { 
	// 	// p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
	// 	// p2=(u8*)strstr((const char*)(p1+2),"\"");
	// 	// p2[0]=0;//���������
	// 	// sprintf((char*)p,"��������:%s",p1+2);
	// 	// DB_PR("-------------p=%s----------------",p);
	// 	// Show_Str(x,y+170,200,16,p,16,0);
	// 	// str_test(USART2_RX_BUF, IMEI_cRes);
	// 	DB_PR("---USART2_RX_BUF----%s---\n", USART2_RX_BUF); 
	// 	uart0_debug_data_h2(USART2_RX_BUF,strlen(USART2_RX_BUF));
	// 	DB_PR("---222---%s\n", IMEI_cRes); 

	// 	USART2_RX_STA=0;		
	// }
	// myfree(p); 













	
	DB_PR("...a-13...\n");

	sprintf(deviceid_decrypt_c2,"device_id=%s",deviceid_decrypt_c);
	DB_PR("--------------------deviceid_decrypt_c2=%s----------------------\n",deviceid_decrypt_c2);

	while(1)
	{
		sim900a_send_cmd("AT+QHTTPURL=49,80","CONNECT",800);
		// sim900a_send_cmd("AT+QHTTPURL=49,80\r\n","CONNECT",800);// != GSM_TRUE) return GSM_FALSE;//"OK"
		DB_PR("...a-9...\n");

		
		// sim900a_send_cmd_tou_data("https://iot.dev.modoubox.com/api/control_app/status","OK",800);
		sim900a_send_cmd_tou_data("https://iot.xintiangui.com/api/control_app/status","OK",800);
		DB_PR("...a-10...\n");


		// //USART2_RX_STA =0;
		// sim900a_send_cmd("AT+QHTTPPOST=?","OK",550);// != GSM_TRUE) return GSM_FALSE;//"OK"
		// DB_PR("...a-11-1...\n");

		// delay_ms(100); //500



		//USART2_RX_STA =0;
		if(0==sim900a_send_cmd("AT+QHTTPPOST=42,80,80","CONNECT",800))// != GSM_TRUE) return GSM_FALSE;//"OK"
		{
			DB_PR("...a-10-1...\n");
		}
		else
		{
			DB_PR("...a-10-2 err...\n");
			continue;
		}
		

		// delay_ms(100); //500
		// if(0==sim900a_send_cmd_tou_data("device_id=00e5c9c6e22e66e2d32c22ef2cdb2a42","OK",1000))//test
		if(0==sim900a_send_cmd_tou_data(deviceid_decrypt_c2,"+QHTTPPOST:",500))
		{
			DB_PR("...a-11-1...\n");
			// if(NULL!=strstr(USART2_RX_BUF,"+QHTTPPOST:"))

		}
		else
		{
			DB_PR("...a-11-2  err...\n");
			continue;
		}
		DB_PR("...a-12...\n");



		//reg_status3 = sim_at_response_https(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
		if(0==sim900a_send_cmd("AT+QHTTPREAD=80","+QHTTPREAD",500))// != GSM_TRUE) return GSM_FALSE;//"OK"
		{
		
			if(USART2_RX_STA&0X8000)		//���յ�һ��������
			{ 
				USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
				DB_PR("%s",USART2_RX_BUF);	//���͵�����

				DB_PR("...bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb...\n");
				reg_status3 = cjson_to_struct_info_register((char*)USART2_RX_BUF);//http2
				if(reg_status3 == 2)
				{
					DB_PR("...a-14   reg ok...\n");
					break;
				}
				//cjson_dbg();

				// if(mode)
				USART2_RX_STA=0;
			} 
		
		

		}

		delay_xs(10); 
	
	}






	// sim900a_send_cmd("AT+QISWTMD=1,2\r\n","OK",2000);

//-------------TCP-------------



	//TCP 2
	//UNINET CMNET
	//GSM_CLEAN_RX();
	//--------
	// sim900a_send_cmd("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n","OK", 100);// != GSM_TRUE) return GSM_FALSE;
	// DB_PR("...1...\n");
	
	//GSM_CLEAN_RX();
	// if(0==sim900a_send_cmd("AT+QIACT=1","OK", 100))// != GSM_TRUE) return GSM_FALSE;
	// {
	// 	DB_PR("...a-4-1-1 aa...\n");
	// }
	// else
	// {
	// 	DB_PR("...a-4-1-2 bb err...\n");
	// }

	DB_PR("...2...\n");
	
	
	
	
	USART2_RX_STA =0;
	//GSM_CLEAN_RX();register:53988b31ffdb2e7db9c9429b84f0f84  register:6ef6d30c49a08134897c6f2cc297063
	
	// express_tcp.xintiangui.com

	// sprintf(at_tcp_ip,"AT+QIOPEN=1,0,\"TCP\",%s,12310,0,2",tcp_ip2);
	sprintf(at_tcp_ip,"AT+QIOPEN=1,0,\"TCP\",%s,8091,0,2",tcp_ip2);
	DB_PR("-----------at_tcp_ip =%s--------------\r\n",at_tcp_ip);	
	
	//  if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"103.46.128.21\",12310,0,2","CONNECT", 1000)==0)//192.168.10.111
	//  if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"39.98.243.128\",8091,0,2",  "CONNECT", 1000)==0)
	// if(sim900a_send_cmd("AT+QIOPEN=1,0,\"TCP\",\"47.94.2.173\",8091,0,2",  "CONNECT", 1000)==0)
	if(sim900a_send_cmd(at_tcp_ip,"CONNECT", 1000)==0)
	{
		DB_PR("----conn-----\r\n");
		send_cmd_to_lcd_pic(0x0003);//---------------kaiji
		daojishi_ongo_flag =0;
		DB_PR("----1-14c---daojishi_ongo_flag=%d\n",daojishi_ongo_flag);
		daojishi_time=30;
		TIM5_Set(0);

		delay_ms(500); //500
			// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
		sim900a_send_cmd_tou_data(regst_key,0,0) ;
		// sim900a_send_cmd("register:43981c0ecf4dfadb2d9cc878c874ab8",0,0) ;
			// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);

	}
	else
	{
		DB_PR("----disconn-----\r\n");
		send_cmd_to_lcd_pic(0x0001);
		if(0==sim900a_send_cmd("AT+QICLOSE=0","OK",200))
		{
			DB_PR("----AT+QICLOSE ok-----\r\n");
		}
		if(0==sim900a_send_cmd("AT+QIDEACT=1","OK",200) )
		{
			DB_PR("----AT+QIDEACT ok-----\r\n");
		}
		goto chengxu_start_2;

	}
	

	//	//����Ƿ�������
	//	while(gsm_cmd_check("CONNECT") != GSM_TRUE)//OK
	//	{		
	//		if(++testConnect >200)//��ȴ�20��   150s----------
	//		{
	//			return GSM_FALSE;
	//		}
	//		GSM_DELAY(100); 		
	//	}


	delay_ms(500); //500
	// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
	sim900a_send_cmd_tou_data("iot",0,0);	// cabinet_heartbeat
	// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);
	

	DB_PR2("\r\n-3-�������ܹ�\r\n");


	// IWDG_Init((4*4),(625*4));    //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ  =64s	 
	IWDG_Init(7,4094);//26s 

// //  	// sim900a_send_cmd("AT+CIPCLOSE=1","CLOSE OK",100);	//�ر�����
// // 	// sim900a_send_cmd("AT+CIPSHUT","SHUT OK",100);		//�ر��ƶ����� 
// // 	// if(sim900a_send_cmd("AT+CGCLASS=\"B\"","OK",1000))return 1;				//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
// // 	// if(sim900a_send_cmd("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",1000))return 2;//����PDP������,��������Э��,��������Ϣ
// // 	// if(sim900a_send_cmd("AT+CGATT=1","OK",500))return 3;					//����GPRSҵ��
// // 	// if(sim900a_send_cmd("AT+CIPCSGP=1,\"CMNET\"","OK",500))return 4;	 	//����ΪGPRS����ģʽ
// // 	// if(sim900a_send_cmd("AT+CIPHEAD=1","OK",500))return 5;	 				//���ý���������ʾIPͷ(�����ж�������Դ)
// // 	// ipbuf[0]=0; 		
	
// 	USART4_RX_STA=0;
// 	delay_ms(100);
	
	while(1)
	{

		timex++;
		timex_t++;
		timex_t2++;
		timex_t3++;
		if(timex==50)
		{
			timex=0;
			LED0=!LED0;
		}

		if(timex_t3==300)//3s
		{
			timex_t3=0;
			IWDG_Feed();
			DB_PR("------------feed dog ongo-----------\n");	
		}

		if(timex_t==6000)//1min   6000
		{
			timex_t =0;
			// sim900a_send_cmd("AT+QISEND=0\r\n","SEND OK", 500);
			sim900a_send_cmd_tou_data("iot",0,0);	
			// sim900a_send_cmd("AT+QISEND=0,0\r\n","OK", 500);
			DB_PR("------------heart-----------\n");	
		}
		if(timex_t2==9500)//1min  30000/60=500=5min
		{
			timex_t2 =0;
			DB_PR("2-xintiao jc-heart_beart_idx=%d\r\n",heart_beart_idx);
			if(0==heart_beart_idx)
			{
				DB_PR("2-xintiao err\r\n");
				send_cmd_to_lcd_pic(0x0001);
				power_down_reset_ec20();
				// at_mode_go();
				// tcp_http_init();
				goto chengxu_start_1;
			}
			else
			{
				DB_PR("2-xintiao ok\r\n");
			}
			heart_beart_idx =0;
		}


		if(qujianma_wait_tcp_flag!=0)
		{
			DB_PR("2-qujianma_wait_tcp_flag wait=%d\r\n",qujianma_wait_tcp_flag);
			timex_t4++;
		}
		
		if((qujianma_wait_tcp_flag!=0)&&(timex_t4==1500))//30s
		{
			DB_PR("2-qujianma_wait_tcp_flag timeout ok\r\n");
			qujianma_wait_tcp_flag=0;
			timex_t4 =0;
			send_cmd_to_lcd_pic(0x0001);
			delay_ms(500); //500
			send_cmd_to_lcd_pic(0x0003);
		}

		delay_ms(10);
		

		sim_at_response(1);//2 ���GSMģ�鷢�͹���������,��ʱ�ϴ�������
		lcd_at_response(1);//4

		lock_at_response(1);
	}
	return 0;
} 
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//ATK-SIM900A GSM/GPRS�����Կ��Ʋ���

//���Խ�����UI
void sim900a_mtest_ui(u16 x,u16 y)
{
	u8 *p,*p1,*p2; 
	p=mymalloc(50);//����50���ֽڵ��ڴ�
	LCD_Clear(WHITE);
	POINT_COLOR=RED;
	Show_Str_Mid(0,y,"ATK-SIM900A ���Գ���",16,240);  
	Show_Str(x,y+25,200,16,"��ѡ��:",16,0); 				    	 
	Show_Str(x,y+45,200,16,"KEY0:���Ų���",16,0); 				    	 
	Show_Str(x,y+65,200,16,"KEY1:���Ų���",16,0);				    	 
	Show_Str(x,y+85,200,16,"WK_UP:GPRS����",16,0);
	POINT_COLOR=BLUE; 	
	USART2_RX_STA=0;
	if(sim900a_send_cmd("AT+CGMI","OK",200)==0)				//��ѯ����������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");
		p1[0]=0;//���������
		sprintf((char*)p,"������:%s",USART2_RX_BUF+2);
		Show_Str(x,y+110,200,16,p,16,0);
		USART2_RX_STA=0;		
	} 
	if(sim900a_send_cmd("AT+CGMM","OK",200)==0)//��ѯģ������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n"); 
		p1[0]=0;//���������
		sprintf((char*)p,"ģ���ͺ�:%s",USART2_RX_BUF+2);
		Show_Str(x,y+130,200,16,p,16,0);
		USART2_RX_STA=0;		
	} 
	if(sim900a_send_cmd("AT+CGSN","OK",200)==0)//��ѯ��Ʒ���к�
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF+2),"\r\n");//���һس�
		p1[0]=0;//��������� 
		sprintf((char*)p,"���к�:%s",USART2_RX_BUF+2);
		Show_Str(x,y+150,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	if(sim900a_send_cmd("AT+CNUM","+CNUM",200)==0)			//��ѯ��������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
		p2=(u8*)strstr((const char*)(p1+2),"\"");
		p2[0]=0;//���������
		sprintf((char*)p,"��������:%s",p1+2);
		Show_Str(x,y+170,200,16,p,16,0);
		USART2_RX_STA=0;		
	}
	myfree(p); 
}
//GSM��Ϣ��ʾ(�ź�����,��ص���,����ʱ��)
//����ֵ:0,����
//    ����,�������
u8 sim900a_gsminfo_show(u16 x,u16 y)
{
	u8 *p,*p1,*p2;
	u8 res=0;
	p=mymalloc(50);//����50���ֽڵ��ڴ�
	POINT_COLOR=BLUE; 	
	USART2_RX_STA=0;
	if(sim900a_send_cmd("AT+CPIN?","OK",200))res|=1<<0;	//��ѯSIM���Ƿ���λ 
	USART2_RX_STA=0;  
	if(sim900a_send_cmd("AT+COPS?","OK",200)==0)		//��ѯ��Ӫ������
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\""); 
		if(p1)//����Ч����
		{
			p2=(u8*)strstr((const char*)(p1+1),"\"");
			p2[0]=0;//���������			
			sprintf((char*)p,"��Ӫ��:%s",p1+1);
			Show_Str(x,y,200,16,p,16,0);
		} 
		USART2_RX_STA=0;		
	}else res|=1<<1;
	if(sim900a_send_cmd("AT+CSQ","+CSQ:",200)==0)		//��ѯ�ź�����
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),":");
		p2=(u8*)strstr((const char*)(p1),",");
		p2[0]=0;//���������
		sprintf((char*)p,"�ź�����:%s",p1+2);
		Show_Str(x,y+20,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<2;
	if(sim900a_send_cmd("AT+CBC","+CBC:",200)==0)		//��ѯ��ص���
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),",");
		p2=(u8*)strstr((const char*)(p1+1),",");
		p2[0]=0;p2[5]=0;//���������
		sprintf((char*)p,"��ص���:%s%%  %smV",p1+1,p2+1);
		Show_Str(x,y+40,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<3; 
	if(sim900a_send_cmd("AT+CCLK?","+CCLK:",200)==0)		//��ѯ��ص���
	{ 
		p1=(u8*)strstr((const char*)(USART2_RX_BUF),"\"");
		p2=(u8*)strstr((const char*)(p1+1),":");
		p2[3]=0;//���������
		sprintf((char*)p,"����ʱ��:%s",p1+1);
		Show_Str(x,y+60,200,16,p,16,0);
		USART2_RX_STA=0;		
	}else res|=1<<4; 
	myfree(p); 
	return res;
} 
//sim900a�����Գ���
void sim900a_test(void)
{
	u8 key=0; 
	u8 timex=0;
	u8 sim_ready=0;
//	POINT_COLOR=RED;
//	Show_Str_Mid(0,30,"ATK-SIM900A ���Գ���",16,240); 
	
	


	
	sim900a_gprs_test();	//GPRS����--------------------











	// while(1)
	// {
	// 	delay_ms(10); 
	// 	sim_at_response(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
	// 	if(sim_ready)//SIM������.
	// 	{
	// 		key=KEY_Scan(0); 
	// 		if(key)
	// 		{
	// 			switch(key)
	// 			{
	// 				case KEY0_PRES:
	// 					sim900a_call_test();	//���Ų���
	// 					break;
	// 				case KEY1_PRES:
	// 					sim900a_sms_test();		//���Ų���
	// 					break;
	// 				case WKUP_PRES:
	// 					sim900a_gprs_test();	//GPRS����--------------------
	// 					break;
	// 			}
	// 			sim900a_mtest_ui(40,30);
	// 			timex=0;
	// 		} 			
	// 	}
	// 	if(timex==0)		//2.5�����Ҹ���һ��
	// 	{
	// 		if(sim900a_gsminfo_show(40,225)==0)sim_ready=1;
	// 		else sim_ready=0;
	// 	}	
	// 	if((timex%20)==0)LED0=!LED0;//200ms��˸ 
	// 	timex++;	 
	// } 	

	// while(1)
	// {
	// 	delay_ms(10); 
	// 	sim_at_response(1);//���GSMģ�鷢�͹���������,��ʱ�ϴ�������
	// 	if(sim_ready)//SIM������.
	// 	{
	// 		key=KEY_Scan(0); 
	// 		if(key)
	// 		{
	// 			switch(key)
	// 			{
	// 				case KEY0_PRES:
	// 					sim900a_call_test();	//���Ų���
	// 					break;
	// 				case KEY1_PRES:
	// 					sim900a_sms_test();		//���Ų���
	// 					break;
	// 				case WKUP_PRES:
	// 					sim900a_gprs_test();	//GPRS����--------------------
	// 					break;
	// 			}
	// 			sim900a_mtest_ui(40,30);
	// 			timex=0;
	// 		} 			
	// 	}
	// 	if(timex==0)		//2.5�����Ҹ���һ��
	// 	{
	// 		if(sim900a_gsminfo_show(40,225)==0)sim_ready=1;
	// 		else sim_ready=0;
	// 	}	
	// 	if((timex%20)==0)LED0=!LED0;//200ms��˸ 
	// 	timex++;	 
	// } 	
}












