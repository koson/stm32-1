#ifndef __DATA_H__
#define __DATA_H__	 
#include "sys.h"

#include "malloc.h"
#include "usart3.h" 
#include "usart.h"
#include "string.h"
#include "includes.h"

/* 
    取样电阻阻值 
*/
#define  RESTEST	 0.20


//参数类别定义
#define PROJECT 		0
#define METHOD 			1
#define TRANSMIT 		2
#define SYSTEM 			3
#define ACQUISITION 4
#define MULTIPOLARIZATION 5

//创建项目参数定义
#define LineNum 0
#define DotNum	1
#define DotIncrease	2
#define DotDirect		3
#define ABDiv2			4
#define MNDiv2			5
#define Side				6
#define Org_A				7
#define BEF50Hz			8
#define Second_delay	9

/* 测量方式中设置以下参数 */
#define Surveytype 	0		  	// 测量类型：1为测深，2为中梯，3为对称四极，4为联剖
#define SynMode			1	    	// 同步方式：0为内同步，其他为自同步*******
#define Areatype		2	    	// 面积方式：0为不测量极化率，1为单极化率,其他为多极化率
#define PolMode			3   		// 极化率计算方式：

/* 发送装置界面下设置以下参数 */
#define PowerTime			0    	// 供电时间
#define PowerTms			1   	// 供电次数
#define Positive_Cur	2			// 正向电流(测量显示)
#define Reverse_Cur		3			// 反向电流(测量显示)


/* 系统设置界面下设置以下参数 */
#define MaxSlfEle		0				// 最大自电
#define MinCurrent	1				// 最小电流	
#define MinOneEle		2				// 最小一次场
#define SPtype			3				// 0系统SP1,1为系统SP2,其他为用户SP
#define Minpoweroff	4				// 自动关机分钟数
#define Dcdrift			5				// 系统直流漂移
#define Average			6				// 系统测试平均值
#define Effect			7				// 系统测试有效值
#define ACEffect		8				// 系统测试有效值
#define Power				9   		// 电池电压
 
/* 数据采集界面下显示以下参数 */
#define SP				 0  			// SP，可以设置
#define PowerVolt		1	    	// 供电电压，可以设置
#define K100				2    		// 100倍的K值，显示 **
#define NatureEle		3    		// 自然电位，显示 **
#define OneEle			4				// 一次电位，显示 **
#define Polarizey_1000	5   // 极化率1，显示 **
#define PowerCurrent		6		// 供电电流，显示 **
#define Resistance			7		// 电阻率，显示 **
#define ADplus			8				// AD增益, 设置
#define HlfDecay		9	    	// 半衰时，显示 **
#define SynPara			10    	// 综合参数，显示 **		//:        供电脉宽<2s，不测量
#define DecayDrg		11	    // 衰减度             	//;      供电脉宽<2s，不测量
#define DepartDrg		12	    // 偏离度，显示 **			//<
#define GNDRes			13   		// 接地电阻，显示 **		//=

/* 添加多极化率时增加的参数 */
#define Ts1	0				   			// 取样宽度
#define Ts2	1			    			// 取样宽度
#define Ts3	2			    			// 取样宽度
#define Ts4	3			    			// 取样宽度
#define Polarizey_1000_1	4	// 极化率1,
#define Polarizey_1000_2	5	// 极化率2,
#define Polarizey_1000_3	6	// 极化率3,
#define Polarizey_1000_4	7	// 极化率4,
#define Polarizey_sys			8	// 取样宽度模式选择1,2,3,4
/*  
    激电仪 参数结构体    
*/
typedef struct DAQ_PARA
{
	/* 添加多极化率时增加的参数 */
	long DAQ_Ts1;				    // 取样宽度
	long DAQ_Ts2;				    // 取样宽度
	long DAQ_Ts3;				    // 取样宽度
	long DAQ_Ts4;				    // 取样宽度
	long DAQ_Polarizey_1000_1;		// 极化率1,
	long DAQ_Polarizey_1000_2;		// 极化率2,
	long DAQ_Polarizey_1000_3;		// 极化率3,
	long DAQ_Polarizey_1000_4;		// 极化率4,
	char DAQ_Polarizey_sys;			// 取样宽度模式选择1,2,3,4

	/* 创建项目中设置以下参数 */
	long DAQ_LineNum;			    // 线号
	long DAQ_DotNum;			    // 点号
	long DAQ_DotIncrease;			// 点号增量
	char DAQ_DotDirect;			  // 点号方:0为减少,非0为增加
	long DAQ_ABDiv2;			    // AB/2的距离
	long DAQ_MNDiv2;			    // MN/2的距离
	long DAQ_Side;				    // 旁侧距离
	long DAQ_Org_A;				    // 原点-A极
	char DAQ_BEF50Hz;			    // 50Hz陷波设置:0为不陷波,非0为陷波
	long DAQ_Second_delay;			// 二次场延时

	/* 测量方式中设置以下参数 */
	char DAQ_Surveytype;		    // 测量类型：1为测深，2为中梯，3为对称四极，4为联剖
	char DAQ_SynMode;			    // 同步方式：0为内同步，其他为自同步*******
	char DAQ_Areatype;			    // 面积方式：0为不测量极化率，1为单极化率,其他为多极化率
	char DAQ_PolMode;			    // 极化率计算方式：
	
	/* 发送装置界面下设置以下参数 */
	long DAQ_PowerTime;			    // 供电时间
	long DAQ_PowerTms;			    // 供电次数
	long DAQ_Positive_Cur;			// 正向电流(测量显示)
	long DAQ_Reverse_Cur;			// 反向电流(测量显示)


	/* 系统设置界面下设置以下参数 */
	long DAQ_MaxSlfEle;			    // 最大自电
	long DAQ_MinCurrent;			// 最小电流	
	long DAQ_MinOneEle;			    // 最小一次场
	char DAQ_SPtype;				// 0系统SP1,1为系统SP2,其他为用户SP
	long DAQ_Minpoweroff;			// 自动关机分钟数
	long DAQ_Dcdrift;				// 系统直流漂移
	long DAQ_Average;			    // 系统测试平均值
	long DAQ_Effect;				// 系统测试有效值
	long DAQ_ACEffect;				// 系统测试有效值
	int  DAQ_Power;				    // 电池电压
	 
	/* 数据采集界面下显示以下参数 */
	long DAQ_SP;				    // SP，可以设置
	long DAQ_PowerVolt;			    // 供电电压，可以设置
	long DAQ_100K;				    // 100倍的K值，显示 **
	long DAQ_NatureEle;			    // 自然电位，显示 **
	long DAQ_OneEle;				// 一次电位，显示 **
	long DAQ_Polarizey_1000;	    // 极化率1，显示 **
	long DAQ_PowerCurrent;			// 供电电流，显示 **
	long DAQ_Resistance;			// 电阻率，显示 **
	char DAQ_ADplus;				// AD增益, 设置
	long DAQ_HlfDecay;			    // 半衰时，显示 **
	long DAQ_SynPara;			    // 综合参数，显示 **        供电脉宽<2s，不测量
	long DAQ_DecayDrg;			    // 衰减度                   供电脉宽<2s，不测量
	long DAQ_DepartDrg;			    // 偏离度，显示 **
	long DAQ_GNDRes;			    // 接地电阻，显示 **
} DAQ_PARA;

void Date_ACQ_Para_Init(void);
u8* atk_8266_check_cmd(u8 *str);
void Set_Data(int type,u8 *p);
void Data_Return(u8 type);
#endif
