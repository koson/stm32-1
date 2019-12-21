#ifndef __AP_H__
#define __AP_H__	 
#include "sys.h"	
#include "delay.h"	
#include "string.h" 	
#include "usart3.h" 
#include "usart.h"
#include "malloc.h"
#include "includes.h"

#define AT_MODE_CONFIG		0				//0,����ES/RSTӲ�����ƽ���ATģʽ;
											//1,���ô�����������(+++1B1B1B)����ATģʽ
											
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_quit_trans(void);
void atk_8266_init(void);
//�û����ò���
extern const u8* portnum;			//���Ӷ˿�

extern const u8* ap_ssid;		//WIFI AP SSID
extern const u8* ap_encryption;	//WIFI AP ���ܷ�ʽ
extern const u8* ap_password; 	//WIFI AP ����

extern const u8* ATK_ESP8266_CWMODE_TBL[3];
extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif
