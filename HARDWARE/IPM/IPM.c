#include "IPM.h"
#include "includes.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//初始化IPM
//IPM_POWER_EN（PA2）
//IPM_SVP（PE2）、IPM_SUP（PC13）、IPM_SVN（PC0）、IPM_SUN（PF9）

void IPM_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
		RCC_APB2Periph_GPIOE  | RCC_APB2Periph_GPIOF, ENABLE);	 //使能相应时钟
	 
	//初始化 IPM_POWER_EN（PA1）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);	 				//初始化 IPM_POWER_EN（PA2）
	 
	//初始化IPM_SVP（PE2）、IPM_SUP（PC13）、IPM_SVN（PC0）、IPM_SUN（PF9）
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	//复位
	GPIO_SetBits(GPIOA,GPIO_Pin_1);							//IPM_POWER_EN 0:on; 1:off.
	GPIO_ResetBits(GPIOC,GPIO_Pin_13 | GPIO_Pin_0);
	GPIO_ResetBits(GPIOE,GPIO_Pin_2);
	GPIO_ResetBits(GPIOF,GPIO_Pin_9);
}




////********************************UCOIII任务创建********************************//
////创建任务
////任务优先级
//#define IPM_TASK_PRIO		2
////任务堆栈大小	
//#define IPM_STK_SIZE 		128
////任务控制块
//OS_TCB IPMTaskTCB;
////任务堆栈	
//CPU_STK IPM_TASK_STK[IPM_STK_SIZE];
////任务函数
//void IPM_Task(void *p_arg);


//void IPM_Task_Create(int *err)
//{
//	OSTaskCreate((OS_TCB 	*   )&IPMTaskTCB,			//任务控制块
//							 (CPU_CHAR	* )"IPM task", 			//任务名字
//               (OS_TASK_PTR )IPM_Task, 				//任务函数
//               (void		*   )0,												//传递给任务函数的参数
//               (OS_PRIO	    )IPM_TASK_PRIO, 	//任务优先级
//               (CPU_STK   * )&IPM_TASK_STK[0],//任务堆栈基地址
//               (CPU_STK_SIZE)IPM_STK_SIZE/10,	//任务堆栈深度限位
//               (CPU_STK_SIZE)IPM_STK_SIZE,		//任务堆栈大小
//               (OS_MSG_QTY  )0,												//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
//               (OS_TICK	    )0,												//当使能时间片轮转时的时间片长度，为0时为默认长度，
//               (void   	*   )0,												//用户补充的存储区
//               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
//               (OS_ERR 	  * )*err);										//存放该函数错误时的返回值
//}

//void IPM_Task(void *p_arg)
//{
////	while(1)
////	{
////		//根据工作模式发射
////	}
//	
//	
//}

/**********************************************************
*  函数名称：IPM_Start_AB( ) 
*  功能描述：控制使能端置高，AB开始供电，注意AB方向
*  输入参数：            
*  返回参数：            
**********************************************************/
void    IPM_Start_AB(void)
{   
			GPIO_ResetBits(IPM_SUN_PORT, IPM_SUN_IO);	//SUP
      GPIO_ResetBits(IPM_SVP_PORT, IPM_SVP_IO);	// 
	
    	GPIO_SetBits(IPM_SUP_PORT, IPM_SUP_IO);	//SUP
    	GPIO_SetBits(IPM_SVN_PORT, IPM_SVN_IO);	//SVN
        
         
}

/**********************************************************
*  函数名称：IPM_Start_BA( ) 
*  功能描述：控制使能端置高，BA开始供电，注意AB方向
*  输入参数：            
*  返回参数：            
**********************************************************/
void    IPM_Start_BA(void)
{    
			GPIO_ResetBits(IPM_SUP_PORT, IPM_SUP_IO);
      GPIO_ResetBits(IPM_SVN_PORT, IPM_SVN_IO);
	
    	GPIO_SetBits(IPM_SVP_PORT, IPM_SVP_IO);
    	GPIO_SetBits(IPM_SUN_PORT, IPM_SUN_IO);
        
        
}

/**********************************************************
*  函数名称：IPM_Stop_AB( ) 
*  功能描述：控制使能端置低，AB供电停止
*  输入参数：            
*  返回参数：            
**********************************************************/
void    IPM_Stop_AB(void)
{  
	GPIO_ResetBits(IPM_SUP_PORT, IPM_SUP_IO);
	GPIO_ResetBits(IPM_SVP_PORT, IPM_SVP_IO);
	GPIO_ResetBits(IPM_SUN_PORT, IPM_SUN_IO);
	GPIO_ResetBits(IPM_SVN_PORT, IPM_SVN_IO);
}


