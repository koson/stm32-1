#include "ap.h"

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* portnum="8086";		 

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* ap_ssid="aaa";			//����SSID��
const u8* ap_encryption="wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* ap_password="123456789"; 		//�������� 


//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STAģʽ ","APģʽ ","AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP������","TCP�ͻ���"," UDP ģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};

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

//��ʼ��ESP8266
void atk_8266_init(void)
{
	int timer = 5;
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p1=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	p2=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	
	
	//���WIFIģ������״̬
	printf("�������״̬\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//���WIFIģ���Ƿ�����
	{
		printf("is connect?\r\n");
		atk_8266_quit_trans();//�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽ģ�飡������\r\n");
		delay_ms(100);
		printf("��������ģ��...\r\n");

		//һ��ʱ��δ������WIFIģ���˳�
		timer--;
		if(timer<0)
		{
			printf("WIFIģ��δ���ӡ�\r\n\r\n");	break;
		}
		else	printf("\r\n");
	}
	
	if(timer<0)
	{
		myfree(SRAMIN,p);		//�ͷ��ڴ� 
		myfree(SRAMIN,p1);		//�ͷ��ڴ� 
		myfree(SRAMIN,p2);		//�ͷ��ڴ�
	}
	else
	{
		printf("������ESP8266,����APģʽ...\r\n");
		while(atk_8266_send_cmd("ATE0","OK",20));//�رջ���
		
		atk_8266_send_cmd("AT+RST","OK",20);		//��λ
		delay_ms(1000);	//��ʱ2s�ȴ�ģ������	
		sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //����ģ��APģʽ���߲���
		atk_8266_send_cmd(p,"OK",1000);	
		atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������
		sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
		atk_8266_send_cmd(p,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086
		printf("������ϣ���ȡAP��Ϣ\r\n");
		
		atk_8266_send_cmd("AT+GMR","OK",20);		//��ȡ�̼��汾��
		p=atk_8266_check_cmd("SDK version:");
		p1=(u8*)strstr((const char*)(p+1),"Ai");
		p2=p1;
		*p1=0;
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
		p2=p1;
		*p1=0;
		printf("IP��ַ��%s	�˿ںţ�%s\r\n",(p+1),(u8*)portnum);
		
		USART3_RX_STA=0;
		
		myfree(SRAMIN,p);		//�ͷ��ڴ� 
		myfree(SRAMIN,p1);		//�ͷ��ڴ� 
		myfree(SRAMIN,p2);		//�ͷ��ڴ� 
	}	
}
