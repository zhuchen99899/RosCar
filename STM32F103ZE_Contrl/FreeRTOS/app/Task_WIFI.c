#include "Task_WIFI.h"




//wifi������
void WIFI_task(void *pvParameters)
{


extern	TaskHandle_t WIFITask_Handler;
#if Car_Wifi_Receive_OFF
vTaskSuspend(WIFITask_Handler);
#endif
	

	//������Ϣ���о��
	extern QueueHandle_t Motor1_Direction_Queue;
	extern QueueHandle_t Motor1_PWM_Queue;
	extern QueueHandle_t Wifi_buffer_Queue;
	//�����ź������
	extern SemaphoreHandle_t BinarySemaphore_Motor1_DirChange;
	extern SemaphoreHandle_t BinarySemaphore_Motor1_SpeedChange;
	extern SemaphoreHandle_t BinarySemaphore_USART2ISR;	//����2�ź������
	//��Ϣ���г�ʼ��
	Motor1_Direction_t Motor1_Dir_init;
	Motor1_Direction_t *Motor1_Dir;
	Motor1_Dir=&Motor1_Dir_init;
	
	wifibuff *wifireceive;
	wifibuff wifi_msgt;
	
	unsigned char arg[5];
	

	//�ź�������
	BaseType_t err=pdFALSE;
	float motor1_Pwm=0.0;
	u8 rec_flag=0;


	wifireceive=&wifi_msgt;
	int i=0;
	

    while(1)
    {
			
			
			
			err=xSemaphoreTake(BinarySemaphore_USART2ISR,portMAX_DELAY);	//��ȡ����2�����ж϶�ֵ�ź���
			
			if(err==pdTRUE) //��ȡ�ź����ɹ�
			{

		  
			rec_flag=xQueuePeek(Wifi_buffer_Queue,(void *)&wifireceive,portMAX_DELAY);
				/******��ȡ�̶�֡����********/
//		printf("wifi_buffer:%s",wifireceive->wifi_buffer);
				for(i=0;i<=buffersize;i++)
				{
				wifireceive->wifi_buffer[i]=wifireceive->wifi_buffer[i];
//				pr_warn_pure("%x",wifireceive->wifi_buffer[i]);
				};
				
			/**��Ϣ���л�ȡ�ɹ�*/
			if(rec_flag){

				DeserializeBuffer(wifireceive->wifi_buffer,arg);
				if(arg[0]==length_true)
				{
				
				pr_warn_pure("ʣ�೤��У��ɹ�");
				
						if(arg[1]==CRC_success)
						{
						pr_warn_pure("CRCУ��ɹ�");
							
							switch(arg[2]){
								
								case Control_direction://�������

												switch(arg[3]){
													case Motor1://���1
														if(arg[4]==Motor_dir1){
															pr_warn_pure("����1");				
															Motor1_Dir->Negative=1;
															Motor1_Dir->Positive=0;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�				
														}
														else if(arg[4]==Motor_dir2){
														
															pr_warn_pure("����2");				
															Motor1_Dir->Negative=0;
															Motor1_Dir->Positive=1;
															xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
															xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�		
														
														}
													break;
														
													case Motor2://���2

													break;
												
													default:
													break;
												
																								
												}//���ƶ���
																
								break;
								
								case control_PWM://PWM����
												switch(arg[3]){
													case Motor1://���1
															pr_warn_pure("arg[4]=%d",arg[4]);
															motor1_Pwm=((float)(arg[4])/100); //�ٶȰٷֱ�
															xQueueOverwrite(Motor1_PWM_Queue,&motor1_Pwm);
															xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//���͵��1�ٶȸ��ı����ź�		
													break;
														
													case Motor2://���2

													break;
												
													default:
													break;
												
																								
												}//���ƶ���
									
			
												
								
								default:
								break;
							}//����/����
						
						}//CRCУ��
					
									
					
					
				}//ʣ�೤��У��
				
				
				
				
				

			}//rec_flag ��Ϣ���л�ȡ�ɹ�


		
    }
			vTaskDelay(20);
	}//while

}

