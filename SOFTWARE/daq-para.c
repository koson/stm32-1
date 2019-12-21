#incldue "daq_para.h"

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