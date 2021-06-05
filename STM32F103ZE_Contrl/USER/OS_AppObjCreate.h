#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"
#include "Car_config.h"



// wifi_buffer 缓冲区长度为DMA 缓冲区长度
#define wifi_buffer_len   256




//电机控制参数消息队列
typedef struct Motor1_ctrl
{
#if Car_ctrl_speed_loop
float Speed;
#endif
	
#if Car_ctrl_length_loop	
	
float Length;
#endif

#if 	Car_ctrl_force_loop
float moment;
#endif
}M1_ctrl;




//电机1PID参数
typedef struct Motor1_PID
	{
		float Kp;
		float Ki;
		float Kd;
		
		
	}M1_PID;






typedef struct Motor1_Direction
{
u8 Positive;
u8 Negative;
	
}Motor1_Direction_t;

//WIFI 接收消息队列结构体

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[wifi_buffer_len];


}wifibuff;

/***编码器1状态结构体***/
typedef struct Encoder1_status
{
#if Car_ctrl_speed_loop	
/*电机转速(计算车轮周长)*/	
 float Encoder1_Speed;	
#endif

#if	Car_ctrl_length_loop
	///*电机总路程(计算车轮周长)*/
 float Encoder1_distance;
#endif

#if	Car_ctrl_dirc_inquiry
	///*电机方向*/
 u8 Encoder1_Direction;

#endif


}Encoder1_status_t;


	

void OS_AppObjCreate(void);


#endif

