#include "ap.h"

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* ap_ssid="aaa";			//对外SSID号
const u8* ap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* ap_password="123456789"; 		//连接密码 


//4个网络模式
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式
//4种工作模式
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式
//5种加密方式
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};

//向ATK-ESP8266发送命令
//cmd:发送的命令字符串
//ack:期待的应答结果,如果为空,则表示不需要等待应答
//waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果)
//       1,发送失败
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART3_RX_STA=0;
	u3_printf("\r\n%s\r\n",cmd);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART3_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					//printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
					USART3_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果
//    其他,期待应答结果的位置(str的位置)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART3_RX_BUF[USART3_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART3_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//ATK-ESP8266退出透传模式
//返回值:0,退出成功;
//       1,退出失败
u8 atk_8266_quit_trans(void)
{
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(15);					//大于串口组帧时间(10ms)
	while((USART3->SR&0X40)==0);	//等待发送空
	USART3->DR='+';      
	delay_ms(500);					//等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}

//初始化ESP8266
void atk_8266_init(void)
{
	int timer = 5;
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	p1=mymalloc(SRAMIN,32);							//申请32字节内存
	p2=mymalloc(SRAMIN,32);							//申请32字节内存
	
	
	//检测WIFI模块连接状态
	printf("检查连接状态\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		printf("is connect?\r\n");
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块！！！！\r\n");
		delay_ms(100);
		printf("尝试连接模块...\r\n");

		//一段时间未搜索到WIFI模块退出
		timer--;
		if(timer<0)
		{
			printf("WIFI模块未连接。\r\n\r\n");	break;
		}
		else	printf("\r\n");
	}
	
	if(timer<0)
	{
		myfree(SRAMIN,p);		//释放内存 
		myfree(SRAMIN,p1);		//释放内存 
		myfree(SRAMIN,p2);		//释放内存
	}
	else
	{
		printf("已连接ESP8266,配置AP模式...\r\n");
		while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
		
		atk_8266_send_cmd("AT+RST","OK",20);		//复位
		delay_ms(1000);	//延时2s等待模块重启	
		sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //配置模块AP模式无线参数
		atk_8266_send_cmd(p,"OK",1000);	
		atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
		sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
		atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
		printf("配置完毕，获取AP信息\r\n");
		
		atk_8266_send_cmd("AT+GMR","OK",20);		//获取固件版本号
		p=atk_8266_check_cmd("SDK version:");
		p1=(u8*)strstr((const char*)(p+1),"Ai");
		p2=p1;
		*p1=0;
		printf("固件版本:%s",p);
		
		atk_8266_send_cmd("AT+CWMODE?","+CWMODE:",20);	//获取网络模式
		p=atk_8266_check_cmd(":");	
		printf("网络模式:%s\r\n",(u8*)ATK_ESP8266_CWMODE_TBL[*(p+1)-'1']);
		
		atk_8266_send_cmd("AT+CWSAP?","+CWSAP",20);	//获取wifi配置
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		printf("SSID号：%s\r\n",(p+1));
		p=(u8*)strstr((const char*)(p2+1),"\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;		
		printf("密码:%s \r\n",(p+1));
		p=(u8*)strstr((const char*)(p2+1),",");
		p1=(u8*)strstr((const char*)(p+1),",");
		*p1=0;
		printf("通道号:%s\r\n",(p+1));
		printf("加密方式:%s\r\n",(u8*)ATK_ESP8266_ECN_TBL[*(p1+1)-'0']);	
		
		atk_8266_send_cmd("AT+CIFSR","OK",20);	//获取IP地址
		p=atk_8266_check_cmd("\"");
		p1=(u8*)strstr((const char*)(p+1),"\"");
		p2=p1;
		*p1=0;
		printf("IP地址：%s	端口号：%s\r\n",(p+1),(u8*)portnum);
		
		USART3_RX_STA=0;
		
		myfree(SRAMIN,p);		//释放内存 
		myfree(SRAMIN,p1);		//释放内存 
		myfree(SRAMIN,p2);		//释放内存 
	}	
}
