#include "Task_IdleTask.h"



void Idle_task(void *pvParameters)
{

	


//extern QueueHandle_t Encoder1_Status_Queue;

//Encoder1_status_t *Encoder1_status_send;
	while(1)
	{

//	xQueuePeek(Encoder1_Status_Queue,&Encoder1_status_send,portMAX_DELAY);

//	pr_warn_pure("Speed:%f\r\n",Encoder1_status_send->Encoder1_Speed);
//		pr_warn_pure("direction:%d",Encoder1_status_send->Encoder1_Direction);
	
    LED1=0;
    vTaskDelay(500);
    LED1=1;
    vTaskDelay(500);
		

		
		
	}




}

