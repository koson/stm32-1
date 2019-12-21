#include "power.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//初始化IPM
//INSTRU_POWER_EN(PC3)

void Power_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能相应时钟
	 
	//INSTRU_POWER_EN(PC3)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;				//端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		//推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;		//降低速度抑制GPIO信号跳变沿的振铃现象
	GPIO_Init(GPIOC, &GPIO_InitStructure);	 				//初始化 INSTRU_POWER_EN(PC3)
	
	//复位
	GPIO_ResetBits(GPIOC,GPIO_Pin_3);						//INSTRU_POWER_EN(PC3) 1:off; 0:on.
}

void Power_Off(void)
{
	//前置条件
	
	
	
	
	
	//关机
	GPIO_SetBits(GPIOC,GPIO_Pin_3);							//关机
}


/*
***********************************************************************************
*	函 数 名: Get_AD( )
*	功能说明: 监测电池电量
*	形    参：
*	返 回 值: 无
***********************************************************************************
*/
//int Get_AD(void)            //监测电池电量
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
