#include "Task_WIFI.h"




//wifi任务函数
void WIFI_task(void *pvParameters)
{


extern	TaskHandle_t WIFITask_Handler;
#if Car_Wifi_Receive_OFF
vTaskSuspend(WIFITask_Handler);
#endif
	

	//声明消息队列句柄
	extern QueueHandle_t Motor1_Direction_Queue;
	extern QueueHandle_t Motor1_PWM_Queue;
	extern QueueHandle_t Motor2_Direction_Queue;
	extern QueueHandle_t Motor2_PWM_Queue;
	extern QueueHandle_t Wifi_buffer_Queue;
	extern QueueHandle_t Motor1_Ctrl_Parameter_Queue;
	extern QueueHandle_t Motor1_PID_Parameter_Queue;
	extern QueueHandle_t Motor2_Ctrl_Parameter_Queue;
	extern QueueHandle_t Motor2_PID_Parameter_Queue;
	//声明信号量句柄
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor2_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor2_SpeedChange;
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//串口2信号量句柄
	//消息队列初始化
	
	wifibuff *wifireceive;
	extern wifibuff wifibuff_struct_init;
	wifireceive=&wifibuff_struct_init;
	
	
	Motor1_Direction_t *Motor1_Dir;
	extern Motor1_Direction_t Motor1_Direction_struct_init;
	Motor1_Dir=&Motor1_Direction_struct_init;
	
	Motor2_Direction_t *Motor2_Dir;
	extern Motor2_Direction_t Motor2_Direction_struct_init;
	Motor2_Dir=&Motor2_Direction_struct_init;
	

	M1_ctrl *M1_ctrl_wifi;
	extern M1_ctrl Motor1_ctrl_struct_init;
	M1_ctrl_wifi=&Motor1_ctrl_struct_init;
	
	M2_ctrl *M2_ctrl_wifi;
	extern M2_ctrl Motor2_ctrl_struct_init;
	M2_ctrl_wifi=&Motor2_ctrl_struct_init;
	
	
  M1_PID *M1_speed_PID_wifiset;
	extern M1_PID  Motor1_PID_struct_init;
	M1_speed_PID_wifiset=&Motor1_PID_struct_init;
	
	
	M2_PID *M2_speed_PID_wifiset;
	extern M2_PID  Motor2_PID_struct_init;
	M2_speed_PID_wifiset=&Motor2_PID_struct_init;
	
	unsigned char arg[6];//解包参数
	float Data[3];//解包数据
		

	//信号量参数
	BaseType_t err=pdFALSE;
	float motor1_Pwm=0.0;
	float motor2_Pwm=0.0;
	u8 rec_flag=0;


	int i=0;
	

    while(1)
    {
			
			
			
			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//获取串口2空闲中断二值信号量
			
			if(err==pdTRUE) //获取信号量成功
			{

		  
			rec_flag=xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
				/******截取固定帧长度********/
//		printf("wifi_buffer:%s",wifireceive->wifi_buffer);
				for(i=0;i<=buffersize;i++)
				{
				wifireceive->wifi_buffer[i]=wifireceive->wifi_buffer[i];
//				printf("%x",wifireceive->wifi_buffer[i]);
				};
				
//				for(i=0;i<=buffersize-1;i++)
//				{
//				wifireceive->wifi_buffer[i]=wifireceive->wifi_buffer[i];
//				printf("%x",wifireceive->wifi_buffer[i]);
//				}
				
			/**消息队列获取成功*/
			if(rec_flag){

				DeserializeBuffer(wifireceive->wifi_buffer,arg,Data);
				if(arg[0]==length_true)
				{
				
				pr_warn_pure("剩余长度校验成功");
				
						if(arg[1]==CRC_success)
						{
						pr_warn_pure("CRC校验成功");
							
							switch(arg[2]){
								
								case Control_direction://方向控制
												pr_warn_pure("电机方向控制");
												switch(arg[3]){
													case Motor1://电机1
														if(arg[4]==Motor_dir1){
															pr_warn_pure("电机1方向1");				
															Motor1_Dir->Negative=1;
															Motor1_Dir->Positive=0;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号				
														}
														else if(arg[4]==Motor_dir2){
														
															pr_warn_pure("电机1方向2");				
															Motor1_Dir->Negative=0;
															Motor1_Dir->Positive=1;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号		
														
														}
													break;
														
													case Motor2://电机2

															if(arg[4]==Motor_dir1){
															pr_warn_pure("电机2方向1");				
															Motor2_Dir->Negative=1;
															Motor2_Dir->Positive=0;
															xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
															xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号				
														}
														else if(arg[4]==Motor_dir2){
														
															pr_warn_pure("电机2方向2");				
															Motor2_Dir->Negative=0;
															Motor2_Dir->Positive=1;
															xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
															xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号		
														
														}
													break;
														
													case Both://同一参数控制
														if(arg[4]==Motor_dir1){
															pr_warn_pure("电机Both方向1");		
															Motor1_Dir->Negative=1;
															Motor1_Dir->Positive=0;															
															Motor2_Dir->Negative=1;
															Motor2_Dir->Positive=0;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号				
															xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号				
														}
														else if(arg[4]==Motor_dir2){
														
															pr_warn_pure("电机Both方向2");		
															Motor1_Dir->Negative=0;
															Motor1_Dir->Positive=1;															
															Motor2_Dir->Negative=0;
															Motor2_Dir->Positive=1;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号	
															xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号															
														
														}
													
																							
													break;
												
													default:
													break;
												
																								
												}//控制对象
																
								break;
								
								case Control_PWM://PWM控制
												switch(arg[3]){
													case Motor1://电机1
															motor1_Pwm=((float)(arg[4])/100); //速度百分比
															xQueueOverwrite(Motor1_PWM_Queue,&motor1_Pwm);
															xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//发送电机1速度更改报文信号		
													break;
														
													case Motor2://电机2
															motor2_Pwm=((float)(arg[4])/100); //速度百分比
															xQueueOverwrite(Motor2_PWM_Queue,&motor2_Pwm);
															xSemaphoreGive(BinarySemaphore_Motor2_SpeedChange);//发送电机2速度更改报文信号
													break;
												
													case Both://同一参数控制
															motor1_Pwm=((float)(arg[4])/100); //速度百分比
															motor2_Pwm=((float)(arg[4])/100); //速度百分比
															xQueueOverwrite(Motor1_PWM_Queue,&motor1_Pwm);
															xQueueOverwrite(Motor2_PWM_Queue,&motor2_Pwm);
															xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//发送电机2速度更改报文信号
															xSemaphoreGive(BinarySemaphore_Motor2_SpeedChange);//发送电机2速度更改报文信号
													break;
								
													
													default:
													break;
												
																								
												}//控制对象
								break;
								case  Control_speed: //速度控制
														pr_warn_pure("ctrl_speed");
														switch(arg[3]){
														case Motor1: //电机1
														pr_warn_pure("M1");
														pr_warn_pure("speed=%f",Data[0]);
														M1_ctrl_wifi->Speed=Data[0];
														xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
														
														
														break;
														 
														case Motor2:  //电机2
														pr_warn_pure("M2");
														pr_warn_pure("speed=%f",Data[0]);
														M2_ctrl_wifi->Speed=Data[0];
														xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
														
														
														break;
														
														case Both:  //同一参数
														pr_warn_pure("M1&M2");
														pr_warn_pure("speed=%f",Data[0]);
														M1_ctrl_wifi->Speed=Data[0];
														M2_ctrl_wifi->Speed=Data[0];
														xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
														xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
														
														break;
															
														case Each:		
														pr_warn_pure("M1|M2");															
														pr_warn_pure("speed=%f",Data[0]);
														pr_warn_pure("speed=%f",Data[1]);
														M1_ctrl_wifi->Speed=Data[0];
														M2_ctrl_wifi->Speed=Data[1];
														xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
														xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
														
														break;
														default:
														break;
														
																									
														}
								break;
								


								case Control_PID:
														pr_warn_pure("PIDset");
														switch(arg[3]){
														case Motor1: //电机1
														pr_warn_pure("M1");
														pr_warn_pure("P=%f",Data[0]);
														pr_warn_pure("I=%f",Data[1]);
														pr_warn_pure("D=%f",Data[2]);
														M1_speed_PID_wifiset->Kp=Data[0];
														M1_speed_PID_wifiset->Ki=Data[1];
														M1_speed_PID_wifiset->Kd=Data[2];
														xQueueOverwrite(Motor1_PID_Parameter_Queue,&M1_speed_PID_wifiset);
														
														
														break;
														case Motor2: //电机2
														pr_warn_pure("M2");
														pr_warn_pure("P=%f",Data[0]);
														pr_warn_pure("I=%f",Data[1]);
														pr_warn_pure("D=%f",Data[2]);
														M2_speed_PID_wifiset->Kp=Data[0];
														M2_speed_PID_wifiset->Ki=Data[1];
														M2_speed_PID_wifiset->Kd=Data[2];
														xQueueOverwrite(Motor2_PID_Parameter_Queue,&M2_speed_PID_wifiset);
														break;
														
														case Both: //同一参数
														pr_warn_pure("M1&M2");
														pr_warn_pure("P=%f",Data[0]);
														pr_warn_pure("I=%f",Data[1]);
														pr_warn_pure("D=%f",Data[2]);
														M1_speed_PID_wifiset->Kp=Data[0];
														M1_speed_PID_wifiset->Ki=Data[1];
														M1_speed_PID_wifiset->Kd=Data[2];
														M2_speed_PID_wifiset->Kp=Data[0];
														M2_speed_PID_wifiset->Ki=Data[1];
														M2_speed_PID_wifiset->Kd=Data[2];
														xQueueOverwrite(Motor1_PID_Parameter_Queue,&M1_speed_PID_wifiset);
														xQueueOverwrite(Motor2_PID_Parameter_Queue,&M2_speed_PID_wifiset);
														break;
														
														default:
														break;
														
																									
														}
								
								
								
								break;
								
														
														
								case Control_speedAnddir:
														pr_warn_pure("Control_speedAnddir");
														switch(arg[3]){
														case Motor1: //电机1
															pr_warn_pure("M1");
															if(arg[4]==Motor_dir1){
															pr_warn_pure("电机1方向1");		
															Motor1_Dir->Negative=1;
															Motor1_Dir->Positive=0;			
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号																				
															}
															else if(arg[4]==Motor_dir2){
															pr_warn_pure("电机1方向2");		
															Motor1_Dir->Negative=0;
															Motor1_Dir->Positive=1;		
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号																				
															}
															pr_warn_pure("speed=%f",Data[0]);
															M1_ctrl_wifi->Speed=Data[0];
															xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
															
																								
														break;
														case Motor2: //电机2
															pr_warn_pure("M2");
															if(arg[4]==Motor_dir1){
																pr_warn_pure("电机2方向1");				
																Motor2_Dir->Negative=1;
																Motor2_Dir->Positive=0;
																xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号				
															}
															else if(arg[4]==Motor_dir2){
														
																pr_warn_pure("电机2方向2");				
																Motor2_Dir->Negative=0;
																Motor2_Dir->Positive=1;
																xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号		
														
															}
															pr_warn_pure("speed=%f",Data[0]);
															M2_ctrl_wifi->Speed=Data[0];
															xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
														
														break;
														
														case Both: //同一参数
															pr_warn_pure("M1&M2");
															if(arg[4]==Motor_dir1){
																pr_warn_pure("电机Both方向1");		
																Motor1_Dir->Negative=1;
																Motor1_Dir->Positive=0;															
																Motor2_Dir->Negative=1;
																Motor2_Dir->Positive=0;
																xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
																xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号				
																xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号				
														}
														else if(arg[4]==Motor_dir2){
														
																pr_warn_pure("电机Both方向2");		
																Motor1_Dir->Negative=0;
																Motor1_Dir->Positive=1;															
																Motor2_Dir->Negative=0;
																Motor2_Dir->Positive=1;
																xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
																xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号	
																xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号															
														
														}
																												
														pr_warn_pure("speed=%f",Data[0]);
														M1_ctrl_wifi->Speed=Data[0];
														M2_ctrl_wifi->Speed=Data[0];
														xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
														xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
														
														break;
														
														
														case Each:
															//方向提取
															switch(arg[4])
															{
																case Motor_dir1:
																	pr_warn_pure("M1_DIR方向1");
																	Motor1_Dir->Negative=1;
																	Motor1_Dir->Positive=0;			
																	xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
																	xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号				
																
																break;
																
																case Motor_dir2:
																	pr_warn_pure("M1_DIR方向2");
																	Motor1_Dir->Negative=0;
																	Motor1_Dir->Positive=1;			
																	xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
																	xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//发送电机1方向更改报文信号			
																
																break;
															
															}
														
															switch(arg[5])
															{
																case Motor_dir1:
																	pr_warn_pure("M2_DIR方向1");
																	Motor2_Dir->Negative=1;
																	Motor2_Dir->Positive=0;			
																	xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																	xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号		
																
																break;
																
																case Motor_dir2:
																	pr_warn_pure("M2_DIR方向2");
																	Motor2_Dir->Negative=0;
																	Motor2_Dir->Positive=1;			
																	xQueueOverwrite(Motor2_Direction_Queue,&Motor2_Dir);
																	xSemaphoreGive(BinarySemaphore_Motor2_DirChange);//发送电机2方向更改报文信号		
																
																break;
																
															}
														
															//速度提取
															pr_warn_pure("M1|M2");															
															pr_warn_pure("speed=%f",Data[0]);
															pr_warn_pure("speed=%f",Data[1]);
															M1_ctrl_wifi->Speed=Data[0];
															M2_ctrl_wifi->Speed=Data[1];
															xQueueOverwrite(Motor1_Ctrl_Parameter_Queue,&M1_ctrl_wifi);
															xQueueOverwrite(Motor2_Ctrl_Parameter_Queue,&M2_ctrl_wifi);
															
															
															
															
														break;
															
														default:
														break;
													}//obj
								break;	//speedAnddir		


														
								default:
								break;
							}//控制/数据
						
						}//CRC校验
					
									
					
					
				}//剩余长度校验
				
				
				
				
				

			}//rec_flag 消息队列获取成功


		
    }
			vTaskDelay(20);
	}//while

}

