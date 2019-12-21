#include "acquisiton.h"
#include "includes.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//初始化IPM
//AD_POWER_EN（PC2）		采集板电源开关		0:; 1:.
//ACQ_SWITCH(PB9)				选择测试MN或是电流	0:; 1:.
//TRAP_50Hz_EN(PB8)					选择是否进行50Hz陷波	0:不陷波;	1:陷波;
//SPI_CS(PB12)、SPI_SDI(PC7)、SPI_SDO(PC6)、SPI_SCLK(PG8)	模数转换
//********************************硬件初始化********************************//
void Acquisition_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
		RCC_APB2Periph_GPIOG, ENABLE);	 					//使能相应时钟
	 
	//初始化 AD_POWER_EN（PC2）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 				//初始化 AD_POWER_EN（PC2）
	
	//初始化 ACQ_SWITCH(PB9)	&	TRAP_50Hz_EN(PB8)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	//SPI_CS(PB12)、SPI_SDI(PC7)、SPI_SDO(PC6)、SPI_SCLK(PG8)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//复位
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);							//AD_POWER_EN 0:off; 1:on.
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_ResetBits(GPIOG,GPIO_Pin_8);
}


//********************************UCOIII任务创建********************************//
//创建任务
//任务优先级
#define ACQUISITION_TASK_PRIO		3
//任务堆栈大小	
#define ACQUISITION_STK_SIZE 		128
//任务控制块
OS_TCB AcqusitionTaskTCB;
//任务堆栈	
CPU_STK ACQUISITION_TASK_STK[ACQUISITION_STK_SIZE];
//任务函数
void Acquisition_Task(void *p_arg);


void Acquisition_Task_Create(int *err)
{
	OSTaskCreate((OS_TCB 	*   )&AcqusitionTaskTCB,			//任务控制块
							 (CPU_CHAR	* )"acquisition task", 			//任务名字
               (OS_TASK_PTR )Acquisition_Task, 				//任务函数
               (void		*   )0,												//传递给任务函数的参数
               (OS_PRIO	    )ACQUISITION_TASK_PRIO, 	//任务优先级
               (CPU_STK   * )&ACQUISITION_TASK_STK[0],//任务堆栈基地址
               (CPU_STK_SIZE)ACQUISITION_STK_SIZE/10,	//任务堆栈深度限位
               (CPU_STK_SIZE)ACQUISITION_STK_SIZE,		//任务堆栈大小
               (OS_MSG_QTY  )0,												//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
               (OS_TICK	    )0,												//当使能时间片轮转时的时间片长度，为0时为默认长度，
               (void   	*   )0,												//用户补充的存储区
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
               (OS_ERR 	  * )*err);										//存放该函数错误时的返回值
}

void Acquisition_Task(void *p_arg)
{
	while(1)
	{
		
	}
}

/*
************************************************************************
*  函数名称：Set_Bef50Hz(char daq_bef50hz)
*  功能描述：50Hz陷波设置	0:不陷波;	1:陷波;
*  输入参数：none
*  返回参数：none
************************************************************************
*/
int Set_Bef50Hz(char daq_bef50hz)
{
	//写文件
	
	
	
	
	
	if(daq_bef50hz==0)
	{
		TRAP_50Hz_EN = 0;
	}
	else
	{
		TRAP_50Hz_EN =1;
	}
	return 1;
}






