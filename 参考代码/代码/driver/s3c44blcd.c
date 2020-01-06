
#include <linux/config.h>
#include <linux/utsname.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fcntl.h>
#include <linux/malloc.h>
#include <linux/timer.h>
#include <linux/init.h>
#include <asm/proc/pgtable.h>
#include <asm/io.h>

#include "s3c44blcd.h"
#include "44b.h"

#define 	M5D(n) 		((n) & 0x1fffff)

#define 	SCR_XSIZE 	(320)  
#define 	SCR_YSIZE 	(240)

#define 	LCD_XSIZE 	(320)
#define 	LCD_YSIZE 	(240)

#define 	HOZVAL_COLOR	(LCD_XSIZE/4-1)
#define 	LINEVAL	(LCD_YSIZE-1)
#define 	MVAL			(13)
#define 	CLKVAL_COLOR 	(12) 	//60Mhz 
#define 	MVAL_USED 		(1)

#define 	BYTESPERLINE 	(LCD_XSIZE)
//#define 	XY2OFF(x,y) 	(tOff)((tOff)y*(tOff)BYTESPERLINE + (x))
#define 	XY2OFF(x,y) 	(tOff)((tOff)y*160 + (x/2+1))

typedef unsigned long 	tOff;

#define     READ_MEM(Off, Data)    Data = LCD_READ_MEM(Off)
#define     WRITE_MEM(Off, Data)   LCD_WRITE_MEM(Off, Data)

#define  SETPIXEL(x, y, c)  SetPixel(x, y, c)
#define  GETPIXEL(x, y)     GetPixelIndex(x, y)

U8 	LCD_COLOR = 0x0f;
U8	LCD_BKCOLOR = 0x00;

unsigned char* Video_StartBuffer;
	U8 Key_Value;
	U8 Key_f;
#define LCD_MAJOR 	60 
/**********************************************
- 函数名称 : void Delay(unsigned int t)
- 函数说明 : 延时一段时间
- 输入参数 : unsigned int t
- 输出参数 : 0
********************************************/
void Delay(unsigned int t)
{
for(;t>0;t--);
}
/*********************************************
- 函数名称 : void CloseLCD(struct inode * inode, struct file * file)
- 函数说明 : LCD关闭
- 输入参数 : struct inode * inode, struct file * file
- 输出参数 : 0
*********************************************/
static void CloseLCD(struct inode * inode, struct file * file)
{
	//printk("LCD is closed\n");
	return ;
}
/*
*************************************************************************************************************
- 函数名称 : static int OpenLCD(struct inode * inode, struct file * file)
- 函数说明 : LCD打开
- 输入参数 : struct inode * inode, struct file * file
- 输出参数 : 0
*************************************************************************************************************
*/
static int OpenLCD(struct inode * inode, struct file * file)
{
	//printk("LCD is open\n");
	return 0;
}
/*
*************************************************************************************************************
- 函数名称 : static int LCDIoctl(struct inode *inode,struct file * file,unsigned int cmd,unsigned long arg)
- 函数说明 : LCD控制输出
- 输入参数 : 
- 输出参数 : 0
*************************************************************************************************************
*/
static int LCDIoctl(struct inode *inode,struct file * file,unsigned int cmd,unsigned long arg)
{
  char color;
	struct para 
	{
	 unsigned long a;
	 unsigned long b;
	 unsigned long c;
	 unsigned long d;
	} *p_arg;
  struct RTC
       { 
        U8 	year;
        U8 	month;
	U8	day;
	U8	date;
	U8	hour;
	U8	minus;
	U8	second;
       } *p_RTC;
  struct RTC current_RTC;
  U16 x,y,r;
  U16 * p; 
       switch(cmd) 
         {
		case 0:
                       	//printk("set front color\n");	
			Set_Color(arg); 
	                //printk("LCD_COLOR =%x\n",LCD_COLOR);
			return 1;

		case 1:
                       	//printk("set back colour!\n");	
			Set_BkColor(arg); 
	                //printk("BACK_COLOR =%x\n",LCD_COLOR);
			return 1;

		case 2:
			//printk("draw h_line\n"); 
 			p_arg =(struct para *)arg;
			LCD_DrawHLine(p_arg->a,p_arg->b,p_arg->c);		// draw h_line
			return 1;
			
	        case 3:
	        	//printk("drwa point\n");
			p = (U16*)arg;
			x = (U16)(*(p++));
			y = (U16)(*(p++));
			r = (U16)(*(p++));
			//printk("@@x=%d,y=%d,r=%d",x,y,r);
	                LCD_DrawPixel(x,y);		// draw circle
			return 1;
			
	 	case 4 :
	        	//printk("drwa circle\n");
			p = (U16*)arg;
			x = (U16)(*(p++));
			y = (U16)(*(p++));
			r = (U16)(*(p++));
			//printk("x=%d,y=%d,r=%d",x,y,r);
	                LCD_DrawCircle(x,y,r);		// draw circle
	                return 1;
	       
		case 5:
			//printk("draw rect\n");
			p_arg =(struct para *)arg;
			LCD_FillRect(p_arg->a,p_arg->b,p_arg->c,p_arg->d);      // draw rect
			return 1;

	 	case 6 : //清屏
	      	        printk("LCD is clear\n");
 			LCD_Clear(0,0,319,239);     // clear screen
	                return 1;

 	        case 7:  //关闭液晶屏
			//printk("Set Lcd OFF!\n");
			rPDATE = rPDATE |0x1;
			return 1;

 	        case 8:  //打开液晶屏
			//printk("Set Lcd ON!\n");
			enable_irq(24);
			rPDATE = rPDATE & 0x1fe;
			return 1;

 	        case 9:  //关闭背景光
 	        	//printk("Set LED of Lcd OFF!\n");
			rPDATE = rPDATE | 0x40;
			return 1;

 	        case 10:  //打开背景光
 	        	//printk("Set LED of Lcd ON!\n");
			rPDATE = rPDATE & 0x1bf;
			return 1;

	        case 11:  //液晶屏变亮
 	        	//printk("Set Lcd Bright more! \n"); 
			rPDATE = rPDATE & 0x1df;   //CS清零
                        Delay(200);
			rPDATE = rPDATE & 0x1ef;   //U/D清零
                        Delay(50);
			rPDATE = rPDATE & 0x1f7;   //INC清零
                        Delay(100);
			rPDATE = rPDATE | 0x08;    //INC置1
                        Delay(50);
			rPDATE = rPDATE & 0x1f7;   //INC清零
                        Delay(100);
			rPDATE = rPDATE | 0x08;    //INC置1
                        Delay(50);
			rPDATE = rPDATE | 0x20;   //CS置1,保存结果
			rPDATE = rPDATE | 0x10;   //U/D置1
                        Delay(50);
			return 1;

	        case 12:   //液晶屏变暗
 	        	//printk("Set Lcd Dark more! \n");    
			rPDATE = rPDATE & 0x1df;   //CS清零
                        Delay(200);
			rPDATE = rPDATE | 0x10;   //U/D置1
                        Delay(50);
			rPDATE = rPDATE & 0x1f7;   //INC清零
                        Delay(100);
			rPDATE = rPDATE | 0x08;    //INC置1
                        Delay(50);
			rPDATE = rPDATE & 0x1f7;   //INC清零
                        Delay(100);
			rPDATE = rPDATE | 0x08;    //INC置1
                        Delay(50);
			rPDATE = rPDATE | 0x20;   //CS置1,保存结果
			rPDATE = rPDATE | 0x10;   //U/D置1
                        Delay(50);
			return 1;

	        case 13:	// 显示字符
		        p = (U16*)arg;
			x = (U16)(*(p++));
			y = (U16)(*(p++));
			LCD_Character(x,y,p);	
			return 1;

		case 14:  			
		        /*if(Key_f==1)
			{
			Key_Value = (~((rPDATG&0xf8)>>3))&0x1f;// 扫描键盘
			Bee(1,200,1);
			if(Key_Value==0)
			{Key_f==0;}
			else;
			return Key_Value;
			}
			else 
			  return 0;
			*/

			Key_Value = (~((rPDATG&0xf8)>>3))&0x1f;// 扫描键盘
			
			if(Key_Value!=0)
			{
			Bee(50,0,1);
			}
			return Key_Value;	
		
	        case 15: Delay(1000);//调用RTC
			p_RTC->year 	= rBCDYEAR;
			p_RTC->month 	= rBCDMON;
			p_RTC->day 	= rBCDDAY;
			p_RTC->date 	= rBCDDATE;
			p_RTC->hour 	= rBCDHOUR;
			p_RTC->minus 	= rBCDMIN;
			p_RTC->second 	= rBCDSEC;
			return (int *)p_RTC;

                default:
         	        return -EINVAL;
             }
             return 1;
}

/*
*************************************************************************************************************
- 函数名称 : struct file_operations LCD_fops
- 函数说明 : 文件结构
- 输入参数 : 无
- 输出参数 : 无
*************************************************************************************************************
*/
static struct file_operations LCD_fops = 
{
	ioctl:	   LCDIoctl,         /* ioctl */
	open:	   OpenLCD,    	    /* just a selector for the real open */
	release:     CloseLCD,        /* release */
        //read:	   Key_Read,        /*读取设备文件*/
};
static void Bee(U8 on, U8 off, U8 times)
{ 
rPDATB |= 0x200; //配置LED2端为高,亮
 for(;times>0;times--)
   {
    rPDATF &= 0x1ef; //配置蜂鸣器端为低,响  
    Delay(on*1000);
    rPDATF |= 0x10; //配置蜂鸣器端为高,不响
    Delay(off*1000);
   }
rPDATB &= 0x5ff; //配置LED2端为低,不亮
}
/***************************************************************************
- 函数名称 : 键盘中断1的服务程序static void Key_ISR(int irq,void* dev_id,struct pt_regs * regs)
- 函数说明 : 文件结构
- 输入参数 : 无
- 输出参数 : 无
***************************************************************************/
static void Key_ISR(int irq,void* dev_id,struct pt_regs * regs)
{
	
        disable_irq(24);
	rEXTINPND &= 0xffffff; //清除中断1的挂起位
	Key_Value = (~((rPDATG&0xf8)>>3))&0x1f;
	Bee(5,2,50);
	Key_f=1;
        //printk("Get a key!Key_Value=%d\n",Key_Value);
        rINTPND = (0x1<<24);
	rI_ISPC = (0x1<<24);
	//if(LCD_COLOR != 0x0f)
	//{enable_irq(24);}
};


/*
*************************************************************************************************************
- 函数名称 : int LCDInit(void)
- 函数说明 : 注册LCD设备
- 输入参数 : 无
- 输出参数 : 0,或-EBUSY
*************************************************************************************************************
*/
int LCDInit(void)
{
    	int     result;
   	printk("Registering S3C44BLCD Device\t--- >\t");
	result = register_chrdev(LCD_MAJOR, "S3C44BLCD", &LCD_fops);//注册设备
    	if (result<0)
	{
	 printk(KERN_INFO"[FALLED: Cannot register S3C44BLCD_driver!]\n");
	 return -EBUSY;
	}
	else printk("[OK]\n");
	if (request_irq(24,Key_ISR,0,"S3C44BLCD",NULL))
	{
   	 printk(KERN_INFO"[FALLED: Cannot register Key_Interrupt!]\n");
	 return -EBUSY;
	}
	else printk("[OK]\n");		
	printk("Initializing S3C44BLCD Device\t--- >\t");
	Setup_LCDInit();
	printk("S3C44BLCD Driver Installed.\n");
	return 0;
}

/*
*************************************************************************************************************
- 函数名称 : U16  LCD_Init(U8 Lcd_Bpp)  
- 函数说明 : LCD硬件初始化函数
- 输入参数 : 
- 输出参数 : 无
*************************************************************************************************************
*/
U16  Setup_LCDInit(void) 
{
 	rPCONG = 0x3f; //配置键盘和键盘使用的中断1	
        rEXTINT &=0xffffff0f; //中断触发方式:下降沿
        rEXTINT |=0x70;
	disable_irq(24);
        rINTMSK &=(~0x5000000);  //使用中断1
        if (rINTPND & 0x1000000)   rEXTINPND &= 0xffffff; //清除中断1的挂起位
 	rPCONF |= 0x100; //配置蜂鸣器
	rPCONF &= ~(0x200); //配置蜂鸣器
	rPCONB &=0x5ff; //配置LED2端口为输出	
	rPDATF |= 0x10; //配置蜂鸣器端为高,关闭蜂鸣器
	rPUPF &= 0x1ef; //配置蜂鸣器端为上拉
        rRTCCON = 0x0;//配置RTC控制器	
        rPCOND = 0xaaaa;
 	rPUPD = 0x00;
 	rPCONC = rPCONC|0xffff;
 	rPUPC = 0x0000; 
        rPCONE =  0x1d569;  //This lcd control such as lcd back light,lcd on/off etc
        rPDATE = 0x1be;
        rPUPD  = 0x6; 
  	Video_StartBuffer = (unsigned char *)kmalloc(0x20000,GFP_KERNEL);
	rDITHMODE=0x12210;
	rDP1_2 =0xa5a5;      
	rDP4_7 =0xba5da65;
	rDP3_5 =0xa5a5f;
	rDP2_3 =0xd6b;
	rDP5_7 =0xeb7b5ed;
	rDP3_4 =0x7dbe;
	rDP4_5 =0x7ebdf;
	rDP6_7 =0x7fdfbfe;	
	rLCDCON1=(0)|(0<<1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(0x6<<12);
	rLCDCON2=(LINEVAL)|(HOZVAL_COLOR<<10)|(1<<21);
	rLCDSADDR1= (0x2<<27) | ( ((U32)Video_StartBuffer>>22)<<21 )| M5D((U32)Video_StartBuffer>>1);
rLCDSADDR2= M5D((((U32)Video_StartBuffer + (SCR_XSIZE*LCD_YSIZE/2))>>1)) | (MVAL<<21)|(0x1<<29);
	rLCDSADDR3= (LCD_XSIZE/4) | ( ((SCR_XSIZE-LCD_XSIZE)/4)<<9 );
	rLCDCON1=(1)|(1<<1)|(1<<5)|(MVAL_USED<<7)|(0x3<<8)|(0x3<<10)|(CLKVAL_COLOR<<12);
	LCD_Clear(0,0,320,240);
	return 0;    
}

/*
*************************************************************************************************************
- 函数名称 : U16 LCD_READ_MEM(U32 off)
- 函数说明 : 针对硬件的读点函数
- 输入参数 : x,y,c
- 输出参数 : 无
*************************************************************************************************************
*/

U16 LCD_READ_MEM(U32 off)
{
	return (*((U8*)Video_StartBuffer + (off)));
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_WRITE_MEM( U32 off,U8 Data)
- 函数说明 : 针对硬件的画点函数
- 输入参数 : x,y,c
- 输出参数 : 无
*************************************************************************************************************
*/	
void LCD_WRITE_MEM( U32 off,U8 Data)
{	
	
	(* ((U8*)Video_StartBuffer +38400- (off)) ) = Data;
}

/*
*************************************************************************************************************
- 函数名称 : static void SetPixel(U16 x,U16 y,int c)
- 函数说明 : 针对硬件的画点函数
- 输入参数 : x,y,c
- 输出参数 : 无
*************************************************************************************************************
*/
static void SetPixel(U16 x, U16 y, U8 c) 
{
    unsigned char cTemp = 0;
    tOff Off = XY2OFF(x,y);
    /*2007年10月4日添加*/
    cTemp =  (* ((U8*)Video_StartBuffer +38400- (Off)) );
    if(x%2)
    {
	cTemp = cTemp&0x0f;
	cTemp = cTemp|(c<<4);
    }
    else
    {
	cTemp = cTemp&0xf0;
	cTemp = cTemp|c;
    }	
    WRITE_MEM(Off, cTemp);
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_DrawPixel  (U16 x, U16 y) 
- 函数说明 : 画点函数
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_DrawPixel  (U16 x, U16 y) 
{
    	SETPIXEL(x, y, LCD_COLOR);
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_Line_Byte  (U16 x, U16 y,U8 data) 
- 函数说明 : 在指定的位置连续画一个字节的图形,字节中1画点,0跳过
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_LCD_Line_Byte  (U16 x, U16 y,U8 data) 
{
   U8 i;
   for(i=0;i<8;i++)
     {

       SETPIXEL(x,y,0);//清除每个点的原有数据.	
       if((data&0x80)==0x80) 
	{
	SETPIXEL(x, y, LCD_COLOR);	
	}
       x++; 
       data <<= 1;
     }	
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_Character  (U16 x, U16 y,U8 *data) 
- 函数说明 : 在指定的位置显示字符,包括汉字
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_Character  (U16 x, U16 y,U16 *data) 
{
   U8 wedth,height,i,*p2;
   U16 x1=x;
   wedth = *(data++);
   height = *(data++);
   p2 = *(U32*)data;
   for(;height>0;height--)
	{
	 for(i=0;i<wedth;i++)
	  { 
	   LCD_LCD_Line_Byte (x, y,*(p2++)) ;
	   x += 8 ;
	  }
	x=x1;
	y++;	
	//x+=320-wedth*8;
	 }   
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_DrawHLine  (U16 x0, U16 y,  U16 x1) 
- 函数说明 : 画水平线函数
- 输入参数 : x,y,x1
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_DrawHLine  (U16 x0, U16 y0,  U16 x1) 
{
    while (x0 <= x1) 
    {
	SETPIXEL(x0, y0, LCD_COLOR);	
     	x0++;
    }
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_DraLCD_SetColorwVLine  (U16 x, U16 y0,  U16 y1) 
- 函数说明 : 画竖直线函数
- 输入参数 : x,y,x1
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_DrawVLine(U16 x0, U16 y0,  U16 y1) 
{
	while (y0 <= y1) 
 	{			
		SETPIXEL(x0, y0, LCD_COLOR);
		y0++;
 	}
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_FillRect(U16 x0, U16 y0, U16 x1, U16 y1)
- 函数说明 : 填充矩形函数
- 输入参数 : x0,y0,x1,y1
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_FillRect(U16 x0, U16 y0, U16 x1, U16 y1) 
{
	for (; y0 <= y1; y0++) 
	{
		 LCD_DrawHLine(x0,y0, x1);
	}
}
/*
*************************************************************************************************************
- 函数名称 : void LCD_Clear(U16 x0, U16 y0, U16 x1, U16 y1)
- 函数说明 :  清屏函数
- 输入参数 : x0,y0,x1,y1
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_Clear(U16 x0, U16 y0, U16 x1, U16 y1) 
{
	LCD_COLOR = 0xff;
	for (; y0 <= y1; y0++) 
	{
		 LCD_DrawHLine(x0,y0, x1);
	}
}
/*
*************************************************************************************************************
- 函数名称 : static  void DrawPoint(U16 x0,U16 y0, U16 xoff, U16 yoff)
- 函数说明 : 画辅助点函数
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/

static  void _DrawPoint(U32 x0,U32 y0, U32 xoff, U32 yoff) 
{
  	LCD_DrawPixel(x0+xoff,y0+yoff);
  	LCD_DrawPixel(x0-xoff,y0+yoff);
  	LCD_DrawPixel(x0+yoff,y0+xoff);
  	LCD_DrawPixel(x0+yoff,y0-xoff);
  	
  	if (yoff) 
  	{
    	LCD_DrawPixel(x0+xoff,y0-yoff);
    	LCD_DrawPixel(x0-xoff,y0-yoff);
    	LCD_DrawPixel(x0-yoff,y0+xoff);
    	LCD_DrawPixel(x0-yoff,y0-xoff);
  	}
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_DrawCircle(U16 x0, U16 y0, U16 r)
- 函数说明 : 画圆函数
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_DrawCircle(U32 x0, U32 y0, U32 r) 
{
  	U32 i;
  	U32 imax = ((int)((int)r*707))/1000 + 1;
  	U32 sqmax = (int)r*(int)r + (int)r/2;
  	U16 y = r;
  	//printk("##x0=%d,y0=%d,r=%d\n",x0,y0,r);
  	_DrawPoint(x0,y0,r,0);
  	
  	for (i=1; i<= imax; i++) 
  	{
    	if ((i*i+y*y) > sqmax) 
    	{
      		_DrawPoint(x0,y0,i,y);
      		y--;
    	}
    	_DrawPoint(x0,y0,i,y);
  	}
}

/*
*************************************************************************************************************
- 函数名称 : void LCD_FillCircle(U16 x0, U16 y0, U16 r)
- 函数说明 : 填充圆函数
- 输入参数 : x,y
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_FillCircle       (U16 x0, U16 y0, U16 r) 
{
  	U32 i;
  	U32 imax = ((int)((int)r*707))/1000+1;
  	U32 sqmax = (int)r*(int)r+(int)r/2;
  	U16 x = r;
  
  	LCD_DrawHLine(x0-r,y0,x0+r);
  
  	for (i=1; i<= imax; i++) 
  	{
    	if ((i*i+x*x) >sqmax) 
    	{
      		if (x>imax) 
      		{
        		LCD_DrawHLine (x0-i+1,y0+x, x0+i-1);
        		LCD_DrawHLine (x0-i+1,y0-x, x0+i-1);
      		}
      		x--;
    	}
       	LCD_DrawHLine(x0-x,y0+i, x0+x);
    	LCD_DrawHLine(x0-x,y0-i, x0+x);
  	}
}
/*
*************************************************************************************************************
- 函数名称 : Log2Phy(int Color)
- 函数说明 : 逻辑颜色转实际颜色函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/

U16 Log2Phy(U32 Color) 
{
//  	U32 r,g,b;
//  	b = Color & 255;
//  	g = (Color >> 8 ) & 255;
//  	r = Color >> 16;
//  	b = (b + 42) / 85;
//  	g = (g * 7 + 127) / 255;
//  	r = (r * 7 + 127) / 255;
  	//return b + (g << 2) + (r << 5);
        return (U16)Color;             /*只有灰度,没有颜色,不必转换*/
}
/*
*************************************************************************************************************
- 函数名称 : LCD_Log2Phy(int Color)
- 函数说明 : 逻辑颜色转实际颜色上层函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/

U16 LCD_Log2Phy(U32 Color) 
{
    U16 PhyColor;
  
  	PhyColor = Log2Phy(Color);
  
  	return PhyColor;
}

/*
*************************************************************************************************************
- 函数名称 : void Set_Color(int color)
- 函数说明 : 设定颜色的上层函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/

void Set_Color(U32 color) 
{
    LCD_SetColor(LCD_Log2Phy(color));
}

/*
*************************************************************************************************************
- 函数名称 : void Set_Color(int color)
- 函数说明 : 设定颜色函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_SetColor(U16 PhyColor)   
{ 
	LCD_COLOR = PhyColor; 
}

/*
*************************************************************************************************************
- 函数名称 : void Set_Color(int color)
- 函数说明 : 设定颜色的上层函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/

void Set_BkColor(U32 color) 
{
    LCD_SetBkColor(LCD_Log2Phy(color));
}

/*
*************************************************************************************************************
- 函数名称 : void Set_Color(int color)
- 函数说明 : 设定颜色函数
- 输入参数 : color
- 输出参数 : 无
*************************************************************************************************************
*/
void LCD_SetBkColor(U16 PhyColor)   
{ 
	LCD_BKCOLOR = PhyColor; 
}
