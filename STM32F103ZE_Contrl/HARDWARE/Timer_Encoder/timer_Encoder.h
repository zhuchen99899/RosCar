#ifndef __TIMER_ENCODER_H	
#define __TIMER_ENCODER_H	 
#include "sys.h"
#include "config.h"
#include "stdio.h" //printf֧��
#include "OS_AppObjCreate.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#define  circle_period  1560  //���ת��һȦ������������= ����������* ������ٱ� *Ƶ�ʱ���


#define  Encoder1_period 65535
#define  Motor_Reduction_ratio  30  //������ٱ�
#define  Encoder1_n_lines 13  //����������
#define  period_mode   4      //�ı�Ƶģʽ
#define  period_rate   4      //�ı�Ƶ����Ϊ4
#define  Perimeter 0.11932 // С�������ܳ�




void Encoder_init(void);

#endif


