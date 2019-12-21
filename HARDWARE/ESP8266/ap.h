#ifndef __AP_H__
#define __AP_H__	 
#include "sys.h"	
#include "delay.h"	
#include "string.h" 	
#include "usart3.h" 
#include "usart.h"
#include "malloc.h"
#include "includes.h"

#define AT_MODE_CONFIG		0				//0,采用ES/RST硬件控制进入AT模式;
											//1,采用串口特殊序列(+++1B1B1B)进入AT模式
											
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* atk_8266_check_cmd(u8 *str);
u8 atk_8266_quit_trans(void);
void atk_8266_init(void);
//用户配置参数
extern const u8* portnum;			//连接端口

extern const u8* ap_ssid;		//WIFI AP SSID
extern const u8* ap_encryption;	//WIFI AP 加密方式
extern const u8* ap_password; 	//WIFI AP 密码

extern const u8* ATK_ESP8266_CWMODE_TBL[3];
extern const u8* ATK_ESP8266_WORKMODE_TBL[3];
extern const u8* ATK_ESP8266_ECN_TBL[5];
#endif
