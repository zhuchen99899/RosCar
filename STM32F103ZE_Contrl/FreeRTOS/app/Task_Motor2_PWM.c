#include "Task_Motor2_PWM.h"



void motor2_pwm_task(void *pvParameters)
{
//������Ϣ���о��
extern QueueHandle_t Motor2_PWM_Queue;
//�����ź������
extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;
//����
BaseType_t err=pdFALSE;
float Motor2_PWM=0.0;

	while(1)
	{
			err=xSemaphoreTake(BinarySemaphore_Motor2_SpeedChange,portMAX_DELAY);	//��ȡ���1�ٶ��ź���	
					if(err==pdTRUE)
			{

			xQueuePeek(Motor2_PWM_Queue,&Motor2_PWM,portMAX_DELAY);	
			Motor2_PWM=Motor2_PWM*PWM2_MAX_VAL;
			pr_warn_pure("���PWM2����:%f\r\n",Motor2_PWM);
			TIM_SetCompare3(TIM2,(Motor2_PWM));
			



			}

		
		vTaskDelay(20);
		
		
	}




}


