#ifndef _CS5532_H_ 
#define _CS5532_H_ 
#include "sys.h"

#define CS5532_CS		PBout(12)	//PB12
#define CS5532_SDI	PCout(7)	//PC7
#define CS5532_SDO	PCout(6)	//PC6
#define CS5532_SCLK	PGout(8)	//PG8

void CS5532_Init(void);
void SPI_Write(u8 dat);
unsigned char SPI_Read(void);
void Read_Ad(void);

#endif
