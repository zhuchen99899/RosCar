#include "Task_Encoder1.h"


//LED1任务函数
void Encoder1_task(void *pvParameters)
{
	extern	TaskHandle_t Encoder1Task_Handler;

	
	
#if Car_Wifi_Send_OFF
vTaskSuspend(Encoder1Task_Handler);
#endif
	
	



	
u8 sendbuff[sendbuffsize];	
extern QueueHandle_t Encoder1_Status_Queue;

Encoder1_status_t *Encoder1_status_send;
Encoder1_status_t Encoder_struct_init;
Encoder1_status_send= &Encoder_struct_init;
	
float speed[2];

    while(1)
    {
	xQueuePeek(Encoder1_Status_Queue,&Encoder1_status_send,portMAX_DELAY);
	/*获取速度值*/
	speed[0]=Encoder1_status_send->Encoder1_Speed;

//	pr_warn_pure("Speed:%f\r\n",Encoder1_status_send->Encoder1_Speed);
	/*组合数据帧*/
	//暂无电机2
	speed[1]=0x00;			
	SerializeBuffer(sendbuff,Data_speed,Motor1,speed);	
	WIFI_send(sendbuff,sendbuffsize);		
			
//	for(int i=0;i<=16;i++)
//	{
//	pr_warn_pure("%x",sendbuff[i]);
//	}
	
			
			
			
			
    vTaskDelay(500);
			
    }
}


