#include "timer_Encoder.h"
	
#define 	Encoder1_period  300

//static void timer_count_init(){
//	
//	
//	
//	
//}

static void Encoder1_init(u32 ENCODER_TIM_PERIOD){
	//�ṹ������
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  //RCCʱ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //GPIO time����
	/*������������������ PA->0   PA->1 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           
	
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  //***Ԥ��Ƶ��ϵͳ***********		
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //***�Զ�����ֵ***************
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //***ʱ�ӷָ�*****************
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//***���ϼ���**************
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	

	/************�������ӿ�ģʽ*****************/
	/************�������ӿ�TIM4��ģʽѡ��IC1+IC2��IC1\IC2����(�����װ)  2��Ƶ ********************/
	/************��װ�Ļ�TIM_ICPolarity_Falling---->TIM_ICPolarity_Rising(����)   2��Ƶ************/
	/************ͬʱ���½��������ؼ��㣬��Ϊ�ı�Ƶģʽ********************************************/
	
  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 9;//ָ�����벶���˲���
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	
	
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�


	
  TIM_Cmd(TIM5, ENABLE); 

}


void Encoder_init(void){
	Encoder1_init(Encoder1_period);
	
}
	


//��ʱ��3�жϷ������
void TIM5_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			pr_warn_pure("�жϴ�ӡ:��ʱ��4������������ж�\r\n");
		}
}



