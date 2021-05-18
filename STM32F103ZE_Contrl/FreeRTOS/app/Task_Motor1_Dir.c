#include "Task_Motor1_Dir.h"

//motor1_dir_task������ 
void motor1_dir_task(void *pvParameters)
{
		
		//������Ϣ���о��
	extern QueueHandle_t Motor1_Direction_Queue;
	//�����ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	
	
	//����
	BaseType_t err=pdFALSE;
	

	
	//��Ϣ���г�ʼ��
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;


    while(1)
    {

		err=xSemaphoreTake(BinarySemaphore_Motor1_DirChange,portMAX_DELAY);	//��ȡ���1�����ź���	
			
			if(err==pdTRUE)
			{
				
				
				/******************����������*******************/
					xQueuePeek(Motor1_Direction_Queue,&Motor1_Dir,portMAX_DELAY);
					if((Motor1_Dir->Negative==1)&&(Motor1_Dir->Positive==0))
					{
					 OC0=0;
					 OC1=1;
						pr_warn_pure("�������\r\n");


					}
					else if((Motor1_Dir->Negative==0)&&(Motor1_Dir->Positive==1)) 
					{
						
					OC0=1;
					OC1=0;
					pr_warn_pure("�������\r\n");

				
					}
					
					else 
					{
						

						
					}
					
								
			
			}//�ź�����ȡ
    vTaskDelay(20);
    }//while(1)
		
}   


