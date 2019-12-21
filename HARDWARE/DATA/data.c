#include "data.h"

DAQ_PARA  DateACQPara;      // JDY 参数结构体

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
	DateACQPara.DAQ_SynMode          = 0;
	//DateACQPara.DAQ_Areatype         = 2;           // 测多极化率 (值不是0和1就是)
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
//	DateACQPara.DAQ_gps_cav          = 65;
//	DateACQPara.DAQ_gps_ns           = 3959.3159;
//	DateACQPara.DAQ_gps_cns          = 78;
//	DateACQPara.DAQ_gps_ew           = 11620.7382;
//	DateACQPara.DAQ_gps_cew          = 69;


	//??????
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

/*
************************************************************************
*  函数名称：Dispageadata(void)( ) 
*  功能描述：创建项目测量方式数据显示函数
*  输入参数：none
*  返回参数：none
************************************************************************
*/
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
