#ifndef __INCLUDES_H
#define __INCLUDES_H

/*32库函数以及系统库*/
#include "sys.h"
#include "delay.h"



/*freertos支持*/
#include "FreeRTOS.h"
#include "task.h"


/*freertos通讯机制*/
#include "queue.h"
#include "semphr.h"
#include "OS_AppObjCreate.h"



/*freertos任务APP*/
#include "Task_Motor1_Dir.h"
#include "Task_Motor1_PWM.h"
#include "Task_Motor1_PID.h"
#include "Task_Motor2_Dir.h"
#include "Task_Motor2_PWM.h"
#include "Task_Motor2_PID.h"
#include "Task_WIFI_send.h"
#include "Task_WIFI.h"
#include "Task_IdleTask.h"
#include "Task_MPU6050.h"


/*bsp驱动*/
#include "Bsp_init.h"
#include "Motor_Dir_Output.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "key.h"
#include "timer_Encoder.h"
#include "timer_read_encoder.h"
#include "iic.h"
#include "mpu6050.h"
#include "inv_mpu.h"

/*其他*/
#include "config.h"
#include "data_init.h"
#include "Car_config.h"
#include "IMU_config.h"
#include "pid.h"



/*通讯协议*/
#include "protocol.h"

#endif



