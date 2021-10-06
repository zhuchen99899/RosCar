#include "PID.h"
#include "pwm.h"
#include "freertos.h"

extern struct Motor1_Speed_pid{
	
	
	float Set_Speed;    //设定速度
	float Actual_Speed;  //实际达到的速度
	float err;           //偏差值
	float err_last;			//上一次的偏差值
	float Kp,Ki,Kd;            //PID参数
	float voltage;				//PWM占空比改变值，PID算法的输出值
	float integral;            //积分项
	
}M1_S_PID;


extern struct Moto2_Speed_pid{

	float Set_Speed;    //设定速度
	float Actual_Speed;  //实际达到的速度
	float err;           //偏差值
	float err_last;			//上一次的偏差值
	float Kp,Ki,Kd;            //PID参数
	float voltage;				//PWM占空比改变值，PID算法的输出值
	float integral;            //积分项


}M2_S_PID;


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
	
	M2_S_PID.Set_Speed=0.0;
	M2_S_PID.Actual_Speed=0.0;
	M2_S_PID.err=0.0;
	M2_S_PID.err_last=0.0;
	M2_S_PID.Kp=0.00;
	M2_S_PID.Ki=0.00;
	M2_S_PID.Kd=0.00;
	M2_S_PID.voltage=0;
	M2_S_PID.integral=0;
}

	

float speed_pid_realize1(float setSpeed,float Actual_Speed)
{

	
	M1_S_PID.Set_Speed=setSpeed;
	M1_S_PID.Actual_Speed=Actual_Speed;
	M1_S_PID.err=M1_S_PID.Set_Speed-M1_S_PID.Actual_Speed;
	M1_S_PID.integral+=M1_S_PID.err;
//	printf("err=%f",M1_S_PID.err);
//	printf("integral=%f",M1_S_PID.integral);
	
//	printf("D:%f",(M1_S_PID.err-M1_S_PID.err_last));
	M1_S_PID.voltage=M1_S_PID.Kp*M1_S_PID.err+M1_S_PID.Ki*M1_S_PID.integral+M1_S_PID.Kd*(M1_S_PID.err-M1_S_PID.err_last);
	M1_S_PID.err_last=M1_S_PID.err;
	

	
	if(M1_S_PID.voltage>PWM1_MAX_VAL)
	{
	M1_S_PID.voltage=PWM1_MAX_VAL;
	}
	else if(M1_S_PID.voltage<0)
	{
	M1_S_PID.voltage=0;
	
	}
	
	return M1_S_PID.voltage;


}


float speed_pid_realize2(float setSpeed,float Actual_Speed)
{

	
	M2_S_PID.Set_Speed=setSpeed;
	M2_S_PID.Actual_Speed=Actual_Speed;
	M2_S_PID.err=M2_S_PID.Set_Speed-M2_S_PID.Actual_Speed;
	M2_S_PID.integral+=M2_S_PID.err;
	
//	printf("integral=%f",M2_S_PID.integral);
//	printf("err=%f",M2_S_PID.err);
//	printf("err=%f",M2_S_PID.err-M2_S_PID.err_last);
	
	M2_S_PID.voltage=M2_S_PID.Kp*M2_S_PID.err+M2_S_PID.Ki*M2_S_PID.integral+M2_S_PID.Kd*(M2_S_PID.err-M2_S_PID.err_last);
	
	M2_S_PID.err_last=M2_S_PID.err;
	

	
	if(M2_S_PID.voltage>PWM2_MAX_VAL)
	{
	M2_S_PID.voltage=PWM2_MAX_VAL;
	}
	
	else if(M2_S_PID.voltage<0)
	{
	M2_S_PID.voltage=0;
		
	
	}
//	printf("M2_S_PID:%f",M2_S_PID.voltage);
	return M2_S_PID.voltage;



}



