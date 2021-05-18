#include "Task_Key.h"




//key������
void key_task(void *pvParameters)
{
	//������Ϣ���о��
	extern QueueHandle_t Motor1_Direction_Queue;
	extern QueueHandle_t Motor1_Speed_Queue;
	//�����ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	//��Ϣ���г�ʼ��
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;
	u8 key=0;
	float motor1_Speed=0;
    while(1)
    {
							key=KEY_Scan(0);

							if(key==WKUP_PRES)  					//����
						{		 		
							 motor1_Speed-=50;
							if(motor1_Speed<=50)motor1_Speed=0;
							xQueueOverwrite(Motor1_Speed_Queue,&motor1_Speed);
							xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//���͵��1�ٶȸ��ı����ź�
							
						}else if(key==KEY1_PRES)			//����
						{
							motor1_Speed+=50;
							if(motor1_Speed>=899)motor1_Speed=899;
							xQueueOverwrite(Motor1_Speed_Queue,&motor1_Speed);
							xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//���͵��1�ٶȸ��ı����ź�
									
						}
						
					if(key==KEY0_PRES)  					//����
						{		 	
							Motor1_Dir->Negative=1;
							Motor1_Dir->Positive=0;
							xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
							xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�
						}else if(key==KEY2_PRES)		//����
						{
							Motor1_Dir->Negative=0;
							Motor1_Dir->Positive=1;
							xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
							xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�
						}	
			
		vTaskDelay(20);
    }
}

