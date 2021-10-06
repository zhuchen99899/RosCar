#ifndef __IIC_H_
#define __IIC_H_
#include "sys.h"
//�ٽ�α���
#define OS_CRITICAL  0

//SDA ����                       &=���0               							   13    12   11   10  9    8
#define MPU_SDA_IN()  {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=8<<20;} //|=1000 0000 0000 0000 0000 0000
#define MPU_SDA_OUT() {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=3<<20;}	//|=0011 0000 0000 0000 0000 0000

//IO��������	 
#define MPU_IIC_SCL    PGout(14) 		//SCL
#define MPU_IIC_SDA    PGout(13) 		//SDA	 
#define MPU_READ_SDA   PGin(13) 		//����SDA 

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	 


#endif
