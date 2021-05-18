#include "Task_IdleTask.h"



void Idle_task(void *pvParameters)
{



	while(1)
	{

    LED1=0;
    vTaskDelay(500);
    LED1=1;
    vTaskDelay(500);
		

		
		
	}




}

