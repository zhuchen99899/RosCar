#include "timer_Encoder.h"
	
int overflow_count_Encoder1=0;
int overflow_count_Encoder2=0;



static void Encoder1_init(u32 ENCODER_TIM_PERIOD,u16 TIM_ICPolarity_Mode1,u16 TIM_ICPolarity_Mode2){
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
	
	
	/*- TIM5������ģʽ���� -*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  //***Ԥ��Ƶ��ϵͳ***********		
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //***�Զ�����ֵ***************
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //***ʱ�ӷָ�*****************
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//***���ϼ���**************
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	

	/************�������ӿ�ģʽ*****************/
	/************�������ӿ�TIM5��ģʽѡ��IC1+IC2��IC1\IC2����(�����װ)  2��Ƶ ********************/
	/************��װ�Ļ�TIM_ICPolarity_Falling---->TIM_ICPolarity_Rising(����)   2��Ƶ************/
	/************ͬʱ���½��������ؼ��㣬��Ϊ�ı�Ƶģʽ********************************************/

  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Mode1 ,TIM_ICPolarity_Mode2);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 9;//ָ�����벶���˲���
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_SetCounter(TIM5,0);

	
	
	
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	


	
  TIM_Cmd(TIM5, ENABLE); 
	TIM_SetCounter(TIM5,0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM5�ж�,��������ж�
}




 static void Encoder2_Init(u32 ENCODER_TIM_PERIOD,u16 TIM_ICPolarity_Mode1,u16 TIM_ICPolarity_Mode2)
 {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
   

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*- ������������������ PB->6   PB->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM4������ģʽ���� -*/

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;//
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;//�ƴ���;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
								 
		/************�������ӿ�ģʽ*****************/
	/************�������ӿ�TIM4��ģʽѡ��IC1+IC2��IC1\IC2����(�����װ)  2��Ƶ ********************/
	/************��װ�Ļ�TIM_ICPolarity_Falling---->TIM_ICPolarity_Rising(����)   2��Ƶ************/
	/************ͬʱ���½��������ؼ��㣬��Ϊ�ı�Ƶģʽ********************************************/

  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Mode1 ,TIM_ICPolarity_Mode2);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 9;//ָ�����벶���˲���
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_SetCounter(TIM4,0);
							 
	
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	
	


	
  TIM_Cmd(TIM4, ENABLE); 
	TIM_SetCounter(TIM4,0);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�
}
		


void Encoder_init(void){
	u32 time_period=0;
	u16 period_mode_1=0,period_mode_2=0;

	

	
	
	switch(period_mode){
		case 0:
		period_mode_1=TIM_ICPolarity_Rising;
		period_mode_1=TIM_ICPolarity_Falling;
		break;
		case 1:
		period_mode_1=TIM_ICPolarity_Falling;
		period_mode_1=TIM_ICPolarity_Rising;
		break;
			
		case 2:
		period_mode_1=TIM_ICPolarity_Rising;
		period_mode_1=TIM_ICPolarity_Rising;	
		break;
		case 3:
		period_mode_1=TIM_ICPolarity_Falling;
		period_mode_1=TIM_ICPolarity_Falling;	
		break;
		case 4: //�ı�Ƶģʽ
		period_mode_1=TIM_ICPolarity_BothEdge;
		period_mode_1=TIM_ICPolarity_BothEdge;
		break;
	}
	
	time_period=Encoder1_period; //��ʱ�����ֵ
	
	/*�趨��ʱ��������ģʽ*/
	Encoder1_init(time_period,period_mode_1,period_mode_2);
	Encoder2_Init(time_period,period_mode_1,period_mode_2);

}
	


//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM5�ж�
{
extern QueueHandle_t Encoder1_Overflow_Queue;

BaseType_t xHigherPriorityTaskWoken;
	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{

			if((TIM5->CR1>>4 & 0x01)==0){ //DIR==1  //����
				overflow_count_Encoder1++;
				xQueueOverwriteFromISR(Encoder1_Overflow_Queue,&overflow_count_Encoder1,&xHigherPriorityTaskWoken);
//				pr_warn_pure("�жϴ�ӡ������1,Overflow_count=%d\r\n",overflow_count_Encoder1);
			}
      else if((TIM5->CR1>>4 & 0x01)==1){//DIR==2  //����
				overflow_count_Encoder1--;
				xQueueOverwriteFromISR(Encoder1_Overflow_Queue,&overflow_count_Encoder1,&xHigherPriorityTaskWoken);
//				pr_warn_pure("�жϴ�ӡ������2,Overflow_count=%d\r\n",overflow_count_Encoder1);		
			}
			


			
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		}
		


}

//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
extern QueueHandle_t Encoder2_Overflow_Queue;

BaseType_t xHigherPriorityTaskWoken;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{

			if((TIM4->CR1>>4 & 0x01)==0){ //DIR==1  //����
				overflow_count_Encoder2++;
				xQueueOverwriteFromISR(Encoder2_Overflow_Queue,&overflow_count_Encoder2,&xHigherPriorityTaskWoken);
//				pr_warn_pure("�жϴ�ӡ������1,Overflow_count=%d\r\n",overflow_count_Encoder1);
			}
      else if((TIM4->CR1>>4 & 0x01)==1){//DIR==2  //����
				overflow_count_Encoder2--;
				xQueueOverwriteFromISR(Encoder2_Overflow_Queue,&overflow_count_Encoder2,&xHigherPriorityTaskWoken);
//				pr_warn_pure("�жϴ�ӡ������2,Overflow_count=%d\r\n",overflow_count_Encoder1);		
			}
			


			
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־ 
		}
		


}


