#include "wifiap.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//ATK-ESP8266 WIFIģ�� WIFI AP��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/4/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 
//#define PORTNUM 8086

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* ap_ssid="aaa";			//����SSID��
const u8* ap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* ap_password="123456789"; 		//�������� 
//#define AP_SSID cubg-IP-wifi
//#define AP_ENCRYPTION wpawpa2_aes
//#define AP_PASSWORD cubg125126


//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ


////********************************UCOIII���񴴽�********************************//
////��������
////�������ȼ�
//#define AP_TASK_PRIO		2
////�����ջ��С	
//#define AP_STK_SIZE 		128
////������ƿ�
//OS_TCB ApTaskTCB;
////�����ջ	
//CPU_STK AP_TASK_STK[AP_STK_SIZE];
////������
//void AP_Task(void *p_arg);


//void AP_Task_Create(int *err)
//{
//	OSTaskCreate((OS_TCB 	*   )&ApTaskTCB,			//������ƿ�
//							 (CPU_CHAR	* )"ap task", 			//��������
//               (OS_TASK_PTR )AP_Task, 				//������
//               (void		*   )0,												//���ݸ��������Ĳ���
//               (OS_PRIO	    )AP_TASK_PRIO, 	//�������ȼ�
//               (CPU_STK   * )&AP_TASK_STK[0],//�����ջ����ַ
//               (CPU_STK_SIZE)AP_STK_SIZE/10,	//�����ջ�����λ
//               (CPU_STK_SIZE)AP_STK_SIZE,		//�����ջ��С
//               (OS_MSG_QTY  )0,												//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
//               (OS_TICK	    )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
//               (void   	*   )0,												//�û�����Ĵ洢��
//               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
//               (OS_ERR 	  * )*err);										//��Ÿú�������ʱ�ķ���ֵ
//}

//void AP_Task(void *p_arg)
//{
//	
//	//���WIFIģ������״̬
//	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
//	{
//		atk_8266_quit_trans();//�˳�͸��
//		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
//		printf("δ��⵽ģ�飡������");
//		delay_ms(800);
//		printf("��������ģ��...");		
//	}
//	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
//	
//	//��ȡESP8266����
//	atk_8266_msg_show();
//	
//	while(1)
//	{	
//	}
//}




//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("\r\n%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART3->SR&0X40)==0);	//�ȴ����Ϳ�
	USART3->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}
//ATK-ESP8266ģ����Ϣ��ʾ
void atk_8266_msg_show(void)
{
//	OS_ERR err;
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	
	//���WIFIģ������״̬
	printf("�������״̬\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		printf("is connect?");
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽ģ�飡������");
		delay_ms(200);
		printf("��������ģ��...");		
	}
	printf("������ESP8266,��ȡ״̬��...\r\n");
	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
	
	
//	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
//	atk_8266_send_cmd("AT+RST","OK",20);
//	delay_ms(2000);
//	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	
	atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
	p=atk_8266_check_cmd("SDK version:");
	p1=(u8*)strstr((const char*)(p+1),"Ai");
	printf("�̼��汾:%s",p);
	
	atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//��ȡ����ģʽ
	p=atk_8266_check_cmd(":");	
	printf("����ģʽ:%s\r\n",(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1']);
	
	atk_8266_send_cmd("AT+CWSAP?","+CWSAP",20);	//��ȡwifi����
	p=atk_8266_check_cmd("\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;
	printf("SSID�ţ�%s\r\n",(p+1));
	p=(u8*)strstr((const char*)(p2+1),"\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	p2=p1;
	*p1=0;		
	printf("����:%s \r\n",(p+1));
	p=(u8*)strstr((const char*)(p2+1),",");
	p1=(u8*)strstr((const char*)(p+1),",");
	*p1=0;
	printf("ͨ����:%s\r\n",(p+1));
	printf("���ܷ�ʽ:%s\r\n",(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0']);	
	
	atk_8266_send_cmd("AT+CIFSR","OK",20);	//��ȡIP��ַ
	p=atk_8266_check_cmd("\"");
	p1=(u8*)strstr((const char*)(p+1),"\"");
	printf("IP��ַ��%s	�˿ںţ�%s\r\n",(p+1),(u8*)portnum);
	
	//����server
//	printf("����AP��...");
//	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
//	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
//	atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
//	printf("AP�ѿ����������ӡ�");
	
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
	myfree(SRAMIN,p1);		//�ͷ��ڴ� 
	myfree(SRAMIN,p2);		//�ͷ��ڴ� 
}


//ATK-ESP8266APģʽ����
void atk_8266_rst(void)
{
	u8 *p;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	printf("\r\n�������״̬\r\n");
	while(atk_8266_send_cmd("AT","OK",20))					//���WIFIģ���Ƿ�����
	{
		printf("is connect?");
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  	//�ر�͸��ģʽ	
		printf("δ��⵽ģ�飡������");
		delay_ms(200);
		printf("��������ģ��...");		
	}
	while(atk_8266_send_cmd("ATE0","OK",20));				//�رջ���
	printf("������ESP8266,��������,���Ե�...\r\n");
	
	atk_8266_send_cmd("AT+RST","OK",20);		//��λ
	printf("1\r\n");
	delay_ms(2000);	//��ʱ2s�ȴ�ģ������
printf("2\r\n");	
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //����ģ��APģʽ���߲���
			
printf("%s\r\n",p);

	atk_8266_send_cmd(p,"OK",1000);
	atk_8266_send_cmd("AT+CWMODE=2","OK",1000);	
	printf("3\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
	atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
	printf("�������");
	
	myfree(SRAMIN,p);		//�ͷ��ڴ� 
}

void atk_8266_init(void)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	
	//���WIFIģ������״̬
	printf("�������״̬\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		printf("is connect?");
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽ģ�飡������");
		delay_ms(200);
		printf("��������ģ��...");		
	}
	printf("������ESP8266,��ȡ״̬��...\r\n");
	while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
	
	atk_8266_send_cmd("AT+RST","OK",20);		//��λ
	printf("1\r\n");
	delay_ms(2000);	//��ʱ2s�ȴ�ģ������
	printf("2\r\n");	
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //����ģ��APģʽ���߲���
	atk_8266_send_cmd(p,"OK",1000);
	delay_ms(500);	
	printf("3\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
	atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
	printf("�������");
}



////ATK-ESP8266 WIFI AP����
////���ڲ���TCP/UDP����
////����ֵ:0,����
////    ����,�������
//u8 atk_8266_wifiap_test(void)
//{
//	u8 netpro=0;	//����ģʽ
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP����
//	u8 *p;
//	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//����״̬
//	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
//	
//PRESTA:
////	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//ѡ������ģʽ
////	if(netpro&0X02)   //UDP
////	{
////				LCD_Clear(WHITE);
////				POINT_COLOR=RED;
////				Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
////				Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
////				if(atk_8266_ip_set("WIFI-AP Զ��UDP IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
////				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��UDP������
////				atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //������ģʽ
////				LCD_Clear(WHITE);
////				while(atk_8266_send_cmd(p,"OK",500));
////			
////	}
////	else     //TCP
////	{
////		if(netpro&0X01)     //TCP Client    ͸��ģʽ����
////		{
////			LCD_Clear(WHITE);
////			POINT_COLOR=RED;
////			Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
////			Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);

////			
////			if(atk_8266_ip_set("WIFI-AP Զ��IP����",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP����
////			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0�������ӣ�1��������
////			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //����Ŀ��TCP������
////			while(atk_8266_send_cmd(p,"OK",200))
////			{
//////				LCD_Clear(WHITE);
//////				POINT_COLOR=RED;
////				Show_Str_Mid(0,40,"WK_UP:������ѡ",16,240);
////				Show_Str_Mid(0,80,"ATK-ESP ����TCP Serverʧ��",12,240); //����ʧ��		 
////				key=KEY_Scan(0);
////				if(key==WKUP_PRES)goto PRESTA;
////			}	
////			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //����ģʽΪ��͸��		
////			}
////			else					//TCP Server
////			{
//					//LCD_Clear(WHITE);
//					//POINT_COLOR=RED;
//					//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240); 
//					//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);
//					printf("��������ATK-ESPģ�飬���Ե�...");
//					atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
//					sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
//					atk_8266_send_cmd(p,"OK",  20);     //����Serverģʽ���˿ں�Ϊ8086
////			}
////	}
////			LCD_Clear(WHITE);
////			POINT_COLOR=RED;
//			//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP ����",16,240);
//			//Show_Str(30,50,200,16,"��������ATK-ESPģ��,���Ե�...",12,0);			
//			//LCD_Fill(30,50,239,50+12,WHITE);			//���֮ǰ����ʾ
//			//Show_Str(30,50,200,16,"����ATK-ESPģ��ɹ�!",12,0);
//			printf("����ATK-ESPģ��ɹ�!");
//			delay_ms(200);
//			//Show_Str(30,50,200,16,"WK_UP:�˳�����  KEY0:��������",12,0);
//			//LCD_Fill(30,80,239,80+12,WHITE);
//			atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
//			sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
////			Show_Str(30,65,200,12,p,12,0);				//��ʾIP��ַ�Ͷ˿�	
////			Show_Str(30,80,200,12,"״̬:",12,0); 		//����״̬
////			Show_Str(120,80,200,12,"ģʽ:",12,0); 		//����״̬
////			Show_Str(30,100,200,12,"��������:",12,0); 	//��������
////			Show_Str(30,115,200,12,"��������:",12,0);	//��������
//			//atk_8266_wificonf_show(30,180,"�����豸����WIFI�ȵ�:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
//			printf("%s\n%s\n%s\n",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
//			//POINT_COLOR=BLUE;
//			//Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//����״̬
//			printf("����״̬��%s",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]);
//			USART3_RX_STA=0;
//			while(1)
//			{
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)			//WK_UP �˳�����		 
//				{  
//					res=0;
//					atk_8266_quit_trans();	//�˳�͸��
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
//					break;												 
//				}
//				else if(key==KEY0_PRES)	//KEY0 �������� 
//				{
//				
//					if((netpro==3)||(netpro==2))   //UDP
//					{
//						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //����ָ�����ȵ�����
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//						timex=100;
//					}
//					else if((netpro==1))   //TCP Client
//					{
//						atk_8266_quit_trans();
//						atk_8266_send_cmd("AT+CIPSEND","OK",20);       //��ʼ͸��
//						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						u3_printf("%s",p);
//						timex=100;
//					}
//					else    //TCP Server
//					{
//						sprintf((char*)p,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//��������
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
//						timex=100;
//					}
//				}
//			
//	if(timex)timex--;
//	if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//	t++;
//	delay_ms(5);
//	if(USART3_RX_STA&0X8000)		//���յ�һ��������
//	{ 
//		rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
//		USART3_RX_BUF[rlen]=0;		//��ӽ����� 
//		printf("%s",USART3_RX_BUF);	//���͵�����   
//		sprintf((char*)p,"�յ�%d�ֽ�,��������",rlen);//���յ����ֽ��� 
//		LCD_Fill(30+54,115,239,130,WHITE);
//		POINT_COLOR=BRED;
//		Show_Str(30+54,115,156,12,p,12,0); 			//��ʾ���յ������ݳ���
//		POINT_COLOR=BLUE;
//		LCD_Fill(30,130,239,319,WHITE);
//		Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//��ʾ���յ�������  
//		USART3_RX_STA=0;
//		if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
//		else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
//	}  
//	if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
//	{
//		constate=atk_8266_consta_check();//�õ�����״̬
//		if(constate=='+')Show_Str(30+30,80,200,12,"���ӳɹ�",12,0);  //����״̬
//		else Show_Str(30+30,80,200,12,"����ʧ��",12,0); 	  	 
//		t=0;
//	}
//	if((t%20)==0)LED0=!LED0;
//	atk_8266_at_response(1);
//}
//	myfree(SRAMIN,p);		//�ͷ��ڴ� 
//	return res;		
//} 







