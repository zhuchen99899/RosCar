#include "Bsp_init.h"


void Bsp_init(void)
{
	
	uart_init(115200);					//初始化串口
	LED_Init();		  					//初始化LED
	Motor_Init();							//电机初始化
	TIM3_PWM_Init(PWM1_MAX_VAL,0);	 //不分频。PWM频率=72000000/1439+1=50khz
	TIM2_PWM_Init(PWM2_MAX_VAL,0);
	Encoder_init();
	wifi_init(115200);					//初始化wifi
	timer6_my_init();
}


