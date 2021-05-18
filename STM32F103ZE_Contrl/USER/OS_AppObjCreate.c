#include <OS_AppObjCreate.h>

/***************��Ϣ���д���**************/


#define KEY_Q_NUM    1  		//������Ϣ���е�����  
#define Motor1_Direction_Q_NUM 1//���1������Ϣ���е�����  
#define Motor1_Speed_Q_NUM 1// ���1�ٶ���Ϣ���е�����
#define WIFIMESSAGE_buffer_Q_NUM   1   	//wifi�������ݵ���Ϣ���е����� 


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
	
Motor1_Direction_Queue =xQueueCreate(Motor1_Direction_Q_NUM,sizeof(Motor1_Direction_t));
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
static void Motor1_Speed_QueueCreate(void)
{
extern QueueHandle_t Motor1_Speed_Queue;

Motor1_Speed_Queue =xQueueCreate(Motor1_Speed_Q_NUM,sizeof(float));
	if (Motor1_Speed_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	pr_warn_pure("���1�ٶ���Ϣ���д���ʧ��\r\n");
	pr_entry_pure("���1�ٶ���Ϣ���д���ʧ��\r\n");

	}
	else 
	{
	pr_warn_pure("���1�ٶ���Ϣ���д����ɹ�\r\n");
	pr_entry_pure("���1�ٶ���Ϣ���д����ɹ�\r\n");

	}
	

}

void WIFI_buffer_QueueCreat(void)
{
	extern QueueHandle_t Wifi_buffer_Queue;
	Wifi_buffer_Queue =xQueueCreate(WIFIMESSAGE_buffer_Q_NUM,sizeof(struct WIFIMSG *));
	
	if (Wifi_buffer_Queue==0)
	{
	/*��Ϣ����ʧ�ܴ������*/
	printf("WIFI������Ϣ���д���ʧ��");
	}
	else 
	{
	printf("WIFI������Ϣ���д����ɹ�");
	}
}




static void QueueCreate(void)
{

Key_QueueCreate();
Motor1_Direction_QueueCreate();
Motor1_Speed_QueueCreate();
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

 







