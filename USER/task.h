#ifndef __TASK_H__
#define __TASK_H__	

#include "sys.h"
#include "includes.h"

//Usart3信号量处理任务创建
#define Usart3_Deal_TASK_PRIO		5
#define Usart3_Deal_STK_SIZE 		128
extern OS_TCB Usart3DealTaskTCB;
extern CPU_STK Usart3_Deal_TASK_STK[Usart3_Deal_STK_SIZE];
void Usart3_Deal_Task(void *p_arg);

#endif
