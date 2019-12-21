#include "stm32f10x.h"
#include <stdio.h>

#include "cs5532.h"
#include "delay.h"
#include "acquisiton.h"

void CS5532_Init(void) 
{	
	int i;
	u32 temp,reg1,reg2,reg3,reg0;
	
	AD_POWER_EN= 1;
	delay_ms(1);
	CS5532_CS = 1;
	
	delay_ms(2);
	CS5532_SCLK = 0;
	CS5532_CS = 0;
	delay_ms(2);
	
	CS5532_SDI = 0;
	CS5532_SDO = 1;
  delay_ms(2); 
    
    //进入命令模式
    for(i=0;i < 31;i++)	SPI_Write(0xff);
	SPI_Write(0xfe);
	delay_ms(2);
	//RS=1复位 (写配置寄存器) 
//  SPI_Write(0x03);
//  SPI_Write(0x20);
//	SPI_Write(0x00);
//	SPI_Write(0x00);
//	SPI_Write(0x00);

	SPI_Write(0x03);    // Soft Reset 
   SPI_Write(0x20);
   SPI_Write(0x00);
   SPI_Write(0x0);
   SPI_Write(0x00);
	delay_ms(20);
	
	//SPI_Write(0x0B);
	  reg0=SPI_Read();
  reg1=SPI_Read();

   reg2=SPI_Read();

   reg3=SPI_Read();

   printf("%d  %d  %d  %d",reg0,reg1,reg2,reg3);

printf("完成初始化\r\n");
	
//	//CONFIGURATION REG,设置参考电压 2.5V<VREF<5.0V 所以VRS=0
//	SPI_Write(0x03);
//	SPI_Write(0x00);
//	SPI_Write(0x08);   // 第4位置1，FRS=1, 5/6 * word rates
//	SPI_Write(0x00);
//	SPI_Write(0x00); 
//	
//	SPI_Write(0x05);
//	SPI_Write(0x06);
//	SPI_Write(0x00);
//	SPI_Write(0x06);
//	SPI_Write(0x00);

   SPI_Write(0x05);    // Setup REG 
   SPI_Write(0x32);
   SPI_Write(0x34);
   SPI_Write(0x00);
   SPI_Write(0x00);
  
   SPI_Write(0x03);    // Config ReG 
   SPI_Write(0x40);
   SPI_Write(0x00);
   SPI_Write(0x00);
   SPI_Write(0x00);

   SPI_Write(0x03);    // Config ReG 
   SPI_Write(0xC0);
   SPI_Write(0x00);
   SPI_Write(0x00);
   SPI_Write(0x00);

   SPI_Write(0x03);    // Config ReG 
   SPI_Write(0x01);
   SPI_Write(0x80);
   SPI_Write(0x00);
   SPI_Write(0x00);
	
	SPI_Write(0xC0);	//连续转换命令	
	delay_ms(1);
	
	Read_Ad();
}

void SPI_Write(u8 dat)//向CS5532写一字节数据
{
	int i;  
  CS5532_CS=0;
	if((dat<<0)&0x80) 	CS5532_SDI=1;   //数据送到SPI的SDI口
	else  CS5532_SDI=0;
	delay_us(1);
	
	for(i=1;i<8;i++)
  {
		CS5532_SCLK=1;    //时钟高   
		delay_us(1);
		
		if((dat<<i)&0x80) 	CS5532_SDI=1;   //数据送到SPI的SDI口
		else  CS5532_SDI=0;
		 
		CS5532_SCLK=0;    //时钟低
		delay_us(1);
		
   }
	CS5532_SCLK=1;    //时钟高   
	delay_us(1);
	CS5532_SCLK=0;    //时钟低
	delay_us(1);
		 
	CS5532_CS = 1;
	 delay_us(1);
}

unsigned char SPI_Read(void) 
{
	int i,j;
	u8 temp;
	
	CS5532_CS = 0;
	delay_us(1);
	for(i = 0; i < 8; i++) 
	{
		CS5532_SCLK = 1; //时钟高    
		temp <<= 1;
		if (CS5532_SDO)
			temp |= 0x01;//从SDO读取数据
		else
			temp &= 0xFE;		
		delay_us(1);
		CS5532_SCLK = 0; //时钟低 
		delay_us(1);
	}
	CS5532_CS = 1;
	delay_us(1);
	return temp;
}

void Read_Ad(void)
{
	u8 temp_byte_1,temp_byte_2,temp_byte_3;
	SPI_Write(0x00);//conversion data REG
	
	temp_byte_1 = SPI_Read();      // 高 8 bit
	temp_byte_2 = SPI_Read();
	temp_byte_3 = SPI_Read();
	SPI_Read();//discard the last byte
	
	printf("%d    %d    %d\r\n",temp_byte_1,temp_byte_2,temp_byte_3);
}

















