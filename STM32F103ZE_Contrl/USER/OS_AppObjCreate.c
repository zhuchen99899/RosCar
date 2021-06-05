#include <OS_AppObjCreate.h>

/*ȫ�ֽṹ��*/
//������Ϣ���д���ָ��
Encoder1_status_t Encoder1_struct_init; //ȫ�ֽṹ��
Encoder2_status_t Encoder2_struct_init;
M1_ctrl   Motor1_ctrl_struct_init;
M1_PID    Motor1_PID_struct_init;
Motor1_Direction_t  Motor1_Direction_struct_init;
Motor2_Direction_t  Motor2_Direction_struct_init;
wifibuff wifibuff_struct_init;





/***************��Ϣ���д���**************/






#define Motor1_Direction_Q_NUM 1//���1������Ϣ���е�����  
#define Motor1_PWM_Q_NUM 1// ���1�ٶ���Ϣ���е�����
#define Motor2_Direction_Q_NUM 1//���1������Ϣ���е�����  
#define Motor2_PWM_Q_NUM 1// ���1�ٶ���Ϣ���е�����


#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 
#define Encoder1_Overflow_Q_NUM 1  //���������������������
#define Encoder2_Overflow_Q_NUM 1  //���������������������
#define Encoder1_Status_Q_NUM 1 //������״̬����
#define Encoder2_Status_Q_NUM 1
#define Motor1_PID_Parameter_Q_NUM 1
#define Motor1_Ctrl_Parameter_Q_NUM  1


//���1��������Ϣ����
static void Motor1_Direction_QueueCreate(void)
{
extern QueueHandle_t Motor1_Direction_Queue;
	
Motor1_Direction_Queue =xQueueCreate(Motor1_Direction_Q_NUM,sizeof(Motor1_Direction_t *));
	if (Motor1_Direction_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("���1������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("���1������Ϣ���д���ʧ��\r\n");

	}
	else 
	{
	pr_warn_pure("���1������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("���1������Ϣ���д����ɹ�\r\n");

	}
	

}

//���1�ٶ���Ϣ����
static void Motor1_PWM_QueueCreate(void)
{
extern QueueHandle_t Motor1_PWM_Queue;

Motor1_PWM_Queue =xQueueCreate(Motor1_PWM_Q_NUM,sizeof(float));
	if (Motor1_PWM_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("���1PWM��Ϣ���д���ʧ��\r\n");
	pr_entry_pure("���1PWM��Ϣ���д���ʧ��\r\n");

	}
	else 
	{
	pr_warn_pure("���1PWM��Ϣ���д����ɹ�\r\n");
	pr_entry_pure("���1PWM��Ϣ���д����ɹ�\r\n");

	}
	

}


//���2��������Ϣ����
static void Motor2_Direction_QueueCreate(void)
{
extern QueueHandle_t Motor2_Direction_Queue;
	
Motor2_Direction_Queue =xQueueCreate(Motor2_Direction_Q_NUM,sizeof(Motor2_Direction_t *));
	if (Motor2_Direction_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("���2������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("���2������Ϣ���д���ʧ��\r\n");

	}
	else 
	{
	pr_warn_pure("���2������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("���2������Ϣ���д����ɹ�\r\n");

	}
	

}

//���2�ٶ���Ϣ����
static void Motor2_PWM_QueueCreate(void)
{
extern QueueHandle_t Motor2_PWM_Queue;

Motor2_PWM_Queue =xQueueCreate(Motor2_PWM_Q_NUM,sizeof(float));
	if (Motor2_PWM_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("���2PWM��Ϣ���д���ʧ��\r\n");
	pr_entry_pure("���2PWM��Ϣ���д���ʧ��\r\n");

	}
	else 
	{
	pr_warn_pure("���2PWM��Ϣ���д����ɹ�\r\n");
	pr_entry_pure("���2PWM��Ϣ���д����ɹ�\r\n");

	}
	

}










static void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct wifibuff *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("WIFI������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("WIFI������Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("WIFI������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("WIFI������Ϣ���д����ɹ�\r\n");
	}
}




static void Encoder1_Overflow_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Overflow_Queue;
	Encoder1_Overflow_Queue =xQueueCreate(Encoder1_Overflow_Q_NUM,sizeof(int));
	
	if (Encoder1_Overflow_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Encoder1_���������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("Encoder1_���������Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1_���������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("Encoder1_���������Ϣ���д����ɹ�\r\n");
	}
}


static void Encoder2_Overflow_QueueCreat(void)
{
	extern QueueHandle_t Encoder2_Overflow_Queue;
	Encoder2_Overflow_Queue =xQueueCreate(Encoder2_Overflow_Q_NUM,sizeof(int));
	
	if (Encoder2_Overflow_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Encoder2_���������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("Encoder2_���������Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder2_���������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("Encoder2_���������Ϣ���д����ɹ�\r\n");
	}
}



static void Encoder1_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Status_Queue;
	Encoder1_Status_Queue =xQueueCreate(Encoder1_Status_Q_NUM,sizeof(struct Encoder1_status_t *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Encoder1_Status��Ϣ���д���ʧ��\r\n");
	pr_entry_pure("Encoder1_Status��Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1_Status��Ϣ���д����ɹ�\r\n");
	pr_entry_pure("Encoder1_Status��Ϣ���д����ɹ�\r\n");
	}
}

static void Encoder2_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder2_Status_Queue;
	Encoder2_Status_Queue =xQueueCreate(Encoder2_Status_Q_NUM,sizeof(struct Encoder2_status_t *));
	
	if (Encoder2_Status_QueueCreat==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Encoder2_Status��Ϣ���д���ʧ��\r\n");
	pr_entry_pure("Encoder2_Status��Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder2_Status��Ϣ���д����ɹ�\r\n");
	pr_entry_pure("Encoder2_Status��Ϣ���д����ɹ�\r\n");
	}
}



//���Ʋ�����Ϣ����


static void Motor1_Ctrl_Parameter_QueueCreat(void)
{
	extern QueueHandle_t Motor1_Ctrl_Parameter_Queue;
	Motor1_Ctrl_Parameter_Queue =xQueueCreate(Motor1_Ctrl_Parameter_Q_NUM,sizeof(struct M1_ctrl *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Ctrl_Parameter_Queue���д���ʧ��\r\n");
	pr_entry_pure("Ctrl_Parameter_Queue���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Ctrl_Parameter_Queue���д����ɹ�\r\n");
	pr_entry_pure("Ctrl_Parameter_Queue���д����ɹ�\r\n");
	}
}











static void Motor1_PID_Parameter_QueueCreat(void)
{
	extern QueueHandle_t Motor1_PID_Parameter_Queue;
	Motor1_PID_Parameter_Queue =xQueueCreate(Motor1_PID_Parameter_Q_NUM,sizeof(struct M1_PID *));
	
	if (Encoder1_Status_QueueCreat==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Motor1_PID_Parameter���д���ʧ��\r\n");
	pr_entry_pure("Motor1_PID_Parameter���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Motor1_PID_Parameter���д����ɹ�\r\n");
	pr_entry_pure("Motor1_PID_Parameter���д����ɹ�\r\n");
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





/***************�ź�������**************/


static void BinarySemaphoreCreate(void)
{
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//���1������ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//���1�ٶȸ��ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;//���2������ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;//���2�ٶȸ��ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
	
	BinarySemaphore_Motor1_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor2_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor2_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
};






void OS_AppObjCreate(void)
{
//��Ϣ���д���
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 







