#include "timer_read_encoder.h"


int last_count=0;
int Overflow_count=0;
int capture_count=0;
float S_K=Perimeter*(1000/timer6_IRQ_time)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate); //速度计算系数
float S_L=Perimeter/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate); //路程系数

void TIM6_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //时钟使能

	//定时器TIM6初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 

//[注意BUG]需要在这加入清除定时器状态函数，否则在第一次中断发生时无法使用ISR函数
	  TIM_ClearFlag(TIM6, TIM_FLAG_Update); 
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;  //TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	


	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //使能指定的TIM6中断,允许更新中断
	TIM_Cmd(TIM6, ENABLE);  //使能TIMx					 
}

void timer6_my_init(void){
	int SetIRQtime=0;
	SetIRQtime=(timer6_IRQ_time*72000/7200)-1; 
	TIM6_Int_Init(SetIRQtime,7199);//10Khz的计数频率，计数到199为20ms  //计算公式 (199+1)(7199+1) / 72000= 20ms

}





//定时器6中断服务程序
void TIM6_IRQHandler(void)   //TIM6中断
{

	extern QueueHandle_t Encoder1_Overflow_Queue;
	extern QueueHandle_t Encoder1_Status_Queue;
	extern QueueHandle_t Motor1_Encoder_speed_Queue;

	


	Encoder1_status_t *Encoder_calculate;

	extern Encoder1_status_t Encoder_struct_init;
	Encoder_calculate = &Encoder_struct_init;
	
	BaseType_t xHigherPriorityTaskWoken;
	

	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)  //检查TIM6更新中断发生与否
		{
			
		last_count=capture_count;




		xQueuePeekFromISR(Encoder1_Overflow_Queue,&Overflow_count);	
//pr_warn_pure("取出Overflow_count %d",Overflow_count);



		/*计算当前总计数，并写入历史总计数消息队列*/
		capture_count=TIM_GetCounter(TIM5)+Overflow_count*Encoder1_period;
		


#if Car_ctrl_speed_loop  		
		/*计算车轮速度*/
		/*速度=小车车轮周长* 单位时间内总计数 *时间系数 / 编码器总分辨率
			@Perimeter 车轮周长: 测量得出,宏定义
			@单位时间内总计数: 当前计数capture_count减去上次计数last_count
			@时间系数 : 定时器时长为i(s)时  时间系数K= 1/ i
			@编码器总分辨率 :编码器线束 * 电机减速比 * 电机齿轮减速(直接连车轮无)*倍频系数
		*/
	//  Encoder_calculate->Encoder1_Speed=fabs(Perimeter*((float)capture_count-(float)last_count)*(1000/20)/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate));
		Encoder_calculate->Encoder1_Speed=fabs(S_K*(capture_count-last_count));

#endif


#if Car_ctrl_length_loop
		/*计算车轮路程*/
		/*小车路程= 累计圈数 *小车车轮周长
		@累计圈数=当前计算计数总数量capture_count/编码器总分辨率
		@Perimeter 车轮周长: 测量得出,宏定义
		*/

		
		//Encoder_calculate->Encoder1_distance=Perimeter*(float)capture_count/(Motor_Reduction_ratio*Encoder1_n_lines*period_rate);
		Encoder_calculate->Encoder1_distance=S_L*capture_count;

#endif

#if Car_ctrl_dirc_inquiry 	
		/*计算车轮方向*/
			
			if((TIM5->CR1>>4 & 0x01)==0){ //DIR==1  //正向

				Encoder_calculate->Encoder1_Direction=1;
			}
      else if((TIM5->CR1>>4 & 0x01)==1){//DIR==2  //反向
				Encoder_calculate->Encoder1_Direction=0;
			}	
		
#endif
			
	xQueueOverwriteFromISR(Encoder1_Status_Queue,(void *)&Encoder_calculate,&xHigherPriorityTaskWoken);	

//		pr_warn_pure("读编码器中断:,速度%f",Encoder_calculate->Encoder1_Speed);
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		
			TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  //清除TIMx更新中断标志
			
				 
		}

}


