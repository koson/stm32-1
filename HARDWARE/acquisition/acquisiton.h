#ifndef __ACQUISITION_H
#define __ACQUISITION_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////

//acquisition�˿ڶ���
//AD_POWER_EN��PC2��	�ɼ����Դ����
//ACQ_SWITCH(PB9)			ѡ�����MN���ǵ���
//TRAP_50Hz_EN(PB8)		ѡ���Ƿ����50Hz�ݲ�
//SPI_CS(PB12)��SPI_SDI(PC7)��SPI_SDO(PC6)��SPI_SCLK(PG8)	ģ��ת��
#define AD_POWER_EN		PCout(2)	
#define ACQ_SWITCH		PBout(9)
#define TRAP_50Hz_EN	PBout(8)
#define SPI_CS			PBout(12)
#define SPI_SDI			PCout(7)
#define SPI_SDO			PCout(6)
#define SPI_SCLK		PGout(8)

void Acquisition_Init(void);			//��ʼ��
void Acquisition_Work(void);		//�ػ�

#endif
