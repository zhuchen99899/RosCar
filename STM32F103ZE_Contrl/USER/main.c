#include "includes.h"

/***************************Freertos通讯句柄初始化***********************/
/*********消息队列句柄********/

QueueHandle_t Motor1_Direction_Queue;
QueueHandle_t Motor1_PWM_Queue;
QueueHandle_t Motor2_Direction_Queue;
QueueHandle_t Motor2_PWM_Queue;
QueueHandle_t Wifi_buffer_Queue;
QueueHandle_t Encoder1_Overflow_Queue;
QueueHandle_t Encoder2_Overflow_Queue;
QueueHandle_t Encoder1_Status_Queue;
QueueHandle_t Encoder2_Status_Queue;
QueueHandle_t Motor1_PID_Parameter_Queue;
QueueHandle_t Motor2_PID_Parameter_Queue;
QueueHandle_t Motor1_Ctrl_Parameter_Queue;
QueueHandle_t Motor2_Ctrl_Parameter_Queue;
/*********信号量句柄********/
SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//电机1方向更改报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//电机1速度更改报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;//电机1方向更改报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;//电机1速度更改报文二值信号量句柄
SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
/********************************FreeRTOS任务****************************/


/**********开始任务***********/

//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);


/**********电机1方向控制任务***********/

//任务优先级
#define MOTOR1_TASK_PRIO		2
//任务堆栈大小	
#define MOTOR1_STK_SIZE 		256  
//任务句柄
TaskHandle_t MOTOR1Task_Handler;
//任务函数
void motor1_dir_task(void *pvParameters);


/**********电机1PWM控制任务***********/

//任务优先级
#define MOTOR1_PWM_TASK_PRIO		2
//任务堆栈大小	
#define MOTOR1_PWM_STK_SIZE 		256  
//任务句柄
TaskHandle_t MOTOR1_PWM_TASK_Handler;
//任务函数
void motor1_pwm_task(void *pvParameters);



/**********电机2方向控制任务***********/

//任务优先级
#define MOTOR2_TASK_PRIO		2
//任务堆栈大小	
#define MOTOR2_STK_SIZE 		256  
//任务句柄
TaskHandle_t MOTOR2Task_Handler;
//任务函数
void motor2_dir_task(void *pvParameters);


/**********电机2PWM控制任务***********/

//任务优先级
#define MOTOR2_PWM_TASK_PRIO		2
//任务堆栈大小	
#define MOTOR2_PWM_STK_SIZE 		256  
//任务句柄
TaskHandle_t MOTOR2_PWM_TASK_Handler;
//任务函数
void motor2_pwm_task(void *pvParameters);



/*********电机1PID计算任务************/

//任务优先级
#define MOTOR1_PID_TASK_PRIO		1
//任务堆栈大小	
#define MOTOR1_PID_STK_SIZE 		512  
//任务句柄
TaskHandle_t MOTOR1_PID_TASK_Handler;
//任务函数
void motor1_PID_task(void *pvParameters);



/*********电机2PID计算任务************/

//任务优先级
#define MOTOR2_PID_TASK_PRIO		1
//任务堆栈大小	
#define MOTOR2_PID_STK_SIZE 		512  
//任务句柄
TaskHandle_t MOTOR2_PID_TASK_Handler;
//任务函数
void motor2_PID_task(void *pvParameters);

/**********WIFI发送任务***********/

//任务优先级
#define WIFI_SEND_TASK_PRIO		2
//任务堆栈大小	
#define WIFI_SEND_STK_SIZE 		512  
//任务句柄
TaskHandle_t WIFI_SEND_Task_Handler;
//任务函数
void Wifi_send_task(void *pvParameters);



/**********WIFI_task任务*************/

//任务优先级
#define WIFI_TASK_PRIO		3
//任务堆栈大小	
#define WIFI_STK_SIZE 		512  
//任务句柄
TaskHandle_t WIFITask_Handler;
//任务函数
void WIFI_task(void *pvParameters);




/**********IMU任务*************/

//任务优先级
#define MPU6050_TASK_PRIO		1
//任务堆栈大小	
#define MPU6050_STK_SIZE 		1024  
//任务句柄
TaskHandle_t MPU6050_Task_Handler;
//任务函数
void MPU6050_task(void *pvParameters);



/********空闲任务***************/
//任务优先级
#define Idle_TASK_PRIO		0
//任务堆栈大小	
#define Idle_STK_SIZE 		256  
//任务句柄
TaskHandle_t IdleTask_Handler;
//任务函数
void Idle_task(void *pvParameters);





int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	  
	Bsp_init();  							//板级驱动初始化
	OS_AppObjCreate();        //Freertos任务通讯机制初始化

	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{

	
		init_set();//开机需要初始化的数据与配置
	
    taskENTER_CRITICAL();           //进入临界区
    //创建电机1方向控制任务
    xTaskCreate((TaskFunction_t )motor1_dir_task,     	
                (const char*    )"motor1_dir_task",   	
                (uint16_t       )MOTOR1_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MOTOR1_TASK_PRIO,	
                (TaskHandle_t*  )&MOTOR1Task_Handler);   
		//创建电机1速度控制任务
    xTaskCreate((TaskFunction_t )motor1_pwm_task,     
                (const char*    )"motor1_pwm_task",   
                (uint16_t       )MOTOR1_PWM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR1_PWM_TASK_PRIO,
                (TaskHandle_t*  )&MOTOR1_PWM_TASK_Handler);     
								
		//创建电机2方向控制任务											
	  xTaskCreate((TaskFunction_t )motor2_dir_task,     	
                (const char*    )"motor2_dir_task",   	
                (uint16_t       )MOTOR2_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MOTOR2_TASK_PRIO,	
                (TaskHandle_t*  )&MOTOR2Task_Handler);   
		//创建电机2速度控制任务
    xTaskCreate((TaskFunction_t )motor2_pwm_task,     
                (const char*    )"motor2_pwm_task",   
                (uint16_t       )MOTOR2_PWM_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR2_PWM_TASK_PRIO,
                (TaskHandle_t*  )&MOTOR2_PWM_TASK_Handler);     					
		//创建电机1PID计算任务
    xTaskCreate((TaskFunction_t )motor1_PID_task,     
                (const char*    )"motor1_PID_task",   
                (uint16_t       )MOTOR1_PID_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR1_PID_TASK_PRIO,
                (TaskHandle_t*  )&MOTOR1_PID_TASK_Handler);     
		//创建电机2PID计算任务
    xTaskCreate((TaskFunction_t )motor2_PID_task,     
                (const char*    )"motor2_PID_task",   
                (uint16_t       )MOTOR2_PID_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR2_PID_TASK_PRIO,
                (TaskHandle_t*  )&MOTOR2_PID_TASK_Handler);     																				
    //创建电机1编码器任务
    xTaskCreate((TaskFunction_t )Wifi_send_task,     
                (const char*    )"Wifi_send_task",   
                (uint16_t       )WIFI_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )WIFI_SEND_TASK_PRIO,
                (TaskHandle_t*  )&WIFI_SEND_Task_Handler);     
    //创建WIFI_task任务
    xTaskCreate((TaskFunction_t )WIFI_task,     
                (const char*    )"wifi_task",   
                (uint16_t       )WIFI_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )WIFI_TASK_PRIO,
                (TaskHandle_t*  )&WIFITask_Handler);   
									
								
		//创建mpu6050任务					
		xTaskCreate((TaskFunction_t )MPU6050_task,     
                (const char*    )"MPU6050_task",   
                (uint16_t       )MPU6050_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MPU6050_TASK_PRIO,
                (TaskHandle_t*  )&MPU6050_Task_Handler);     
								
																				
    //创建空闲任务
    xTaskCreate((TaskFunction_t )Idle_task,     
                (const char*    )"Idle_task",   
                (uint16_t       )Idle_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Idle_TASK_PRIO,
                (TaskHandle_t*  )&IdleTask_Handler);     

								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}


