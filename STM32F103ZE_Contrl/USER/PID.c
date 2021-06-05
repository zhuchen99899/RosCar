#include "PID.h"
#include "pwm.h"
#include "freertos.h"

extern struct Motor1_Speed_pid{
	
	
	float Set_Speed;    //�趨�ٶ�
	float Actual_Speed;  //ʵ�ʴﵽ���ٶ�
	float err;           //ƫ��ֵ
	float err_last;			//��һ�ε�ƫ��ֵ
	float Kp,Ki,Kd;            //PID����
	float voltage;				//PWMռ�ձȸı�ֵ��PID�㷨�����ֵ
	float integral;            //������
	
}M1_S_PID;


void pid_init()
{
	
	M1_S_PID.Set_Speed=0.0;
	M1_S_PID.Actual_Speed=0.0;
	M1_S_PID.err=0.0;
	M1_S_PID.err_last=0.0;
	M1_S_PID.Kp=0.00;
	M1_S_PID.Ki=0.00;
	M1_S_PID.Kd=0.00;
	M1_S_PID.voltage=0;
	M1_S_PID.integral=0;
}

	

float speed_pid_realize(float setSpeed,float Actual_Speed)
{

	
	M1_S_PID.Set_Speed=setSpeed;
	M1_S_PID.Actual_Speed=Actual_Speed;
	M1_S_PID.err=M1_S_PID.Set_Speed-M1_S_PID.Actual_Speed;
	M1_S_PID.integral+=M1_S_PID.err;
	
	printf("integral=%f",M1_S_PID.integral);
	
	
	M1_S_PID.voltage=M1_S_PID.Kp*M1_S_PID.err+M1_S_PID.Ki*M1_S_PID.integral+M1_S_PID.Kd*(M1_S_PID.err-M1_S_PID.err_last);
	M1_S_PID.err_last=M1_S_PID.err;
	

	
	if(M1_S_PID.voltage>PWM1_MAX_VAL)
	{
	M1_S_PID.voltage=PWM1_MAX_VAL;
	}
	
	return M1_S_PID.voltage;


}



