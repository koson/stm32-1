#include "power.h"

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
*	�� �� ��: Get_AD( )
*	����˵��: ����ص���
*	��    �Σ�
*	�� �� ֵ: ��
***********************************************************************************
*/
//int Get_AD(void)            //����ص���
//{
//    int i;
//	uint16_t j;
//    int val=0;
//	
//	

//    for(i=0; i<8; i++)//convert 8 times
//    {
//        j=get_ADC1_val();
//        val=val+j;
//    }
//    val=val>>3;//get average
//    
//    //myprintf("## ad_val = %d\r\n", val);
//	
//    return val;
//}
