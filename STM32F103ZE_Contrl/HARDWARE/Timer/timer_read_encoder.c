#include "timer_read_encoder.h"


int Encoder1_last_count=0;
int Encoder1_Overflow_count=0;
int Encoder1_capture_count=0;


int Encoder2_last_count=0;
int Encoder2_Overflow_count=0;
int Encoder2_capture_count=0;

//���1��2ʹ�ñ�������ͬ
float S_K=Perimeter*(1000/timer6_IRQ_time)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate); //�ٶȼ���ϵ��
float S_L=Perimeter/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate); //·��ϵ��



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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //��ռ���ȼ�0��
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

	extern QueueHandle_t Encoder1_Overflow_Queue;
	extern QueueHandle_t Encoder2_Overflow_Queue;
	extern QueueHandle_t Encoder1_Status_Queue;
	extern QueueHandle_t Encoder2_Status_Queue;
	
	Encoder1_status_t *Encoder1_calculate;
	extern Encoder1_status_t Encoder1_struct_init;
	Encoder1_calculate = &Encoder1_struct_init;
	
	Encoder2_status_t *Encoder2_calculate;
	extern Encoder2_status_t Encoder2_struct_init;
	Encoder2_calculate = &Encoder2_struct_init;
	
	BaseType_t xHigherPriorityTaskWoken;
	

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //���TIM6�����жϷ������
		{
			
		Encoder1_last_count=Encoder1_capture_count;
		Encoder2_last_count=Encoder2_capture_count;


		xQueuePeekFromISR(Encoder1_Overflow_Queue,&Encoder1_Overflow_count);	
		xQueuePeekFromISR(Encoder2_Overflow_Queue,&Encoder2_Overflow_count);	
//pr_warn_pure("ȡ��Overflow_count2 %d",Encoder2_Overflow_count);



		/*���㵱ǰ�ܼ�������д����ʷ�ܼ�����Ϣ����*/
		Encoder1_capture_count=TIM_GetCounter(TIM5)+Encoder1_Overflow_count*Encoder1_period;
		Encoder2_capture_count=TIM_GetCounter(TIM4)+Encoder2_Overflow_count*Encoder2_period;

//	pr_warn_pure("Encoder1_last_count=%d,Encoder1_capture_count=%d\r\n",Encoder1_last_count,Encoder1_capture_count);		
//	pr_warn_pure("Encoder2_last_count=%d,Encoder2_capture_count=%d\r\n",Encoder2_last_count,Encoder2_capture_count);

			

#if Car_ctrl_speed_loop  		
		/*���㳵���ٶ�*/
		/*�ٶ�=С�������ܳ�* ��λʱ�����ܼ��� *ʱ��ϵ�� / �������ֱܷ���
			@Perimeter �����ܳ�: �����ó�,�궨��
			@��λʱ�����ܼ���: ��ǰ����capture_count��ȥ�ϴμ���last_count
			@ʱ��ϵ�� : ��ʱ��ʱ��Ϊi(s)ʱ  ʱ��ϵ��K= 1/ i
			@�������ֱܷ��� :���������� * ������ٱ� * ������ּ���(ֱ����������)*��Ƶϵ��
		*/
	//  Encoder_calculate->Encoder1_Speed=fabs(Perimeter*((float)capture_count-(float)last_count)*(1000/20)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate));
		Encoder1_calculate->Encoder1_Speed=fabs(S_K*(Encoder1_capture_count-Encoder1_last_count));
		Encoder2_calculate->Encoder2_Speed=fabs(S_K*(Encoder2_capture_count-Encoder2_last_count));
#endif


#if Car_ctrl_length_loop
		/*���㳵��·��*/
		/*С��·��= �ۼ�Ȧ�� *С�������ܳ�
		@�ۼ�Ȧ��=��ǰ�������������capture_count/�������ֱܷ���
		@Perimeter �����ܳ�: �����ó�,�궨��
		*/

		
		//Encoder_calculate->Encoder1_distance=Perimeter*(float)Encoder1_capture_count/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate);
		Encoder1_calculate->Encoder1_distance=S_L*Encoder1_capture_count;
		Encoder2_calculate->Encoder2_distance=S_L*Encoder2_capture_count;
#endif

#if Car_ctrl_dirc_inquiry 	
		/*���㳵�ַ���*/
			
			if((TIM5->CR1>>4 & 0x01)==0){ //DIR==1  //����

				Encoder1_calculate->Encoder1_Direction=1;
				Encoder2_calculate->Encoder2_Direction=1;
			}
      else if((TIM5->CR1>>4 & 0x01)==1){//DIR==2  //����
				Encoder1_calculate->Encoder1_Direction=0;
				Encoder2_calculate->Encoder2_Direction=0;
			}	
		
#endif
			
xQueueOverwriteFromISR(Encoder1_Status_Queue,(void *)&Encoder1_calculate,&xHigherPriorityTaskWoken);	
xQueueOverwriteFromISR(Encoder2_Status_Queue,(void *)&Encoder2_calculate,&xHigherPriorityTaskWoken);	

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //���TIMx�����жϱ�־
			
				 
		}

}


