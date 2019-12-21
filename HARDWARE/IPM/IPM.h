#ifndef __IPM_H
#define __IPM_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//IPM端口定义
//IPM_POWER_EN（PA1）
//IPM_SVP（PE2）、IPM_SUP（PC13）、IPM_SVN（PC0）、IPM_SUN（PF9）
#define IPM_POWER_EN 	PAout(1)
#define IPM_SVP 		PEout(2)
#define IPM_SUP			PCout(13)
#define IPM_SVN 		PCout(0)
#define IPM_SUN			PFout(9)

// 控制管脚宏定义
#define		IPM_SUP_CLK		RCC_APB2Periph_GPIOC	//	PC13
#define  	IPM_SVP_CLK        	RCC_APB2Periph_GPIOE	//	PE2
#define  	IPM_SUN_CLK        	RCC_APB2Periph_GPIOF	//	PF9
#define  	IPM_SVN_CLK        	RCC_APB2Periph_GPIOC	//	PC0

#define  	IPM_SUP_PORT       	GPIOC
#define  	IPM_SVP_PORT       	GPIOE
#define  	IPM_SUN_PORT       	GPIOF
#define  	IPM_SVN_PORT       	GPIOC

#define  	IPM_SUP_IO         	GPIO_Pin_13
#define  	IPM_SVP_IO         	GPIO_Pin_2
#define  	IPM_SUN_IO         	GPIO_Pin_9
#define  	IPM_SVN_IO         	GPIO_Pin_0

void IPM_Init(void);	//初始化
void IPM_Transmit(void);	//发射
void IPM_Start_AB(void);			// AB方向供电
void IPM_Start_BA(void);			// BA方向供电
void IPM_Stop_AB(void);			// 停止供电

#endif

