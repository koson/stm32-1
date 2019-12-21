#ifndef __ACQUISITION_H
#define __ACQUISITION_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//acquisition端口定义
//AD_POWER_EN（PC2）	采集板电源开关
//ACQ_SWITCH(PB9)			选择测试MN或是电流
//TRAP_50Hz_EN(PB8)		选择是否进行50Hz陷波
//SPI_CS(PB12)、SPI_SDI(PC7)、SPI_SDO(PC6)、SPI_SCLK(PG8)	模数转换
#define AD_POWER_EN		PCout(2)	
#define ACQ_SWITCH		PBout(9)
#define TRAP_50Hz_EN	PBout(8)
#define SPI_CS			PBout(12)
#define SPI_SDI			PCout(7)
#define SPI_SDO			PCout(6)
#define SPI_SCLK		PGout(8)

void Acquisition_Init(void);			//初始化
void Acquisition_Work(void);		//关机

#endif
