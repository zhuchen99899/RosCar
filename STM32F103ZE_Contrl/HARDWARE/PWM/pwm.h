#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************

#define PWM1_MAX_VAL 1449
#define PWM2_MAX_VAL 1449

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);

void TIM2_Int_Init(u16 arr,u16 psc);
void TIM2_PWM_Init(u16 arr,u16 psc);




//void TIM7_Int_Init(u16 arr,u16 psc);

//extern volatile unsigned long long FreeRTOSRunTimeTicks;
//void ConfigureTimeForRunTimeStats(void);
#endif
