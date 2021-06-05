#include "Task_Motor2_Dir.h"

//motor2_dir_task任务函数 
void motor2_dir_task(void *pvParameters)
{
		
		//声明消息队列句柄
	extern QueueHandle_t Motor2_Direction_Queue;
	//声明信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;
	
	
	//变量
	BaseType_t err=pdFALSE;
	

	
	//消息队列初始化
	Motor2_Direction_t *Motor2_Dir;
	Motor2_Direction_t  Motor2_Direction_struct_init;
	Motor2_Dir=&Motor2_Direction_struct_init;


    while(1)
    {

		err=xSemaphoreTake(BinarySemaphore_Motor2_DirChange,portMAX_DELAY);	//获取电机1方向信号量	
			
			if(err==pdTRUE)
			{
				
				
				/******************电机方向控制*******************/
					xQueuePeek(Motor2_Direction_Queue,&Motor2_Dir,portMAX_DELAY);
					if((Motor2_Dir->Negative==1)&&(Motor2_Dir->Positive==0))
					{

					 M2_OC2=0;
					 M2_OC3=1;
						pr_warn_pure("电机2反向\r\n");


					}
					else if((Motor2_Dir->Negative==0)&&(Motor2_Dir->Positive==1)) 
					{
						
					M2_OC2=1;
					M2_OC3=0;
					pr_warn_pure("电机2正向\r\n");

				
					}
					
					else 
					{
						

						
					}
					
								
			
			}//信号量获取
    vTaskDelay(20);
    }//while(1)
		
}   


