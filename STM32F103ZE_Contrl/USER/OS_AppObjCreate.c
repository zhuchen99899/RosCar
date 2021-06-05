#include <OS_AppObjCreate.h>

/*全局结构体*/
//用于消息队列传递指针
Encoder1_status_t Encoder1_struct_init; //全局结构体
Encoder2_status_t Encoder2_struct_init;
M1_ctrl   Motor1_ctrl_struct_init;
M1_PID    Motor1_PID_struct_init;
Motor1_Direction_t  Motor1_Direction_struct_init;
Motor2_Direction_t  Motor2_Direction_struct_init;
wifibuff wifibuff_struct_init;





/***************消息队列创建**************/






#define Motor1_Direction_Q_NUM 1//电机1方向消息队列的数量  
#define Motor1_PWM_Q_NUM 1// 电机1速度消息队列的数量
#define Motor2_Direction_Q_NUM 1//电机1方向消息队列的数量  
#define Motor2_PWM_Q_NUM 1// 电机1速度消息队列的数量


#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi接收数据的消息队列的数量 
#define Encoder1_Overflow_Q_NUM 1  //编码器溢出计数队列数量
#define Encoder2_Overflow_Q_NUM 1  //编码器溢出计数队列数量
#define Encoder1_Status_Q_NUM 1 //编码器状态计数
#define Encoder2_Status_Q_NUM 1
#define Motor1_PID_Parameter_Q_NUM 1
#define Motor1_Ctrl_Parameter_Q_NUM  1


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


//电机2方向向消息队列
static void Motor2_Direction_QueueCreate(void)
{
extern QueueHandle_t Motor2_Direction_Queue;
	
Motor2_Direction_Queue =xQueueCreate(Motor2_Direction_Q_NUM,sizeof(Motor2_Direction_t *));
	if (Motor2_Direction_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("电机2方向消息队列创建失败\r\n");
	pr_entry_pure("电机2方向消息队列创建失败\r\n");

	}
	else 
	{
	pr_warn_pure("电机2方向消息队列创建成功\r\n");
	pr_entry_pure("电机2方向消息队列创建成功\r\n");

	}
	

}

//电机2速度消息队列
static void Motor2_PWM_QueueCreate(void)
{
extern QueueHandle_t Motor2_PWM_Queue;

Motor2_PWM_Queue =xQueueCreate(Motor2_PWM_Q_NUM,sizeof(float));
	if (Motor2_PWM_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("电机2PWM消息队列创建失败\r\n");
	pr_entry_pure("电机2PWM消息队列创建失败\r\n");

	}
	else 
	{
	pr_warn_pure("电机2PWM消息队列创建成功\r\n");
	pr_entry_pure("电机2PWM消息队列创建成功\r\n");

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


static void Encoder2_Overflow_QueueCreat(void)
{
	extern QueueHandle_t Encoder2_Overflow_Queue;
	Encoder2_Overflow_Queue =xQueueCreate(Encoder2_Overflow_Q_NUM,sizeof(int));
	
	if (Encoder2_Overflow_Queue==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Encoder2_溢出计数消息队列创建失败\r\n");
	pr_entry_pure("Encoder2_溢出计数消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder2_溢出计数消息队列创建成功\r\n");
	pr_entry_pure("Encoder2_溢出计数消息队列创建成功\r\n");
	}
}



static void Encoder1_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Status_Queue;
	Encoder1_Status_Queue =xQueueCreate(Encoder1_Status_Q_NUM,sizeof(struct Encoder1_status_t *));
	
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

static void Encoder2_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder2_Status_Queue;
	Encoder2_Status_Queue =xQueueCreate(Encoder2_Status_Q_NUM,sizeof(struct Encoder2_status_t *));
	
	if (Encoder2_Status_QueueCreat==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Encoder2_Status消息队列创建失败\r\n");
	pr_entry_pure("Encoder2_Status消息队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder2_Status消息队列创建成功\r\n");
	pr_entry_pure("Encoder2_Status消息队列创建成功\r\n");
	}
}



//控制参数消息队列


static void Motor1_Ctrl_Parameter_QueueCreat(void)
{
	extern QueueHandle_t Motor1_Ctrl_Parameter_Queue;
	Motor1_Ctrl_Parameter_Queue =xQueueCreate(Motor1_Ctrl_Parameter_Q_NUM,sizeof(struct M1_ctrl *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Ctrl_Parameter_Queue队列创建失败\r\n");
	pr_entry_pure("Ctrl_Parameter_Queue队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Ctrl_Parameter_Queue队列创建成功\r\n");
	pr_entry_pure("Ctrl_Parameter_Queue队列创建成功\r\n");
	}
}











static void Motor1_PID_Parameter_QueueCreat(void)
{
	extern QueueHandle_t Motor1_PID_Parameter_Queue;
	Motor1_PID_Parameter_Queue =xQueueCreate(Motor1_PID_Parameter_Q_NUM,sizeof(struct M1_PID *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*消息创建失败处理机制*/
	pr_warn_pure("Motor1_PID_Parameter队列创建失败\r\n");
	pr_entry_pure("Motor1_PID_Parameter队列创建失败\r\n");
	}
	else 
	{
	pr_warn_pure("Motor1_PID_Parameter队列创建成功\r\n");
	pr_entry_pure("Motor1_PID_Parameter队列创建成功\r\n");
	}
}



static void QueueCreate(void)
{

Motor1_Direction_QueueCreate();
Motor1_PWM_QueueCreate();
Motor2_Direction_QueueCreate();
Motor2_PWM_QueueCreate();
WIFI_buffer_QueueCreat();
Encoder1_Overflow_QueueCreat();
Encoder2_Overflow_QueueCreat();
Encoder1_Status_QueueCreat();
Encoder2_Status_QueueCreat();
Motor1_PID_Parameter_QueueCreat();
Motor1_Ctrl_Parameter_QueueCreat();
	

}





/***************信号量创建**************/


static void BinarySemaphoreCreate(void)
{
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//电机1方向更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//电机1速度更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;//电机2方向更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;//电机2速度更改报文二值信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2空闲中断二值信号量句柄
	
	BinarySemaphore_Motor1_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor2_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor2_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
};






void OS_AppObjCreate(void)
{
//消息队列创建
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 







