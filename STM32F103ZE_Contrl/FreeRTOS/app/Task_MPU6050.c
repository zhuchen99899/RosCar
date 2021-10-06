#include "Task_MPU6050.h"


								
								
void MPU6050_task(void *pvParameters)
{



	short aacx,aacy,aacz;		//加速度传感器原始数据
	short gyrox,gyroy,gyroz;	//陀螺仪原始数据
	short temp;

	float data_aacx,data_aacy,data_aacz,data_gyrox,data_gyroy,data_gyroz,data_temp;

	u8 DMP_return;
	
	float pitch,roll,yaw; 		//欧拉角

	while(1){
	
	
	



		
			temp=MPU_Get_Temperature();	//得到温度值
			MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
			MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据

			data_temp=temp_data(temp);
			data_aacx=get_Accelerometer(aacx);
			data_aacy=get_Accelerometer(aacy);
			data_aacz=get_Accelerometer(aacz);
		
			data_gyrox=get_Gyroscope(gyrox);
			data_gyroy=get_Gyroscope(gyroy);
			data_gyroz=get_Gyroscope(gyroz);
		

		
		printf("MPU温度为:%f\r\n",data_temp);
		printf("加速度为:accx=%f\r\n",data_aacx);
		printf("加速度为:accy=%f\r\n",data_aacy);
		printf("加速度为:accz=%f\r\n",data_aacz);
		
		printf("陀螺仪为:gyrox=%f\r\n",data_gyrox);
		printf("陀螺仪为:gyroy=%f\r\n",data_gyroy);
		printf("陀螺仪为:gyroz=%f\r\n",data_gyroz);
		
		DMP_return=mpu_dmp_get_data(&pitch,&roll,&yaw);
		if(DMP_return==0)//DMP获取输出成功
		{
					printf("pitch角:pitch=%f\r\n",pitch);
		printf("roll角:roll=%f\r\n",roll);
		printf("yaw角:yaw=%f\r\n",yaw);
		
		}
		else{
			printf("DMP获取失败");		

		}

	
	vTaskDelay(50);
		
	}







	
	
}



