#include "data.h"

DAQ_PARA DateACQPara;

int Data_Trasfer(u8 len,u8 *p);
/*
************************************************************************
*  函数名称：Date_ACQ_Para_Init( ) 
*  功能描述：初始化变量
*  输入参数：none
*  返回参数：none
************************************************************************
*/
void Date_ACQ_Para_Init(void)
{
	DateACQPara.DAQ_LineNum          = 1;
	DateACQPara.DAQ_DotNum           = 1;
	DateACQPara.DAQ_DotIncrease      = 1;	        
	DateACQPara.DAQ_DotDirect        = 1;		// 非0 为增加，0 为减小
	DateACQPara.DAQ_ABDiv2           = 300;         // 初始化极距 2 m
	DateACQPara.DAQ_MNDiv2           = 100;
	DateACQPara.DAQ_Side             = 0;
	DateACQPara.DAQ_Org_A            = 300;
	DateACQPara.DAQ_BEF50Hz          = 1;
	DateACQPara.DAQ_Second_delay     = 200;
	DateACQPara.DAQ_Surveytype       = 3;
	DateACQPara.DAQ_SynMode          = 0;          // 测多极化率 (值不是0和1就是)
  DateACQPara.DAQ_Areatype         = 1;             // 默认 单极化率
	DateACQPara.DAQ_PolMode          = 0;
	DateACQPara.DAQ_SPtype           = 1;   //2;	//user
	DateACQPara.DAQ_Average          = 0;
	DateACQPara.DAQ_Effect           = 0;

	DateACQPara.DAQ_PowerTime        = 2;       // 供电脉宽，单位秒
	DateACQPara.DAQ_PowerTms         = 1;
	DateACQPara.DAQ_MaxSlfEle        = 1000;
	DateACQPara.DAQ_MinCurrent       = 10;
	DateACQPara.DAQ_MinOneEle        = 100;
	DateACQPara.DAQ_Minpoweroff      = 120;
	DateACQPara.DAQ_Dcdrift          = 0;
	DateACQPara.DAQ_SP               = 1;
	DateACQPara.DAQ_PowerVolt        = 500;
	DateACQPara.DAQ_100K             = 100;
	DateACQPara.DAQ_NatureEle        = 0;
	DateACQPara.DAQ_OneEle           = 0;
	DateACQPara.DAQ_Polarizey_1000   = 0;
	DateACQPara.DAQ_PowerCurrent     = 1;
	DateACQPara.DAQ_Resistance       = 0;
	DateACQPara.DAQ_ADplus           = 1;//2;
	DateACQPara.DAQ_HlfDecay         = 0;
	DateACQPara.DAQ_SynPara          = 0;
	DateACQPara.DAQ_DecayDrg         = 0;
	DateACQPara.DAQ_DepartDrg        = 0;
	DateACQPara.DAQ_GNDRes           = 0;

	DateACQPara.DAQ_Ts1              = 40;
	DateACQPara.DAQ_Ts2              = 80;
	DateACQPara.DAQ_Ts3              = 80;
	DateACQPara.DAQ_Ts4              = 80;
	DateACQPara.DAQ_Polarizey_1000_1 = 0;
	DateACQPara.DAQ_Polarizey_1000_2 = 0;
	DateACQPara.DAQ_Polarizey_1000_3 = 0;
	DateACQPara.DAQ_Polarizey_1000_4 = 0;
	DateACQPara.DAQ_Polarizey_sys    = 1;
}

/*
************************************************************************
*  函数名称：Dispageadata(void)( ) 
*  功能描述：创建项目测量方式数据显示函数
*  输入参数：none
*  返回参数：none
************************************************************************
*/
void Data_Return(u8 type)
{
	switch(type){
		case PROJECT:
			//创建项目参数	
			u3_printf("DAQ_LineNum:%d;\r\n",DateACQPara.DAQ_LineNum);
			u3_printf("DAQ_DotNum:%d;\r\n",DateACQPara.DAQ_DotNum);
			u3_printf("DAQ_DotIncrease:%d;\r\n",DateACQPara.DAQ_DotIncrease);
			u3_printf("DAQ_DotDirect:%d;\r\n",DateACQPara.DAQ_DotDirect);
			u3_printf("DAQ_ABDiv2:%d;\r\n",DateACQPara.DAQ_ABDiv2);
			u3_printf("DAQ_MNDiv2:%d;\r\n",DateACQPara.DAQ_MNDiv2);
			u3_printf("DAQ_Side:%d;\r\n",DateACQPara.DAQ_Side);
			u3_printf("DAQ_Org_A:%d;\r\n",DateACQPara.DAQ_Org_A);
			u3_printf("DAQ_BEF50Hz:%d;\r\n",DateACQPara.DAQ_BEF50Hz);
			u3_printf("DAQ_Second_delay:%d;\r\n",DateACQPara.DAQ_Second_delay);
			break;
		case METHOD:
			//测量方式参数
			u3_printf("DAQ_Surveytype:%d;\r\n",DateACQPara.DAQ_Surveytype);
			u3_printf("DAQ_SynMode:%d;\r\n",DateACQPara.DAQ_SynMode);
			u3_printf("DAQ_Areatype:%d;\r\n",DateACQPara.DAQ_Areatype);	
			u3_printf("DAQ_PolMode:%d;\r\n",DateACQPara.DAQ_PolMode);
			break;
		case TRANSMIT:
			//发送装置参数
			u3_printf("DAQ_PowerTime:%d;\r\n",DateACQPara.DAQ_PowerTime);
			u3_printf("DAQ_PowerTms:%d;\r\n",DateACQPara.DAQ_PowerTms);
			u3_printf("DAQ_Positive_Cur:%d;\r\n",DateACQPara.DAQ_Positive_Cur);
			u3_printf("DAQ_Reverse_Cur:%d;\r\n",DateACQPara.DAQ_Reverse_Cur);
			break;
		case SYSTEM:
			//系统设置参数
			u3_printf("DAQ_MaxSlfEle:%d;\r\n",DateACQPara.DAQ_MaxSlfEle);
			u3_printf("DAQ_MinCurrent:%d;\r\n",DateACQPara.DAQ_MinCurrent);
			u3_printf("DAQ_MinOneEle:%d;\r\n",DateACQPara.DAQ_MinOneEle);
			u3_printf("DAQ_SPtype:%d;\r\n",DateACQPara.DAQ_SPtype);
			u3_printf("DAQ_Minpoweroff:%d;\r\n",DateACQPara.DAQ_Minpoweroff);
			u3_printf("DAQ_Dcdrift:%d;\r\n",DateACQPara.DAQ_Dcdrift);
			u3_printf("DAQ_Average:%d;\r\n",DateACQPara.DAQ_Average);
			u3_printf("DAQ_Effect:%d;\r\n",DateACQPara.DAQ_Effect);
			u3_printf("DAQ_ACEffect:%d;\r\n",DateACQPara.DAQ_ACEffect);
			u3_printf("DAQ_Power:%d;\r\n",DateACQPara.DAQ_Power);
			break;
		case ACQUISITION:
			//数据采集参数
			u3_printf("DAQ_SP:%d;\r\n",DateACQPara.DAQ_SP);
			u3_printf("DAQ_PowerVolt:%d;\r\n",DateACQPara.DAQ_PowerVolt);
			u3_printf("DAQ_100K:%d;\r\n",DateACQPara.DAQ_100K);
			u3_printf("DAQ_NatureEle:%d;\r\n",DateACQPara.DAQ_NatureEle);
			u3_printf("DAQ_OneEle:%d;\r\n",DateACQPara.DAQ_OneEle);
			u3_printf("DAQ_Polarizey_1000:%d;\r\n",DateACQPara.DAQ_Polarizey_1000);
			u3_printf("DAQ_PowerCurrent:%d;\r\n",DateACQPara.DAQ_PowerCurrent);
			u3_printf("DAQ_Resistance:%d;\r\n",DateACQPara.DAQ_Resistance);
			u3_printf("DAQ_ADplus:%d;\r\n",DateACQPara.DAQ_ADplus);
			u3_printf("DAQ_HlfDecay:%d;\r\n",DateACQPara.DAQ_HlfDecay);
			u3_printf("DAQ_SynPara:%d;\r\n",DateACQPara.DAQ_SynPara);
			u3_printf("DAQ_DecayDrg:%d;\r\n",DateACQPara.DAQ_DecayDrg);
			u3_printf("DAQ_DepartDrg:%d;\r\n",DateACQPara.DAQ_DepartDrg);
			u3_printf("DAQ_GNDRes:%d;\r\n",DateACQPara.DAQ_GNDRes);
			break;
		case MULTIPOLARIZATION:
			//添加多极化率时增加的参数
			u3_printf("DAQ_Ts1:%d;\r\n",DateACQPara.DAQ_Ts1);
			u3_printf("DAQ_Ts2:%d;\r\n",DateACQPara.DAQ_Ts2);
			u3_printf("DAQ_Ts3:%d;\r\n",DateACQPara.DAQ_Ts3);
			u3_printf("DAQ_Ts4:%d;\r\n",DateACQPara.DAQ_Ts4);
			u3_printf("DAQ_Polarizey_1000_1:%d;\r\n",DateACQPara.DAQ_Polarizey_1000_1);
			u3_printf("DAQ_Polarizey_1000_2:%d;\r\n",DateACQPara.DAQ_Polarizey_1000_2);
			u3_printf("DAQ_Polarizey_1000_3:%d;\r\n",DateACQPara.DAQ_Polarizey_1000_3);
			u3_printf("DAQ_Polarizey_1000_4:%d;\r\n",DateACQPara.DAQ_Polarizey_1000_4);
			u3_printf("DAQ_Polarizey_sys:%d;\r\n",DateACQPara.DAQ_Polarizey_sys);
			break;
		default:
			u3_printf("Unknow Print_Type.\r\n");
	}
}

//检测 WIFI 接收到的应答
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
	//if(strx != NULL)	
		return (u8*)strx;
	//else
		//return NULL;
}

//设置参数
void Set_Data(int type,u8 *p)
{
	u8 *p1,*p2,*p3;
	p1=mymalloc(SRAMEX,32);							//申请32字节内存
	p2=mymalloc(SRAMEX,32);							//申请32字节内存
	p3=mymalloc(SRAMEX,32);
	
	p1 = (u8*)strstr((const char*)(p+1),":");
	p2 = p1+1;
	*p1 = 0;
	
	//参数类别
	switch(type){
		case PROJECT:											//project参数
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}
					
				switch((u8)*p-48){
					case LineNum:
						DateACQPara.DAQ_LineNum = *p2 - 48;	break;
					case DotNum:
						DateACQPara.DAQ_DotNum = *p2 - 48;	break;
					case DotIncrease:
						DateACQPara.DAQ_DotIncrease = *p2 - 48;	break;
					case DotDirect:
						DateACQPara.DAQ_DotDirect = *p2 - 48;	break;
					case ABDiv2:
						printf("%s %p %p\r\n",p2,p1,p2);
						DateACQPara.DAQ_ABDiv2 = Data_Trasfer(p1-p2,p2);	break;
					case MNDiv2:
						DateACQPara.DAQ_MNDiv2 = Data_Trasfer(p1-p2,p2);	break;
					case Side:
						DateACQPara.DAQ_Side = *p2 - 48;	break;
					case Org_A:
						DateACQPara.DAQ_Org_A = Data_Trasfer(p1-p2,p2);	break;
					case BEF50Hz:
						DateACQPara.DAQ_BEF50Hz = *p2 - 48;	break;
					case Second_delay:
						DateACQPara.DAQ_Second_delay = Data_Trasfer(p1-p2,p2);	break;
					default:
						u3_printf("Unknow Project_Member\r\n");	
				}
				
				if(p3 == NULL)	break;
				
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(PROJECT);	break;
		case METHOD:
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}	
				switch((u8)*p-48){
					case Surveytype:
						DateACQPara.DAQ_Surveytype = *p2 - 48;	break;
					case SynMode:
						DateACQPara.DAQ_SynMode = *p2 - 48;	break;
					case Areatype:
						DateACQPara.DAQ_Areatype = *p2 - 48;	break;
					case PolMode:
						DateACQPara.DAQ_PolMode = *p2 - 48;	break;
					default:
						u3_printf("Unknow Method_Member\r\n");	
				}
				if(p3 == NULL)	break;
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(METHOD);	break;
		case TRANSMIT:
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}
				switch((u8)*p-48){
					case PowerTime:
						DateACQPara.DAQ_PowerTime = *p2 - 48;	break;
					case PowerTms:
						DateACQPara.DAQ_PowerTms = *p2 - 48;	break;					default:
						u3_printf("Unknow Trasmit_Member\r\n");	
				}
				if(p3 == NULL)	break;
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(TRANSMIT);	break;
		case SYSTEM:
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}	
				switch((u8)*p-48){
					case MaxSlfEle:
						DateACQPara.DAQ_MaxSlfEle = Data_Trasfer(p1-p2,p2);	break;
					case MinCurrent:
						DateACQPara.DAQ_MinCurrent = Data_Trasfer(p1-p2,p2);	break;
					case MinOneEle:
						DateACQPara.DAQ_MinOneEle = Data_Trasfer(p1-p2,p2);	break;
					case SPtype:
						DateACQPara.DAQ_SPtype = *p2 - 48;	break;
					case Minpoweroff:
						DateACQPara.DAQ_Minpoweroff = Data_Trasfer(p1-p2,p2);	break;
					default:
						u3_printf("Unknow System_Member\r\n");	
				}
				if(p3 == NULL)	break;
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(SYSTEM);	break;
		case ACQUISITION:
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}	
				switch((u8)*p-48){
					case SP:
						DateACQPara.DAQ_SP = *p2 - 48;	break;
					case PowerVolt:
						DateACQPara.DAQ_PowerVolt = Data_Trasfer(p1-p2,p2);	break;
					case ADplus:
						DateACQPara.DAQ_ADplus = Data_Trasfer(p1-p2,p2);	break;
					default:
						u3_printf("Unknow Acquisition_Member\r\n");	
				}
				if(p3 == NULL)	break;
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(ACQUISITION);	break;
		case MULTIPOLARIZATION:
			while(1){
				p1 = (u8*)strstr((const char*)p2,",");
				if(p1 == NULL){
					p1 = (u8*)strstr((const char*)p2,"}");
					*p1 = 0;
					p3 = NULL;
				}
				else{
					p3 = p1+1;
					*p1 = 0;
				}	
				switch((u8)*p-48){
					case Ts1:
						DateACQPara.DAQ_Ts1 = Data_Trasfer(p1-p2,p2);	break;
					case Ts2:
						DateACQPara.DAQ_Ts2 = Data_Trasfer(p1-p2,p2);	break;
					case Ts3:
						DateACQPara.DAQ_Ts3 = Data_Trasfer(p1-p2,p2);	break;
					case Ts4:
						DateACQPara.DAQ_Ts4 = Data_Trasfer(p1-p2,p2);	break;
					case Polarizey_sys:
						DateACQPara.DAQ_Polarizey_sys = *p2 - 48;	break;
					default:
						u3_printf("Unknow Multipolarization_Member\r\n");	
				}
				if(p3 == NULL)	break;
				p = p3;	
				p1 = (u8*)strstr((const char*)(p+1),":");
				p2 = p1+1;
				*p1 = 0;
			}	Data_Return(MULTIPOLARIZATION);	break;
		default:
			u3_printf("Unknow Set_Type");	
	}
	        		
	myfree(SRAMEX,p1);		//释放内存 
	myfree(SRAMEX,p2);		//释放内存 
	myfree(SRAMEX,p3);
}

int Data_Trasfer(u8 len,u8 *p){
	int i;
	int data = 0;
	printf("len:%d\r\n",len);
	for(i = 0;i < len;i++){
		data = data * 10 + (*(p+i) - 48);
	}
	return data;
}
/*
************************************************************************
*  函数名称：Dispageadata(void)( ) 
*  功能描述：创建项目测量方式数据显示函数
*  输入参数：none
*  返回参数：none
************************************************************************
*/
/********************dispagegps_data()******************/
//int dispagegps_data(void)
//{	
//	if(DateACQPara.DAQ_gps_year_buf<10)
//	{
//	Character_Generate(72,56,1,16,char_font8x16);
//	disdata(80,56,DateACQPara.DAQ_gps_year_buf);//数据显示
//	}
//	else
//	{
//	disdata(72,56,DateACQPara.DAQ_gps_year_buf);//数据显示
//	}
//	Character_Generate(88,60,2,11,charnian);
//	disdata(104,56,DateACQPara.DAQ_gps_month_buf);//数据显示
//	Character_Generate(120,60,2,11,charyue);
//	disdata(136,56,DateACQPara.DAQ_gps_day_buf);//数据显示
//	Character_Generate(152,60,2,11,charri);
//	disdata(168,56,DateACQPara.DAQ_gps_hour_buf);//数据显示
//	Character_Generate(184,56,1,12,char_font8x12+12*1*11);
//	disdata(192,56,DateACQPara.DAQ_gps_minus_buf);//数据显示
//	Character_Generate(208,56,1,12,char_font8x12+12*1*11);
//	disdata(216,56,DateACQPara.DAQ_gps_second_buf);//数据显示
//        DateACQParaDAQ_gps_ew1=DateACQPara.DAQ_gps_ew_buf*10000;
//        DateACQParaDAQ_gps_ns1=DateACQPara.DAQ_gps_ns_buf*10000;	        
//        disdata(78,78,DateACQParaDAQ_gps_ew1/1000000);//数据显示
//	disdata(120,78,DateACQParaDAQ_gps_ew1%1000000/10000);//数据显示
//	disdata(141,78,DateACQParaDAQ_gps_ew1%10000);//数据显示
//	if(DateACQPara.DAQ_gps_cew_buf=='E')
//	{Character_Generate(195,78,2,20,char_e);} //显示e
//	else
//	{Character_Generate(195,78,2,20,char_w);} //显示w	        
//        disdata(78,96,DateACQParaDAQ_gps_ns1/1000000);//数据显示
//	disdata(120,96,DateACQParaDAQ_gps_ns1%1000000/10000);//数据显示
//	disdata(141,96,DateACQParaDAQ_gps_ns1%10000);//数据显示
//	if(DateACQPara.DAQ_gps_cns_buf=='N')	       
//          {Character_Generate(195,96,2,20,char_n);} //显示n
//	else
//	 {Character_Generate(195,96,2,20,char_s);} //显示s	       
//        DateACQParaDAQ_gps_ew1=DateACQPara.DAQ_gps_ew*10000;
//	DateACQParaDAQ_gps_ns1=DateACQPara.DAQ_gps_ns*10000;
//	disdata(52,148,DateACQParaDAQ_gps_ew1/1000000);//数据显示
//	disdata(92,148,DateACQParaDAQ_gps_ew1%1000000/10000);//数据显示        
//        disdata(111,148,DateACQParaDAQ_gps_ew1%10000);//数据显示
//	if(DateACQPara.DAQ_gps_cew_buf=='E')
//	{Character_Generate(158,148,2,20,char_e);} //显示e
//	else	        {Character_Generate(158,148,2,20,char_w);} //显示w
//	disdata(52,179,DateACQParaDAQ_gps_ns1/1000000);//数据显示
//	disdata(92,179,DateACQParaDAQ_gps_ns1%1000000/10000);//数据显示
//	disdata(111,179,DateACQParaDAQ_gps_ns1%10000);//数据显示
//	if(DateACQPara.DAQ_gps_cns_buf=='N')	        
//         {Character_Generate(158,179,2,20,char_n);} //显示n
//	else
//	{Character_Generate(158,179,2,20,char_s);} //显示s
//	        return 1;
//}





/******************************数据处理部分******************************/

/*
************************************************************************
*  函数名称：Dispageadata(void)( ) 
*  功能描述：创建项目测量方式数据显示函数
*  输入参数：none
*  返回参数：none
************************************************************************
*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#define MAX_LINE_LEN 30
//signed int ReadLine(char *Line_Buf,FILE *fp)
////Return :
////       -1 Line too long.
////        n Line length.example:"12CRLF"---return 2
//{
//	int i=0;
//	char ch1,ch2;
//	ch1=0;
//	ch2=getc(fp);
//	while(!(ch1==0x0d&&ch2==0x0a)&&(!feof(fp))&&(i<MAX_LINE_LEN))
//    	{
//		Line_Buf[i]=ch2;
//		ch1=ch2;
//		ch2=getc(fp);
//		i++;
//	}
//	Line_Buf[i-1]='\0';
//	if(feof(fp))
//		return 0;
//	if(i>=MAX_LINE_LEN)
//		return -1;
//	else
//		return i-1;             //i not including CR and LF
//}
//unsigned int ReadSettings(char *Key,char *Ret1,char *Ret2,FILE *fp,char NoFirstStart)
///*
//	 Read the .ini parameter file.
//	 [KEY]=...
//Input:Key---Group name
//     Ret---Return number
//Output: 0:Can't find the KEY specified.
//	1:OK
//*/
//{
//	#define KEY_HEAD '['
//	char LineBuf[MAX_LINE_LEN],LineBufDataTemp[MAX_LINE_LEN],cFind=0;
//	if( !NoFirstStart )
//		fseek(fp,0L,SEEK_SET); //This may consume much time!!!
//	while(!feof(fp)&&!cFind)
//	{
//	if(ReadLine(LineBuf,fp)==-1)
//		return 0;
//	strcpy( Ret1,Ret2 );
//	strcpy( Ret2,LineBufDataTemp );
//	strcpy( LineBufDataTemp,LineBuf );
//	if(LineBuf[0]==KEY_HEAD&&strstr(LineBuf,Key))//The key found!
//	{
//		cFind = 1;
//		break;
//	}//If end
//	}//while end
//	if(feof(fp))
//		return 0;
//	return 1;
//}
//long data_change(long data_a)
//{
//	long lTemp = 0;
//	double dfTemp = 0;
//	dfTemp = data_a;
//	lTemp = dfTemp*1000*ADVREF/ADBIT;
//	return lTemp;
//}
//long GetNatureEle( FILE *fp,long PowerTms )//获取自然电位
//{
//	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];
//        long Data1=0,Data2=0;
//	long NatureEle = 0;
//	int i=0;
//	for( i=0;i<PowerTms;i++ )
//	{
//		if(ReadSettings("[sample freq(N)]=7.5hz",LineBufData1,LineBufData2,fp,i))
//		{
//			Data1=atol(LineBufData1);
//			Data2=atol(LineBufData2);
//			NatureEle = NatureEle+Data1+Data2;
//		}
//	}
//	NatureEle = NatureEle/(2*i);
//	NatureEle= data_change(NatureEle);
//	return NatureEle;
//}
//long GetOneEle( FILE *fp,long PowerTms )//获取一次电位
//{
//	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];

//	long Data1=0,Data2=0;
//	long OneEle = 0;	int i=0;
//	for( i=0;i<PowerTms;i++ )
//	{
//		if( ReadSettings("[sample freq(P-G)]=960hz",LineBufData1,LineBufData2,fp,i) )
//		{
//			Data1=atol(LineBufData1);
//			Data2=atol(LineBufData2);
//			OneEle = OneEle+Data1+Data2;
//		}
//	}
//	OneEle = OneEle/(2*i);
//	OneEle= data_change(OneEle);
//	return OneEle;
//}
//long Get100K(long Surveytype,long a,long b,long x,long y,long SP_OB)//获取K值
//{
//	U32 K100=0;
//printf("100k:a=%d,b=%d,x=%d,y=%d\n",a,b,x,y);switch(Surveytype)
//	{		
//		case 2:	//K100=(2*M_PI)/(abs(1/sqrt(y*y+(x+a-b)*(x+a-b))-1/sqrt(y*y+(x+a+b)*(x+a+b))
//			//-1/sqrt(y*y+(x-a-b)*(x-a-b))+1/sqrt(y*y+(x-a+b)*(x-a+b))));
//		K100=((2*M_PI)/((abs((1/sqrt(y*y+(x-b)*(x-b))-1/sqrt(y*y+(x+b)*(x+b))
//		-1/sqrt(y*y+(x-2*a-b)*(x-2*a-b))+1/sqrt(y*y+(x-2*a+b)*(x-2*a+b)))*1000000))*1.0))*1000000;
//			//改动因为Org_A=x;
//			break;
//		case 4:	K100=M_PI*(a*a-b*b)/(b);
//			break;
//		case 1:		
//		case 3:	K100=M_PI*(a*a-b*b)/(2*b);
//			break;
//	}
//	return K100*100;
//}
//long GetResistance(long OneEle,long PowerCurrent,long K100)//获取电阻率
//{
//	double lfTemp = K100;
//	long lTemp = 0;
//	lfTemp = lfTemp*OneEle/PowerCurrent/100;

//	lTemp = lfTemp;

//	return lTemp;

////	return K100*OneEle/PowerCurrent;

//}

//long GetPolarizey_1000( FILE *fp,long OneEle,long PowerTms,long Delay )//获取极化率

//{

//	char LineBufData1[MAX_LINE_LEN],LineBufData2[MAX_LINE_LEN];

//	long Polarizey_1000 = 0;

//	double lfPolarizey_1000 = 0;

//	int i=0,j=0;

//	if( OneEle==0 )

//	{

//		return 99999999;

//	}

//	for( i=0;i<PowerTms;i++ )

//	{
//		lfPolarizey_1000 = 0;

//		if(ReadSettings("[sample freq(P-G)]=960hz",LineBufData1,LineBufData2,fp,i))
//		{
//			for( j=0;j<Delay;j++ )

//			{

//				if(ReadLine(LineBufData1,fp)==-1)
//					return 0;

//			}

//			for( j=0;j<10;j++ )

//			{

//				if(ReadLine(LineBufData1,fp)==-1)
//					return 0;

//				lfPolarizey_1000 = lfPolarizey_1000 + atol(LineBufData1);

//			}

//			lfPolarizey_1000 = lfPolarizey_1000/10;

//			//Polarizey_1000 = Polarizey_1000 + lfPolarizey;

//			Polarizey_1000 = Polarizey_1000 + lfPolarizey_1000;
//		}

//	}

//	Polarizey_1000= data_change(Polarizey_1000);

//	Polarizey_1000 = Polarizey_1000*1000/(i*OneEle);

//	return Polarizey_1000;

//}
//int dataprocess(char filename[50])
//{
//	FILE *fpData;
//	if((fpData=fopen(filename,"rb"))==NULL)
//	{
//		return -1;
//	}

//	DateACQPara.DAQ_NatureEle=GetNatureEle( fpData,DateACQPara.DAQ_PowerTms);

//	DateACQPara.DAQ_OneEle=GetOneEle( fpData,DateACQPara.DAQ_PowerTms );

//	DateACQPara.DAQ_100K=Get100K(DateACQPara.DAQ_Surveytype,DateACQPara.DAQ_ABDiv2,DateACQPara.DAQ_MNDiv2,DateACQPara.DAQ_Org_A,DateACQPara.DAQ_Side,DateACQPara.DAQ_SP);

//	DateACQPara.DAQ_Resistance=GetResistance(DateACQPara.DAQ_OneEle,DateACQPara.DAQ_PowerCurrent,	DateACQPara.DAQ_100K);

//	DateACQPara.DAQ_Polarizey_1000=GetPolarizey_1000( fpData,DateACQPara.DAQ_OneEle,DateACQPara.DAQ_PowerTms,DateACQPara.DAQ_Second_delay );//获取极化率



//printf( "NatureEle=%d,OneEle=%d,100K=%d,Resistance=%d,Polarizey_1000=%d\n",DateACQPara.DAQ_NatureEle,DateACQPara.DAQ_OneEle,DateACQPara.DAQ_100K,DateACQPara.DAQ_Resistance,DateACQPara.DAQ_Polarizey_1000);

//	fclose(fpData);

//	return 1;
//}
