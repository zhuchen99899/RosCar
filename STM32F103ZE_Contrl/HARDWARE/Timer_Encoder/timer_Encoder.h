#ifndef __TIMER_ENCODER_H	
#define __TIMER_ENCODER_H	 
#include "sys.h"
#include "config.h"
#include "stdio.h" //printf支持
#include "OS_AppObjCreate.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#define  circle_period  1560  //电机转动一圈编码器脉冲数= 编码器线数* 电机减速比 *频率倍数


#define  Encoder1_period 65535
#define  Motor_Reduction_ratio  30  //电机减速比
#define  Encoder1_n_lines 13  //编码器线数
#define  period_mode   4      //四倍频模式
#define  period_rate   4      //四倍频倍率为4
#define  Perimeter 0.11932 // 小车车轮周长




void Encoder_init(void);

#endif


