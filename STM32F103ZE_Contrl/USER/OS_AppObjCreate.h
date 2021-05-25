#ifndef __OS_APPOBJCREATE_H
#define __OS_APPOBJCREATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "config.h"




// wifi_buffer ����������ΪDMA ����������
#define wifi_buffer_len   256
//���1������Ϣ���нṹ��
typedef struct Motor1_Direction
{
u8 Positive;
u8 Negative;
	
}Motor1_Direction_t;

//WIFI ������Ϣ���нṹ��

typedef struct wifibuff
{
u32 wifi_lenth;
u8 wifi_buffer[wifi_buffer_len];


}wifibuff;

/***������״̬�ṹ��***/
typedef struct Encoder1_status
{
/*�������*/
__IO u8 Encoder1_Direction;


/*���ת��(���㳵���ܳ�)*/	
__IO float Encoder1_Speed;	

	
/*�����·��(���㳵���ܳ�)*/
__IO float Encoder1_distance;

	
}Encoder1_status_t;


	

void OS_AppObjCreate(void);


#endif

