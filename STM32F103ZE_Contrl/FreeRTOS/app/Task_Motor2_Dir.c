#include "Task_Motor2_Dir.h"

//motor2_dir_task������ 
void motor2_dir_task(void *pvParameters)
{
		
		//������Ϣ���о��
	extern QueueHandle_t Motor2_Direction_Queue;
	//�����ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;
	
	
	//����
	BaseType_t err=pdFALSE;
	

	
	//��Ϣ���г�ʼ��
	Motor2_Direction_t *Motor2_Dir;
	Motor2_Direction_t  Motor2_Direction_struct_init;
	Motor2_Dir=&Motor2_Direction_struct_init;


    while(1)
    {

		err=xSemaphoreTake(BinarySemaphore_Motor2_DirChange,portMAX_DELAY);	//��ȡ���1�����ź���	
			
			if(err==pdTRUE)
			{
				
				
				/******************����������*******************/
					xQueuePeek(Motor2_Direction_Queue,&Motor2_Dir,portMAX_DELAY);
					if((Motor2_Dir->Negative==1)&&(Motor2_Dir->Positive==0))
					{

					 M2_OC2=0;
					 M2_OC3=1;
						pr_warn_pure("���2����\r\n");


					}
					else if((Motor2_Dir->Negative==0)&&(Motor2_Dir->Positive==1)) 
					{
						
					M2_OC2=1;
					M2_OC3=0;
					pr_warn_pure("���2����\r\n");

				
					}
					
					else 
					{
						

						
					}
					
								
			
			}//�ź�����ȡ
    vTaskDelay(20);
    }//while(1)
		
}   


