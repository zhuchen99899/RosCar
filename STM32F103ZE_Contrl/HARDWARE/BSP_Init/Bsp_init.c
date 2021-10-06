#include "Bsp_init.h"


void Bsp_init(void)
{
	u8 imu_init_res;
	

	
	TIM3_PWM_Init(PWM1_MAX_VAL,0);	 //0不分频。PWM频率=72000000/1439+1=50khz
	TIM2_PWM_Init(PWM2_MAX_VAL,0);
	uart_init(115200);					//初始化串口
	
		imu_init_res=MPU_Init();
		if(imu_init_res!=1)
	{
	printf("IMU初始化成功");
	}
	else{
	printf("IMU初始化失败");
	}
	
	
	mpu_dmp_init();

	
	
	LED_Init();		  					//初始化LED
	Motor_Init();							//电机初始化

	Encoder_init();
	wifi_init(115200);					//初始化wifi
	timer6_my_init();
	

}


