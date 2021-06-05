#ifndef __TIMER_READ_ENCODER_H	
#define __TIMER_READ_ENCODER_H	 
#include "sys.h"
#include "config.h"
#include <math.h> //fabs֧��
#include <stdlib.h>
#include "stdio.h" //printf֧��
#include "timer_Encoder.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <OS_AppObjCreate.h>



//S_K=Perimeter*(1000/timer6_IRQ_time)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate);
#define timer6_IRQ_time 20  //100ms����һ�ζ�ʱ���ж�
void TIM6_Int_Init(u16 arr,u16 psc);
void timer6_my_init(void);
#endif

