#include "Bsp_init.h"


void Bsp_init(void)
{
	
	uart_init(115200);					//��ʼ������
	LED_Init();		  					//��ʼ��LED
	Motor_Init();							//�����ʼ��
	TIM3_PWM_Init(899,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	Encoder_init();
	wifi_init(115200);					//��ʼ��wifi
	timer6_my_init();
}


