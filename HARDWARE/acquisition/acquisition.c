#include "acquisiton.h"
#include "includes.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//��ʼ��IPM
//AD_POWER_EN��PC2��		�ɼ����Դ����		0:; 1:.
//ACQ_SWITCH(PB9)				ѡ�����MN���ǵ���	0:; 1:.
//TRAP_50Hz_EN(PB8)					ѡ���Ƿ����50Hz�ݲ�	0:���ݲ�;	1:�ݲ�;
//SPI_CS(PB12)��SPI_SDI(PC7)��SPI_SDO(PC6)��SPI_SCLK(PG8)	ģ��ת��
//********************************Ӳ����ʼ��********************************//
void Acquisition_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
		RCC_APB2Periph_GPIOG, ENABLE);	 					//ʹ����Ӧʱ��
	 
	//��ʼ�� AD_POWER_EN��PC2��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�Ϊ50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 				//��ʼ�� AD_POWER_EN��PC2��
	
	//��ʼ�� ACQ_SWITCH(PB9)	&	TRAP_50Hz_EN(PB8)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	 
	//SPI_CS(PB12)��SPI_SDI(PC7)��SPI_SDO(PC6)��SPI_SCLK(PG8)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	//��λ
	GPIO_ResetBits(GPIOC,GPIO_Pin_2);							//AD_POWER_EN 0:off; 1:on.
	GPIO_ResetBits(GPIOB,GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_12);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6 | GPIO_Pin_7);
	GPIO_ResetBits(GPIOG,GPIO_Pin_8);
}


//********************************UCOIII���񴴽�********************************//
//��������
//�������ȼ�
#define ACQUISITION_TASK_PRIO		3
//�����ջ��С	
#define ACQUISITION_STK_SIZE 		128
//������ƿ�
OS_TCB AcqusitionTaskTCB;
//�����ջ	
CPU_STK ACQUISITION_TASK_STK[ACQUISITION_STK_SIZE];
//������
void Acquisition_Task(void *p_arg);


void Acquisition_Task_Create(int *err)
{
	OSTaskCreate((OS_TCB 	*   )&AcqusitionTaskTCB,			//������ƿ�
							 (CPU_CHAR	* )"acquisition task", 			//��������
               (OS_TASK_PTR )Acquisition_Task, 				//������
               (void		*   )0,												//���ݸ��������Ĳ���
               (OS_PRIO	    )ACQUISITION_TASK_PRIO, 	//�������ȼ�
               (CPU_STK   * )&ACQUISITION_TASK_STK[0],//�����ջ����ַ
               (CPU_STK_SIZE)ACQUISITION_STK_SIZE/10,	//�����ջ�����λ
               (CPU_STK_SIZE)ACQUISITION_STK_SIZE,		//�����ջ��С
               (OS_MSG_QTY  )0,												//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
               (OS_TICK	    )0,												//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
               (void   	*   )0,												//�û�����Ĵ洢��
               (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
               (OS_ERR 	  * )*err);										//��Ÿú�������ʱ�ķ���ֵ
}

void Acquisition_Task(void *p_arg)
{
	while(1)
	{
		
	}
}

/*
************************************************************************
*  �������ƣ�Set_Bef50Hz(char daq_bef50hz)
*  ����������50Hz�ݲ�����	0:���ݲ�;	1:�ݲ�;
*  ���������none
*  ���ز�����none
************************************************************************
*/
int Set_Bef50Hz(char daq_bef50hz)
{
	//д�ļ�
	
	
	
	
	
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






