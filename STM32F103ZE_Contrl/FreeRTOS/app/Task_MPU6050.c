#include "Task_MPU6050.h"


								
								
void MPU6050_task(void *pvParameters)
{



	short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
	short gyrox,gyroy,gyroz;	//������ԭʼ����
	short temp;

	float data_aacx,data_aacy,data_aacz,data_gyrox,data_gyroy,data_gyroz,data_temp;

	u8 DMP_return;
	
	float pitch,roll,yaw; 		//ŷ����

	while(1){
	
	
	



		
			temp=MPU_Get_Temperature();	//�õ��¶�ֵ
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������

			data_temp=temp_data(temp);
			data_aacx=get_Accelerometer(aacx);
			data_aacy=get_Accelerometer(aacy);
			data_aacz=get_Accelerometer(aacz);
		
			data_gyrox=get_Gyroscope(gyrox);
			data_gyroy=get_Gyroscope(gyroy);
			data_gyroz=get_Gyroscope(gyroz);
		

		
		printf("MPU�¶�Ϊ:%f\r\n",data_temp);
		printf("���ٶ�Ϊ:accx=%f\r\n",data_aacx);
		printf("���ٶ�Ϊ:accy=%f\r\n",data_aacy);
		printf("���ٶ�Ϊ:accz=%f\r\n",data_aacz);
		
		printf("������Ϊ:gyrox=%f\r\n",data_gyrox);
		printf("������Ϊ:gyroy=%f\r\n",data_gyroy);
		printf("������Ϊ:gyroz=%f\r\n",data_gyroz);
		
		DMP_return=mpu_dmp_get_data(&pitch,&roll,&yaw);
		if(DMP_return==0)//DMP��ȡ����ɹ�
		{
					printf("pitch��:pitch=%f\r\n",pitch);
		printf("roll��:roll=%f\r\n",roll);
		printf("yaw��:yaw=%f\r\n",yaw);
		
		}
		else{
			printf("DMP��ȡʧ��");		

		}

	
	vTaskDelay(50);
		
	}







	
	
}



