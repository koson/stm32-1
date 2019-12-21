#include "power.h"
#include "adc.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//��ʼ��IPM
//INSTRU_POWER_EN(PC3)

void Power_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ����Ӧʱ��
	 
	//INSTRU_POWER_EN(PC3)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		//�����ٶ�����GPIO�ź������ص���������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 				//��ʼ�� INSTRU_POWER_EN(PC3)
	
	//��λ
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);						//INSTRU_POWER_EN(PC3) 1:off; 0:on.
}

void Power_Off(void)
{
	//ǰ������
	
	
	
	
	
	//�ػ�
	GPIO_SetBits(GPIOC,GPIO_Pin_3);							//�ػ�
}


/*
***********************************************************************************
*	�� �� ��: Get_Battery( )
*	����˵��: ����ص���
*	��    �Σ�
*	�� �� ֵ: ��
***********************************************************************************
*/
float Get_Battery(void)            //����ص���
{
	int val = 0;
	
	val = Get_Adc_Average(6,5);		//adc1ͨ��6����5��ȡƽ��ֵ
	
  return ((float)val*(3.3/4096)/30*130);
}

//���Ժ���
//void Get_Battery_test(void)
//{
//	OS_ERR err;
//	CPU_SR_ALLOC();
//	
//	while(1)
//	{
//		float i;
//		i = Get_Battery();
//		printf("��Դ��ѹֵ��%.2f V.    ������%.1f %%\r\n",i,(100-(12-i)*30));
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//	}
//}
