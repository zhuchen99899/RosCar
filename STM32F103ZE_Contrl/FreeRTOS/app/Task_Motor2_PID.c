#include "Task_Motor2_PID.h"

struct Motor2_Speed_pid{
	
	
	float Set_Speed;    //�趨�ٶ�
	float Actual_Speed;  //ʵ�ʴﵽ���ٶ�
	float err;           //ƫ��ֵ
	float err_last;			//��һ�ε�ƫ��ֵ
	float Kp,Ki,Kd;            //PID����
	float voltage;				//PWMռ�ձȸı�ֵ��PID�㷨�����ֵ
	float integral;            //������
	
}M2_S_PID;



void motor2_PID_task(void *pvParameters)
{
	
extern TaskHandle_t MOTOR2_PID_TASK_Handler;
	
#if Car_PWM_ctrl   
vTaskSuspend(MOTOR2_PID_TASK_Handler);
#endif


	extern QueueHandle_t Encoder2_Status_Queue;
	extern QueueHandle_t Motor2_Ctrl_Parameter_Queue;
	extern QueueHandle_t Motor2_PWM_Queue;
	extern QueueHandle_t Motor2_PID_Parameter_Queue;
	extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;
	
	
	
	
extern QueueHandle_t Motor2_Encoder_speed_Queue;


	
float Actual_Speed2;
float Set_Speed=0.0;
float pid_result=0.0;
float Motor2_PWM=0.0;




	
Encoder2_status_t *Encoder2_status_send_toPID;
Encoder2_status_t Encoder2_pidcacl_init;	 //ȫ�ֽṹ��
Encoder2_status_send_toPID=&Encoder2_pidcacl_init;


	M2_ctrl *M2_ctrl_r;
	M2_ctrl M2_ctrl_init;		
	
	M2_PID *M2_speed_PID_wifiset;
	M2_PID M2_speed_PID_wifiset_init;


M2_ctrl_r=&M2_ctrl_init;	
M2_speed_PID_wifiset=&M2_speed_PID_wifiset_init;
	
pid_init();

	while(1)
	{


		//������2
		xQueuePeek(Encoder2_Status_Queue,(void *)&Encoder2_status_send_toPID,10);
		xQueuePeek(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_r,10);	
		xQueuePeek(Motor2_PID_Parameter_Queue,&M2_speed_PID_wifiset,portMAX_DELAY);
	

		Actual_Speed2 = Encoder2_status_send_toPID->Encoder2_Speed;
		
		pr_warn_pure("Actual_Speed2=%f",Actual_Speed2);

		Set_Speed= M2_ctrl_r->Speed;
		
	M2_S_PID.Kp=M2_speed_PID_wifiset->Kp;
	M2_S_PID.Ki=M2_speed_PID_wifiset->Ki;
	M2_S_PID.Kd=M2_speed_PID_wifiset->Kd;
		


		pid_result=speed_pid_realize2(Set_Speed,Actual_Speed2);

//		pr_warn_pure("pid_result2=%f",pid_result);
		Motor2_PWM=pid_result/PWM2_MAX_VAL;
		
//		pr_warn_pure("Motor2_PWM=%f",Motor2_PWM);
		xQueueOverwrite(Motor2_PWM_Queue,&Motor2_PWM);	
		xSemaphoreGive(BinarySemaphore_Motor2_SpeedChange);//���͵��2�ٶȸ��ı����ź�		
		


		
		vTaskDelay(100);
		
		
	}




}


