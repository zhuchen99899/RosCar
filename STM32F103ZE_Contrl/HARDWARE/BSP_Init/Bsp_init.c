#include "Bsp_init.h"


void Bsp_init(void)
{
	
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	Motor_Init();							//�����ʼ��
	TIM3_PWM_Init(PWM1_MAX_VAL,0);	 //����Ƶ��PWMƵ��=72000000/1439+1=50khz
	TIM2_PWM_Init(PWM2_MAX_VAL,0);
	Encoder_init();
	wifi_init(115200);					//��ʼ��wifi
	timer6_my_init();
}


