#include "Task_Motor1_Speed.h"



void motor1_speed_task(void *pvParameters)
{
//������Ϣ���о��
extern QueueHandle_t Motor1_Speed_Queue;
//�����ź������
extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
//����
BaseType_t err=pdFALSE;
float Motor1_Speed=0.0;

	while(1)
	{
			err=xSemaphoreTake(BinarySemaphore_Motor1_SpeedChange,portMAX_DELAY);	//��ȡ���1�ٶ��ź���	
					if(err==pdTRUE)
			{

			xQueuePeek(Motor1_Speed_Queue,&Motor1_Speed,10);	
			TIM_SetCompare2(TIM3,(Motor1_Speed));
			
				pr_warn_pure("����ٶȲ���:%f\r\n",Motor1_Speed);


			}

		
		vTaskDelay(20);
		
		
	}




}


