#include "Task_WIFI.h"

#define buffersize 6  //协议固定长度 由于上位机可能速度很快，造成帧粘连，丢弃缓冲区中粘连长度


//wifi任务函数
void WIFI_task(void *pvParameters)
{
	//声明消息队列句柄
	extern QueueHandle_t Motor1_Direction_Queue;
	extern QueueHandle_t Motor1_PWM_Queue;
	extern QueueHandle_t Wifi_buffer_Queue;
	//声明信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//串口2信号量句柄
	//消息队列初始化
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;
	
	wifibuff *wifireceive;
	wifibuff wifi_msgt;
	
	
	u16 crc_ret;//CRC
	u16 crc_2byte;//尾部crc校验值
	//信号量参数
	BaseType_t err=pdFALSE;
	float motor1_Pwm=0.0;
	u8 rec_flag=0;

	wifireceive=&wifi_msgt;
	int i=0;
    while(1)
    {
			
			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
			
			if(err==pdTRUE) //获取信号量成功
			{
				
		
			rec_flag=xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
//		printf("wifi_buffer:%s",wifireceive->wifi_buffer);
				for(i=0;i<6;i++)
				{
				wifireceive->wifi_buffer[i]=wifireceive->wifi_buffer[i];
//				pr_warn_pure("%x",wifireceive->wifi_buffer[i]);
				};
			if(rec_flag){
			/******************CRC校验**************************/

			crc_ret=CRC16(wifireceive->wifi_buffer,buffersize-2);//去除一帧最后两个CRC校验位计算CRC16值
			crc_2byte=Combine_CRC_2Byte(wifireceive->wifi_buffer[4],wifireceive->wifi_buffer[5]);
//			pr_warn_pure("打印CRC:%x\r\n",crc_ret);
//			pr_warn_pure("打印CRC组合:%x\r\n",crc_2byte);
				if(crc_ret == crc_2byte){
					pr_warn_pure("CRC校验成功\r\n");
					/**********数据指令判断************/
					switch(wifireceive->wifi_buffer[0]){
						case 0x00://数据
							pr_warn_pure("数据\r\n");
							break;
						
						/*************电机方向控制************/
						case 0x01://方向控制指令
							pr_warn_pure("方向控制指令\r\n");
								/*********校验长度(不包含前两个字节)********/					
								if(wifireceive->wifi_buffer[1]==buffersize-2){
									pr_warn_pure("帧长度校验通过\r\n");
								
									/***********电机选择*****************/
									switch(wifireceive->wifi_buffer[2]){
										case 0x00:
										pr_warn_pure("电机1");
										switch(wifireceive->wifi_buffer[3]){
											case 0x00:
											pr_warn_pure("方向1");				
											Motor1_Dir->Negative=1;
											Motor1_Dir->Positive=0;
											xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
											xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号													
											break;
											case 0x01:
											pr_warn_pure("方向2");	
											Motor1_Dir->Negative=0;
											Motor1_Dir->Positive=1;
											xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
											xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号
											break;
									
										}//电机1方向
										break;
										
										
										case 0x01:
										pr_warn_pure("电机2");
										break;

									
									
									}//方向电机选择
								
								
				
								
								}//帧长度校验成功
							break;//方向控制指令 case 0x01
								
							/*************电机速度控制************/	
							case 0x02://速度控制指令
								/*********校验长度(不包含前两个字节)********/					
								if(wifireceive->wifi_buffer[1]==buffersize-2){
									pr_warn_pure("帧长度校验通过\r\n");
									
									switch(wifireceive->wifi_buffer[2]){
										/***********电机选择*****************/
										case 0x00:
										pr_warn_pure("电机1");
												/**********电机速度设置**********/
												motor1_Pwm=(((float)wifireceive->wifi_buffer[3])/100); //速度百分比
												xQueueOverwrite(Motor1_PWM_Queue,&motor1_Pwm);
												xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//发送电机1速度更改报文信号
										break;
										
										
										case 0x01:
										pr_warn_pure("电机2");
										break;
										
					
									}//速度电机选择
									
								}//帧长度检验
							
							break;//速度控制指令case 0x02
											
					
					}//数据指令判断
			
				}//CRC比对成功
			}//rec_flag 消息队列获取成功


		
    }
			vTaskDelay(20);
	}//while

}

