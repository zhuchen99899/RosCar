#ifndef __MOTOR_DIR_OUTPUT_H
#define __MOTOR_DIR_OUTPUT_H

#include "sys.h"

#define M1_OC0 PCout(0)// PC0输出
#define M1_OC1 PCout(1)// PC1输出

#define M2_OC2 PCout(2)// PC2输出
#define M2_OC3 PCout(3)// PC3输出
void Motor_Init(void);


#endif



