#include "Task_Motor1_Speed.h"



void motor1_speed_task(void *pvParameters)
{
//声明消息队列句柄
extern QueueHandle_t Motor1_PWM_Queue;
//声明信号量句柄
extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
//变量
BaseType_t err=pdFALSE;
float Motor1_PWM=0.0;

	while(1)
	{
			err=xSemaphoreTake(BinarySemaphore_Motor1_SpeedChange,portMAX_DELAY);	//获取电机1速度信号量	
					if(err==pdTRUE)
			{

			xQueuePeek(Motor1_PWM_Queue,&Motor1_PWM,portMAX_DELAY);	
			Motor1_PWM=Motor1_PWM*899;
			TIM_SetCompare2(TIM3,(Motor1_PWM));
			
				pr_warn_pure("电机速度参数:%f\r\n",Motor1_PWM);


			}

		
		vTaskDelay(20);
		
		
	}




}


