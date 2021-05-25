#include "Task_WIFI.h"

#define buffersize 6  //Э��̶����� ������λ�������ٶȺܿ죬���֡ճ����������������ճ������


//wifi������
void WIFI_task(void *pvParameters)
{
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
	
	
	u16 crc_ret;//CRC
	u16 crc_2byte;//β��crcУ��ֵ
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
//		printf("wifi_buffer:%s",wifireceive->wifi_buffer);
				for(i=0;i<6;i++)
				{
				wifireceive->wifi_buffer[i]=wifireceive->wifi_buffer[i];
//				pr_warn_pure("%x",wifireceive->wifi_buffer[i]);
				};
			if(rec_flag){
			/******************CRCУ��**************************/

			crc_ret=CRC16(wifireceive->wifi_buffer,buffersize-2);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
			crc_2byte=Combine_CRC_2Byte(wifireceive->wifi_buffer[4],wifireceive->wifi_buffer[5]);
//			pr_warn_pure("��ӡCRC:%x\r\n",crc_ret);
//			pr_warn_pure("��ӡCRC���:%x\r\n",crc_2byte);
				if(crc_ret == crc_2byte){
					pr_warn_pure("CRCУ��ɹ�\r\n");
					/**********����ָ���ж�************/
					switch(wifireceive->wifi_buffer[0]){
						case 0x00://����
							pr_warn_pure("����\r\n");
							break;
						
						/*************����������************/
						case 0x01://�������ָ��
							pr_warn_pure("�������ָ��\r\n");
								/*********У�鳤��(������ǰ�����ֽ�)********/					
								if(wifireceive->wifi_buffer[1]==buffersize-2){
									pr_warn_pure("֡����У��ͨ��\r\n");
								
									/***********���ѡ��*****************/
									switch(wifireceive->wifi_buffer[2]){
										case 0x00:
										pr_warn_pure("���1");
										switch(wifireceive->wifi_buffer[3]){
											case 0x00:
											pr_warn_pure("����1");				
											Motor1_Dir->Negative=1;
											Motor1_Dir->Positive=0;
											xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
											xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�													
											break;
											case 0x01:
											pr_warn_pure("����2");	
											Motor1_Dir->Negative=0;
											Motor1_Dir->Positive=1;
											xQueueOverwrite(Motor1_Direction_Queue,&Motor1_Dir);
											xSemaphoreGive(BinarySemaphore_Motor1_DirChange);//���͵��1������ı����ź�
											break;
									
										}//���1����
										break;
										
										
										case 0x01:
										pr_warn_pure("���2");
										break;

									
									
									}//������ѡ��
								
								
				
								
								}//֡����У��ɹ�
							break;//�������ָ�� case 0x01
								
							/*************����ٶȿ���************/	
							case 0x02://�ٶȿ���ָ��
								/*********У�鳤��(������ǰ�����ֽ�)********/					
								if(wifireceive->wifi_buffer[1]==buffersize-2){
									pr_warn_pure("֡����У��ͨ��\r\n");
									
									switch(wifireceive->wifi_buffer[2]){
										/***********���ѡ��*****************/
										case 0x00:
										pr_warn_pure("���1");
												/**********����ٶ�����**********/
												motor1_Pwm=(((float)wifireceive->wifi_buffer[3])/100); //�ٶȰٷֱ�
												xQueueOverwrite(Motor1_PWM_Queue,&motor1_Pwm);
												xSemaphoreGive(BinarySemaphore_Motor1_SpeedChange);//���͵��1�ٶȸ��ı����ź�
										break;
										
										
										case 0x01:
										pr_warn_pure("���2");
										break;
										
					
									}//�ٶȵ��ѡ��
									
								}//֡���ȼ���
							
							break;//�ٶȿ���ָ��case 0x02
											
					
					}//����ָ���ж�
			
				}//CRC�ȶԳɹ�
			}//rec_flag ��Ϣ���л�ȡ�ɹ�


		
    }
			vTaskDelay(20);
	}//while

}

