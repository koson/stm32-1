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

//WIFI�źŴ������񴴽�
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

extern DAQ_PARA DateACQPara;      // JDY �����ṹ��


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
	//exit_init();
	
	Power_Init();
	Adc_Init();
	Acquisition_Init();
	CS5532_Init();
	IPM_Init();
	Date_ACQ_Para_Init();
		
	while(SD_Init())
	{
		printf("���SD����...\r\n");
	}
	printf("SD����׼�� \r\n");	
	exfuns_init();							//Ϊfatfs��ر��������ڴ�	
  f_mount(fs[0],"0:",1); 					//����SD�� 
	
//	AP_Init();
	
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
                (OS_TICK	 )100,          //10*10=100ms
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

//����WIFI
//WIFI���֣�CUGB	125IP		���룺cugb125126
//IP:192.168.1.1	�˿ںţ�8899
//������ʽ��S0 = {0:1,1:2};
void Wifi_Deal_Callback(void *p_tmr, void *p_arg)
{
	CPU_SR_ALLOC();
	OS_CRITICAL_ENTER();
	if(USART3_RX_STA&0X8000)		//���յ�һ��������
	{		
		u8 *p;
		p=mymalloc(SRAMEX,32);							//����32�ֽ��ڴ�
		
		//Wifiͨ�Ų���
		//���Ը�ʽ��T+���� �� ��������
		if(USART3_RX_BUF[0] == 'T'){
			p = atk_8266_check_cmd("T") + 1;
			u3_printf("%s\r\n",p+1);
		}
		//���ò���
		//�����ʽ��S0 = {0:1,1:2}
		else if(USART3_RX_BUF[0] == 'S'){
			p = atk_8266_check_cmd("{")+1;
			Set_Data((u8)USART3_RX_BUF[1]-48,p);
		}
		//��ȡ����
		else if(USART3_RX_BUF[0] == 'G'){
			Data_Return((u8)USART3_RX_BUF[1] - 48);
		}
		
		USART3_RX_STA=0;	//��־λ����
		myfree(SRAMEX,p);		//�ͷ��ڴ� 		
	}	
	OS_CRITICAL_EXIT();
}



//AD����
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












