#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"
//���1������Ϣ���нṹ��
typedef struct Motor1_Direction
{
u8 Positive;
u8 Negative;
	
}Motor1_Direction_t;

//WIFI ������Ϣ���нṹ��
typedef struct WIFIMSG
{

char  address[256];
u32 length;

} WIFIMSG;
	

void OS_AppObjCreate(void);


#endif

