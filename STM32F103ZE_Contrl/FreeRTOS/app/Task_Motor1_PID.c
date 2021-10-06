#include "Task_Motor1_PID.h"

struct Motor1_Speed_pid{
	
	
	float Set_Speed;    //�趨�ٶ�
	float Actual_Speed;  //ʵ�ʴﵽ���ٶ�
	float err;           //ƫ��ֵ
	float err_last;			//��һ�ε�ƫ��ֵ
	float Kp,Ki,Kd;            //PID����
	float voltage;				//PWMռ�ձȸı�ֵ��PID�㷨�����ֵ
	float integral;            //������
	
}M1_S_PID;

void motor1_PID_task(void *pvParameters)
{
	
extern TaskHandle_t MOTOR1_PID_TASK_Handler;
	
#if Car_PWM_ctrl   
vTaskSuspend(MOTOR1_PID_TASK_Handler);
#endif

	extern QueueHandle_t Encoder1_Status_Queue;

	extern QueueHandle_t Motor1_Ctrl_Parameter_Queue;
	extern QueueHandle_t Motor1_PWM_Queue;
	extern QueueHandle_t Motor1_PID_Parameter_Queue;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	
	
	
	
extern QueueHandle_t Motor1_Encoder_speed_Queue;


	
float Actual_Speed1;
float Set_Speed=0.0;
float pid_result=0.0;
float Motor1_PWM=0.0;




	
Encoder1_status_t *Encoder1_status_send_toPID;
Encoder1_status_t Encoder1_pidcacl_init;	 //ȫ�ֽṹ��
Encoder1_status_send_toPID=&Encoder1_pidcacl_init;



	M1_ctrl *M1_ctrl_r;
	M1_ctrl M1_ctrl_init;		
	
	M1_PID *M1_speed_PID_wifiset;
	M1_PID M1_speed_PID_wifiset_init;


M1_ctrl_r=&M1_ctrl_init;	
M1_speed_PID_wifiset=&M1_speed_PID_wifiset_init;
	
pid_init();

	while(1)
	{


		//������1
		xQueuePeek(Encoder1_Status_Queue,(void *)&Encoder1_status_send_toPID,10);
		xQueuePeek(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_r,10);	
		xQueuePeek(Motor1_PID_Parameter_Queue,&M1_speed_PID_wifiset,10);
		
		//������1
	
		
		Actual_Speed1 = Encoder1_status_send_toPID->Encoder1_Speed;

		
//		pr_warn_pure("Actual_Speed1=%f",Actual_Speed1);

		Set_Speed= M1_ctrl_r->Speed;
		
	M1_S_PID.Kp=M1_speed_PID_wifiset->Kp;
	M1_S_PID.Ki=M1_speed_PID_wifiset->Ki;
	M1_S_PID.Kd=M1_speed_PID_wifiset->Kd;
		


		pid_result=speed_pid_realize1(Set_Speed,Actual_Speed1);

//		pr_warn_pure("pid_result1=%f",pid_result);
		Motor1_PWM=fabs(pid_result/PWM1_MAX_VAL);

		xQueueOverwrite(Motor1_PWM_Queue,&Motor1_PWM);	
		xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//���͵��1�ٶȸ��ı����ź�		
		


		
		vTaskDelay(100);
		
		
	}




}


