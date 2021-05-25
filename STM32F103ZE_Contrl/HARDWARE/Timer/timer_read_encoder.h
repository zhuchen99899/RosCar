#ifndef __TIMER_READ_ENCODER_H	
#define __TIMER_READ_ENCODER_H	 
#include "sys.h"
#include "config.h"
#include <math.h> //fabs支持
#include <stdlib.h>
#include "stdio.h" //printf支持
#include "timer_Encoder.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <OS_AppObjCreate.h>




#define timer6_IRQ_time 60  //100ms进入一次定时器中断
void TIM6_Int_Init(u16 arr,u16 psc);
void timer6_my_init(void);
#endif

