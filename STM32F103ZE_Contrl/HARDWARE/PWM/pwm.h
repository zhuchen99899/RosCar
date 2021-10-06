#ifndef __PWM_H
#define __PWM_H
#include "sys.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
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
