#include "Task_Key.h"




//key任务函数
void key_task(void *pvParameters)
{
	//声明消息队列句柄
	extern QueueHandle_t Motor1_Direction_Queue;
	extern QueueHandle_t Motor1_Speed_Queue;
	//声明信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	//消息队列初始化
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;
	u8 key=0;
	float motor1_Speed=0;
    while(1)
    {
							key=KEY_Scan(0);

							if(key==WKUP_PRES)  					//加速
						{		 		
							 motor1_Speed-=50;
							if(motor1_Speed<=50)motor1_Speed=0;
							xQueueOverwrite(Motor1_Speed_Queue,&motor1_Speed);
							xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//发送电机1速度更改报文信号
							
						}else if(key==KEY1_PRES)			//减速
						{
							motor1_Speed+=50;
							if(motor1_Speed>=899)motor1_Speed=899;
							xQueueOverwrite(Motor1_Speed_Queue,&motor1_Speed);
							xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//发送电机1速度更改报文信号
									
						}
						
					if(key==KEY0_PRES)  					//反向
						{		 	
							Motor1_Dir->Negative=1;
							Motor1_Dir->Positive=0;
							xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
							xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号
						}else if(key==KEY2_PRES)		//正向
						{
							Motor1_Dir->Negative=0;
							Motor1_Dir->Positive=1;
							xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
							xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号
						}	
			
		vTaskDelay(20);
    }
}

