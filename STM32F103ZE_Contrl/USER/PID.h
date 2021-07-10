#ifndef __PID_H
#define __PID_H

#include <math.h> //fabs÷ß≥÷


void pid_init(void);
float speed_pid_realize1(float setSpeed,float Actual_Speed);
float speed_pid_realize2(float setSpeed,float Actual_Speed);

#endif






