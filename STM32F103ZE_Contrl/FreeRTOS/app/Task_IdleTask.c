#include "Task_IdleTask.h"



void Idle_task(void *pvParameters)
{

extern QueueHandle_t Encoder1_Overflow_Queue;
int overflow_count=0;

	while(1)
	{
xQueuePeek(Encoder1_Overflow_Queue,&overflow_count,10);
pr_warn_pure("Òç³ö¼ÆÊý:%d\r\n",overflow_count);
    LED1=0;
    vTaskDelay(500);
    LED1=1;
    vTaskDelay(500);
		

		
		
	}




}

