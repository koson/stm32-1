#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "includes.h"
#include "wifiap.h"
#include "usart3.h"
#include "ap.h"

void system_init(void);

//创建开始任务
//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//Usart3信号量处理任务创建
#define Usart3_Deal_TASK_PRIO		5
#define Usart3_Deal_STK_SIZE 		128
OS_TCB Usart3DealTaskTCB;
CPU_STK Usart3_Deal_TASK_STK[Usart3_Deal_STK_SIZE];
void Usart3_Deal_Task(void *p_arg);

//AP任务创建
#define AP_TASK_PRIO		4
#define AP_STK_SIZE 		128
OS_TCB APTaskTCB;
CPU_STK AP_TASK_STK[AP_STK_SIZE];
void AP_Task(void *p_arg);

//int main(void)
//{
	//OS_ERR err;
	//CPU_SR_ALLOC();
	
	//delay_init();       //延时初始化
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断分组配置
	//uart_init(115200);    //串口波特率设置
	
	//OSInit(&err);		//初始化UCOSIII
	//OS_CRITICAL_ENTER();//进入临界区
	////创建开始任务
	//OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 //(CPU_CHAR	* )"start task", 		//任务名字
                 //(OS_TASK_PTR )start_task, 			//任务函数
                 //(void		* )0,					//传递给任务函数的参数
                 //(OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 //(CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 //(CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 //(CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 //(OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 //(OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 //(void   	* )0,					//用户补充的存储区
                 //(OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 //(OS_ERR 	* )&err);				//存放该函数错误时的返回值
	//OS_CRITICAL_EXIT();	//退出临界区	 
	//OSStart(&err);  //开启UCOSIII
	//while(1);
//}

//main函数	  					
int main(void)
{ 	
	OS_ERR err;
	CPU_SR_ALLOC();
	
	OS_CRITICAL_ENTER();//进入临界区
	system_init();		//系统初始化 
	OS_CRITICAL_EXIT();		//退出临界区
	
	
	
  OSInit(&err);   
 	//OS_CRITICAL_ENTER();//进入临界区
	////创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区
	OSStart(&err);	  						    
}

//系统初始化
void system_init(void)
{
// 	u16 okoffset=162;
// 	u16 ypos=0;
//	u16 j=0;
//	u16 temp=0;
//	u8 res;
//	u32 dtsize,dfsize;
//	u8 *stastr=0;
//	u8 *version=0; 
//	u8 verbuf[12];
//	u8 fsize;
//	u8 icowidth;
	
 	Stm32_Clock_Init(9);		//设置时钟,72Mhz 
	uart_init(115200);		//初始化串口波特率为115200
 	//exeplay_app_check();		//检测是否需要运行APP代码.  
	delay_init();				//延时初始化  
	usart3_init(36,115200);		//初始化串口2波特率为115200
	//usmart_dev.init(72);		//初始化USMART
 	//LED_Init();					//初始化LED 
 	//LCD_Init();					//LCD初始化    
 	//BEEP_Init();				//蜂鸣器初始化
 	//KEY_Init();					//按键初始化 
 	//FSMC_SRAM_Init();			//初始化外部SRAM
 	//AT24CXX_Init();    			//EEPROM初始化
	//W25QXX_Init();				//初始化W25Q128
	//Adc_Init();					//初始化内部温度传感器 
 	//Lsens_Init();				//初始化光敏传感器
	my_mem_init(SRAMIN);		//初始化内部内存池
	//my_mem_init(SRAMEX);		//初始化外部SRAM内存池 
	//tp_dev.init(); 				//触摸屏初始化
	//gui_init();	  				//gui初始化
	//piclib_init();				//piclib初始化	  
	//slcd_dma_init(); 
	//exfuns_init();				//FATFS 申请内存
	atk_8266_init();

	
	
	
	//version=mymalloc(SRAMIN,31);//申请31个字节内存
//REINIT://重新初始化
	//LCD_Clear(BLACK);			//黑屏
	//POINT_COLOR=WHITE;
	//BACK_COLOR=BLACK;
	//j=0;   
///////////////////////////////////////////////////////////////////////////
////显示版权信息
	//ypos=2;
	//if(lcddev.width==240)
	//{
		//fsize=12;
		//icowidth=18;
		//okoffset=190;
		//app_show_mono_icos(5,ypos,icowidth,24,(u8*)APP_ALIENTEK_ICO1824,YELLOW,BLACK);
	//}else if(lcddev.width==320)
	//{
		//fsize=16;
		//icowidth=24;
		//okoffset=250;
		//app_show_mono_icos(5,ypos,icowidth,32,(u8*)APP_ALIENTEK_ICO2432,YELLOW,BLACK);		
	//}else if(lcddev.width==480)
	//{
		//fsize=24;
		//icowidth=36;
		//okoffset=370;
		//app_show_mono_icos(5,ypos,icowidth,48,(u8*)APP_ALIENTEK_ICO3648,YELLOW,BLACK);		
	//}
	//LCD_ShowString(icowidth+5*2,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "ALIENTEK STM32F1 WarShip V3");
	//LCD_ShowString(icowidth+5*2,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"Copyright (C) 2015-2025");    
	//app_get_version(verbuf,HARDWARE_VERSION,2);
	//strcpy((char*)version,"HARDWARE:");
	//strcat((char*)version,(const char*)verbuf);
	//strcat((char*)version,", SOFTWARE:");
	//app_get_version(verbuf,SOFTWARE_VERSION,3);
	//strcat((char*)version,(const char*)verbuf);
	//LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,version);
	//sprintf((char*)verbuf,"LCD ID:%04X",lcddev.id);		//LCD ID打印到verbuf里面
	//LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, verbuf);	//显示LCD ID 
////////////////////////////////////////////////////////////////////////////
////开始硬件检测初始化 
	//LED0=0;LED1=0;				//同时点亮两个LED
	//LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "CPU:STM32F103ZET6 72Mhz");
	//LCD_ShowString(5,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "FLASH:512KB   SRAM:64KB");	
	//if(system_exsram_test(5,ypos+fsize*j,fsize))system_error_show(5,ypos+fsize*j++,"EX Memory Error!",fsize);
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"OK");			 
	//my_mem_init(SRAMEX);		//初始化外部内存池,必须放在内存检测之后
	//LED0=1;LED1=1;				//同时关闭两个LED 
	//if(W25QXX_ReadID()!=W25Q128)//检测不到W25Q128
	//{	 
		//system_error_show(5,ypos+fsize*j++,"Ex Flash Error!!",fsize); 
	//}else temp=16*1024;			//16M字节大小
	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Ex Flash:     KB");			   
	//LCD_ShowxNum(5+9*(fsize/2),ypos+fsize*j,temp,5,fsize,0);//显示flash大小  
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");   
	////检测是否需要擦除SPI FLASH?
	//res=KEY_Scan(1);//
	//if(res==KEY2_PRES)
	//{
		//res=system_files_erase(5,ypos+fsize*j,fsize);
		//if(res)goto REINIT; 
	//}
    ////RTC检测
  	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "RTC Check...");			   
 	//if(RTC_Init())system_error_show(5,ypos+fsize*(j+1),"RTC Error!",fsize);//RTC检测
	//else 
	//{ 
		//RTC_Get();//获取时间
		//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	//}
	////检查SPI FLASH的文件系统
	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "FATFS Check...");//FATFS检测			   
  	//f_mount(fs[0],"0:",1); 		//挂载SD卡  
  	//f_mount(fs[1],"1:",1); 		//挂载挂载FLASH. 
 	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
	////SD卡检测
	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SD Card:     MB");//FATFS检测
	//temp=0;	
 	//do
	//{
		//temp++;
 		//res=exf_getfree("0:",&dtsize,&dfsize);//得到SD卡剩余容量和总容量
		//delay_ms(200);		   
	//}while(res&&temp<5);//连续检测5次
 	//if(res==0)//得到容量正常
	//{ 
		//gui_phy.memdevflag|=1<<0;	//设置SD卡在位.
		//temp=dtsize>>10;//单位转换为MB
		//stastr="OK";
 	//}else 
	//{
 		//temp=0;//出错了,单位为0
		//stastr="ERROR";
	//}
 	//LCD_ShowxNum(5+8*(fsize/2),ypos+fsize*j,temp,5,fsize,0);					//显示SD卡容量大小
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,stastr);	//SD卡状态			   
	////W25Q128检测,如果不存在文件系统,则先创建.
	//temp=0;	
 	//do
	//{
		//temp++;
 		//res=exf_getfree("1:",&dtsize,&dfsize);//得到FLASH剩余容量和总容量
		//delay_ms(200);		   
	//}while(res&&temp<20);//连续检测20次		  
	//if(res==0X0D)//文件系统不存在
	//{
		//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk Formatting...");	//格式化FLASH
		//res=f_mkfs("1:",1,4096);//格式化FLASH,1,盘符;1,不需要引导区,8个扇区为1个簇
		//if(res==0)
		//{
			//f_setlabel((const TCHAR *)"1:ALIENTEK");				//设置Flash磁盘的名字为：ALIENTEK
			//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//标志格式化成功
 			//res=exf_getfree("1:",&dtsize,&dfsize);//重新获取容量
		//}
	//}   
	//if(res==0)//得到FLASH卡剩余容量和总容量
	//{
		//gui_phy.memdevflag|=1<<1;	//设置SPI FLASH在位.
		//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Flash Disk:     KB");//FATFS检测			   
		//temp=dtsize; 	   
 	//}else system_error_show(5,ypos+fsize*(j+1),"Flash Fat Error!",fsize);	//flash 文件系统错误 
 	//LCD_ShowxNum(5+11*(fsize/2),ypos+fsize*j,temp,5,fsize,0);						//显示FLASH容量大小
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize,"OK");			//FLASH卡状态	
	////TPAD检测		 
 	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "TPAD Check...");			   
 	//if(TPAD_Init(6))system_error_show(5,ypos+fsize*(j+1),"TPAD Error!",fsize);//触摸按键检测
	//else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK"); 
	////DM9000检测   
	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "DM9000 Check...");			   
 	//if(DM9000_Init(0))system_error_show(5,ypos+fsize*(j+1),"DM9000 Error!",fsize);//DM9000检测
	//else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK"); 
	//DM9000_RST=0;		//复位DM9000
	//EXTI->IMR&=~(1<<6);	//关闭line6中断	 
	////24C02检测
   	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "24C02 Check...");			   
 	//if(AT24CXX_Check())system_error_show(5,ypos+fsize*(j+1),"24C02 Error!",fsize);//24C02检测
	//else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");  
  	////VS1053检测
 	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize,"VS1053 Check...");	 
	//VS_Init();		//初始化VS1053接口
	//VS_Sine_Test();	//正弦测试
	//BEEP=0;		   
	//if(VS_Ram_Test()!=0X83FF)system_error_show(5,ypos+fsize*(j+1),"VS1053 Error!",fsize); 
	//else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK"); 
	//VS_HD_Reset();
	////字库检测									    
   	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");
	//res=KEY_Scan(1);//检测按键
	//if(res==KEY1_PRES)//更新？确认
	//{
		//res=system_font_update_confirm(5,ypos+fsize*(j+1),fsize);
	//}else res=0;
	//if(font_init()||(res==1))//检测字体,如果字体不存在/强制更新,则更新字库	
	//{
		//res=0;//按键无效
 		//if(update_font(5,ypos+fsize*j,fsize,"0:")!=0)//从SD卡更新
		//{ 
			//if(update_font(5,ypos+fsize*j,fsize,"1:")!=0)//从SPI FLASH更新
			//{ 
				//res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);	//是否考虑从USB拷贝文件到SPI FLASH?
				//if(res)goto REINIT;
				//system_error_show(5,ypos+fsize*(j+1),"Font Error!",fsize);	//字体错误
			//} 
		//}			
		//LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//填充底色
    	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Font Check...");			   
 	//} 
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//字库检测OK
	////系统文件检测
   	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check...");			   
 	//while(app_system_file_check("1"))//SPI FLASH 系统文件检测
	//{
		//delay_ms(100);
		//LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);		//填充底色
    	//LCD_ShowString(5,ypos+fsize*j,(fsize/2)*8,fsize,fsize, "Updating"); //显示updating	
		//app_boot_cpdmsg_set(5,ypos+fsize*j,fsize);		//设置到坐标
		//if(app_system_file_check("0"))					//检查SD卡系统文件完整性
		//{ 
			//res=system_usb_update_confirm(5,ypos+fsize*(j+1),fsize);//SD卡不完整，是否考虑从USB拷贝文件到SPI FLASH?
			//if(res)goto REINIT;
			//system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize); 	//更新失败，报错  
		//}else if(app_system_update(app_boot_cpdmsg,"0:"))//SD卡更新 
		//{ 
			//system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Error!",fsize);	//更新失败，报错  
		//}
		//LCD_Fill(5,ypos+fsize*j,lcddev.width,ypos+fsize*(j+1),BLACK);//填充底色
    	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Files Check..."); 
		//if(app_system_file_check("1"))//更新了一次，再检测，如果还有不全，说明SD卡文件就不全！
		//{
			//system_error_show(5,ypos+fsize*(j+1),"SYSTEM File Lost!",fsize);
		//}else break;	
	//}
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");	
 	////触摸屏检测 
	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "Touch Check...");			   
	//res=KEY_Scan(1);//检测按键			   
	//if(TP_Init()||(res==KEY0_PRES&&(tp_dev.touchtype&0X80)==0))//有更新/按下了KEY0且不是电容屏,执行校准 	
	//{
		//if(res==1)TP_Adjust();
		//res=0;//按键无效
		//goto REINIT;				//重新开始初始化
	//}
	//LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");//触摸屏检测OK
   	////系统参数加载			   
 	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Parameter Load...");			   
 	//if(app_system_parameter_init())system_error_show(5,ypos+fsize*(j+1),"Parameter Load Error!",fsize);//参数加载
	//else LCD_ShowString(5+okoffset,ypos+fsize*j++,lcddev.width,lcddev.height,fsize, "OK");			   
  	//LCD_ShowString(5,ypos+fsize*j,lcddev.width,lcddev.height,fsize, "SYSTEM Starting...");  
	////蜂鸣器短叫,提示正常启动
	//BEEP=1;
	//delay_ms(100);
	//BEEP=0; 
	//myfree(SRAMIN,version);	
}   



//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	OS_CRITICAL_ENTER();	//进入临界区
	
	//创建Usart3信号量处理任务
	OSTaskCreate((OS_TCB 	* )&Usart3DealTaskTCB,		
				 (CPU_CHAR	* )"Usart3 Deal task", 		
                 (OS_TASK_PTR )Usart3_Deal_Task, 			
                 (void		* )0,					
                 (OS_PRIO	  )Usart3_Deal_TASK_PRIO,     	
                 (CPU_STK   * )&Usart3_Deal_TASK_STK[0],	
                 (CPU_STK_SIZE)Usart3_Deal_STK_SIZE/10,	
                 (CPU_STK_SIZE)Usart3_Deal_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
								 
		//AP任务处理任务						 
		OSTaskCreate((OS_TCB 	*   )&APTaskTCB,			//任务控制块
								 (CPU_CHAR	* )"ap task", 			//任务名字
								 (OS_TASK_PTR )AP_Task, 				//任务函数
								 (void		*   )0,												//传递给任务函数的参数
								 (OS_PRIO	    )AP_TASK_PRIO, 	//任务优先级
								 (CPU_STK   * )&AP_TASK_STK[0],//任务堆栈基地址
								 (CPU_STK_SIZE)AP_STK_SIZE/10,	//任务堆栈深度限位
								 (CPU_STK_SIZE)AP_STK_SIZE,		//任务堆栈大小
								 (OS_MSG_QTY  )0,												//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
								 (OS_TICK	    )0,												//当使能时间片轮转时的时间片长度，为0时为默认长度，
								 (void   	*   )0,												//用户补充的存储区
								 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
								 (OS_ERR 	  * )&err);										//存放该函数错误时的返回值				
				 
	
//			 
//	//创建浮点测试任务
//	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
//				 (CPU_CHAR	* )"float test task", 		
//                 (OS_TASK_PTR )float_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
//                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
//                 (OS_ERR 	* )&err);								 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//进入临界区
}

void AP_Task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		//atk_8266_msg_show();
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);		//请求任务内建的信号量
		//atk_8266_rst();
	}
}

void Usart3_Deal_Task(void *arg)
{
//	u8 netpro=0;	//网络模式
//	u8 key;
//	u8 timex=0; 
//	u8 ipbuf[16]; 	//IP缓存

//	u16 t=999;		//加速第一次获取链接状态
//	u8 res=0;
	u16 rlen=0;
	
	OS_ERR err;
//	u8 constate=0;	//连接状态

	
	
	while(1)
	{
		u8 *p;
		p=mymalloc(SRAMIN,32);							//申请32字节内存
		
		OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err);		//请求任务内建的信号量
		if(USART3_RX_STA&0X8000)		//接收到一次数据了
		{ 
			rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
			USART3_RX_BUF[rlen]=0;		//添加结束符 
			printf("%s",USART3_RX_BUF);	//发送到串口 
			
			sprintf((char*)p,"收到%d字节,内容如下",rlen);//接收到的字节数 
//			LCD_Fill(30+54,115,239,130,WHITE);
//			POINT_COLOR=BRED;
//			Show_Str(30+54,115,156,12,p,12,0); 			//显示接收到的数据长度
//			POINT_COLOR=BLUE;
//			LCD_Fill(30,130,239,319,WHITE);
//			Show_Str(30,130,180,190,USART3_RX_BUF,12,0);//显示接收到的数据
			if(USART3_RX_BUF[0]=='A' & USART3_RX_BUF[0]=='P')
			{
				printf("成功!!!!\r\n");
			}
			USART3_RX_STA=0;
//			if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
//			else t=0;                   //状态为已经连接了,10秒后再检查
		}
	}
}

////led0任务函数
//void led0_task(void *p_arg)
//{
	//OS_ERR err;
	//p_arg = p_arg;
	//while(1)
	//{
////		LED0=0;
		//OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
////		LED0=1;
		//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	//}
//}

////led1任务函数
//void led1_task(void *p_arg)
//{
	//OS_ERR err;
	//p_arg = p_arg;
	//while(1)
	//{
////		LED1=~LED1;
		//OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
	//}
//}

////浮点测试任务
//void float_task(void *p_arg)
//{
	//CPU_SR_ALLOC();
	//static float float_num=0.01;
	//while(1)
	//{
		//float_num+=0.01f;
		//OS_CRITICAL_ENTER();	//进入临界区
		//printf("float_num的值为: %.4f\r\n",float_num);
		//OS_CRITICAL_EXIT();		//退出临界区
		//delay_ms(500);			//延时500ms
	//}
//}
