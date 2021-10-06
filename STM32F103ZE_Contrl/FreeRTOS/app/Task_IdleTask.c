#include "Task_IdleTask.h"



void Idle_task(void *pvParameters)
{



	


//extern QueueHandle_t Encoder1_Status_Queue;

//Encoder1_status_t *Encoder1_status_send;
	while(1)
	{


	

    LED1=0;
    vTaskDelay(500);
    LED1=1;
    vTaskDelay(500);


		
		
	}




}

