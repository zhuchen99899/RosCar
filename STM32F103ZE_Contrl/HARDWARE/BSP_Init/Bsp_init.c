#include "Bsp_init.h"


void Bsp_init(void)
{
	u8 imu_init_res;
	

	
	TIM3_PWM_Init(PWM1_MAX_VAL,0);	 //0����Ƶ��PWMƵ��=72000000/1439+1=50khz
	TIM2_PWM_Init(PWM2_MAX_VAL,0);
	uart_init(115200);					//��ʼ������
	
		imu_init_res=MPU_Init();
		if(imu_init_res!=1)
	{
	printf("IMU��ʼ���ɹ�");
	}
	else{
	printf("IMU��ʼ��ʧ��");
	}
	
	
	mpu_dmp_init();

	
	
	LED_Init();		  					//��ʼ��LED
	Motor_Init();							//�����ʼ��

	Encoder_init();
	wifi_init(115200);					//��ʼ��wifi
	timer6_my_init();
	

}


