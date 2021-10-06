#include "iic.h"
#include "delay.h"
#include "freertos.h"
#include "task.h"
/***************
iic����
��ע:�ٴ˶���������Ҫ��ʱ��ĵط�ע���ٽ�α���
��PG14->IIC_SCL  PG13->IIC_SDA

******************/


 //MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	delay_us(2);
}



//��ʼ��IIC IO
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);//��ʹ������IO PORTGʱ�� 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
  GPIO_SetBits(GPIOG,GPIO_Pin_13|GPIO_Pin_14);						 //Pg13,Pg14 �����	
 
}

//����IIC��ʼ�ź�
void MPU_IIC_Start(void)
{
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_SDA_OUT();     //sda�����
	MPU_IIC_SDA=1;	  	  
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
 	MPU_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif
	//�˳��ٽ��
}	  

//����IICֹͣ�ź�
void MPU_IIC_Stop(void)
{
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_SDA_OUT();//sda�����
	MPU_IIC_SCL=0;
	MPU_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	MPU_IIC_Delay();
	MPU_IIC_SCL=1; 
	MPU_IIC_SDA=1;//����I2C���߽����ź�
	MPU_IIC_Delay();	
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif	
	//�˳��ٽ��
}


//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	MPU_SDA_IN();      //SDA����Ϊ����  
	MPU_IIC_SDA=1;MPU_IIC_Delay();	   
	MPU_IIC_SCL=1;MPU_IIC_Delay();	 
	while(MPU_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
	MPU_IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void MPU_IIC_Ack(void)
{	
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=0;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif
	//�˳��ٽ��
}
//������ACKӦ��		    
void MPU_IIC_NAck(void)
{
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_IIC_SCL=0;
	MPU_SDA_OUT();
	MPU_IIC_SDA=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=1;
	MPU_IIC_Delay();
	MPU_IIC_SCL=0;
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif
	//�˳��ٽ��
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_SDA_OUT(); 	    
    MPU_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        MPU_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		    MPU_IIC_SCL=1;
		    MPU_IIC_Delay(); 
		    MPU_IIC_SCL=0;	
		    MPU_IIC_Delay();
    }	 
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif
	//�˳��ٽ��
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	#if OS_CRITICAL
	taskENTER_CRITICAL();
	#endif
	//�����ٽ��
	MPU_SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        MPU_IIC_SCL=0; 
        MPU_IIC_Delay();
		MPU_IIC_SCL=1;
        receive<<=1;
        if(MPU_READ_SDA)receive++;   
		MPU_IIC_Delay(); 
    }		
	#if OS_CRITICAL
	taskEXIT_CRITICAL();
	#endif
	//�˳��ٽ��	
    if (!ack)
        MPU_IIC_NAck();//����nACK
    else
        MPU_IIC_Ack(); //����ACK   
    return receive;
}

