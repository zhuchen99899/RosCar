#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"
//电机1方向消息队列结构体
typedef struct Motor1_Direction
{
u8 Positive;
u8 Negative;
	
}Motor1_Direction_t;

//WIFI 接收消息队列结构体
typedef struct WIFIMSG
{

char  address[256];
u32 length;

} WIFIMSG;
	

void OS_AppObjCreate(void);


#endif

