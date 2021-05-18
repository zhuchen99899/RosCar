#include <OS_AppObjCreate.h>

/***************消息队列创建**************/


#define KEY_Q_NUM    1  		//按键消息队列的数量  
#define Motor1_Direction_Q_NUM 1//电机1方向消息队列的数量  
#define Motor1_Speed_Q_NUM 1// 电机1速度消息队列的数量
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 


//按键消息队列
static void Key_QueueCreate(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEY_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("按键消息队列创建失败\r\n");
	pr_entry_pure("按键消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("按键消息队列创建成功\r\n");
	pr_entry_pure("按键消息队列创建成功\r\n");
	}
}

//电机1方向向消息队列
static void Motor1_Direction_QueueCreate(void)
{
extern QueueHandle_t Motor1_Direction_Queue;
	
Motor1_Direction_Queue =xQueueCreate(Motor1_Direction_Q_NUM,sizeof(Motor1_Direction_t));
	if (Motor1_Direction_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("电机1方向消息队列创建失败\r\n");
	pr_entry_pure("电机1方向消息队列创建失败\r\n");

	}
	else 
	{
	pr_warn_pure("电机1方向消息队列创建成功\r\n");
	pr_entry_pure("电机1方向消息队列创建成功\r\n");

	}
	

}

//电机1速度消息队列
static void Motor1_Speed_QueueCreate(void)
{
extern QueueHandle_t Motor1_Speed_Queue;

Motor1_Speed_Queue =xQueueCreate(Motor1_Speed_Q_NUM,sizeof(float));
	if (Motor1_Speed_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("电机1速度消息队列创建失败\r\n");
	pr_entry_pure("电机1速度消息队列创建失败\r\n");

	}
	else 
	{
	pr_warn_pure("电机1速度消息队列创建成功\r\n");
	pr_entry_pure("电机1速度消息队列创建成功\r\n");

	}
	

}

void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	printf("WIFI接收消息队列创建失败");
	}
	else 
	{
	printf("WIFI接收消息队列创建成功");
	}
}




static void QueueCreate(void)
{

Key_QueueCreate();
Motor1_Direction_QueueCreate();
Motor1_Speed_QueueCreate();
}





/***************信号量创建**************/


static void BinarySemaphoreCreate(void)
{
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//电机1方向更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//电机1速度更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
	
	BinarySemaphore_Motor1_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
};






void OS_AppObjCreate(void)
{
//消息队列创建
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 







