#include "timer_read_encoder.h"


int last_count=0;
int Overflow_count=0;
int capture_count=0;

 void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //ʱ��ʹ��

	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 

//[ע��BUG]��Ҫ������������ʱ��״̬�����������ڵ�һ���жϷ���ʱ�޷�ʹ��ISR����
	  TIM_ClearFlag(TIM6, TIM_FLAG_Update); 
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	


	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //ʹ��ָ����TIM6�ж�,��������ж�
	TIM_Cmd(TIM6, ENABLE);  //ʹ��TIMx					 
}

void timer6_my_init(void){
	int SetIRQtime=0;
	SetIRQtime=(timer6_IRQ_time*72000/7200)-1; 
	TIM6_Int_Init(SetIRQtime,7199);//10Khz�ļ���Ƶ�ʣ�������199Ϊ20ms  //���㹫ʽ (199+1)(7199+1) / 72000= 20ms

}





//��ʱ��6�жϷ������
void TIM6_IRQHandler(void)   //TIM6�ж�
{
	extern QueueHandle_t Encoder1_last_count_Queue;
	extern QueueHandle_t Encoder1_Overflow_Queue;
	extern QueueHandle_t Encoder1_Status_Queue;
	extern QueueHandle_t Motor1_Speed_Queue;


	BaseType_t xHigherPriorityTaskWoken;

	Encoder1_status_t *Encoder_calculate;
	Encoder1_status_t Encoder_struct_init;
	Encoder_calculate = &Encoder_struct_init;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
		{
			
		

			/*ȡ���ϴμ�¼���ܼ������������*/
		xQueueReceiveFromISR(Encoder1_last_count_Queue,&last_count,&xHigherPriorityTaskWoken);


		xQueuePeekFromISR(Encoder1_Overflow_Queue,&Overflow_count);	
//								pr_warn_pure("ȡ��Overflow_count %d",Overflow_count);



		/*���㵱ǰ�ܼ�������д����ʷ�ܼ�����Ϣ����*/
		capture_count=TIM_GetCounter(TIM5)+Overflow_count*Encoder1_period;
		xQueueSendFromISR(Encoder1_last_count_Queue,&capture_count,&xHigherPriorityTaskWoken);
		/*���㳵���ٶ�*/
		/*�ٶ�=С�������ܳ�* ��λʱ�����ܼ��� *ʱ��ϵ�� / �������ֱܷ���
			@Perimeter �����ܳ�: �����ó�,�궨��
			@��λʱ�����ܼ���: ��ǰ����capture_count��ȥ�ϴμ���last_count
			@ʱ��ϵ�� : ��ʱ��ʱ��Ϊi(s)ʱ  ʱ��ϵ��K= 1/ i
			@�������ֱܷ��� :���������� * ������ٱ� * ������ּ���(ֱ����������)*��Ƶϵ��
		*/
	  Encoder_calculate->Encoder1_Speed=fabs(Perimeter*((float)capture_count-(float)last_count)*(1000/20)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate));

		/*���㳵��·��*/
		/*С��·��= �ۼ�Ȧ�� *С�������ܳ�
		@�ۼ�Ȧ��=��ǰ�������������capture_count/�������ֱܷ���
		@Perimeter �����ܳ�: �����ó�,�궨��
		*/
		
		
		Encoder_calculate->Encoder1_distance=Perimeter*(float)capture_count/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate);
		

		/*���㳵�ַ���*/
			
			if((TIM5->CR1>>4 & 0x01)==0){ //DIR==1  //����

				Encoder_calculate->Encoder1_Direction=1;
			}
      else if((TIM5->CR1>>4 & 0x01)==1){//DIR==2  //����
				Encoder_calculate->Encoder1_Direction=0;
			}	
			
		xQueueOverwriteFromISR(Encoder1_Status_Queue,&Encoder_calculate,&xHigherPriorityTaskWoken);	
			
			
			
//		pr_warn_pure("���������ж�:,�ٶ�%f",Encoder_calculate->Encoder1_Speed);
			
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx�����жϱ�־
			
				 
		}

}



