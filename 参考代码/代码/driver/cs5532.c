#include <linux/config.h>
#include <linux/utsname.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/malloc.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/mach/irq.h>
#include <asm/hardware.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <asm/system.h>

#include <linux/poll.h>
#include <asm/arch/io.h>
#include "44b.h"




#define clrclk     rPDATC = rPDATC & (~(0x1<<10)) 	
#define setclk     rPDATC = rPDATC | (0x1<<10)

#define clrout     rPDATC = rPDATC & (~(0x1<<11))
#define setout     rPDATC = rPDATC | (0x1<<11)

#define clrcs     rPDATC = rPDATC & (~(0x1<<14))
#define setcs     rPDATC = rPDATC | (0x1<<14)

#define readin    rPDATG & (0x04)

//#define clrtest1    rPDATG = rPDATG & (~(0x1<<4))
//#define settest1    rPDATG = rPDATG | (0x1<<4)
//#define clrtest    rPDATG = rPDATG & (~(0x1<<5))
//#define settest    rPDATG = rPDATG | (0x1<<5)
#define SET_IOG_INT rPCONG =(rPCONG&0xFFCF)|(0x0030)       //设置GPIO_G2为中断口
#define SET_IOG_IN rPCONG =(rPCONG&0xFFCF)                //设置GPIO_G2为输入口

unsigned char flag_alternative;
unsigned char plus;
unsigned char t=0,tx=0;
unsigned char t1=0;
unsigned char t2=0;
unsigned char flag;
	int count;
unsigned char flag1,flag2;
unsigned char data1[40],data2[40];
unsigned int  init_time;

unsigned char  uart2[2];
/***************************************************************************	
延时
***************************************************************************/

void short_Delay(void)
{ 
	int i;
  	for(i=0;i<1;i++)
  	{
  	}
}

void delay_uart2(void)
{
int i;
for(i=0;i<1000;i++);
}
 
/***************************************************************************	
	
SPI写函数
***************************************************************************/
void spi_write(unsigned char out_byte)
{


	unsigned char i,j;
	setcs;
	clrclk;
	short_Delay();
	clrcs;
	for (i=0;i<8;i++)
	  	{ 
	        clrclk;           
	  	if (0x80 == (out_byte & 0x80))  
	      	setout;
	      	else
	      	clrout;
	        for(j=0;j<10;j++);
	      	setclk;
	        for(j=0;j<8;j++);
	      	out_byte = out_byte<<1 ;
	}
	clrclk;
	for(j=0;j<5;j++);
	setcs;
	clrout;
	clrcs;
}
 
	
/***************************************************************************	
SPI读函数
***************************************************************************/
unsigned char spi_read (void)    
{
  	unsigned char i,j,in_byte=0;
	for(i=0;i<8;i++)
	    {
        	in_byte = in_byte<<1;
	        for(j=0;j<4;j++);
                setclk;
	        for(j=0;j<2;j++);
	   	if ( (readin)==0x04)
	    	in_byte = (in_byte | (0x01));
	    	if ((readin)==0x00)
		in_byte=(in_byte & (0xfe));
	        for(j=0;j<2;j++);
		clrclk;
	    }
	return(in_byte); 
}
 


	
/***************************************************************************	
关闭设备	
***************************************************************************/
static int cs5532_Close(struct inode * inode, struct file * file)
{

	printk("CS5532 Close successful\n");
	return 0;
}


/***************************************************************************	
打开设备	
***************************************************************************/
static int cs5532_Open(struct inode * inode, struct file * file)
{
 	printk("CS5532 Open successful\n");	
	return 0;
}

 
	
	
/***************************************************************************	
初始化cs5532采样率
***************************************************************************/
void cs5532_init(char p)
{
	unsigned char i,data0=0;
	SET_IOG_IN;
	for(i=0;i<50;i++);
	for(i=0;i<15;i++)
	spi_write(0xFF);
	spi_write(0xFE);
	spi_write(0x03);
	spi_write(0x20);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
	
	
	while(data0!=0x10)
	{
	spi_write(0x0B);
	clrcs;
	data0=spi_read();
	spi_read();
	spi_read();
	spi_read();
	setcs;
	}
	
	spi_write(0x03);
	spi_write(0x02);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
	
	
	switch(p)
	{
	case '0':					//初始化cs5532采样率为7.5HZ
	spi_write(0x05);
	spi_write((0x02)|plus);
	spi_write(0x00);
	spi_write((0x02)|plus);
	spi_write(0x00);break;
	
	case '1':					//初始化cs5532采样率为15HZ
	spi_write(0x05);
	spi_write(0x01);
	spi_write(0x80);
	spi_write(0x01);
	spi_write(0x80);break;
	case '2':					//初始化cs5532采样率为30HZ
	spi_write(0x05);
	spi_write(0x01);
	spi_write(0x00);
	spi_write(0x01);
	spi_write(0x00);break;
	case '3':					//初始化cs5532采样率为60HZ
	spi_write(0x05);
	spi_write(0x00);
	spi_write(0x80);
	spi_write(0x00);
	spi_write(0x80);break;
	case '4':					//初始化cs5532采样率为120HZ
	spi_write(0x05);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);
	spi_write(0x00);break;
	case '5':					//初始化cs5532采样率为240HZ
	spi_write(0x05);
	spi_write(0x06);
	spi_write(0x00);
	spi_write(0x06);
	spi_write(0x00);break;
	case '6':					//初始化cs5532采样率为480HZ
	spi_write(0x05);
	spi_write(0x05);
	spi_write(0x80);
	spi_write(0x05);
	spi_write(0x80);break;
	
	case '7':					//初始化cs5532采样率为960HZ
	spi_write(0x05);
	spi_write((0x05)|plus);
	spi_write(0x00);
	spi_write((0x05)|plus);
	spi_write(0x00);break;
	case '8':					//初始化cs5532采样率为1920HZ
	spi_write(0x05);
	spi_write(0x04);
	spi_write(0x80);
	spi_write(0x04);
	spi_write(0x80);break;
	case '9':					//初始化cs5532采样率为3840HZ
	spi_write(0x05);
	spi_write(0x04);
	spi_write(0x00);
	spi_write(0x04);
	spi_write(0x00);break;
	
	default:
	spi_write(0x05);
	spi_write(0x04);
	spi_write(0x00);
	spi_write(0x04);
	spi_write(0x00);break;
	
	
	}
	
	spi_write(0xC0);
	SET_IOG_INT;
	}




/***************************************************************************	
读取AD数据
***************************************************************************/
void read_ad(void)					
{
	
	SET_IOG_IN;
	

	spi_write(0x00);
	
	if ((flag1==0)&&(flag==0))
	{
		data1[t1]=spi_read();
		data1[t1+1]=spi_read();
		data1[t1+2]=spi_read();
		data1[t1+3]=flag_alternative;
		//printk("data1=%x%x%x\n",data1[t1],data1[t1+1],data1[t1+2]);		
		


		spi_read();
		t1=t1+4;
		if (t1>=40)
			{t1=0;
			flag1=1;
			flag=1;}
	}
	else if ((flag2==0)&&(flag==1))
	{ 
		data2[t2]=spi_read();
		data2[t2+1]=spi_read();
		data2[t2+2]=spi_read();
		data2[t2+3]=flag_alternative;
		spi_read();
		//printk("data2=%x%x%x\n",data2[t2],data2[t2+1],data2[t2+2]);	
		t2=t2+4;
		if (t2>=40)
		{
			t2=0;
			flag2=1;
			flag=0;
		}
	}

	else 
	{
		printk(">>");
		spi_read();
		spi_read();
		spi_read();
		spi_read();
	}


	setcs;
	setcs;
	clrcs;
}




/***************************************************************************	
	
***************************************************************************/

static int cs5532_Ioctl(struct inode *inode,struct file *file, 
                          unsigned int cmd, char arg)//unsigned long arg
{
	if (cmd==0)
	{
	switch(arg)
	{
		case '1': init_time=0x1E60;
			break;
		case '2': init_time=0x39A0;
			break;
		case '3': init_time=0x5600;	
			break;
		case '4': init_time=0x7300;
			break;
		case '5': init_time=0x9000;
			break;
		case '6': init_time=0xAC00;
			break;
		case '7': init_time=0xC900;
			break;
		case '8': init_time=0xE600;	
			break;
		default:  init_time=0x1E60;
			break;
	
	}
	}
	if (cmd==1)
	{
	switch(arg)
	{
		case '1': plus=0x00;
			break;
		case '2': plus=0x08;
			break;
		case '3': plus=0x10;	
			break;
		case '4': plus=0x18;
			break;
		case '5': plus=0x20;
			break;
		case '6': plus=0x28;
			break;
		case '7': plus=0x30;
			break;
		default:  plus=0x00;
			break;
	
	}
	}




	return 0;
}


/***************************************************************************	
外部中断2子程序	
***************************************************************************/	
static void cs5532_ISR(int irq,void* dev_id,struct pt_regs *regs)
{
	rI_ISPC=BIT_EINT2;
	if ((flag_alternative==1)||(flag_alternative==4))
	count++;
	read_ad();
	SET_IOG_INT;
	if (count==384)
	
	{count=0;
	
	cs5532_init('0');
	flag_alternative++;	
	}

//	tx++;				//频率测试信号
//	if(tx==1)
//	settest;
//	if (tx==2)
//	{clrtest;tx=0;}
}






void uartputc(unsigned char data1)
{
	
	rUTXH1 = data1;
	while(!(rUTRSTAT1 & 0x2));
	
}


/***************************************************************

发送字符串 

****************************************************************/


void send_str1(unsigned char *str)
{	


	
	unsigned int i=0;
	
	while(1)
	{	
		if ((*(str+i))=='\0') break;
		uartputc(*(str+i));
		i++;
	}

}




void Setup_Uart21(void)
{
	
	int i;
  	//UART0 配置各控制寄存器
 	rUFCON1 = 0x0;	        // 禁用 FIFO
   	rUMCON1 = 0x0;
        rULCON1 = 0x3;          //正常模式,无奇偶校验,一个停止位,8个数据位
	rUCON1  = 0x345;
	//RX电平触发,TX电平触发,禁用延时中断,使用RX 错误中断,正常操作模式,中断请求或表决模式
        rUBRDIV1=0x40;	
        for(i=0;i<100;i++);
}

unsigned char getc(void)
{
		if(rUTRSTAT1 & 0x1)    						                                           //接收数据就绪
	    	return rURXH1;
		else
	    	return 0;
}



/***************************************************************************	

***************************************************************************/	
static int cs5532_read(struct file *ff,unsigned char *command,int count,loff_t *l)
{
	int i;
	if (count==1)				//初始化参数 开计时\外部中断 
	{
	
	flag1=0;
	flag2=0;
	flag=0;
	flag_alternative=0;
	uart2[0]=0;
	uart2[1]=0;
	Setup_Uart21();
	//interrupt1();
	//read_ad();
	
	cs5532_init('0');	
	//spi_write(0x00);spi_read();spi_read();spi_read();spi_read();
			//初始化cs5532采样频率为7.5HZ
	enable_irq(23);
	enable_irq(13);
	send_str1("StartAB\n");
	rTCNTB0=init_time;
	rTCMPB0=0x0;
	rTCON&=0xFFFFFFF0;			//手动更新计数初值
	rTCON|=0x02;         
	rTCON&=0xFFFFFFF0;			//开始计时 
	rTCON|=0x09;
	spi_write(0x00);spi_read();spi_read();spi_read();spi_read();
	t1=0;
	t2=0;	
	return 0;
	}
	
	
	if (count==2)
	{
		disable_irq(23);			//关中断停止计时 
		disable_irq(13);
		cs5532_init('0');	
		//for(i=0;i<15;i++)
		//spi_write(0xFF);
		//spi_write(0xFE);
		//spi_write(0x03);
		//spi_write(0x20);
		//spi_write(0x00);
		//spi_write(0x00);
		//spi_write(0x00);
		//SET_IOG_IN;
		rTCON&=0xFFFFFFF0;


     /***************************************		
	串口获得电流 
***************************************/
	
	send_str1("StopAB!\n");
	delay_uart2();
	send_str1("StartAB\n");
	//delay_uart2();
	//uart2_int1=1;uart2_int2=0;
	//send_str1("Getcrnt\n");
	//while(1)
	//{
	//if(uart2_int1==0)break;
	//}
	i=0;
	while(i<20)
	{i++;
	delay_uart2();delay_uart2();delay_uart2();
	send_str1("Getcrnt\n");
	uart2[0]=getc();
	if (uart2[0]!=0) break;

		}
	printk("uart2[0]=getc();=%x\n",uart2[0]);
	
	send_str1("StopAB!\n");
	delay_uart2();delay_uart2();delay_uart2();//delay_uart2();delay_uart2();delay_uart2();
	send_str1("StartBA\n");
	//uart2_int2=1;
	

	i=0;
	while(i<20)
	{i++;
	delay_uart2();delay_uart2();delay_uart2();
	send_str1("Getcrnt\n");
	uart2[1]=getc();
	if (uart2[1]!=0) break;

		}
	printk("uart2[1]=getc();=%x\n",uart2[1]);
	

	
	
	
	send_str1("StopAB!\n");
	copy_to_user(command,uart2,count);
	

	

		printk("disable-irq(13 23)\n");
		return 0;
	}
		
	while(1)					//开始数据采集
	{
	 	if (flag1==1)
		{	
			copy_to_user(command,data1,count);
			flag1=0;
			return 0;
		}
		 if (flag2==1)
		{
			copy_to_user(command,data2,count);
			flag2=0;
			return 0;	
		}
	
	}
	
}

/***************************************************************************	
	
***************************************************************************/	
struct file_operations cs5532_fops = 
{
	open:	  cs5532_Open, 
	//write:     cs5532_write,
	ioctl:	  cs5532_Ioctl,
	read:	 cs5532_read,
	release:  cs5532_Close, 
};
	

/***************************************************************************	
定时计数器0中断子程序
***************************************************************************/	

static void cs5532_ISR_time(int irq,void* dev_id,struct pt_regs *regs)
{
	int i;
	flag_alternative++;
	rI_ISPC=BIT_TIMER0;
//	t++;				//频率测试信号 
//	if(t==1)
//	settest1;
//	if (t==2)
//	{clrtest1;t=0;}
	
	if ((flag_alternative==1)||(flag_alternative==4))
	{	
		cs5532_init('7');
		count=0;
		for (i=0;i<150;i++)
		delay_uart2();
		
		
	}
		
	//else
	//{
	//	cs5532_init('0');
	//}
	if (flag_alternative==6)
		flag_alternative=0;
	
	printk("count = %d\n",count);
	switch(flag_alternative)
	{
	case 0:send_str1("StartAB\n");break;
	case 1:send_str1("StopAB!\n");break;
	case 2:break;
	case 3:send_str1("StartBA\n");break;
	case 4:send_str1("StopAB!\n");break;
	case 5:break;
	default:send_str1("StopAB!\n");break;

	}
	
	
}


/***************************************************************************	
硬件初始化
注册设备\中断	
***************************************************************************/

int cs5532_Init(void)
{
   	int     result;
	printk("Registering cs5532 Device\t--- >\t");
	result = register_chrdev(125, "cs5532", &cs5532_fops);
	if (result<0)
	{
		printk(KERN_INFO"[FALLED: Cannot register cs5532_driver!]\n");
		return result;
	}
	else
		printk("[OK]\n");
	printk("Initializing cs5532 Device\t--- >\t");
	rPCONC=(rPCONC&0xCF0FFFFF)|(0x10500000);
	//SET_IOG_INT;
//	rPCONG =(rPCONG&0xF0FF) |0x0500;		//测试信号PG4\PG5配置为输出
	rEXTINT|=0x30;
 	rTCFG0&=0xFFFFFF00;
	rTCFG0 |=0xFF;
	rTCFG1&=0xFFFFFFF0;
	rTCFG1|=0x4;
	//rTCNTB0=0xf400;
	rTCMPB0=0x0;
	//rTCON&=0xFFFFFFF0;
	//rTCON|=0x02;

	printk("cs5532 Driver Installed.\n");
        if (request_irq(23,cs5532_ISR,0,"cs5532",NULL))
        {
		printk(KERN_INFO"[FALLED:cannot register cs5532_Interrupt!]\n");
	        return -EBUSY;
	}
        else 
	        printk("[OK]1\n");
	


        if (request_irq(13,cs5532_ISR_time,0,"cs5532",NULL))
        {
		printk(KERN_INFO"[FALLED:cannot register cs5532_Interrupt!]\n");
	        return -EBUSY;
	}
        else 
	        printk("[OK]3\n");
       





	//disable_irq(23);
	//disable_irq(13);
	//disable_irq(6);
	return 0;
}
	
	
