#include "Task_Motor1_PWM.h"



void motor1_pwm_task(void *pvParameters)
{
//������Ϣ���о��
extern QueueHandle_t Motor1_PWM_Queue;
//�����ź������
extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
//����
BaseType_t err=pdFALSE;
float Motor1_PWM=0.0;

	while(1)
	{
			err=xSemaphoreTake(BinarySemaphore_Motor1_SpeedChange,portMAX_DELAY);	//��ȡ���1�ٶ��ź���	
					if(err==pdTRUE)
			{

			xQueuePeek(Motor1_PWM_Queue,&Motor1_PWM,portMAX_DELAY);	
			Motor1_PWM=Motor1_PWM*PWM1_MAX_VAL;
				
//			pr_warn_pure("���1PWM����:%f\r\n",Motor1_PWM);
			TIM_SetCompare2(TIM3,(Motor1_PWM));
			



			}

		
		vTaskDelay(20);
		
		
	}




}


