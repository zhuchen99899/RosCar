#include "Task_WIFI_send.h"


//LED1任务函数
void Wifi_send_task(void *pvParameters)
{
	extern	TaskHandle_t WIFI_SEND_Task_Handler;

#if Car_Wifi_Send_OFF
vTaskSuspend(WIFI_SEND_Task_Handler);
#endif
	
//发送缓冲
u8 sendbuff[sendbuffsize];	
	
	
extern QueueHandle_t Encoder1_Status_Queue;
extern QueueHandle_t Encoder2_Status_Queue;
	

Encoder1_status_t *Encoder1_status_send;
Encoder1_status_t Encoder1_struct_init;
Encoder1_status_send= &Encoder1_struct_init;
	
	
Encoder2_status_t *Encoder2_status_send;
Encoder2_status_t Encoder2_struct_init;
Encoder2_status_send= &Encoder2_struct_init;	

float speed[2];

    while(1)
    {
	xQueuePeek(Encoder1_Status_Queue,&Encoder1_status_send,portMAX_DELAY);
	xQueuePeek(Encoder2_Status_Queue,&Encoder2_status_send,portMAX_DELAY);
	/*获取速度值*/
	speed[0]=Encoder1_status_send->Encoder1_Speed;
	speed[1]=Encoder2_status_send->Encoder2_Speed;
//	pr_warn_pure("Speed:%f\r\n",Encoder1_status_send->Encoder1_Speed);
	/*组合数据帧*/
	//暂无电机2
		
	SerializeBuffer(sendbuff,Data_speed,Each,speed);	
	WIFI_send(sendbuff,sendbuffsize);		
			
//	for(int i=0;i<=16;i++)
//	{
//	pr_warn_pure("%x",sendbuff[i]);
//	}
	
			
			
			
			
    vTaskDelay(600);
			
    }
}


