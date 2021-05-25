#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"




// wifi_buffer 缓冲区长度为DMA 缓冲区长度
#define wifi_buffer_len   256
//电机1方向消息队列结构体
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

/***编码器状态结构体***/
typedef struct Encoder1_status
{
/*电机方向*/
__IO u8 Encoder1_Direction;


/*电机转速(计算车轮周长)*/	
__IO float Encoder1_Speed;	

	
/*电机总路程(计算车轮周长)*/
__IO float Encoder1_distance;

	
}Encoder1_status_t;


	

void OS_AppObjCreate(void);


#endif

