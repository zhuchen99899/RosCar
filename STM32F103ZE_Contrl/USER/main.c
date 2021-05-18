#include "includes.h"

/***************************FreertosͨѶ�����ʼ��***********************/
/*********��Ϣ���о��********/
QueueHandle_t Key_Queue;
QueueHandle_t Motor1_Direction_Queue;
QueueHandle_t Motor1_Speed_Queue;
QueueHandle_t Wifi_buffer_Queue;
/*********�ź������********/
SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//���1������ı��Ķ�ֵ�ź������
SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//���1�ٶȸ��ı��Ķ�ֵ�ź������
SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
/********************************FreeRTOS����****************************/


/**********��ʼ����***********/

//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);


/**********���1�����������***********/

//�������ȼ�
#define MOTOR1_TASK_PRIO		4
//�����ջ��С	
#define MOTOR1_STK_SIZE 		256  
//������
TaskHandle_t MOTOR1Task_Handler;
//������
void motor1_dir_task(void *pvParameters);


/**********���1�ٶȿ�������***********/

//�������ȼ�
#define MOTOR1_SPEED_TASK_PRIO		2
//�����ջ��С	
#define MOTOR1_SPEED_STK_SIZE 		256  
//������
TaskHandle_t MOTOR1_SPEED_TASK_Handler;
//������
void motor1_speed_task(void *pvParameters);


/**********����������***********/

//�������ȼ�
#define Encoder1_TASK_PRIO		1
//�����ջ��С	
#define Encoder1_STK_SIZE 		256  
//������
TaskHandle_t Encoder1Task_Handler;
//������
void Encoder1_task(void *pvParameters);



/**********��������*************/

//�������ȼ�
#define KEY_TASK_PRIO		1
//�����ջ��С	
#define KEY_STK_SIZE 		256  
//������
TaskHandle_t KEYTask_Handler;
//������
void key_task(void *pvParameters);


/********��������***************/
//�������ȼ�
#define Idle_TASK_PRIO		0
//�����ջ��С	
#define Idle_STK_SIZE 		256  
//������
TaskHandle_t IdleTask_Handler;
//������
void Idle_task(void *pvParameters);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	  
	Bsp_init();  							//�弶������ʼ��
	OS_AppObjCreate();        //Freertos����ͨѶ���Ƴ�ʼ��
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //�������1�����������
    xTaskCreate((TaskFunction_t )motor1_dir_task,     	
                (const char*    )"motor1_dir_task",   	
                (uint16_t       )MOTOR1_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )MOTOR1_TASK_PRIO,	
                (TaskHandle_t*  )&MOTOR1Task_Handler);   
		//�������1�ٶȿ�������
    xTaskCreate((TaskFunction_t )motor1_speed_task,     
                (const char*    )"motor1_speed_task",   
                (uint16_t       )MOTOR1_SPEED_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )MOTOR1_SPEED_TASK_PRIO,
                (TaskHandle_t*  )&MOTOR1_SPEED_TASK_Handler);     
    //�������1����������
    xTaskCreate((TaskFunction_t )Encoder1_task,     
                (const char*    )"Encoder1_task",   
                (uint16_t       )Encoder1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Encoder1_TASK_PRIO,
                (TaskHandle_t*  )&Encoder1Task_Handler);     
    //������������
    xTaskCreate((TaskFunction_t )key_task,     
                (const char*    )"key_task",   
                (uint16_t       )KEY_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )KEY_TASK_PRIO,
                (TaskHandle_t*  )&KEYTask_Handler);     
    //������������
    xTaskCreate((TaskFunction_t )Idle_task,     
                (const char*    )"Idle_task",   
                (uint16_t       )Idle_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )Idle_TASK_PRIO,
                (TaskHandle_t*  )&IdleTask_Handler);     

								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}


