#include "timer_Encoder.h"
	
int overflow_count_Encoder1=0;
int overflow_count_Encoder2=0;



static void Encoder1_init(u32 ENCODER_TIM_PERIOD,u16 TIM_ICPolarity_Mode1,u16 TIM_ICPolarity_Mode2){
	//结构体声明
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
  TIM_ICInitTypeDef TIM_ICInitStructure;  
  GPIO_InitTypeDef GPIO_InitStructure;
  //RCC时钟

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //GPIO time复用
	/*正交编码器输入引脚 PA->0   PA->1 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                           
	
	
	/*- TIM5编码器模式配置 -*/
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  //***预分频率系统***********		
  TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //***自动重载值***************
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //***时钟分割*****************
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//***向上计数**************
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  
	

	/************编码器接口模式*****************/
	/************编码器接口TIM5、模式选择IC1+IC2、IC1\IC2反向(电机反装)  2倍频 ********************/
	/************正装的话TIM_ICPolarity_Falling---->TIM_ICPolarity_Rising(正相)   2倍频************/
	/************同时对下降沿上升沿计算，即为四倍频模式********************************************/

  TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Mode1 ,TIM_ICPolarity_Mode2);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 9;//指定输入捕获滤波器
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_SetCounter(TIM5,0);

	
	
	
  TIM_ClearFlag(TIM5, TIM_FLAG_Update);
//  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	


	
  TIM_Cmd(TIM5, ENABLE); 
	TIM_SetCounter(TIM5,0);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM5中断,允许更新中断
}




 static void Encoder2_Init(u32 ENCODER_TIM_PERIOD,u16 TIM_ICPolarity_Mode1,u16 TIM_ICPolarity_Mode2)
 {

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
   

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*- 正交编码器输入引脚 PB->6   PB->7 -*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                           

	/*- TIM4编码器模式配置 -*/

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;//
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;//计次数;
	TIM_TimeBaseStructure.TIM_ClockDivision =TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);              
                 
								 
		/************编码器接口模式*****************/
	/************编码器接口TIM4、模式选择IC1+IC2、IC1\IC2反向(电机反装)  2倍频 ********************/
	/************正装的话TIM_ICPolarity_Falling---->TIM_ICPolarity_Rising(正相)   2倍频************/
	/************同时对下降沿上升沿计算，即为四倍频模式********************************************/

  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Mode1 ,TIM_ICPolarity_Mode2);
//  TIM_ICStructInit(&TIM_ICInitStructure);
//  TIM_ICInitStructure.TIM_ICFilter = 9;//指定输入捕获滤波器
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_SetCounter(TIM4,0);
							 
	
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);
//  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	
	


	
  TIM_Cmd(TIM4, ENABLE); 
	TIM_SetCounter(TIM4,0);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断
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
		case 4: //四倍频模式
		period_mode_1=TIM_ICPolarity_BothEdge;
		period_mode_1=TIM_ICPolarity_BothEdge;
		break;
	}
	
	time_period=Encoder1_period; //定时器溢出值
	
	/*设定定时器编码器模式*/
	Encoder1_init(time_period,period_mode_1,period_mode_2);
	Encoder2_Init(time_period,period_mode_1,period_mode_2);

}
	


//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
extern QueueHandle_t Encoder1_Overflow_Queue;

BaseType_t xHigherPriorityTaskWoken;
	
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{

			if((TIM5->CR1>>4 & 0x01)==0){ //DIR==1  //上溢
				overflow_count_Encoder1++;
				xQueueOverwriteFromISR(Encoder1_Overflow_Queue,&overflow_count_Encoder1,&xHigherPriorityTaskWoken);
//				pr_warn_pure("中断打印：方向1,Overflow_count=%d\r\n",overflow_count_Encoder1);
			}
      else if((TIM5->CR1>>4 & 0x01)==1){//DIR==2  //下溢
				overflow_count_Encoder1--;
				xQueueOverwriteFromISR(Encoder1_Overflow_Queue,&overflow_count_Encoder1,&xHigherPriorityTaskWoken);
//				pr_warn_pure("中断打印：方向2,Overflow_count=%d\r\n",overflow_count_Encoder1);		
			}
			


			
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);  //清除TIMx更新中断标志 
		}
		


}

//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
extern QueueHandle_t Encoder2_Overflow_Queue;

BaseType_t xHigherPriorityTaskWoken;
	
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{

			if((TIM4->CR1>>4 & 0x01)==0){ //DIR==1  //上溢
				overflow_count_Encoder2++;
				xQueueOverwriteFromISR(Encoder2_Overflow_Queue,&overflow_count_Encoder2,&xHigherPriorityTaskWoken);
//				pr_warn_pure("中断打印：方向1,Overflow_count=%d\r\n",overflow_count_Encoder1);
			}
      else if((TIM4->CR1>>4 & 0x01)==1){//DIR==2  //下溢
				overflow_count_Encoder2--;
				xQueueOverwriteFromISR(Encoder2_Overflow_Queue,&overflow_count_Encoder2,&xHigherPriorityTaskWoken);
//				pr_warn_pure("中断打印：方向2,Overflow_count=%d\r\n",overflow_count_Encoder1);		
			}
			


			
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志 
		}
		


}


