#include "Task_Motor1_Dir.h"

//motor1_dir_task任务函数 
void motor1_dir_task(void *pvParameters)
{
		
		//声明消息队列句柄
	extern QueueHandle_t Motor1_Direction_Queue;
	//声明信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	
	
	//变量
	BaseType_t err=pdFALSE;
	

	
	//消息队列初始化
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;


    while(1)
    {

		err=xSemaphoreTake(BinarySemaphore_Motor1_DirChange,portMAX_DELAY);	//获取电机1方向信号量	
			
			if(err==pdTRUE)
			{
				
				
				/******************电机方向控制*******************/
					xQueuePeek(Motor1_Direction_Queue,&Motor1_Dir,portMAX_DELAY);
					if((Motor1_Dir->Negative==1)&&(Motor1_Dir->Positive==0))
					{
					 OC0=0;
					 OC1=1;
						pr_warn_pure("电机反向\r\n");


					}
					else if((Motor1_Dir->Negative==0)&&(Motor1_Dir->Positive==1)) 
					{
						
					OC0=1;
					OC1=0;
					pr_warn_pure("电机正向\r\n");

				
					}
					
					else 
					{
						

						
					}
					
								
			
			}//信号量获取
    vTaskDelay(20);
    }//while(1)
		
}   


