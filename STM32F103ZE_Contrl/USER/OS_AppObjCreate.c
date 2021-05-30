#include <OS_AppObjCreate.h>

/***************��Ϣ���д���**************/


#define KEY_Q_NUM    1  		//������Ϣ���е�����  
#define Motor1_Direction_Q_NUM 1//���1������Ϣ���е�����  
#define Motor1_PWM_Q_NUM 1// ���1�ٶ���Ϣ���е�����
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 
#define Encoder1_Overflow_Q_NUM 1  //���������������������
#define Encoder1_Status_Q_NUM 1 //������״̬����
#define Encoder1_last_count_Q_NUM 5 //���������������������
//������Ϣ����
static void Key_QueueCreate(void)
{
	extern QueueHandle_t Key_Queue;
	Key_Queue =xQueueCreate(KEY_Q_NUM,sizeof(uint8_t));
	
	if (Key_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("������Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("������Ϣ���д����ɹ�\r\n");
	}
}

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




static void Encoder1_last_count_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_last_count_Queue;
	Encoder1_last_count_Queue =xQueueCreate(Encoder1_last_count_Q_NUM,sizeof(int));
	
	if (Encoder1_last_count_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("Encoder1����������Ϣ���д���ʧ��\r\n");
	pr_entry_pure("Encoder1����������Ϣ���д���ʧ��\r\n");
	}
	else 
	{
	pr_warn_pure("Encoder1����������Ϣ���д����ɹ�\r\n");
	pr_entry_pure("Encoder1����������Ϣ���д����ɹ�\r\n");
	}
}


static void Encoder1_Status_QueueCreat(void)
{
	extern QueueHandle_t Encoder1_Status_Queue;
	Encoder1_Status_Queue =xQueueCreate(Encoder1_Status_Q_NUM,sizeof(struct Encoder1_status *));
	
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





/***************�ź�������**************/


static void BinarySemaphoreCreate(void)
{
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;//���1������ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;//���1�ٶȸ��ı��Ķ�ֵ�ź������
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//USART2�����ж϶�ֵ�ź������
	
	BinarySemaphore_Motor1_DirChange=xSemaphoreCreateBinary();
	BinarySemaphore_Motor1_SpeedChange=xSemaphoreCreateBinary();
	BinarySemaphore_USART2ISR=xSemaphoreCreateBinary();	
};






void OS_AppObjCreate(void)
{
//��Ϣ���д���
QueueCreate();
BinarySemaphoreCreate();

	
	
}

 







