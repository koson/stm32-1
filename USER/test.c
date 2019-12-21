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

#include "includes.h"

void system_init(void);

//������ʼ����
//�������ȼ�
#define START_TASK_PRIO		2
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//WIFI�ź����������񴴽�
#define Wifi_Deal_TASK_PRIO		4
#define Wifi_Deal_STK_SIZE 		128
OS_TCB WifiDealTaskTCB;
CPU_STK Wifi_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void Wifi_Deal_Task(void *p_arg);

//�ɼ����񴴽�
#define Acquisition_Deal_TASK_PRIO		3
#define Acquisition_Deal_STK_SIZE 		128
OS_TCB 	AcqusitionDealTaskTCB;
CPU_STK Acquisition_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void 		Acquisition_Deal_Task(void *p_arg);

//������񴴽�
#define Monitor_Deal_TASK_PRIO		5
#define Monitor_Deal_STK_SIZE 		128
OS_TCB 	MonitorDealTaskTCB;
CPU_STK Monitor_Deal_TASK_STK[Wifi_Deal_STK_SIZE];
void 		Monitor_Deal_Task(void *p_arg);

//Wifi�����ʱ��
OS_TMR 	wifi_deal_tmr;
void Wifi_Deal_Callback(void *p_tmr, void *p_arg);

//�������ʱ��
OS_TMR 	monitor_deal_tmr;
void Monitor_Deal_Callback(void *p_tmr, void *p_arg);




//main����	  					
int main(void)
{ 	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	system_init();		//ϵͳ��ʼ�� 
	
	OSInit(&err);
	
 	OS_CRITICAL_ENTER();//�����ٽ���
	////������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
							 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���					 
								 
	OSStart(&err);	  						    
}

void system_init(void)
{
	delay_init();  //ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	uart_init(115200);								//��ʼ�����ڲ�����Ϊ115200
	usart3_init(115200);							//��ʼ������2������Ϊ115200
	
	FSMC_SRAM_Init();			//��ʼ���ⲿSRAM  
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	my_mem_init(SRAMEX);		//��ʼ���ⲿ�ڴ��

	//flash_init();
	//rtc_init();

	Adc_Init();
	Power_Init();
	IPM_Init();
	Acquisition_Init();
	CS5532_Init();
		
	while(SD_Init())
	{
		printf("���SD����...\r\n");
	}
	printf("SD����׼�� \r\n");	
	exfuns_init();							//Ϊfatfs��ر��������ڴ�	
  f_mount(fs[0],"0:",1); 					//����SD�� 
			
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
	
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//�����ٽ���	
		
	//����Usart3��ʱ��
	OSTmrCreate((OS_TMR		*)&wifi_deal_tmr,		//Usart3��ʱ��
                (CPU_CHAR	*)"wifi deal tmr",		//��ʱ������
                (OS_TICK	 )0,			
                (OS_TICK	 )10,          //10*10=100ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)Wifi_Deal_Callback,//Usart3��ʱ���ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	  *)&err);		//���صĴ�����
//	//����Usart��ʱ��
//	OSTmrCreate((OS_TMR		*)&usart_deal_tmr,		//Usart��ʱ��
//                (CPU_CHAR	*)"usart deal tmr",		//��ʱ������
//                (OS_TICK	 )0,			
//                (OS_TICK	 )10,          //10*10=100ms
//                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
//                (OS_TMR_CALLBACK_PTR)Usart_Deal_Callback,//Usart��ʱ���ص�����
//                (void	    *)0,			//����Ϊ0
//                (OS_ERR	    *)&err);		//���صĴ�����
								
	OSTmrStart(&wifi_deal_tmr,&err);
//	OSTmrStart(&usart_deal_tmr,&err);
	
	//����Usart3�ź�����������
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
								 							 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);	//����ʼ����			 
	OS_CRITICAL_EXIT();								//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}

void Wifi_Deal_Callback(void *p_tmr, void *p_arg)
{
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{
		u16 rlen=0;
		
		rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
		USART3_RX_BUF[rlen]=0;		//��ӽ����� 
		USART3_RX_STA=0;
		
		printf("%s",USART3_RX_BUF);
		
		if(USART3_RX_BUF[11]=='A' & USART3_RX_BUF[12]=='P')
		{
			printf("APָ��\r\n");
		}
		else
		{
			printf("δָ֪��\r\n");
			printf("%c %c",USART3_RX_BUF[11],USART3_RX_BUF[12]);
		}
	}	
}

//void Usart_Deal_Callback(void *p_tmr, void *p_arg)
//{
//	OS_ERR err;
//	printf("APָ��\r\n");
//	OSTimeDlyHMSM(0,0,5,0,OS_OPT_TIME_HMSM_STRICT,&err);
//	
////	if(USART_RX_STA&0X8000)		//���յ�һ��������
////	{		
////		printf("%s\r\n",USART_RX_BUF);
////		USART_RX_STA=0;
////		
////		if(USART_RX_BUF[0]=='A' & USART_RX_BUF[1]=='P')
////		{
////			printf("APָ��\r\n");
////		}
////		else
////		{
////			printf("δָ֪��\r\n");
////			printf("%c %c",USART_RX_BUF[11],USART_RX_BUF[12]);
////		}
////	}	
//}

//IPM����
//void Usart3_Deal_Task(void *p_arg)
//{
//	OS_ERR err;
////	CPU_SR_ALLOC();
//	
//	IPM_POWER_EN= 0;
//	
//	while(1)
//	{
//		IPM_Start_AB();
//		printf("SUP��%lu��SVP��%lu��SUN��%lu��SUN��%lu��\r\n",IPM_SUP,IPM_SVP,IPM_SUN,IPM_SVN);		
//		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		IPM_Start_BA();
//		printf("SUP��%lu��SVP��%lu��SUN��%lu��SUN��%lu��\r\n",IPM_SUP,IPM_SVP,IPM_SUN,IPM_SVN);		
//		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		IPM_Stop_AB();	
//		printf("SUP��%lu��SVP��%lu��SUN��%lu��SUN��%lu��\r\n",IPM_SUP,IPM_SVP,IPM_SUN,IPM_SVN);	
//		OSTimeDlyHMSM(0,0,2,0,OS_OPT_TIME_HMSM_STRICT,&err);
//		
//	}
//}

//AD����
void Wifi_Deal_Task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	
	while(1)
	{
		float i;
		i = Get_Battery();
		printf("��Դ��ѹֵ��%.2f V.    ������%.1f %%\r\n",i,(100-(12-i)*30));
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
	}
}












