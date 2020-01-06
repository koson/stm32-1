#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "wifiap.h"
#include "usart3.h"
#include "malloc.h"
#include "sram.h"
#include "usmart.h"
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h" 

#include "adc.h"
#include "acquisiton.h"
#include "IPM.h"
#include "power.h"
#include "cs5532.h"
//#include "data.h"
#include "string.h"
#include "includes.h"
#include "data.h"

void system_init(void);

//创建开始任务
//任务优先级
#define START_TASK_PRIO		2
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//WIFI信号处理任务创建
#define Wifi_Deal_TASK_PRIO		4
#define Wifi_Deal_STK_SIZE 		128
OS_TCB WifiDealTaskTCB;
CPU_STK Wifi_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void Wifi_Deal_Task(void *p_arg);

//采集任务创建
#define Acquisition_Deal_TASK_PRIO		3
#define Acquisition_Deal_STK_SIZE 		128
OS_TCB 	AcqusitionDealTaskTCB;
CPU_STK Acquisition_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void 		Acquisition_Deal_Task(void *p_arg);

//监测任务创建
#define Monitor_Deal_TASK_PRIO		5
#define Monitor_Deal_STK_SIZE 		128
OS_TCB 	MonitorDealTaskTCB;
CPU_STK Monitor_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void 		Monitor_Deal_Task(void *p_arg);

//Wifi软件定时器
OS_TMR 	wifi_deal_tmr;
void Wifi_Deal_Callback(void *p_tmr, void *p_arg);

//监测任务定时器
OS_TMR 	monitor_deal_tmr;
void Monitor_Deal_Callback(void *p_tmr, void *p_arg);

extern DAQ_PARA DateACQPara;      // JDY 参数结构体


//main函数	  					
int main(void)
{ 	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	system_init();		//系统初始化 
	
	OSInit(&err);
	
 	OS_CRITICAL_ENTER();//进入临界区
	////创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
							 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区					 
								 
	OSStart(&err);	  						    
}

void system_init(void)
{
	delay_init();  //时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	uart_init(115200);								//初始化串口波特率为115200
	usart3_init(115200);							//初始化串口2波特率为115200
	
	FSMC_SRAM_Init();			//初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池

	//flash_init();
	//rtc_init();
	//exit_init();
	
	Power_Init();
	Adc_Init();
	Acquisition_Init();
	CS5532_Init();
	IPM_Init();
	Date_ACQ_Para_Init();
		
	while(SD_Init())
	{
		printf("检测SD卡中...\r\n");
	}
	printf("SD卡已准备 \r\n");	
	exfuns_init();							//为fatfs相关变量申请内存	
  f_mount(fs[0],"0:",1); 					//挂载SD卡 
	
//	AP_Init();
	
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区	
		
	//创建Usart3定时器
	OSTmrCreate((OS_TMR		*)&wifi_deal_tmr,		//Usart3定时器
                (CPU_CHAR	*)"wifi deal tmr",		//定时器名字
                (OS_TICK	 )0,			
                (OS_TICK	 )100,          //10*10=100ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)Wifi_Deal_Callback,//Usart3定时器回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	  *)&err);		//返回的错误码
//	//创建Usart定时器
//	OSTmrCreate((OS_TMR		*)&usart_deal_tmr,		//Usart定时器
//                (CPU_CHAR	*)"usart deal tmr",		//定时器名字
//                (OS_TICK	 )0,			
//                (OS_TICK	 )10,          //10*10=100ms
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
//                (OS_TMR_CALLBACK_PTR)Usart_Deal_Callback,//Usart定时器回调函数
//                (void	    *)0,			//参数为0
//                (OS_ERR	    *)&err);		//返回的错误码
								
	OSTmrStart(&wifi_deal_tmr,&err);
//	OSTmrStart(&usart_deal_tmr,&err);
	
	//创建Usart3信号量处理任务
	OSTaskCreate((OS_TCB 	* )&WifiDealTaskTCB,		
				 (CPU_CHAR	* )"Wifi Deal task", 		
                 (OS_TASK_PTR )Wifi_Deal_Task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Wifi_Deal_TASK_PRIO,     	
                 (CPU_STK   * )&Wifi_Deal_TASK_STK[0],	
                 (CPU_STK_SIZE)Wifi_Deal_STK_SIZE/10,	
                 (CPU_STK_SIZE)Wifi_Deal_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
								 							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);	//挂起开始任务			 
	OS_CRITICAL_EXIT();								//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}

//有人WIFI
//WIFI名字：CUGB	125IP		密码：cugb125126
//IP:192.168.1.1	端口号：8899
//命令形式：S0 = {0:1,1:2};
void Wifi_Deal_Callback(void *p_tmr, void *p_arg)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	if(USART3_RX_STA&0X8000)		//接收到一次数据了
	{		
		u8 *p;
		p=mymalloc(SRAMEX,32);							//申请32字节内存
		
		//Wifi通信测试
		//测试格式：T+数据 → 返回数据
		if(USART3_RX_BUF[0] == 'T'){
			p = atk_8266_check_cmd("T") + 1;
			u3_printf("%s\r\n",p+1);
		}
		//设置参数
		//命令格式：S0 = {0:1,1:2}
		else if(USART3_RX_BUF[0] == 'S'){
			p = atk_8266_check_cmd("{")+1;
			Set_Data((u8)USART3_RX_BUF[1]-48,p);
		}
		//获取数据
		else if(USART3_RX_BUF[0] == 'G'){
			Data_Return((u8)USART3_RX_BUF[1] - 48);
		}
		
		USART3_RX_STA=0;	//标志位清零
		myfree(SRAMEX,p);		//释放内存 		
	}	
	OS_CRITICAL_EXIT();
}



//AD测试
void Wifi_Deal_Task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	while(1)
	{
		FILE *fp;
		printf("11111\r\n");
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}   
}












