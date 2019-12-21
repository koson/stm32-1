#include "wifiap.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//ATK-ESP8266 WIFI模块 WIFI AP驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/4/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//连接端口号:8086,可自行修改为其他端口.
const u8* portnum="8086";		 
//#define PORTNUM 8086

//WIFI AP模式,模块对外的无线参数,可自行修改.
const u8* ap_ssid="aaa";			//对外SSID号
const u8* ap_encryption="wpawpa2_aes";	//wpa/wpa2 aes加密方式
const u8* ap_password="123456789"; 		//连接密码 
//#define AP_SSID cubg-IP-wifi
//#define AP_ENCRYPTION wpawpa2_aes
//#define AP_PASSWORD cubg125126


//4个网络模式
const u8 *ATK_ESP8266_CWMODE_TBL[3]={"STA模式 ","AP模式 ","AP&STA模式 "};	//ATK-ESP8266,3种网络模式,默认为路由器(ROUTER)模式
//5种加密方式
const u8 *ATK_ESP8266_ECN_TBL[5]={"OPEN","WEP","WPA_PSK","WPA2_PSK","WPA_WAP2_PSK"};
//4种工作模式
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={"TCP服务器","TCP客户端"," UDP 模式"};	//ATK-ESP8266,4种工作模式


////********************************UCOIII任务创建********************************//
////创建任务
////任务优先级
//#define AP_TASK_PRIO		2
////任务堆栈大小	
//#define AP_STK_SIZE 		128
////任务控制块
//OS_TCB ApTaskTCB;
////任务堆栈	
//CPU_STK AP_TASK_STK[AP_STK_SIZE];
////任务函数
//void AP_Task(void *p_arg);


//void AP_Task_Create(int *err)
//{
//	OSTaskCreate((OS_TCB 	*   )&ApTaskTCB,			//任务控制块
//							 (CPU_CHAR	* )"ap task", 			//任务名字
//               (OS_TASK_PTR )AP_Task, 				//任务函数
//               (void		*   )0,												//传递给任务函数的参数
//               (OS_PRIO	    )AP_TASK_PRIO, 	//任务优先级
//               (CPU_STK   * )&AP_TASK_STK[0],//任务堆栈基地址
//               (CPU_STK_SIZE)AP_STK_SIZE/10,	//任务堆栈深度限位
//               (CPU_STK_SIZE)AP_STK_SIZE,		//任务堆栈大小
//               (OS_MSG_QTY  )0,												//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
//               (OS_TICK	    )0,												//当使能时间片轮转时的时间片长度，为0时为默认长度，
//               (void   	*   )0,												//用户补充的存储区
//               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
//               (OS_ERR 	  * )*err);										//存放该函数错误时的返回值
//}

//void AP_Task(void *p_arg)
//{
//	
//	//检测WIFI模块连接状态
//	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
//	{
//		atk_8266_quit_trans();//退出透传
//		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
//		printf("未检测到模块！！！！");
//		delay_ms(800);
//		printf("尝试连接模块...");		
//	}
//	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
//	
//	//获取ESP8266参数
//	atk_8266_msg_show();
//	
//	while(1)
//	{	
//	}
//}




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
//ATK-ESP8266模块信息显示
void atk_8266_msg_show(void)
{
//	OS_ERR err;
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	p1=mymalloc(SRAMIN,32);							//申请32字节内存
	p2=mymalloc(SRAMIN,32);							//申请32字节内存
	
	//检测WIFI模块连接状态
	printf("检查连接状态\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		printf("is connect?");
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块！！！！");
		delay_ms(200);
		printf("尝试连接模块...");		
	}
	printf("已连接ESP8266,获取状态中...\r\n");
	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
	
	
//	atk_8266_send_cmd("AT+CWMODE=2","OK",20);
//	atk_8266_send_cmd("AT+RST","OK",20);
//	delay_ms(2000);
//	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);
	
	atk_8266_send_cmd("AT+GMR","OK",20);		//获取固件版本号
	p=atk_8266_check_cmd("SDK version:");
	p1=(u8*)strstr((const char*)(p+1),"Ai");
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
	printf("IP地址：%s	端口号：%s\r\n",(p+1),(u8*)portnum);
	
	//开启server
//	printf("开启AP中...");
//	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
//	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
//	atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
//	printf("AP已开启，请连接。");
	
	myfree(SRAMIN,p);		//释放内存 
	myfree(SRAMIN,p1);		//释放内存 
	myfree(SRAMIN,p2);		//释放内存 
}


//ATK-ESP8266AP模式重置
void atk_8266_rst(void)
{
	u8 *p;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	printf("\r\n检查连接状态\r\n");
	while(atk_8266_send_cmd("AT","OK",20))					//检查WIFI模块是否在线
	{
		printf("is connect?");
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  	//关闭透传模式	
		printf("未检测到模块！！！！");
		delay_ms(200);
		printf("尝试连接模块...");		
	}
	while(atk_8266_send_cmd("ATE0","OK",20));				//关闭回显
	printf("已连接ESP8266,正在配置,请稍等...\r\n");
	
	atk_8266_send_cmd("AT+RST","OK",20);		//复位
	printf("1\r\n");
	delay_ms(2000);	//延时2s等待模块重启
printf("2\r\n");	
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //配置模块AP模式无线参数
			
printf("%s\r\n",p);

	atk_8266_send_cmd(p,"OK",1000);
	atk_8266_send_cmd("AT+CWMODE=2","OK",1000);	
	printf("3\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
	atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
	printf("配置完毕");
	
	myfree(SRAMIN,p);		//释放内存 
}

void atk_8266_init(void)
{
	u8 *p,*p1,*p2;
	p=mymalloc(SRAMIN,32);							//申请32字节内存
	p1=mymalloc(SRAMIN,32);							//申请32字节内存
	p2=mymalloc(SRAMIN,32);							//申请32字节内存
	
	//检测WIFI模块连接状态
	printf("检查连接状态\r\n");
	while(atk_8266_send_cmd("AT","OK",20))//检查WIFI模块是否在线
	{
		printf("is connect?");
		atk_8266_quit_trans();//退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块！！！！");
		delay_ms(200);
		printf("尝试连接模块...");		
	}
	printf("已连接ESP8266,获取状态中...\r\n");
	while(atk_8266_send_cmd("ATE0","OK",20));//关闭回显
	
	atk_8266_send_cmd("AT+RST","OK",20);		//复位
	printf("1\r\n");
	delay_ms(2000);	//延时2s等待模块重启
	printf("2\r\n");	
	sprintf((char*)p,"AT+CWSAP=\"%s\",\"%s\",1,4",ap_ssid,ap_password);    //配置模块AP模式无线参数
	atk_8266_send_cmd(p,"OK",1000);
	delay_ms(500);	
	printf("3\r\n");
	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
	sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);	
	atk_8266_send_cmd(p,"OK",20);     //开启Server模式，端口号为8086
	printf("配置完毕");
}



////ATK-ESP8266 WIFI AP测试
////用于测试TCP/UDP连接
////返回值:0,正常
////    其他,错误代码
//u8 atk_8266_wifiap_test(void)
//{
//	u8 netpro=0;	//网络模式
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP缓存
//	u8 *p;
//	u16 t=999;		//加速第一次获取链接状态
//	u8 res=0;
//	u16 rlen=0;
//	u8 constate=0;	//连接状态
//	p=mymalloc(SRAMIN,32);							//申请32字节内存
//	
//PRESTA:
////	netpro=atk_8266_netpro_sel(50,30,(u8*)ATK_ESP8266_CWMODE_TBL[1]);	//选择网络模式
////	if(netpro&0X02)   //UDP
////	{
////				LCD_Clear(WHITE);
////				POINT_COLOR=RED;
////				Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
////				Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
////				if(atk_8266_ip_set("WIFI-AP 远端UDP IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
////				sprintf((char*)p,"AT+CIPSTART=\"UDP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标UDP服务器
////				atk_8266_send_cmd("AT+CIPMUX=0","OK",100);  //单链接模式
////				LCD_Clear(WHITE);
////				while(atk_8266_send_cmd(p,"OK",500));
////			
////	}
////	else     //TCP
////	{
////		if(netpro&0X01)     //TCP Client    透传模式测试
////		{
////			LCD_Clear(WHITE);
////			POINT_COLOR=RED;
////			Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
////			Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);

////			
////			if(atk_8266_ip_set("WIFI-AP 远端IP设置",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],(u8*)portnum,ipbuf))goto PRESTA;	//IP输入
////			atk_8266_send_cmd("AT+CIPMUX=0","OK",20);   //0：单连接，1：多连接
////			sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",ipbuf,(u8*)portnum);    //配置目标TCP服务器
////			while(atk_8266_send_cmd(p,"OK",200))
////			{
//////				LCD_Clear(WHITE);
//////				POINT_COLOR=RED;
////				Show_Str_Mid(0,40,"WK_UP:返回重选",16,240);
////				Show_Str_Mid(0,80,"ATK-ESP 连接TCP Server失败",12,240); //连接失败		 
////				key=KEY_Scan(0);
////				if(key==WKUP_PRES)goto PRESTA;
////			}	
////			atk_8266_send_cmd("AT+CIPMODE=1","OK",200);      //传输模式为：透传		
////			}
////			else					//TCP Server
////			{
//					//LCD_Clear(WHITE);
//					//POINT_COLOR=RED;
//					//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240); 
//					//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);
//					printf("正在配置ATK-ESP模块，请稍等...");
//					atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接
//					sprintf((char*)p,"AT+CIPSERVER=1,%s",(u8*)portnum);
//					atk_8266_send_cmd(p,"OK",  20);     //开启Server模式，端口号为8086
////			}
////	}
////			LCD_Clear(WHITE);
////			POINT_COLOR=RED;
//			//Show_Str_Mid(0,30,"ATK-ESP WIFI-AP 测试",16,240);
//			//Show_Str(30,50,200,16,"正在配置ATK-ESP模块,请稍等...",12,0);			
//			//LCD_Fill(30,50,239,50+12,WHITE);			//清除之前的显示
//			//Show_Str(30,50,200,16,"配置ATK-ESP模块成功!",12,0);
//			printf("配置ATK-ESP模块成功!");
//			delay_ms(200);
//			//Show_Str(30,50,200,16,"WK_UP:退出测试  KEY0:发送数据",12,0);
//			//LCD_Fill(30,80,239,80+12,WHITE);
//			atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
//			sprintf((char*)p,"IP地址:%s 端口:%s",ipbuf,(u8*)portnum);
////			Show_Str(30,65,200,12,p,12,0);				//显示IP地址和端口	
////			Show_Str(30,80,200,12,"状态:",12,0); 		//连接状态
////			Show_Str(120,80,200,12,"模式:",12,0); 		//连接状态
////			Show_Str(30,100,200,12,"发送数据:",12,0); 	//发送数据
////			Show_Str(30,115,200,12,"接收数据:",12,0);	//接收数据
//			//atk_8266_wificonf_show(30,180,"请用设备连接WIFI热点:",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
//			printf("%s\n%s\n%s\n",(u8*)wifiap_ssid,(u8*)wifiap_encryption,(u8*)wifiap_password);
//			//POINT_COLOR=BLUE;
//			//Show_Str(120+30,80,200,12,(u8*)ATK_ESP8266_WORKMODE_TBL[netpro],12,0); 		//连接状态
//			printf("连接状态：%s",(u8*)ATK_ESP8266_WORKMODE_TBL[netpro]);
//			USART3_RX_STA=0;
//			while(1)
//			{
//				key=KEY_Scan(0);
//				if(key==WKUP_PRES)			//WK_UP 退出测试		 
//				{  
//					res=0;
//					atk_8266_quit_trans();	//退出透传
//					atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
//					break;												 
//				}
//				else if(key==KEY0_PRES)	//KEY0 发送数据 
//				{
//				
//					if((netpro==3)||(netpro==2))   //UDP
//					{
//						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=25","OK",200);  //发送指定长度的数据
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//						timex=100;
//					}
//					else if((netpro==1))   //TCP Client
//					{
//						atk_8266_quit_trans();
//						atk_8266_send_cmd("AT+CIPSEND","OK",20);       //开始透传
//						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						u3_printf("%s",p);
//						timex=100;
//					}
//					else    //TCP Server
//					{
//						sprintf((char*)p,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[netpro],t/10);//测试数据
//						Show_Str(30+54,100,200,12,p,12,0);
//						atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
//						delay_ms(200);
//						atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
//						timex=100;
//					}
//				}
//			
//	if(timex)timex--;
//	if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
//	t++;
//	delay_ms(5);
//	if(USART3_RX_STA&0X8000)		//接收到一次数据了
//	{ 
//		rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
//		USART3_RX_BUF[rlen]=0;		//添加结束符 
//		printf("%s",USART3_RX_BUF);	//发送到串口   
//		sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
//		LCD_Fill(30+54,115,239,130,WHITE);
//		POINT_COLOR=BRED;
//		Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
//		POINT_COLOR=BLUE;
//		LCD_Fill(30,130,239,319,WHITE);
//		Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据  
//		USART3_RX_STA=0;
//		if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//		else t=0;                   //状态为已经连接了,10秒后再检查
//	}  
//	if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
//	{
//		constate=atk_8266_consta_check();//得到连接状态
//		if(constate=='+')Show_Str(30+30,80,200,12,"连接成功",12,0);  //连接状态
//		else Show_Str(30+30,80,200,12,"连接失败",12,0); 	  	 
//		t=0;
//	}
//	if((t%20)==0)LED0=!LED0;
//	atk_8266_at_response(1);
//}
//	myfree(SRAMIN,p);		//释放内存 
//	return res;		
//} 







