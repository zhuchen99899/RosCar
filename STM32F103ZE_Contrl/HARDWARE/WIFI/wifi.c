#include "wifi.h"

/*串口2连接友人WIFI模块 RXD->PA3 TXD->PA2
波特率推荐96000
*/


#define DMA_Send_Len   256   //发送数据长度 
#define DMA_Rec_Len   256    //接收数据长度
static u8 DMA_Send_Buf[DMA_Send_Len];	//发送数据缓冲区
static u8 DMA_Receive_Buf[DMA_Rec_Len];	//接收数据缓冲区



void usart2_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;			//GPIO初始化结构体
	USART_InitTypeDef USART_InitStructure;		//USART2初始化结构体

	/************USART/GPIO RCC时钟使能*********************/
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //串口2时钟使能  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//串口2引脚TX,RX(PA2,PA3)时钟使能
	
	/************gpio配设************************/
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //串口2TX （PA2）要设为推挽复用
	//PA9初始化结构体成员变量赋值
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    //串口2RX( PA3)要设为浮空输入或上拉输入
	//PA10初始化结构体成员变量赋值
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/********************串口配设********** *****/
	USART_InitStructure.USART_BaudRate=115200;									//波特率115200
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //无硬件流控制
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;		//输入输出模式
	USART_InitStructure.USART_Parity=USART_Parity_No;		 		//无奇偶校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1个停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;       //数据长度为8位
	//串口初始化结构体成员变量赋值
	USART_Init(USART2,&USART_InitStructure);

	
	/********串口中断配设*************/
	USART_ITConfig(USART2,USART_IT_TC,DISABLE);  //关闭传输完成中断
	USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);//关闭接收中断(接收第一个字节时开启的中断)
	
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE); //开启串口空闲中断
	USART_ITConfig(USART2,USART_IT_ERR,ENABLE);	//开启错误中断
	USART_ITConfig(USART2,USART_IT_PE,ENABLE);	  //开启奇偶错误中断

	/**********************开启串口********************/
	USART_Cmd(USART2,ENABLE);           //使能串口2
}


/*****************
DMA初始化
USART2_RX->channel6
USART2_TX->channel7
******************/
//发送地址以及发送缓存大小

//接收地址以及接收缓存大小

void    DMA_USART2_Init(void)
{
	
	DMA_InitTypeDef DMA_InitStructure;
	
	
	/****************DMA1 RCC时钟使能************************************/
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA1rcc时钟
	/*******************USART2发送DMA配设(channel7)***************************/
	
	DMA_DeInit(DMA1_Channel7);  //将DMA的通道7寄存器重设为缺省值

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; //DMA 外设USART2地址

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Send_Buf;  //DMA发送内存基地址

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //数据方向，从内部到外设

  DMA_InitStructure.DMA_BufferSize = DMA_Send_Len;  //DMA通道的缓存大小

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为8位

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位

  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式

  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道使用中优先级

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA无内存到内存传输

  DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //DMA通道7结构体赋值

	//中断配设

DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);//使能传输完成中断

DMA_ITConfig(DMA1_Channel7, DMA_IT_TE, ENABLE); //使能传输错误中断
	
DMA_Cmd(DMA1_Channel7, ENABLE);  //DMA通道7传输使能

	//串口发送DMA使能

//USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);   //使能串口2 DMA发送使能
		
DMA_Cmd(DMA1_Channel7, DISABLE);//关闭发送通道 【注意】 发送时再开启，防止DMA接收发生错误





	/***************USART2接收DMA配设(channel6)***************************/
	
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道6寄存器重设为缺省值

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR; //DMA 外设USART2地址

  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DMA_Receive_Buf;  //DMA接收内存基地址

  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据方向，从外设到内部

  DMA_InitStructure.DMA_BufferSize = DMA_Rec_Len;  //DMA通道的缓存大小

  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址寄存器不变

  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增

  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //数据宽度为8位

  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位


  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式

  DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道使用高优先级

  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA无内存到内存传输

  DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //DMA通道6结构体赋值

//中断配设
DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);//使能传输缓冲溢出中断

DMA_ITConfig(DMA1_Channel6, DMA_IT_TE, ENABLE); //使能传输错误中断

//串口接收DMA使能 【注】建议在NVIC初始化后开启

/*
USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   //使能串口2DMA接收使能
	
DMA_Cmd(DMA1_Channel6, ENABLE);  //DMA通道6传输使能
*/
	
}




/***********************************
NVIC初始化

串口2NVIC
DMA1_channel6 NVIC
DMA1_channel7 NVIC

**************************/

void NVIC_cofig_Init(void)
{
		
	NVIC_InitTypeDef NVIC_InitStructure;            //NVIC初始化结构体
	
	
	/********USART2 NVIC中断初始化***********/
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;				//中断通道为串口2中断
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;				//通道使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=13;    //抢占优先级为13
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;			//响应优先级为0
	//NVIC初始化结构体成员变量赋值
	NVIC_Init(&NVIC_InitStructure);
	
	 /****DMA 通道7 NVIC中断  发送*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);


	 /****DMA1 通道6 NVIC中断  接收*********/
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 
  NVIC_Init(&NVIC_InitStructure);

/****在此处开启串口2接收DMA****/
USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);   //使能串口2DMA接收使能
	
DMA_Cmd(DMA1_Channel6, ENABLE);  //DMA通道6接收传输使能

};




/************wifi初始化************/
void wifi_init(u32 bound)
{

usart2_init(bound);
DMA_USART2_Init();
NVIC_cofig_Init();
};


/********************
串口2中断服务函数
*********************/


void USART2_IRQHandler(void)  
{
	wifibuff wifi_tMsg;

	extern QueueHandle_t Wifi_buffer_Queue;
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//二值信号量句柄
	wifibuff *receivebuff;
	BaseType_t xHigherPriorityTaskWoken;
	int16_t 	DATA_LEN;
	u16 i;
	DMA_Channel_TypeDef *DMA1_CH6 = DMA1_Channel6;
	USART_TypeDef *uart2 = USART2;
	
			receivebuff=&wifi_tMsg;//消息结构体初始化
			receivebuff->wifi_lenth=0;//消息结构体初始化
	

	DMA_Cmd(DMA1_Channel6,DISABLE); //关闭DMA ，DMA防止中断处理期间有数据
	//printf("标记1");
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)//如果为串口2空闲中断
	{
		DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);//清除标志位
	 //printf("标记2");

			DATA_LEN = DMA_Rec_Len - DMA_GetCurrDataCounter(DMA1_Channel6); 
			if(DATA_LEN > 0 && DATA_LEN <= DMA_Rec_Len)
			{	 
				//printf("标记3");
			 DMA_Cmd(DMA1_Channel6, DISABLE); //改变通道datasize前要禁止通道工作
		   DMA1_CH6->CNDTR = DMA_Rec_Len; //修改DMA1数据传输量

				

			memset(receivebuff->wifi_buffer,0x00,sizeof(receivebuff->wifi_buffer));
			//传输消息队列
					//printf("标记4");
//				pr_warn_pure("WIFI模组接收到数据\r\n");
				receivebuff->wifi_lenth=DATA_LEN;
					//printf("标记5");

				memcpy(receivebuff->wifi_buffer,DMA_Receive_Buf,DMA_Rec_Len);
				xQueueOverwriteFromISR(Wifi_buffer_Queue,(void *)&receivebuff,&xHigherPriorityTaskWoken);		

					//printf("标记6");
				xSemaphoreGiveFromISR(BinarySemaphore_USART2ISR,&xHigherPriorityTaskWoken);	//释放二值信号量
				
//			uart_to_keyboard_msg.address = DMA_Receive_Buf;
//			uart_to_keyboard_msg.length = DATA_LEN;
//			total_rx++;
//			xQueueSendToBackFromISR(Keyboard_Queue,(void*)&uart_to_keyboard_msg,&xHigherPriorityTaskWoken);
			//xSemaphoreGiveFromISR(BinarySemaphore,&xHigherPriorityTaskWoken);
				

		 	//printf("标记7");

			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
				//退出中断切换
			}//数据无溢出	
			
	      //先读SR后读DR,清除IDLE空闲中断标志
	/****【注】进入空闲中断后，串口CR1寄存器的IDLE位被硬件置1，所以此动作很重要，防止反复进空闲中断，清除空闲中断不能使用库函数，必须使用先读SR后读DR的方法。****/
     i = uart2->SR;
     i = uart2->DR;
	   i = i;
				 //	printf("标记8");
	}//空闲中断判断
		
	else if(USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE) != RESET)//出错
	{
		USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE|USART_IT_ORE);//清除中断
		//错误类型计数total_err2++;
		//printf("出错 err2");
		i = uart2->SR;
		i = uart2->DR;
		
	}
	else
	{
		//错误类型计数total_err2++;
		//funcCode.all[FUNCCODE_P11_15] = total_err2;
		//printf("出错 err3");
		i = uart2->SR;
		i = uart2->DR;
			
	}
			 	//printf("标记9");
	
	DMA_Cmd(DMA1_Channel6, DISABLE);//关闭DMA
	DMA1_CH6->CNDTR = DMA_Rec_Len;//重装填
	//清除dma缓冲区数据
	memset(DMA_Receive_Buf,0x00,sizeof(DMA_Receive_Buf));
	DMA_Cmd(DMA1_Channel6, ENABLE);//处理完成，重启DMA
		 //	printf("标记10");
	
	//清除空闲标志 【注】除了库函数清除标志一定要有上面步骤 读取SR DR寄存器值
	USART_ClearITPendingBit(USART2, USART_IT_TC);
	USART_ClearITPendingBit(USART2, USART_IT_IDLE);
				// printf("标记11");
			
}


 /****DMA1 通道7  发送完成中断*********/
void DMA1_Channel7_IRQHandler(void)
{
 
  DMA_ClearITPendingBit(DMA1_IT_TC7);
 
  DMA_ClearITPendingBit(DMA1_IT_TE7); //清除空闲中断
 
  DMA_Cmd(DMA1_Channel7, DISABLE);//DMA关闭

  
}

 /****DMA1 通道6  接收完成中断*********/
 //数据缓冲溢出产生
void DMA1_Channel6_IRQHandler(void)
{
		
	DMA_Channel_TypeDef *DMA1_CH6 = DMA1_Channel6;
	 	//printf("标记12");
	DMA_ClearITPendingBit(DMA1_IT_TC6);

	DMA_ClearITPendingBit(DMA1_IT_TE6); //清除空闲中断

	DMA_Cmd(DMA1_Channel6, DISABLE);//DMA接收关闭
	

	DMA1_CH6->CNDTR = DMA_Rec_Len;//重新装填数据大小
	//清除缓冲区数据
	memset(DMA_Receive_Buf,0x00,sizeof(DMA_Receive_Buf));
	DMA_Cmd(DMA1_Channel6, ENABLE);//DMA接收开启
		 	//printf("标记13");
}


/*****************
发送接口函数·
发送后任务函数需要做延时
vTaskDelay(time);
预计time=(len/40)+2
******************/
void WIFI_send(u8 * buffer,u32 len)
{


	
	memset(DMA_Send_Buf, 0x00, sizeof(DMA_Send_Buf));//缓冲清0
	
	memcpy(DMA_Send_Buf,buffer,len);
	
	
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE); //使能串口1的DMA发送      
	
	DMA_Cmd(DMA1_Channel7, DISABLE);//DMA 关闭
	DMA_SetCurrDataCounter(DMA1_Channel7,len);//DMA通道的DMA缓存的大小
	DMA_Cmd(DMA1_Channel7, ENABLE);        //DMA开启

	//total_tx++;


}


/**************
接收接口函数
不常用
已经从串口空闲中断中消息队列传出
***************/
/*
void KeyboardRs485RxFrame(unsigned char *buffer,unsigned int timeout)
{
	unsigned int len = 0;
	unsigned int i = 0;

	msg_object_t msg;
	KeyboardRs485RxMode();	

	if(xQueueReceive(Keyboard_Queue,&msg,timeout) == pdFALSE)
	//if(xSemaphoreTake(BinarySemaphore, timeout) == pdFALSE)
	{
		
		return len;
	}

	len = msg.length;
	//len = uart_to_keyboard_msg.length;
	//copy to buffer
	for(i = 0; i < len; i++)
	{
		buffer[i] = msg.address[i];
		//buffer[i] = uart_to_keyboard_msg.address[i];
	}
	

};
*/




