/****************DJZ-1型 多功能激电侦测系统main程序************/
/****************实现功能:LCD及键盘应用程序,调用   ************/
/****************CS5532采集程序,GPS应用程序       ************/
/****************参数说明:LCD 为320*240点阵16色液晶**********/

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include "data/LCDdata.h"
#include "data/44b.h"
//ttyS1相关
#include <termios.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h> 
//文件名读取相关//
#include<sys/types.h>
#include<dirent.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include"ourhdr.h"
#define ADVREF 5
#define ADBIT 8388608    //即为(2^23)
void *test2(void);
void *test3(void);
void *get_gps(void);
void *get_Cur(void);
void *savedata(void);
pthread_t th_1,th_2,th_3,th_4,th_5,th_6,th_7;
void *v1,*v2,*v3,*v4,*v5,*v6,*v7;
struct character
        {
	 U16 x;
	 U16 y;
	 U16 wedth; 
	 U16 height;   
	 U8 *pchar;     
	};
struct DAQ_PARA
{
	//创建项目中设置以下参数
	long DAQ_LineNum;			//线号
	long DAQ_DotNum;			//点号
	long DAQ_DotIncrease;			//点号增量
	char DAQ_DotDirect;			//点号方:0为减少,非0为增加
	long DAQ_ABDiv2;			//AB/2的距离
	long DAQ_MNDiv2;			//MN/2的距离
	long DAQ_Side;				//旁侧距离
	long DAQ_Org_A;				//原点-A极
	char DAQ_BEF50Hz;			//50Hz陷波设置:0为不陷波,非0为陷波
	long DAQ_Second_delay;			//二次场延时

	//测量方式中设置以下参数
	long DAQ_Surveytype;		//测量类型：1为测深，2为中梯，3为对称四极，4为联剖
	char DAQ_SynMode;			//同步方式：0为内同步，其他为自同步*******
	char DAQ_PolMode;			//极化率计算方式：0为内同步，其他为自同步******
	
	//发送装置界面下设置以下参数
	long DAQ_PowerTime;			//供电时间
	long DAQ_PowerTms;			//供电次数
	long DAQ_Positive_Cur;			//正向电流(测量显示)
	long DAQ_Reverse_Cur;			//反向电流(测量显示)

	//系统设置界面下设置以下参数
	long DAQ_MaxSlfEle;			//最大自电
	long DAQ_MinCurrent;			//最小电流	
	long DAQ_MinOneEle;			//最小一次场
	
	//数据采集界面下显示以下参数
	long DAQ_SP;				//SP，可以设置
	long DAQ_PowerVolt;			//供电电压，可以设置
	U32 DAQ_100K;				//100倍的K值，显示 **
	long DAQ_NatureEle;			//自然电位，显示 **
	long DAQ_OneEle;				//一次电位，显示 **
	long DAQ_Polarizey_1000;			//极化率，显示 **
	long DAQ_PowerCurrent;			//供电电流，显示 **
	long DAQ_Resistance;			//电阻率，显示 **
	char DAQ_ADplus;				//AD增益, 设置
	long DAQ_HlfDecay;			//半衰时，显示 **
	long DAQ_SynPara;			//综合参数，显示 **
	long DAQ_AddSum;			//累加和，显示 **
	long DAQ_DepartDrg;			//偏离度，显示 **
	long DAQ_GNDRes;			//接地电阻，显示 **
	
	//GPS相关设置
	char 	DAQ_gps_year;			//GPS 年 
        char 	DAQ_gps_month;			//GPS 月
	char	DAQ_gps_day;			//GPS 日
	char	DAQ_gps_date;			//GPS 星期
	char	DAQ_gps_hour;			//GPS 小时
	char	DAQ_gps_minus;			//GPS 分钟
	char	DAQ_gps_second;			//GPS 秒
	char 	DAQ_gps_cav;			//GPS 
	float 	DAQ_gps_ns;			//GPS 北纬
	char 	DAQ_gps_cns;
	float 	DAQ_gps_ew;			//GPS 东经
	char 	DAQ_gps_cew;
        char 	DAQ_gps_year_buf;			//GPS 年 暂存
        char 	DAQ_gps_month_buf;			//GPS 月暂存
	char	DAQ_gps_day_buf;			//GPS 日暂存
	char	DAQ_gps_date_buf;			//GPS 星期暂存
	char	DAQ_gps_hour_buf;			//GPS 小时暂存
	char	DAQ_gps_minus_buf;			//GPS 分钟暂存
	char	DAQ_gps_second_buf;			//GPS 秒暂存
	char 	DAQ_gps_cav_buf;			//GPS 暂存
	float 	DAQ_gps_ns_buf;				//GPS 北纬暂存
	char 	DAQ_gps_cns_buf;
	float 	DAQ_gps_ew_buf;				//GPS 东经暂存
	char 	DAQ_gps_cew_buf;
} DateACQPara;
/////////////////////////////////////////////////////////////////////////////////////
U32 DateACQParaDAQ_gps_ew1,DateACQParaDAQ_gps_ns1;//为了显示
int fd,rt;   
U8 key_vv,key_vv1;//键盘值
unsigned short arg_G0=0x00,arg_G1=0xff; //显示颜色深浅相关
U8 dir_name[100][30];	//记录当前文件夹下 ,所有文件名,最多同时有100个文件,每个文件名最多30字符
U8 dir_route[3][30];	//记录路径,便于操作和返回上一级,[0]是线号[1]是点号,[2]是文件名
U8 dir_route_total[100]="/mnt/work";	//用于打开
U8 dir_rm[100];
U8 dir_floor=0;		//记录当前位于第几层,0表示/mnt/work,1表示/mnt/work/线号,
			//2表示/mnt/work/线号/点号
U8 dir_number=3;		//记录指向的文件号,1表示根目录,2表示上一级,3开始指示文件名,初始指向上一级
U8 dir_total_number=2;	//当前文件夹下文件名总数+2;
U8 dir_figure[100]="";	//成图时传递文件位置于文件名
struct character charc;
/****************GPS相关************/
#define  CMSPAR 010000000000
void setTermios(struct termios * pNewtio, U16 uBaudRate)
{
	bzero(pNewtio, sizeof(struct termios)); /* clear struct for new port settings */
	//8N1
	pNewtio->c_cflag = uBaudRate | CS8 |  CREAD | CLOCAL;
	pNewtio->c_iflag = IGNPAR;

	pNewtio->c_cc[VINTR]    = 0;     /* Ctrl-c */ 
	pNewtio->c_cc[VQUIT]    = 0;     /* Ctrl-\ */
	pNewtio->c_cc[VERASE]   = 0;     /* del */
	pNewtio->c_cc[VKILL]    = 0;     /* @ */
	pNewtio->c_cc[VEOF]     = 4;     /* Ctrl-d */
	pNewtio->c_cc[VTIME]    = 5;     /* inter-character timer, timeout VTIME*0.1 */
	pNewtio->c_cc[VMIN]     = 0;     /* blocking read until VMIN character arrives */
	pNewtio->c_cc[VSWTC]    = 0;     /* '\0' */
	pNewtio->c_cc[VSTART]   = 0;     /* Ctrl-q */ 
	pNewtio->c_cc[VSTOP]    = 0;     /* Ctrl-s */
	pNewtio->c_cc[VSUSP]    = 0;     /* Ctrl-z */
	pNewtio->c_cc[VEOL]     = 0;     /* '\0' */
	pNewtio->c_cc[VREPRINT] = 0;     /* Ctrl-r */
	pNewtio->c_cc[VDISCARD] = 0;     /* Ctrl-u */
	pNewtio->c_cc[VWERASE]  = 0;     /* Ctrl-w */
	pNewtio->c_cc[VLNEXT]   = 0;     /* Ctrl-v */
	pNewtio->c_cc[VEOL2]    = 0;     /* '\0' */
}
void delay(U16 i)
{
	while(i--)
	{
	rt=dis_rtc();  //时钟显示;
	}
}
/****************初始化变量************/
void DateACQParaInitialize(void)
{
	DateACQPara.DAQ_LineNum = 1;
	DateACQPara.DAQ_DotNum = 1;
	DateACQPara.DAQ_DotIncrease = 10;	
	DateACQPara.DAQ_DotDirect = 1;
	DateACQPara.DAQ_ABDiv2 = 100;
	DateACQPara.DAQ_MNDiv2 = 100;
	DateACQPara.DAQ_Side = 100;
	DateACQPara.DAQ_Org_A = 100;
	DateACQPara.DAQ_BEF50Hz = 1;
	DateACQPara.DAQ_Second_delay = 11;
	DateACQPara.DAQ_Surveytype = 2;
	DateACQPara.DAQ_SynMode = 1;
	DateACQPara.DAQ_PolMode = 1;
	DateACQPara.DAQ_PowerTime = 1;
	DateACQPara.DAQ_PowerTms = 1;
	DateACQPara.DAQ_MaxSlfEle = 100;
	DateACQPara.DAQ_MinCurrent = 100;
	DateACQPara.DAQ_MinOneEle = 100;
	DateACQPara.DAQ_SP = 100;
	DateACQPara.DAQ_PowerVolt = 500;
	DateACQPara.DAQ_100K = 100;
	DateACQPara.DAQ_NatureEle = 0;
	DateACQPara.DAQ_OneEle = 0;
	DateACQPara.DAQ_Polarizey_1000 = 0;
	DateACQPara.DAQ_PowerCurrent = 1;
	DateACQPara.DAQ_Resistance = 0;
	DateACQPara.DAQ_ADplus=1;
	DateACQPara.DAQ_HlfDecay = 0;
	DateACQPara.DAQ_SynPara = 0;
	DateACQPara.DAQ_AddSum = 0;
	DateACQPara.DAQ_DepartDrg = 0;
	DateACQPara.DAQ_GNDRes = 0;
	DateACQPara.DAQ_gps_cav = 65;
	DateACQPara.DAQ_gps_ns = 3959.3159;
	DateACQPara.DAQ_gps_cns = 78;
	DateACQPara.DAQ_gps_ew = 11620.7382;
	DateACQPara.DAQ_gps_cew = 69;
}
/****************************************/
/****************输出字符预处理函数************/
Character_Generate(U16 x,U16 y,U16 wedth,U16 height,U8 *P)
  {
   charc.x = x;
   charc.y = y;
   charc.wedth = wedth;
   charc.height = height;
   charc.pchar = P;
   switch(height)
	{
		case 0: rt = ioctl(fd, 3,(unsigned long)&charc); break;	// 画点
		case 1: rt = ioctl(fd, 4,(unsigned long)&charc); break;	// 画空心圆
		default: rt = ioctl(fd, 13,(unsigned long)&charc); break;	// 显示字符
	}
   return 0;
  }
/****************************************/
/****************key扫描and RTC显示函数**********/
U8 dis_rtc(void)
{
	static U8 tmp; //记录秒
  	struct RTC
       { 
        	U8 	year;
       		U8 	month;
		U8	day;
		U8	date;
		U8	hour;
		U8	minus;
		U8	second;
       } *p_RTC;
	rt = ioctl(fd, 15,0); // 读实时时钟
	p_RTC = ( struct RTC *)rt;
	if((p_RTC->second == tmp)||(p_RTC->second>0x60)) return 0; //秒 变化才显示
	rt = ioctl(fd, 0,arg_G1);	        Character_Generate(178,5,1,12,char_font8x12+12*1*((p_RTC->month>>4)&0x1));
	Character_Generate(186,5,1,12,char_font8x12+12*1*(p_RTC->month&0xf));
	Character_Generate(194,5,2,11,charyue);
  	Character_Generate(210,5,1,12,char_font8x12+12*1*((p_RTC->day>>4)&0x3));
	Character_Generate(218,5,1,12,char_font8x12+12*1*(p_RTC->day&0xf));
	Character_Generate(226,5,2,11,charri);
	Character_Generate(180,225,1,12,char_font8x12+12*1*((p_RTC->hour&0x30)>>4));
	Character_Generate(188,225,1,12,char_font8x12+12*1*(p_RTC->hour&0xf));
	Character_Generate(196,225,1,12,char_font8x12+12*1*11);
 	Character_Generate(204,225,1,12,char_font8x12+12*1*((p_RTC->minus&0x70)>>4));
	Character_Generate(212,225,1,12,char_font8x12+12*1*(p_RTC->minus&0xf));
	Character_Generate(220,225,1,12,char_font8x12+12*1*11);
	Character_Generate(228,225,1,12,char_font8x12+12*1*((p_RTC->second>>4)&0x7));
	Character_Generate(236,225,1,12,char_font8x12+12*1*(p_RTC->second&0xf));
	tmp = p_RTC->second;
	return 1;
}
void key(void)
{
	key_vv = ioctl(fd, 14,0); // 读键值
	key_vv1=key_vv;
	        while(key_vv1!=0)	
{
		key_vv1 = ioctl(fd, 14,0); 
// 读键值	}
}
/****************************************/
/*************输入函数************************/
/*输入  待输入变量名点阵数组头指针,数据本身*/
/*输出  输入的值 ,十进制U16形或者数据本身 */
char dealwith(U8 deal_data)
{
	U8 return_v=0;
	switch(deal_data)
	{
		case 0x08:return_v=1;break;
		case 0x10:return_v=2;break;
		case 0x02:return_v=3;break;
		case 0x01:return_v=4;break;
		case 0x18:return_v=5;break;
		case 0x12:return_v=6;break;
	        case 0x05:return_v=7;break;
		case 0x09:return_v=8;break;
		case 0x0a:return_v=9;break;
		case 0x06:return_v=0;break;
		default: break;
	}
	return return_v;
}

unsigned short inputchar(U8 *P,U16 data)  //弹出输入对话框,等待输入,反回输入的值
{
	U8 inputdata;
	U8 i=78,rt;
	U16 return_data=0;
	key_vv=0;
	Character_Generate(70,90,15,52,P); //弹出窗口
	while(key_vv!=0x04&&key_vv!=0x14&&key_vv!=0x16)
	{
		while(key_vv==0)
		{
			key_vv = ioctl(fd, 14,0);// 读键值
			rt=dis_rtc();	//时钟显示
		} 
		if(key_vv!=0x04&&key_vv!=0x14&&key_vv!=0x16)

		{
			key_vv1=key_vv;  
			key_vv=dealwith(key_vv1);	//转成十进制对应键值
			Character_Generate(i,119,1,16,char_font8x16+16*1*key_vv);
			i+=8;
			return_data=return_data*10+key_vv;
		}
		key_vv = ioctl(fd, 14,0); // 读键值
		while(key_vv==key_vv1) //等待键盘松开
		{
			key_vv = ioctl(fd, 14,0); // 读键值
			rt=dis_rtc();	//时钟显示
		}		
	}	
	key_vv = ioctl(fd, 14,0); // 读键值
	key_vv1 = key_vv;
	while(key_vv==key_vv1) //等待键盘松开
	{
		          key_vv = ioctl(fd, 14,0); // 读键值
	}
	if(key_vv1==0x14||key_vv1==0x16){return_data=data;}	
	return 	return_data;
	 	
}
/*************************************************/
/******************显示数据函数***********************/
/****输入x,y,data****************************************/
void disdata(U16 dis_x,U16 dis_y,long dis_data)
{
	U8 dis_char[10]="";
	U8 dis_n=0;	
	sprintf(dis_char,"%d",dis_data);
	if(dis_data==0){dis_char[0]=0x30;dis_char[1]=0x0a;}
	while((dis_char[dis_n])>=0x2d)
	{
	if((dis_char[dis_n])==0x2d)	{Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*32);}
	else
{Character_Generate(dis_x,dis_y,1,16,char_font8x16+16*1*(dis_char[dis_n]-0x30));}
	dis_n++;
	dis_x+=8;
	}
}
/*************************************************/
/******************显示数据函数小字***********************/
/****输入x,y,data****************************************/
void disdata1(U16 dis_x,U16 dis_y,long dis_data)
{
	U8 dis_char[10]={0,0,0,0,0,0,0,0,0,0};
	U8 dis_n=0;	
	sprintf(dis_char,"%d",dis_data);
	if(dis_data==0){dis_char[0]=0x30;dis_char[1]=0x0a;}while((dis_char[dis_n])>=0x2d)
	{
	if((dis_char[dis_n])==0x2d)
	{Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*32);}
	else
        {Character_Generate(dis_x,dis_y,1,12,char_font8x12+12*1*(dis_char[dis_n]-0x30));}
	dis_n++;
	dis_x+=8;
	}
}
/*************************************************/
/******************显示字符串函数***********************/
/****输入x,y,dis_data****************************************/
void dischars(U16 dis_x,U16 dis_y,char char_data[30])
{
	U8 dis_n=0;
	while(char_data[dis_n]>=0x20) //文件名为有效字符
     {
	if((char_data[is_n]>=0x30)&&(char_data[dis_n]<=0x39)) //数字
	{

		Character_Generate(dis_x,dis_y,1,16,char_font8x16+16*1*(char_data[dis_n]-0x30));
	}
	if(((char_data[dis_n]>=0x41)&&(char_data[dis_n]<=0x5a))||
	  ((char_data[dis_n]>=0x61)&&(char_data[dis_n]<=0x7a)))//字母,不区分大小写,一律显示小写
	{
		if((char_data[dis_n]>=0x41)&&(char_data[dis_n]<=0x5a)){char_data[dis_n]+=0x20;}//转小写
		Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*(char_data[dis_n]-0x61));
	}
		switch(char_data[dis_n])
	{
		case 0x2e:Character_Generate
(dis_x,dis_y,1,16,char_char8x16+16*1*26);break;
		case 0x2f:Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*27);break;
		case 0x28:Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*29);break;
		case 0x29:Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*30);break;
		case 0x2c:Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*31);break;
		case 0x2d:Character_Generate(dis_x,dis_y,1,16,char_char8x16+16*1*32);break;
		default: break;
	}
	dis_x+=8;
	dis_n++;
     }
}
/*************************************************/
/******************RTC时钟设置***********************/
U8 set_rtc(void)
{
	U8 data[10]={0,0,0,0,0,0,0,0,0,0};
	U8 i;
	U8 x=47,y=96;//x=35+40-16-12,y=38+60
	Character_Generate(35,60,22,113,charset_rtc); //弹出窗口
	for(i=0;i<10;i++)//输入10个数字
	{
	while(key_vv==0)
	{key_vv = ioctl(fd, 14,0);rt=dis_rtc();}// 读键值 时钟显示
	if(key_vv==0x04||key_vv==0x14){return 0;}//按[确认键]和[取消键]中断设置
	data[i]=dealwith(key_vv);	//转成十进制对应键值
	if(i==6){x=13;y=114;}
	x+=12;
	if(i%2==0){x+=16;}	//都是为了在正确的位置显示输入的数字
	Character_Generate(x,y,1,16,char_font8x16+16*1*data[i]);
	key_vv1=key_vv;
	while(key_vv==key_vv1) //等待键盘松开
	{key_vv = ioctl(fd, 14,0);} // 读键值	
	}
	        rRTCCON=0x01;
	rBCDYEAR=data[0]*10+data[1];	
	rBCDMON=(data[2]<<4)+data[3];
	rBCDDAY=(data[4]<<4)+data[5];        rBCDHOUR=(data[6]<<4)+data[7];
	rBCDMIN=(data[8]<<4)+data[9];
	rRTCCON=0x00;
	return 1;
}
/**************************************************/
/**************pagec数据显示***************************/
void dispagecdata(void)
{
	U8 x=10,y=55;
	switch(DateACQPara.DAQ_Surveytype)
	{
		case 1:	Character_Generate(x,y,2,13,charce);
			x+=48;
			Character_Generate(x,y,2,13,charshen);
			break;
		case 2:	Character_Generate(x,y,2,13,charzhong);
			x+=16;
			Character_Generate(x,y,2,13,charti);
			x+=16;
			Character_Generate(x,y,2,13,charzhuang);
			x+=16;
			Character_Generate(x,y,2,13,charzhi);
			break;
		case 3:	Character_Generate(x,y,2,13,chardui);
			x+=16;
			Character_Generate(x,y,2,13,charchen);
			x+=16;
			Character_Generate(x,y,2,13,charsi);
			x+=16;
			Character_Generate(x,y,2,13,charji);
			break;
		case 4:	Character_Generate(x,y,2,13,charlian);
			x+=16;
			Character_Generate(x,y,2,13,charhe);
			x+=16;
			Character_Generate(x,y,2,13,charpou);
			x+=16;
			Character_Generate(x,y,2,13,charmian);
			break;
		default: break;
	}
	disdata1(105,79,DateACQPara.DAQ_LineNum);//数据显示
	disdata1(105,95,DateACQPara.DAQ_DotNum);//数据显示
	disdata1(105,111,DateACQPara.DAQ_DotIncrease);//数据显示
	if(DateACQPara.DAQ_DotDirect==1)
	{
		Character_Generate(105,126,2,13,charzeng);
		Character_Generate(125,126,2,13,charjia);
	}
	else
	{
		Character_Generate(105,126,2,13,charjian);
		Character_Generate(125,126,2,13,charshao);
	}
	disdata1(105,143,DateACQPara.DAQ_ABDiv2);//数据显示
	disdata1(105,159,DateACQPara.DAQ_MNDiv2);//数据显示
	disdata1(105,175,DateACQPara.DAQ_Side);//数据显示
	disdata1(105,191,DateACQPara.DAQ_Org_A);//数据显示
	if(DateACQPara.DAQ_BEF50Hz==1)
	{
		Character_Generate(115,207,2,13,charxian);
		Character_Generate(135,207,2,13,charbo);
	}
	else
	{
		Character_Generate(115,207,2,13,charbu);
		Character_Generate(135,207,2,13,charxian);
		Character_Generate(155,207,2,13,charbo);
	}
}
/**************************************************/
/*****************创建项目测量方式数据显示函数****************/
void dispagec_sdata(void)
{
	U8 x=10,y=55;
	switch(DateACQPara.DAQ_Surveytype)
	{
		case 1:	Character_Generate(x,y,2,13,charce);
			x+=48;
			Character_Generate(x,y,2,13,charshen);
			Character_Generate(11,88,1,12,charpoint);
			break;
		case 2:	Character_Generate(x,y,2,13,charzhong);
			x+=16;
			Character_Generate(x,y,2,13,charti);
			x+=16;
			Character_Generate(x,y,2,13,charzhuang);
			x+=16;
			Character_Generate(x,y,2,13,charzhi);
			Character_Generate(127,88,1,12,charpoint);
			break;
		case 3:	Character_Generate(x,y,2,13,chardui);
			x+=16;
			Character_Generate(x,y,2,13,charchen);
			x+=16;
			Character_Generate(x,y,2,13,charsi);
			x+=16;
			Character_Generate(x,y,2,13,charji);
			Character_Generate(11,106,1,12,charpoint);
			break;
		case 4:	Character_Generate(x,y,2,13,charlian);
			x+=16;
			Character_Generate(x,y,2,13,charhe);
			x+=16;
			Character_Generate(x,y,2,13,charpou);
			x+=16;
			Character_Generate(x,y,2,13,charmian);
			Character_Generate(127,106,1,12,charpoint);
			break;
		default: break;
	}
	Character_Generate(11,124,1,13,charpoint);
	Character_Generate(11,159,1,13,charpoint);
}
/**************************************************/
/*****************创建项目测量方式数据显示函数****************/
void dispageadata(void)
{
	disdata(70,45,DateACQPara.DAQ_ABDiv2);//数据显示
	disdata(130,45,DateACQPara.DAQ_MNDiv2);//数据显示
	disdata(15,45,DateACQPara.DAQ_SP);//数据显示
	disdata(171,165,DateACQPara.DAQ_PowerVolt);//数据显示
	disdata(192,45,DateACQPara.DAQ_100K);//数据显示
	disdata(15,84,DateACQPara.DAQ_NatureEle);//数据显示
//	dfTemp = DateACQPara.DAQ_NatureEle;
//	lTemp = dfTemp*1000*ADVREF/ADBIT;
//	disdata(15,84,lTemp);//数据显示
	disdata(90,84,DateACQPara.DAQ_OneEle);//数据显示
//	dfTemp = DateACQPara.DAQ_OneEle;
//	lTemp = dfTemp*1000*ADVREF/ADBIT;
//	disdata(90,84,lTemp);//数据显示
	disdata(171,84,DateACQPara.DAQ_Polarizey_1000);//数据显示
	disdata(15,124,DateACQPara.DAQ_PowerCurrent);//数据显示
	disdata(90,124,DateACQPara.DAQ_Resistance);//数据显示
	disdata(171,124,DateACQPara.DAQ_ADplus);//数据显示
	disdata(15,165,DateACQPara.DAQ_HlfDecay);//数据显示
	disdata(90,165,DateACQPara.DAQ_SynPara);//数据显示
	disdata(15,205,DateACQPara.DAQ_AddSum);//数据显示
	disdata(90,205,DateACQPara.DAQ_DepartDrg);//数据显示
	disdata(171,205,DateACQPara.DAQ_GNDRes);//数据显示
}
/**************************************************/
/********************dispagegps_data()******************/
int dispagegps_data(void)
{	
	if(DateACQPara.DAQ_gps_year_buf<10)
	{
	Character_Generate(72,56,1,16,char_font8x16);
	disdata(80,56,DateACQPara.DAQ_gps_year_buf);//数据显示
	}
	else
	{
	disdata(72,56,DateACQPara.DAQ_gps_year_buf);//数据显示
	}
	Character_Generate(88,60,2,11,charnian);
	disdata(104,56,DateACQPara.DAQ_gps_month_buf);//数据显示
	Character_Generate(120,60,2,11,charyue);
	disdata(136,56,DateACQPara.DAQ_gps_day_buf);//数据显示
	Character_Generate(152,60,2,11,charri);
	disdata(168,56,DateACQPara.DAQ_gps_hour_buf);//数据显示
	Character_Generate(184,56,1,12,char_font8x12+12*1*11);
	disdata(192,56,DateACQPara.DAQ_gps_minus_buf);//数据显示
	Character_Generate(208,56,1,12,char_font8x12+12*1*11);
	disdata(216,56,DateACQPara.DAQ_gps_second_buf);//数据显示
        DateACQParaDAQ_gps_ew1=DateACQPara.DAQ_gps_ew_buf*10000;
        DateACQParaDAQ_gps_ns1=DateACQPara.DAQ_gps_ns_buf*10000;	        
        disdata(78,78,DateACQParaDAQ_gps_ew1/1000000);//数据显示
	disdata(120,78,DateACQParaDAQ_gps_ew1%1000000/10000);//数据显示
	disdata(141,78,DateACQParaDAQ_gps_ew1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cew_buf=='E')
	{Character_Generate(195,78,2,20,char_e);} //显示e
	else
	{Character_Generate(195,78,2,20,char_w);} //显示w	        
        disdata(78,96,DateACQParaDAQ_gps_ns1/1000000);//数据显示
	disdata(120,96,DateACQParaDAQ_gps_ns1%1000000/10000);//数据显示
	disdata(141,96,DateACQParaDAQ_gps_ns1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cns_buf=='N')	       
          {Character_Generate(195,96,2,20,char_n);} //显示n
	else
	 {Character_Generate(195,96,2,20,char_s);} //显示s	       
        DateACQParaDAQ_gps_ew1=DateACQPara.DAQ_gps_ew*10000;
	DateACQParaDAQ_gps_ns1=DateACQPara.DAQ_gps_ns*10000;
	disdata(52,148,DateACQParaDAQ_gps_ew1/1000000);//数据显示
	disdata(92,148,DateACQParaDAQ_gps_ew1%1000000/10000);//数据显示        
        disdata(111,148,DateACQParaDAQ_gps_ew1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cew_buf=='E')
	{Character_Generate(158,148,2,20,char_e);} //显示e
	else	        {Character_Generate(158,148,2,20,char_w);} //显示w
	disdata(52,179,DateACQParaDAQ_gps_ns1/1000000);//数据显示
	disdata(92,179,DateACQParaDAQ_gps_ns1%1000000/10000);//数据显示
	disdata(111,179,DateACQParaDAQ_gps_ns1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cns_buf=='N')	        
         {Character_Generate(158,179,2,20,char_n);} //显示n
	else
	{Character_Generate(158,179,2,20,char_s);} //显示s
	        return 1;
}
/*************************************************/
/********************RTC与GPS同步******************/
int set_rtc_gps(void)
{
	pthread_create( &th_6,NULL,get_gps,0 );
	sleep(1);//等待GPS读完
	pthread_cancel(th_6); //取消get_gps线程
	rRTCCON=0x01;        rBCDYEAR=DateACQPara.DAQ_gps_year;	
	rBCDMON=((DateACQPara.DAQ_gps_month/10)<<4)+DateACQPara.DAQ_gps_month%10;	       
        rBCDDAY=((DateACQPara.DAQ_gps_day/10)<<4)+DateACQPara.DAQ_gps_day%10;
	rBCDHOUR=((DateACQPara.DAQ_gps_hour/10)<<4)+DateACQPara.DAQ_gps_hour%10;
	rBCDMIN=((DateACQPara.DAQ_gps_minus/10)<<4)+DateACQPara.DAQ_gps_minus%10;
	rBCDSEC=((DateACQPara.DAQ_gps_second/10)<<4)+DateACQPara.DAQ_gps_second%10;
	rRTCCON=0x00;
	return 1;
}
/*************************************************/
/********************保存GPS信息******************/
int save_gps(void)
{
	DateACQPara.DAQ_gps_year=DateACQPara.DAQ_gps_year_buf;		                DateACQPara.DAQ_gps_month=DateACQPara.DAQ_gps_month_buf;
	DateACQPara.DAQ_gps_day=DateACQPara.DAQ_gps_day_buf;	                DateACQPara.DAQ_gps_hour=DateACQPara.DAQ_gps_hour_buf;
	DateACQPara.DAQ_gps_minus=DateACQPara.DAQ_gps_minus_buf;
	DateACQPara.DAQ_gps_second=DateACQPara.DAQ_gps_second_buf;
	DateACQPara.DAQ_gps_ns=DateACQPara.DAQ_gps_ns_buf;
        DateACQPara.DAQ_gps_ew=DateACQPara.DAQ_gps_ew_buf;
	DateACQPara.DAQ_gps_cav=DateACQPara.DAQ_gps_cav_buf;
        DateACQPara.DAQ_gps_cns=DateACQPara.DAQ_gps_cns_buf;
	DateACQPara.DAQ_gps_cew=DateACQPara.DAQ_gps_cew_buf;
        DateACQParaDAQ_gps_ew1=DateACQPara.DAQ_gps_ew*10000;
	DateACQParaDAQ_gps_ns1=DateACQPara.DAQ_gps_ns*10000;
	disdata(52,148,DateACQParaDAQ_gps_ew1/1000000);//数据显示
	disdata(92,148,DateACQParaDAQ_gps_ew1%1000000/10000);//数据显示
	disdata(111,148,DateACQParaDAQ_gps_ew1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cew_buf=='E')
	{Character_Generate(158,148,2,20,char_e);} //显示e
	else
	{Character_Generate(158,148,2,20,char_w);} //显示w
	disdata(52,179,DateACQParaDAQ_gps_ns1/1000000);//数据显示
	disdata(92,179,DateACQParaDAQ_gps_ns1%1000000/10000);//数据显示
	disdata(111,179,DateACQParaDAQ_gps_ns1%10000);//数据显示
	if(DateACQPara.DAQ_gps_cns_buf=='N')
	{Character_Generate(158,179,2,20,char_n);} //显示n
	else
	{Character_Generate(158,179,2,20,char_s);} //显示s
	return 1;
}
/*************************************************/
/********************陷波函数******************/
int set_bef50Hz(char data)
{
	int rt_adg419,fd_adg419;
	if ((fd_adg419 = open("/dev/adg419", 0)) < 0) 
	{
		printf("cannot open /dev/adg419\n");
		exit(0);
	};
	if (data==1){rt_adg419 = ioctl(fd_adg419, 1,0);}
else
        {rt_adg419 = ioctl(fd_adg419, 0,0);}	        close(fd_adg419);
	return 1;
}
/*************************************************/
/********************显示文件夹下所有文件名******************/
int dis_dir_name(char dirname[50],char dir_page)
{
	DIR*dp;
	struct dirent*dirp;
	unsigned int i=0; 
	U16 x=30,y=30;
	if ( (dp = opendir(dirname)) == NULL)
	{
	strcat(dirname,"can not be open");
	dischars(x,y,dirname);	//基本上没用
	return 1;
	}
	while ( (dirp = readdir(dp)) != NULL)
	{
		sprintf(dir_name[i],"%s",dirp->d_name);
		i++;
	}
	dir_total_number=i;//将文件数量传给dir_total_number;
	for (i=0;i<12;i++)
	{
		if(i==0)
		{dischars(x,y,"/mnt/work");}
		else if(i==1)
		{dischars(x,y,dirname);}
		if(i+(dir_page*10)>=dir_total_number) {break;} 
		if(i>=2) {dischars(x,y,dir_name[i+(dir_page*10)]);}
		y+=16;
	}
	close(dp);
	return 1;
}
/*************************************************/
/*****************数据处理部分************************/
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#define MAX_LINE_LEN 30
signed int ReadLine(char *Line_Buf,FILE *fp)
//Return :
//       -1 Line too long.
//        n Line length.example:"12CRLF"---return 2
{
	int i=0;
	char ch1,ch2;
	ch1=0;
	ch2=getc(fp);
	while(!(ch1==0x0d&&ch2==0x0a)&&(!feof(fp))&&(i<MAX_LINE_LEN))
    	{
		Line_Buf[i]=ch2;
		ch1=ch2;
		ch2=getc(fp);
		i++;
	}
	Line_Buf[i-1]='\0';
	if(feof(fp))
		return 0;
	if(i>=MAX_LINE_LEN)
		return -1;
	else
		return i-1;             //i not including CR and LF
}
unsigned int ReadSettings(char *Key,char *Ret1,char *Ret2,FILE *fp,char NoFirstStart)
/*
	 Read the .ini parameter file.
	 [KEY]=...
Input:Key---Group name
     Ret---Return number
Output: 0:Can't find the KEY specified.
	1:OK
*/
{
	#define KEY_HEAD '['
	char LineBuf[MAX_LINE_LEN],LineBufDataTemp[MAX_LINE_LEN],cFind=0;
	if( !NoFirstStart )
		fseek(fp,0L,SEEK_SET); //This may consume much time!!!
	while(!feof(fp)&&!cFind)
	{
	if(ReadLine(LineBuf,fp)==-1)
		return 0;
	strcpy( Ret1,Ret2 );
	strcpy( Ret2,LineBufDataTemp );
	strcpy( LineBufDataTemp,LineBuf );
	if(LineBuf[0]==KEY_HEAD&&strstr(LineBuf,Key))//The key found!
	{
		cFind = 1;
		break;
	}//If end
	}//while end
	if(feof(fp))
		return 0;
	return 1;
}
long data_change(long data_a)
{
	long lTemp = 0;
	double dfTemp = 0;
	dfTemp = data_a;
	lTemp = dfTemp*1000*ADVREF/ADBIT;
	return lTemp;
}
long GetNatureEle( FILE *fp,long PowerTms )//获取自然电位
{
	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];
        long Data1=0,Data2=0;
	long NatureEle = 0;
	int i=0;
	for( i=0;i<PowerTms;i++ )
	{
		if(ReadSettings("[sample freq(N)]=7.5hz",LineBufData1,LineBufData2,fp,i))
		{
			Data1=atol(LineBufData1);
			Data2=atol(LineBufData2);
			NatureEle = NatureEle+Data1+Data2;
		}
	}
	NatureEle = NatureEle/(2*i);
	NatureEle= data_change(NatureEle);
	return NatureEle;
}
long GetOneEle( FILE *fp,long PowerTms )//获取一次电位
{
	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];

	long Data1=0,Data2=0;
	long OneEle = 0;	int i=0;
	for( i=0;i<PowerTms;i++ )
	{
		if( ReadSettings("[sample freq(P-G)]=960hz",LineBufData1,LineBufData2,fp,i) )
		{
			Data1=atol(LineBufData1);
			Data2=atol(LineBufData2);
			OneEle = OneEle+Data1+Data2;
		}
	}
	OneEle = OneEle/(2*i);
	OneEle= data_change(OneEle);
	return OneEle;
}
long Get100K(long Surveytype,long a,long b,long x,long y,long SP_OB)//获取K值
{
	U32 K100=0;
printf("100k:a=%d,b=%d,x=%d,y=%d\n",a,b,x,y);switch(Surveytype)
	{		
		case 2:	//K100=(2*M_PI)/(abs(1/sqrt(y*y+(x+a-b)*(x+a-b))-1/sqrt(y*y+(x+a+b)*(x+a+b))
			//-1/sqrt(y*y+(x-a-b)*(x-a-b))+1/sqrt(y*y+(x-a+b)*(x-a+b))));
		K100=((2*M_PI)/((abs((1/sqrt(y*y+(x-b)*(x-b))-1/sqrt(y*y+(x+b)*(x+b))
		-1/sqrt(y*y+(x-2*a-b)*(x-2*a-b))+1/sqrt(y*y+(x-2*a+b)*(x-2*a+b)))*1000000))*1.0))*1000000;
			//改动因为Org_A=x;
			break;
		case 4:	K100=M_PI*(a*a-b*b)/(b);
			break;
		case 1:		
		case 3:	K100=M_PI*(a*a-b*b)/(2*b);
			break;
	}
	return K100*100;
}
long GetResistance(long OneEle,long PowerCurrent,long K100)//获取电阻率
{
	double lfTemp = K100;
	long lTemp = 0;
	lfTemp = lfTemp*OneEle/PowerCurrent/100;

	lTemp = lfTemp;

	return lTemp;

//	return K100*OneEle/PowerCurrent;

}

long GetPolarizey_1000( FILE *fp,long OneEle,long PowerTms,long Delay )//获取极化率

{

	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];

	long Polarizey_1000 = 0;

	double lfPolarizey_1000 = 0;

	int i=0,j=0;

	if( OneEle==0 )

	{

		return 99999999;

	}

	for( i=0;i<PowerTms;i++ )

	{
		lfPolarizey_1000 = 0;

		if(ReadSettings("[sample freq(P-G)]=960hz",LineBufData1,LineBufData2,fp,i))
		{
			for( j=0;j<Delay;j++ )

			{

				if(ReadLine(LineBufData1,fp)==-1)
					return 0;

			}

			for( j=0;j<10;j++ )

			{

				if(ReadLine(LineBufData1,fp)==-1)
					return 0;

				lfPolarizey_1000 = lfPolarizey_1000 + atol(LineBufData1);

			}

			lfPolarizey_1000 = lfPolarizey_1000/10;

			//Polarizey_1000 = Polarizey_1000 + lfPolarizey;

			Polarizey_1000 = Polarizey_1000 + lfPolarizey_1000;
		}

	}

	Polarizey_1000= data_change(Polarizey_1000);

	Polarizey_1000 = Polarizey_1000*1000/(i*OneEle);

	return Polarizey_1000;

}
int dataprocess(char filename[50])
{
	FILE *fpData;
	if((fpData=fopen(filename,"rb"))==NULL)
	{
		return -1;
	}

	DateACQPara.DAQ_NatureEle=GetNatureEle( fpData,DateACQPara.DAQ_PowerTms);

	DateACQPara.DAQ_OneEle=GetOneEle( fpData,DateACQPara.DAQ_PowerTms );

	DateACQPara.DAQ_100K=Get100K(DateACQPara.DAQ_Surveytype,DateACQPara.DAQ_ABDiv2,DateACQPara.DAQ_MNDiv2,DateACQPara.DAQ_Org_A,DateACQPara.DAQ_Side,DateACQPara.DAQ_SP);

	DateACQPara.DAQ_Resistance=GetResistance(DateACQPara.DAQ_OneEle,DateACQPara.DAQ_PowerCurrent,	DateACQPara.DAQ_100K);

	DateACQPara.DAQ_Polarizey_1000=GetPolarizey_1000( fpData,DateACQPara.DAQ_OneEle,DateACQPara.DAQ_PowerTms,DateACQPara.DAQ_Second_delay );//获取极化率



printf( "NatureEle=%d,OneEle=%d,100K=%d,Resistance=%d,Polarizey_1000=%d\n",DateACQPara.DAQ_NatureEle,DateACQPara.DAQ_OneEle,DateACQPara.DAQ_100K,DateACQPara.DAQ_Resistance,DateACQPara.DAQ_Polarizey_1000);

	fclose(fpData);

	return 1;
}


/*************************************************/

/******************数据成图函数*********************/
int figure(U8 dir_name[100])
{
	FILE *fpData,*fpData1;
	U8 LineBuf[30];
	U8 command[100]="cp ";
	U8 command1[100]="cp ";
	U8 key_v=0;
	long x_data,y_data,x_data_max=0,y_data_max=-999999,x_data_min=999999,y_data_min=999999;
	long x_data1,y_data1,x_data1_max=-999999;
	U16 x,y,x1=0,y1,y2;	//x1,y1保存上次点数据,y2连线y轴
	U16 i;
	U8 charxy[15];
	strcat(command,dir_name);
	strcat(command," /tmp/data.txt");
	system(command);		//复制成图数据文件到/tmp/下

	strcat(command1,dir_name);
	strcat(command1," /tmp/data1.txt");
	system(command1);		//复制成图数据文件到/tmp/下

	if((fpData=fopen("/tmp/data.txt","rb"))==NULL)
	{
		printf("can not open dir;\n");
		return -1;
	}


	while(1)	//找到数据中最大数和 最小数,为归一化做准备
	{
		if(ReadLine(LineBuf,fpData)==0) break;		
		sscanf(LineBuf,"[%d]=%d",&x_data,&y_data);
		if(x_data>x_data_max){x_data_max=x_data;}
		if((y_data>y_data_max)&&(y_data!=99999999)){y_data_max=y_data;}
		if(x_data<x_data_min){x_data_min=x_data;}
		if(y_data<y_data_min){y_data_min=y_data;}
	}
	fclose(fpData);
	if((fpData=fopen("/tmp/data.txt","rb"))==NULL)
	{
		printf("can not open dir;\n");
		return -1;
	}
	Character_Generate(0,0,40,240,charfigure); //显示figure
	dischars(35,205,dir_name);
	while(1)	//画点,归一化的点 画点范围x=(35,270);y=(50,180);
	{
		if(ReadLine(LineBuf,fpData)==0) break;		
		sscanf(LineBuf,"[%d]=%d",&x_data,&y_data);
		printf("x_data=%d,y_data=%d\n",x_data,y_data);
		x=((x_data-x_data_min)*235)/(x_data_max-x_data_min)+35;
		y=180-((y_data-y_data_min)*130)/(y_data_max-y_data_min);
		rt = ioctl(fd, 0,arg_G0); 
		Character_Generate(x,y,2,1,NULL);		
		rt = ioctl(fd, 0,arg_G1); 

		//标示四个最值		
		//if(x_data==x_data_min){sprintf(charxy,"(%d,%d)",x_data,y_data);dischars(35,180,charxy);}
		//if(x_data==x_data_max){sprintf(charxy,"(%d,%d)",x_data,y_data);dischars(235,180,charxy);}
		if(y_data==y_data_min){sprintf(charxy,"min(%d)",y_data);dischars(230,183,charxy);}
		if(y_data==y_data_max){sprintf(charxy,"max(%d)",y_data);dischars(230,30,charxy);}
			
		printf("**x=%d,y=%d\n",x,y);
		//(x1,y1)是横轴比x小的点中横轴最大的点
		if((fpData1=fopen("/tmp/data1.txt","rb"))==NULL)
		{
			printf("can not open dir1;\n");
			return -1;
		}
		while(1)	//(x1,y1)是横轴比x小的点中横轴最大的点
		{
			if(ReadLine(LineBuf,fpData1)==0) break;		
			sscanf(LineBuf,"[%d]=%d",&x_data1,&y_data1);
			if(x_data1<x_data&&x_data1>x_data1_max)
			{
			x1=((x_data1-x_data_min)*235)/(x_data_max-x_data_min)+35;
			y1=180-((y_data1-y_data_min)*130)/(y_data_max-y_data_min);
			}
			
		}
		fclose(fpData1);
		printf("x1=%d,y1=%d\n",x1,y1);

		if(x_data!=x_data_min)
		{	
			for(i=x1;i<x;i++)	//连线,只考虑上一点,将现在的点于上一点相连
			{
				y2=((i-x1)*(y-y1))/(x-x1)+y1;	//取得(i,y2)坐标
				rt = ioctl(fd, 0,arg_G0);
				Character_Generate(i,y2,0,0,NULL);
				rt = ioctl(fd, 0,arg_G1);
			}
			x1=x;
			y1=y;
		}
	}
	while(key_v==0)
	{
		key_v = ioctl(fd, 14,0);// 读键值
		rt=dis_rtc();	//时钟显示
	}
	while(key_v!=0) //等待有键盘按下
	{
		key_v = ioctl(fd, 14,0); // 读键值
		rt=dis_rtc();  //时钟显示
	}
	fclose(fpData);
	system("rm /tmp/data.txt");

	system("rm /tmp/data1.txt");

	rt = ioctl(fd, 0,arg_G1); 
	return 0;	
}
/*****************写成图数据文件函数*********************/

void write_figure_dir(U8 var_name[50])
{
	char a[10]=".txt";
	char b[100]="";   //日志文件名
	char c[10]="";
	char d[20]="/mnt/work/";
	int ini_fd,op_flag;
	char data_ini[100]="";

	strcat(b,d);
	sprintf(c,"%d/",DateACQPara.DAQ_LineNum);//线号
	strcat(b,c);
	strcat(b,var_name);//文件名
	strcat(b,a);//后缀

printf("b=%s\n",b);

	op_flag = O_RDWR | O_CREAT | O_APPEND;
	if ((ini_fd = open(b,op_flag)) < 0) 
	{
		printf("cannot open %s\n",b);
		exit(0);
	};
	if(strcmp(var_name,"polarizey_1000")==0)
	{sprintf(data_ini,"[%d]=%d\r\n",DateACQPara.DAQ_Org_A,DateACQPara.DAQ_Polarizey_1000);}
	if(strcmp(var_name,"oneele")==0)
	{sprintf(data_ini,"[%d]=%d\r\n",DateACQPara.DAQ_Org_A,DateACQPara.DAQ_OneEle);}
	if(strcmp(var_name,"natureele")==0)
	{sprintf(data_ini,"[%d]=%d\r\n",DateACQPara.DAQ_Org_A,DateACQPara.DAQ_NatureEle);}
	if(strcmp(var_name,"resistance")==0)
	{sprintf(data_ini,"[%d]=%d\r\n",DateACQPara.DAQ_Org_A,DateACQPara.DAQ_Resistance);}

	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));

	close(ini_fd);
}

/*************************************************/

/*************主函数*********************/
void *lcd_main()
{
	U8 out=1;
	U8 menu[10]={0,0,0,0,0,0,0,0,0,0}; //菜单路径记录数组
	int i=1;	//i为了控制LCD背光,奇数灭,偶数亮	
	U8 dis_f=1;	//显示菜单中用到的变量1显示,0不显示
	if ((fd = open("/dev/S3C44BLCD", 0)) < 0) 
	{
		printf("cannot open /dev/S3C44BLCD\n");
		exit(0);
	};
	DateACQParaInitialize();//初始化数据变量函数
	rt = ioctl(fd, 8,arg_G0); 
	rt = ioctl(fd, 0,arg_G1); 
	rt = ioctl(fd, 9,arg_G0);//关背光灯
	rt=set_bef50Hz(1);		  //50Hz陷波
//rt = ioctl(fd, 11,0);

	printf("test13\n");
	system("mkdir /mnt/work");
while(out)
{	
	
	/*********************菜单开始***********************/
     if (menu[0]==0&&menu[1]==0&&menu[2]==0&&dis_f==1)
     {	
	Character_Generate(0,0,40,240,charpage1); //显示page1
	dis_f=0;
     }	
     if(menu[0]==0&&(key_vv==0x04||key_vv==0x16||key_vv==0x14||key_vv==0x06||key_vv==0x08
	||key_vv==0x11||key_vv==0x03||key_vv==0x10||key_vv==0x02||key_vv==0x18||key_vv==0x0c
	||key_vv==0x1a))
     {menu[0]=key_vv;dis_f=1;key_vv=0;}
     switch(menu[0]) 
     {
     	case 0x18://1.1  设置液晶颜色变亮
		if (dis_f==1)
     		{
		printf("\n\n Out of code! Please input [pthread] for restart !\n\n\n");
		delay(1000);
		dis_f=0;menu[0]=0;
		}
		out=0;
     		break;
     	case 0x04://1.1  设置液晶颜色变亮
		if (dis_f==1)
     		{dis_f=0;menu[0]=0;
		rt = ioctl(fd, 11,0);} // Light of LCD high
     		break;
	case 0x14://1.2   设置液晶颜色变暗
		if (dis_f==1)
     		{dis_f=0;menu[0]=0;
		rt = ioctl(fd, 12,0);} // Light of LCD low
     		break;
	case 0x06://1.3  设置液晶背光开/关
		if (dis_f==1)
     		{dis_f=0;menu[0]=0;
		if((i%2)==1){rt = ioctl(fd, 10,arg_G0);i++;} // set pin LED_OFF ON
		else {rt = ioctl(fd, 9,arg_G0);i++;} // set pin LCDOFF OFF
		}		 
     		break;
     	case 0x08://1.4 
		if (menu[1]==0&&menu[2]==0&&dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpage1_1); //显示page1_1
		disdata(147,61,DateACQPara.DAQ_PowerTime);//数据显示
		disdata(147,87,DateACQPara.DAQ_PowerTms);//数据显示
		disdata(147,111,DateACQPara.DAQ_Positive_Cur);//数据显示
		disdata(147,134,DateACQPara.DAQ_Reverse_Cur);//数据显示
		dis_f=0;
		}
     		if(key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;}	
     		if(menu[1]==0&&(key_vv==0x08||key_vv==0x10||key_vv==0x03))
     		{menu[1]=key_vv;dis_f=1;key_vv=0;}
     		switch(menu[1])
     		{
		case 0x08: //1.1.1
     			DateACQPara.DAQ_PowerTime=inputchar(charinput111,DateACQPara.DAQ_PowerTime);
			menu[1]=0;key_vv=0; //返回上一级
			break;
		case 0x10: //1.1.2
     			DateACQPara.DAQ_PowerTms=inputchar(charinput112,DateACQPara.DAQ_PowerTms);
			menu[1]=0;key_vv=0; //返回上一级
			break;
		case 0x03: //1.1.2 待加入电流测量
			Character_Generate(70,90,15,52,charinputmeasure_2); //显示"开始测量"
			pthread_create( &th_7,NULL,get_Cur,0 );
			sleep(10);	
			pthread_cancel(th_7);	//关闭测试电流进程				
			menu[1]=0;key_vv=0; //返回上一级
			break;		
		default: break;
		}
		break;
     	case 0x0c://1.p
     	case 0x1a://1.d
		if (menu[1]==0&&menu[2]==0&&dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpaged); //显示paged
		if (menu[0]==0x0c){Character_Generate(248,150,9,43,charpaged_p);} //显示paged_p
		rt = dis_dir_name(dir_route_total,(dir_number-3)/10);
		if(dir_number>=3)	{Character_Generate(4,((dir_number-3)%10)*16+62,3,16,charpp);} //显示"指示"
		else{Character_Generate(4,dir_number*16+14,3,16,charpp); }//显示"指示"
		dis_f=0;
		}
     		if(key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;dir_floor=0;memset(dir_route_total,0,strlen(dir_route_total));//清除
		sprintf(dir_route_total,"/mnt/work");dir_number=2;}	
     		if(menu[1]==0&&(key_vv==0x03||key_vv==0x0c||key_vv==0x11||key_vv==0x1a||key_vv==0x01
				||key_vv==0x10||key_vv==0x12||key_vv==0x18||key_vv==0x09
				||key_vv==0x02||key_vv==0x0a))
     		{menu[1]=key_vv;dis_f=1;key_vv=0;}
     		switch(menu[1])
     		{
		case 0x03: //1.1.1up
		case 0x10: //1.1.1up
			if(dir_number!=1){dir_number--;}			
			if(dir_number>=3)
			{
			Character_Generate(4,((dir_number-3)%10+1)*16+62,3,16,charpp_f); //清除"指示"
			Character_Generate(4,((dir_number-3)%10)*16+62,3,16,charpp); //显示"指示"
			}
			else
			{
			Character_Generate(4,(dir_number+1)*16+14,3,16,charpp_f); //清除"指示"
			Character_Generate(4,62,3,16,charpp_f); //清除"指示"
			Character_Generate(4,dir_number*16+14,3,16,charpp); //显示"指示"
			}
			menu[1]=0;key_vv=0;dis_f=0; //返回上一级,不显示背景
			if(dir_number%10==2&&dir_number!=2)	{dis_f=1;}
			break;
		case 0x02: //1.1.1page up
			if(dir_number>=13){dir_number-=10;}			
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级,显示背景
			break;
		case 0x0a: //1.1.1page down
			if((dir_number<((dir_total_number)/10+1)*10))
			{
			if(dir_number<=2){dir_number+=12;}else {dir_number+=10;}
			if(dir_number>dir_total_number){dir_number=dir_total_number;}
			}			
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级,显示背景			
			break;
		case 0x0c: //1.1.2down
		case 0x09: //1.1.2down
			if(dir_number!=dir_total_number){dir_number++;}
			if(dir_number>=3)
			{
			Character_Generate(4,46,3,16,charpp_f); //清除"指示"
			Character_Generate(4,((dir_number-3)%10-1)*16+62,3,16,charpp_f); //清除"指示"
			Character_Generate(4,((dir_number-3)%10)*16+62,3,16,charpp); //显示"指示"
			}
			else
			{
			Character_Generate(4,(dir_number-1)*16+14,3,16,charpp_f); //清除"指示"
			Character_Generate(4,dir_number*16+14,3,16,charpp); //显示"指示"
			}
			menu[1]=0;key_vv=0;dis_f=0; //返回上一级,不显示背景
			if(dir_number%10==3&&dir_number!=3){dis_f=1;}
			break;
		case 0x11: //1.1.2insert
		case 0x18: //1.1.2insert
		case 0x12: //1.1.2insert
		    	if(dir_number==1)
			{
				dir_floor=0;
				memset(dir_route_total,0,strlen(dir_route_total));//清除
				sprintf(dir_route_total,"/mnt/work");
			}
			else if(dir_number==2)
			{
				if(dir_floor!=0){dir_floor--;}
				memset(dir_route_total,0,strlen(dir_route_total));//清除
				sprintf(dir_route_total,"/mnt/work");
				if(dir_floor==0){;}
				else if(dir_floor==1){strcat(dir_route_total,"/");
						strcat(dir_route_total,dir_route[dir_floor-1]);}
			}
			else 
			{	
				memset(dir_route[dir_floor],0,strlen(dir_route[dir_floor]));//清除
				sprintf(dir_route[dir_floor],"%s",dir_name[dir_number-1]);
	
				if(dir_floor!=2){	strcat(dir_route_total,"/");
						strcat(dir_route_total,dir_name[dir_number-1]);}
	
				if(dir_floor!=2){dir_floor++;}
				dir_number=3;
			}
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级,显示背景
			break;
		case 0x01: //1.1.2back_dir
			if(dir_floor!=0){dir_floor--;}
			memset(dir_route_total,0,strlen(dir_route_total));//清除
			sprintf(dir_route_total,"/mnt/work");
			if(dir_floor==0){;}
			else if(dir_floor==1){strcat(dir_route_total,"/");
					strcat(dir_route_total,dir_route[dir_floor-1]);}
			dir_number=2;
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级,不显示背景
			break;	
		case 0x1a: //1.1.2delet
			if(menu[0]==0x1a)
			{	
			   	if(dir_number==1)
				{
				rt=inputchar(charinputdeletall,1);
				if (rt==0)
				{	
					system("rm -r /mnt/work");system("mkdir /mnt/work");
					memset(dir_route_total,0,strlen(dir_route_total));//清除
					sprintf(dir_route_total,"/mnt/work");dir_floor=0;
					dir_number=2;
				}
				}
				else if(dir_number==2){;}
				else
				{	
					memset(dir_rm,0,strlen(dir_rm));//清除
					sprintf(dir_rm,"rm -rf %s",dir_route_total);
					strcat(dir_rm,"/");
					strcat(dir_rm,dir_name[dir_number-1]);
					system(dir_rm);
					dir_number=2;	
				}				
			}
			else
			{
				sprintf(dir_figure,"%s",dir_route_total);
				strcat(dir_figure,"/");
				strcat(dir_figure,dir_name[dir_number-1]);
				rt=figure(dir_figure);
			}
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级,显示背景
			break;	
		default: break;
		}
		break;

     	case 0x11://1.c 
		if (menu[1]==0&&menu[2]==0&&dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpagec); //显示pagec
		dispagecdata();//数据显示
		dis_f=0;
		}
     		if(menu[1]==0&&key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;}	
     		if(menu[1]==0&&(key_vv==0x08||key_vv==0x10||key_vv==0x02||key_vv==0x05||key_vv==0x01
		||key_vv==0x18||key_vv==0x12||key_vv==0x09||key_vv==0x0a||key_vv==0x0c||key_vv==0x1a
		||key_vv==0x06||key_vv==0x11))
     		{menu[1]=key_vv;dis_f=1;key_vv=0;}
     		switch(menu[1])
     		{
		case 0x08: //1.c.1
     			DateACQPara.DAQ_LineNum=inputchar(charinputc1,DateACQPara.DAQ_LineNum);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x06: //1.c.1
			rt=inputchar(charinputmeasure_1,1);
			if (rt==0)
			{
				Character_Generate(70,90,15,52,charinputmeasure_2); //显示"开始测量"
				pthread_create( &th_2,NULL,test2,0 );					
				pthread_join( th_2,&v2 );//等待线程结束
				pthread_create( &th_3,NULL,test3,0 );				
				pthread_join( th_3,&v3 );//等待线程结束
				rt=dataprocess("/var/tmp/data.txt");
				pthread_create( &th_5,NULL,savedata,0 );				
				pthread_join( th_5,&v5 );//等待线程结束
				Character_Generate(70,90,15,52,charinputmeasure_3); //显示"测量结束"
				sleep(3);						
			}
			menu[1]=0;key_vv=0;dis_f=1;	//返回上一级
			break;
		case 0x1a: //1.c.1
     			DateACQParaInitialize();
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x10: //1.c.2
     			DateACQPara.DAQ_DotNum=inputchar(charinputc2,DateACQPara.DAQ_DotNum);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x02: //1.c.3
     			DateACQPara.DAQ_DotIncrease=inputchar(charinputc3,DateACQPara.DAQ_DotIncrease);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x11: //1.c.sd
     			DateACQPara.DAQ_Second_delay=
			inputchar(charinputcsd,DateACQPara.DAQ_Second_delay);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x01: //1.c.4
     			DateACQPara.DAQ_DotDirect=inputchar(charinputc4,DateACQPara.DAQ_DotDirect);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x18: //1.c.5
     			DateACQPara.DAQ_ABDiv2=inputchar(charinputc5,DateACQPara.DAQ_ABDiv2);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x12: //1.c.6
			DateACQPara.DAQ_MNDiv2=inputchar(charinputc6,DateACQPara.DAQ_MNDiv2);
			menu[1]=0;key_vv=0;dis_f=1;//返回上一级
			break;
		case 0x05: //1.c.7
     			DateACQPara.DAQ_Side=inputchar(charinputc7,DateACQPara.DAQ_Side);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x09: //1.c.8
     			DateACQPara.DAQ_Org_A=inputchar(charinputc8,DateACQPara.DAQ_Org_A);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x0a: //1.c.9
     			DateACQPara.DAQ_BEF50Hz=inputchar(charinputc9,DateACQPara.DAQ_BEF50Hz);
			if (DateACQPara.DAQ_BEF50Hz==1)
			{rt=set_bef50Hz(1);	}	  //50Hz陷波
			else {rt=set_bef50Hz(0);	}//50Hz不陷波
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x0c: //1.c.s创建项目-测量方式
			if (menu[2]==0&&dis_f==1)
     			{
			Character_Generate(0,0,40,240,charpagec_s); //显示page6
			dispagec_sdata();//数据显示
			dis_f=0;
			}
     			if(menu[2]==0&&key_vv==0x16)
     			{menu[1]=0;dis_f=1;key_vv=0;}	
     			if(menu[2]==0&&(key_vv==0x08||key_vv==0x10||key_vv==0x02||key_vv==0x01))
     			{menu[2]=key_vv;dis_f=1;key_vv=0;}
     			switch(menu[2])
     			{
			case 0x08: //1.s.1
     				DateACQPara.DAQ_Surveytype=1;
				menu[2]=0;key_vv=0;dis_f=1; //返回上一级
				break;
			case 0x10: //1.s.2
     				DateACQPara.DAQ_Surveytype=2;
				menu[2]=0;key_vv=0;dis_f=1; //返回上一级
				break;
			case 0x02: //1.s.3
     				DateACQPara.DAQ_Surveytype=3;
				menu[2]=0;key_vv=0;dis_f=1; //返回上一级
				break;
			case 0x01: //1.s.3
     				DateACQPara.DAQ_Surveytype=4;
				menu[2]=0;key_vv=0;dis_f=1; //返回上一级
				break;
			default: break;
			}
			break;		
		default: break;
		}
		break;
	case 0x16://1.s 
		if (dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpages); //显示page6
		disdata(160,64,DateACQPara.DAQ_MaxSlfEle);//数据显示
		disdata(160,84,DateACQPara.DAQ_MinCurrent);//数据显示
		disdata(160,104,DateACQPara.DAQ_MinOneEle);//数据显示
		dis_f=0;
		}
     		if(menu[1]==0&&key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;}	
     		if(menu[1]==0&&(key_vv==0x08||key_vv==0x10||key_vv==0x02||key_vv==0x01))
     		{menu[1]=key_vv;dis_f=1;key_vv=0;}
     		switch(menu[1])
     		{
		case 0x08: //1.s.1
     			DateACQPara.DAQ_MaxSlfEle=inputchar(charinputs1,DateACQPara.DAQ_MaxSlfEle);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x10: //1.s.2
     			DateACQPara.DAQ_MinCurrent=inputchar(charinputs2,DateACQPara.DAQ_MinCurrent);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x02: //1.s.3
     			DateACQPara.DAQ_MinOneEle=inputchar(charinputs3,DateACQPara.DAQ_MinOneEle);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x01: //1.s.3
     			rt=set_rtc();
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		default: break;
		}
		break;
	case 0x03://1.a 
	case 0x10://1.a 
		if (dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpagea); //显示pagea
		DateACQPara.DAQ_GNDRes=
		(DateACQPara.DAQ_PowerVolt*1000)/DateACQPara.DAQ_PowerCurrent;
		dispageadata();//数据显示		
		dis_f=0;
		}
     		if(menu[1]==0&&key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;}	
     		if(menu[1]==0&&(key_vv==0x03||key_vv==0x1a||key_vv==0x06||key_vv==0x11||key_vv==0x0c))
     		{menu[1]=key_vv;dis_f=1;key_vv=0;}
     		switch(menu[1])
     		{
		case 0x03: //1.s.1
		case 0x06: //1.s.1
			rt=inputchar(charinputmeasure_1,1);
			if (rt==0)
			{
				Character_Generate(70,90,15,52,charinputmeasure_2); //显示"开始测量"
				pthread_create( &th_2,NULL,test2,0 );					
				pthread_join( th_2,&v2 );//等待线程结束
				pthread_create( &th_3,NULL,test3,0 );				
				pthread_join( th_3,&v3 );//等待线程结束
				rt=dataprocess("/var/tmp/data.txt");
				pthread_create( &th_5,NULL,savedata,0 );				
				pthread_join( th_5,&v5 );//等待线程结束
				Character_Generate(70,90,15,52,charinputmeasure_3); //显示"测量结束"
				sleep(3);
			}
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x11: //1.s.2
     			DateACQPara.DAQ_SP=inputchar(charinputasp,DateACQPara.DAQ_SP);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x1a: //1.s.3
     			DateACQPara.DAQ_PowerVolt=inputchar(charinputav,DateACQPara.DAQ_PowerVolt);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;
		case 0x0c: //1.s.4
     			DateACQPara.DAQ_ADplus=inputchar(charinputaad,DateACQPara.DAQ_ADplus);
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;			
		default: break;
		}
		break;
	case 0x02://1.gps
		if (dis_f==1)
     		{
		Character_Generate(0,0,40,240,charpagegps); //显示pagegps
		rt = dispagegps_data();	
		dis_f=0;
		}
     		if(menu[1]==0&&key_vv==0x16)
     		{menu[0]=0;dis_f=1;key_vv=0;}	
     		if(menu[1]==0&&(key_vv==0x03||key_vv==0x1a||key_vv==0x0c))
     		{
		menu[1]=key_vv;dis_f=1;key_vv=0;		
		}
     		switch(menu[1])
     		{
		case 0x03: //1.s.1
			if (dis_f==1)
     			{pthread_create( &th_6,NULL,get_gps,0 );
			sleep(1);//等待GPS读完
			pthread_cancel(th_6); //取消get_gps线程
			menu[1]=0;key_vv=0;dis_f=1;} //返回上一级
			break;
		case 0x1a: //1.s.2
			rt=inputchar(charinput_rtc_gps,1);
			if(rt==0)
			{rt=set_rtc_gps();}
			menu[1]=0;key_vv=0;dis_f=1;
			break;
		case 0x0c: //1.s.3
     			rt=save_gps();
			menu[1]=0;key_vv=0;dis_f=1; //返回上一级
			break;		
		default: break;
		}
		break;
     default: break;
     }
     /********************菜单结束****************/
     rt=dis_rtc();  //时钟显示
     key();	//扫描键盘
}	   
close(fd);
}
/*************************************************/


void *test2(void)	//make dir
{
	char a[50]="mkdir /mnt/work/";
	char b[50]="mkdir /mnt/work/";
	char c[10]="";
	char d[10]="";
	char e[20]="mkdir /mnt/work";
	system( e );
	sprintf(c,"%d",DateACQPara.DAQ_LineNum);//线号
	strcat(a,c);
	sprintf(c,"%d/",DateACQPara.DAQ_LineNum);//线号
	strcat(b,c);
	sprintf(d,"%d",DateACQPara.DAQ_DotNum);//点号
	strcat(b,d);	
	system( a );
	system( b );
	return NULL;
}

void *savedata(void)
{
	char a[20]=""; //时间数组
	char b[100]="mv /var/tmp/data.txt /mnt/work/"; //命令数组
	char c[10]="";
	char d[10]="";
	char e[10]=".txt";

	char f[100]="/mnt/work/";   //日志文件名
	char g[10]="";
	char h[10]="";
	char i[10]=".ini";
	char data_ini[100]="";
	int ini_fd,op_flag;
  	struct RTC
       { 
        	U8 	year;
       		U8 	month;
		U8	day;
		U8	date;
		U8	hour;
		U8	minus;
		U8	second;
       } *p_RTC;

	rt = ioctl(fd, 15,0); // 读实时时钟
	p_RTC = ( struct RTC *)rt;
	a[0]=0x30;
	a[1]=(p_RTC->year)+0x30;
	a[2]=((p_RTC->month>>4)&0x1)+0x30;
	a[3]=((p_RTC->month)&0xf)+0x30;
	a[4]=((p_RTC->day>>4)&0x3)+0x30;
	a[5]=(p_RTC->day&0xf)+0x30;
	a[6]=((p_RTC->hour&0x30)>>4)+0x30;
	a[7]=(p_RTC->hour&0xf)+0x30;
	a[8]=((p_RTC->minus&0x70)>>4)+0x30;
	a[9]=(p_RTC->minus&0xf)+0x30;
	a[10]=((p_RTC->second>>4)&0x7)+0x30;
	a[11]=(p_RTC->second&0xf)+0x30;
	sprintf(c,"%d/",DateACQPara.DAQ_LineNum);//线号
	strcat(b,c);
	sprintf(d,"%d/",DateACQPara.DAQ_DotNum);//点号
	strcat(b,d);
	strcat(b,a);//文件名
	strcat(b,e);//后缀
	system( b );
	
	sprintf(g,"%d/",DateACQPara.DAQ_LineNum);//线号
	strcat(f,g);
	sprintf(h,"%d/",DateACQPara.DAQ_DotNum);//点号
	strcat(f,h);
	strcat(f,a);//文件名
	strcat(f,i);//后缀
	op_flag = O_RDWR | O_CREAT;
	if ((ini_fd = open(f,op_flag)) < 0) 
	{
		printf("cannot open %s\n",f);
		exit(0);
	};
	sprintf(data_ini,"[DAQ_LineNum]=%d\r\n",DateACQPara.DAQ_LineNum); //以下是写日志文件
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_DotNum]=%d\r\n",DateACQPara.DAQ_DotNum);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_DotIncrease]=%d\r\n",DateACQPara.DAQ_DotIncrease);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_DotDirect]=%d\r\n",DateACQPara.DAQ_DotDirect);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_ABDiv2]=%d\r\n",DateACQPara.DAQ_ABDiv2);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_MNDiv2]=%d\r\n",DateACQPara.DAQ_MNDiv2);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Side]=%d\r\n",DateACQPara.DAQ_Side);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Org_A]=%d\r\n",DateACQPara.DAQ_Org_A);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_BEF50Hz]=%d\r\n",DateACQPara.DAQ_BEF50Hz);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Second_delay]=%d\r\n",DateACQPara.DAQ_Second_delay);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Surveytype]=%d\r\n",DateACQPara.DAQ_Surveytype);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_SynMode]=%d\r\n",DateACQPara.DAQ_SynMode);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PolMode]=%d\r\n",DateACQPara.DAQ_PolMode);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PowerTime]=%d\r\n",DateACQPara.DAQ_PowerTime);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PowerTms]=%d\r\n",DateACQPara.DAQ_PowerTms);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Positive_Cur]=%d\r\n",DateACQPara.DAQ_Positive_Cur);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Reverse_Cur]=%d\r\n",DateACQPara.DAQ_Reverse_Cur);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_MaxSlfEle]=%d\r\n",DateACQPara.DAQ_MaxSlfEle);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_MinCurrent]=%d\r\n",DateACQPara.DAQ_MinCurrent);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PolMode]=%d\r\n",DateACQPara.DAQ_PolMode);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_MinOneEle]=%d\r\n",DateACQPara.DAQ_MinOneEle);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_SP]=%d\r\n",DateACQPara.DAQ_SP);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PowerVolt]=%d\r\n",DateACQPara.DAQ_PowerVolt);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_100K]=%d\r\n",DateACQPara.DAQ_100K);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_NatureEle]=%d\r\n",DateACQPara.DAQ_NatureEle);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_OneEle]=%d\r\n",DateACQPara.DAQ_OneEle);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Polarizey_1000]=%d\r\n",DateACQPara.DAQ_Polarizey_1000);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_PowerCurrent]=%d\r\n",DateACQPara.DAQ_PowerCurrent);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_Resistance]=%d\r\n",DateACQPara.DAQ_Resistance);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DateACQPara.DAQ_ADplus]=%d\r\n",DateACQPara.DAQ_ADplus);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_HlfDecay]=%d\r\n",DateACQPara.DAQ_HlfDecay);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_SynPara]=%d\r\n",DateACQPara.DAQ_SynPara);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_DepartDrg]=%d\r\n",DateACQPara.DAQ_DepartDrg);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_gps_cav]=%c\r\n",DateACQPara.DAQ_gps_cav);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_gps_ns]=%4.4f\r\n",DateACQPara.DAQ_gps_ns);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_gps_cns]=%c\r\n",DateACQPara.DAQ_gps_cns);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_gps_ew]=%5.4f\r\n",DateACQPara.DAQ_gps_ew);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));
	sprintf(data_ini,"[DAQ_gps_cew]=%c\r\n",DateACQPara.DAQ_gps_cew);
	write(ini_fd,data_ini,strlen(data_ini));memset(data_ini,0,strlen(data_ini));

	close(ini_fd);
/***写成图文件***/
	
	write_figure_dir("polarizey_1000");//注意文件名不能有大写,否则打不开
	write_figure_dir("oneele");
	write_figure_dir("natureele");
	write_figure_dir("resistance");
}

void *test3(void)//数据采集
{	
	char a[100]="";
	char c[10];
	char d[10];
	char e[10];	
	char b[30]="/bin/cs ";
	strcat(a,b);
	sprintf(b," ");
	strcat(a,b);
	sprintf(c,"%d",DateACQPara.DAQ_PowerTime);
	strcat(a,c);
	strcat(a,b);
	sprintf(d,"%d",DateACQPara.DAQ_PowerTms);
	strcat(a,d);
	strcat(a,b);
	sprintf(d,"%d",DateACQPara.DAQ_ADplus);
	strcat(a,d);
	sprintf(b," /var/tmp/data.txt");
	strcat(a,b);
	system( a ); //开始数据采集
	return NULL;
}

#define BUFSIZE 100
void *get_gps(void) //"读取GPS"进程
{
	int f_ttys0 = 0;
	int rev = 0;
	int i, nCount;
	struct termios oldtio,newtio;
	struct timeval tv;
	char *dev ="/dev/ttyS0";
	char *p=NULL;
	int nread;
	char buff[BUFSIZE];
	fd_set rfds;
	
	double ftime;
	int ldate;
	float f1,f2;

	
	printf("waiting for GPS!\n");
	
	if ( (f_ttys0 = open(dev,O_RDWR | O_NOCTTY ))<0)
	{
		printf("Can't Open Serial Port! 0\n");
		return -1;
	}
	tcgetattr(f_ttys0,&oldtio); /* save current serial port settings */
	setTermios(&newtio,B38400);
	//setTermios(&newtio,B115200);
	tcflush(f_ttys0, TCIFLUSH);
	tcsetattr(f_ttys0,TCSANOW,&newtio);
	tv.tv_sec=30;
	tv.tv_usec=0;
	for (i=0; i<1; i++)
  	{
		FD_ZERO(&rfds);
		FD_SET(f_ttys0,&rfds);
		if (select(1+f_ttys0,&rfds,NULL,NULL,&tv)>0)
		{
			if(FD_ISSET(f_ttys0,&rfds))
			{
		delay(1000);	
		nread=read(f_ttys0, buff, BUFSIZE);
		buff[nread]='\0';
		printf("%s\n",buff);
		p=strstr(buff,"$GPRMC");
		if (p==NULL)
		{
			printf("GPS ERROR!\n");
		}
		//$GPRMC,024811.19,A,3959.3159,N,11620.7382,E,0.00,349.8,030906,5.8,
		//W,A*14
		sscanf(p+7,"%f,%c,%f,%c,%f,%c,%f,%f,%d,", &ftime, &DateACQPara.DAQ_gps_cav_buf,&DateACQPara.DAQ_gps_ns_buf, &DateACQPara.DAQ_gps_cns_buf, &DateACQPara.DAQ_gps_ew_buf, &DateACQPara.DAQ_gps_cew_buf, &f1, &f2, &ldate);
		printf("%06.2f,%c,%05.4f,%c,%05.4f,%c,%4.2f,%4.2f,%d,\n", ftime,DateACQPara.DAQ_gps_cav_buf,	DateACQPara.DAQ_gps_ns_buf, DateACQPara.DAQ_gps_cns_buf,DateACQPara.DAQ_gps_ew_buf, DateACQPara.DAQ_gps_cew_buf, f1,f2,ldate);
		DateACQPara.DAQ_gps_hour_buf = ftime/10000;
		DateACQPara.DAQ_gps_minus_buf = (int)ftime%10000/100;
		DateACQPara.DAQ_gps_second_buf = (int)ftime%100;
		DateACQPara.DAQ_gps_date_buf = 1;
		DateACQPara.DAQ_gps_day_buf = ldate/10000;
		DateACQPara.DAQ_gps_month_buf = ldate%10000/100;
		DateACQPara.DAQ_gps_year_buf = ldate%100;	
		rev = 1;	
		}
		}	
  	}	
	tcsetattr(f_ttys0,TCSANOW,&oldtio);
    	close(f_ttys0);
   	return rev;
}

/**************************************/
long SendVoltToI(long a)
{
	double k1=0.345362875;
	double k2=0.10685625;
	long lTemp;
	lTemp=(1000*((3.3*a)/256-k2))/k1;
	return lTemp;
}

void *get_Cur(void) //"读取发送机电流"进程
{
	int f_ttys1 = 0;
	int rev = 0;
	int i, nCount;
	struct termios oldtio,newtio;
	struct timeval tv;
	char *dev ="/dev/ttyS1";
	//char *dev ="/dev/ttyS0";
	char *p=NULL;
	int nread;
	char buff[1];
	fd_set rfds;
	
	float ftime;
	int ldate;
	float f1,f2;

	long zeroele;

	
	printf("waiting for ttys1!\n");
	
	if ( (f_ttys1 = open(dev,O_RDWR | O_NOCTTY ))<0)
	{
		printf("Can't Open Serial Port!\n");
		return -1;
	}
	tcgetattr(f_ttys1,&oldtio); /* save current serial port settings */
	setTermios(&newtio,B57600);
	//setTermios(&newtio,B115200);
	tcflush(f_ttys1, TCIFLUSH);
	tcsetattr(f_ttys1,TCSANOW,&newtio);
	


	nCount=write(f_ttys1, "StopAB!\n", 8);
	sleep(1);

	nCount=write(f_ttys1, "Getcrnt\n", 8);
	nread=read(f_ttys1,buff,1);
	zeroele=buff[0];
	printf("buff[0]=%d\nzeroele=%x\n",buff[0],zeroele);
	zeroele= SendVoltToI(zeroele);


	nCount=write(f_ttys1, "StartAB\n", 8);
	sleep(3);
	nCount=write(f_ttys1, "Getcrnt\n", 8);
	nread=read(f_ttys1,buff,1);
	DateACQPara.DAQ_Positive_Cur=buff[0];
	printf("buff[0]=%d\nDAQ_Positive_Cur=%x\n",buff[0],DateACQPara.DAQ_Positive_Cur);

	nCount=write(f_ttys1, "StopAB!\n", 8);
	sleep(1);

	nCount=write(f_ttys1, "StartBA\n", 8);
	sleep(3);
	nCount=write(f_ttys1, "Getcrnt\n", 8);
	nread=read(f_ttys1,buff,1);
	DateACQPara.DAQ_Reverse_Cur=buff[0];
	printf("buff[0]=%d\nDAQ_Reverse_Cur=%x\n",buff[0],DateACQPara.DAQ_Reverse_Cur);

	nCount=write(f_ttys1, "StopAB!\n",8);
	sleep(1);
	tcsetattr(f_ttys1,TCSANOW,&oldtio);
    	close(f_ttys1);

	DateACQPara.DAQ_Positive_Cur= SendVoltToI(DateACQPara.DAQ_Positive_Cur)-zeroele;
	if( DateACQPara.DAQ_Positive_Cur<0 )
		DateACQPara.DAQ_Positive_Cur = 0;
	DateACQPara.DAQ_Reverse_Cur= SendVoltToI(DateACQPara.DAQ_Reverse_Cur)-zeroele;
	if( DateACQPara.DAQ_Reverse_Cur<0 )
		DateACQPara.DAQ_Reverse_Cur = 0;
	DateACQPara.DAQ_PowerCurrent=(DateACQPara.DAQ_Positive_Cur+DateACQPara.DAQ_Reverse_Cur)/2;
   	return rev;
}

/**************************************/

int main()
{
	pthread_create( &th_1,NULL,lcd_main,0 );
	pthread_join( th_1,&v1 );
	return 1;
}

