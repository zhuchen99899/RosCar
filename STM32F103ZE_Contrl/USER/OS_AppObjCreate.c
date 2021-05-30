#include <OS_AppObjCreate.h>

/***************消息队列创建**************/


#define KEY_Q_NUM    1  		//按键消息队列的数量  
#define Motor1_Direction_Q_NUM 1//电机1方向消息队列的数量  
#define Motor1_PWM_Q_NUM 1// 电机1速度消息队列的数量
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 
#define Encoder1_Overflow_Q_NUM 1  //编码器溢出计数队列数量
#define Encoder1_Status_Q_NUM 1 //编码器状态计数
#define Encoder1_last_count_Q_NUM 5 //编码器计数缓存队列数量
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
	
Motor1_Direction_Queue =xQueueCreate(Motor1_Direction_Q_NUM,sizeof(Motor1_Direction_t *));
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
static void Motor1_PWM_QueueCreate(void)
{
extern QueueHandle_t Motor1_PWM_Queue;

Motor1_PWM_Queue =xQueueCreate(Motor1_PWM_Q_NUM,sizeof(float));
	if (Motor1_PWM_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("电机1PWM消息队列创建失败\r\n");
	pr_entry_pure("电机1PWM消息队列创建失败\r\n");

	}
	else 
	{
	pr_warn_pure("电机1PWM消息队列创建成功\r\n");
	pr_entry_pure("电机1PWM消息队列创建成功\r\n");

	}
	

}





static void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct wifibuff *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("WIFI接收消息队列创建失败\r\n");
	pr_entry_pure("WIFI接收消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("WIFI接收消息队列创建成功\r\n");
	pr_entry_pure("WIFI接收消息队列创建成功\r\n");
	}
}




static void Encoder1_Overflow_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Overflow_Queue;
	Encoder1_Overflow_Queue =xQueueCreate(Encoder1_Overflow_Q_NUM,sizeof(int));
	
	if (Encoder1_Overflow_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Encoder1_溢出计数消息队列创建失败\r\n");
	pr_entry_pure("Encoder1_溢出计数消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1_溢出计数消息队列创建成功\r\n");
	pr_entry_pure("Encoder1_溢出计数消息队列创建成功\r\n");
	}
}




static void Encoder1_last_count_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_last_count_Queue;
	Encoder1_last_count_Queue =xQueueCreate(Encoder1_last_count_Q_NUM,sizeof(int));
	
	if (Encoder1_last_count_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Encoder1计数缓存消息队列创建失败\r\n");
	pr_entry_pure("Encoder1计数缓存消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1计数缓存消息队列创建成功\r\n");
	pr_entry_pure("Encoder1计数缓存消息队列创建成功\r\n");
	}
}


static void Encoder1_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Status_Queue;
	Encoder1_Status_Queue =xQueueCreate(Encoder1_Status_Q_NUM,sizeof(struct Encoder1_status *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Encoder1_Status消息队列创建失败\r\n");
	pr_entry_pure("Encoder1_Status消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1_Status消息队列创建成功\r\n");
	pr_entry_pure("Encoder1_Status消息队列创建成功\r\n");
	}
}


static void QueueCreate(void)
{

Key_QueueCreate();
Motor1_Direction_QueueCreate();
Motor1_PWM_QueueCreate();
WIFI_buffer_QueueCreat();
Encoder1_Overflow_QueueCreat();
Encoder1_last_count_QueueCreat();
Encoder1_Status_QueueCreat();
	
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

 







