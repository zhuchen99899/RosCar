#include "Task_Motor2_PWM.h"



void motor2_pwm_task(void *pvParameters)
{
//声明消息队列句柄
extern QueueHandle_t Motor2_PWM_Queue;
//声明信号量句柄
extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;
//变量
BaseType_t err=pdFALSE;
float Motor2_PWM=0.0;

	while(1)
	{
			err=xSemaphoreTake(BinarySemaphore_Motor2_SpeedChange,portMAX_DELAY);	//获取电机1速度信号量	
					if(err==pdTRUE)
			{

			xQueuePeek(Motor2_PWM_Queue,&Motor2_PWM,portMAX_DELAY);	
			Motor2_PWM=Motor2_PWM*PWM2_MAX_VAL;
			pr_warn_pure("电机PWM2参数:%f\r\n",Motor2_PWM);
			TIM_SetCompare3(TIM2,(Motor2_PWM));
			



			}

		
		vTaskDelay(20);
		
		
	}




}


