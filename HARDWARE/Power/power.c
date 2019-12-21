#include "power.h"
#include "adc.h"

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
*	函 数 名: Get_Battery( )
*	功能说明: 监测电池电量
*	形    参：
*	返 回 值: 无
***********************************************************************************
*/
float Get_Battery(void)            //监测电池电量
{
	int val = 0;
	
	val = Get_Adc_Average(6,5);		//adc1通道6采样5次取平均值
	
  return ((float)val*(3.3/4096)/30*130);
}

//测试函数
//void Get_Battery_test(void)
//{
//	OS_ERR err;
//	CPU_SR_ALLOC();
//	
//	while(1)
//	{
//		float i;
//		i = Get_Battery();
//		printf("电源电压值：%.2f V.    电量：%.1f %%\r\n",i,(100-(12-i)*30));
//		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err);
//	}
//}
