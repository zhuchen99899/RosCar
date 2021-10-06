#ifndef __IIC_H_
#define __IIC_H_
#include "sys.h"
//临界段保护
#define OS_CRITICAL  0

//SDA 方向                       &=清除0               							   13    12   11   10  9    8
#define MPU_SDA_IN()  {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=8<<20;} //|=1000 0000 0000 0000 0000 0000
#define MPU_SDA_OUT() {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=3<<20;}	//|=0011 0000 0000 0000 0000 0000

//IO操作函数	 
#define MPU_IIC_SCL    PGout(14) 		//SCL
#define MPU_IIC_SDA    PGout(13) 		//SDA	 
#define MPU_READ_SDA   PGin(13) 		//输入SDA 

//IIC所有操作函数
void MPU_IIC_Delay(void);				//MPU IIC延时函数
void MPU_IIC_Init(void);                //初始化IIC的IO口				 
void MPU_IIC_Start(void);				//发送IIC开始信号
void MPU_IIC_Stop(void);	  			//发送IIC停止信号
void MPU_IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 MPU_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void MPU_IIC_Ack(void);					//IIC发送ACK信号
void MPU_IIC_NAck(void);				//IIC不发送ACK信号

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	 


#endif
