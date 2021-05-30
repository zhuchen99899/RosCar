#ifndef __PROTOCOL_H	
#define __PROTOCOL_H	 
#include "Car_config.h"
#include "crc16.h"
#include "sys.h"
#include "config.h"
#include "FreeRTOS.h"

/************���ⶨͨѶЭ�����*******************/
#define sendbuffsize 17 //����Э�������С
#define buffersize 16  //Э��̶����� ������λ�������ٶȺܿ죬���֡ճ����������������ճ������


#define CRC_success        0x01
#define CRC_fail 					 0x00


//���ֽ�
#define Data_speed         0x00
#define Control_direction  0x01
#define Control_PWM        0x02
#define Control_speed      0x03
#define Control_PID        0x04
#define Data_Line_speed    0x05
#define Data_Angular_speed 0x06

//ʣ�೤���ֽ�
#define residue_length     0x0f

#define length_true        0x01
#define length_err         0x00


//���ƶ����ֽ� 
#define Both               0x00
#define Each               0x03
#define Motor1             0x01
#define Motor2             0x02

//������������غ�

#define Motor_dir1         0x01
#define Motor_dir2         0x02

//���PWM������غ�
#define control_PWM        0x02





////��������װ
//typedef struct protocol
//{
//unsigned char (*Deserialize_Ctrl_dir)(unsigned char *buffer,unsigned char dir);
//unsigned char (*Deserialize_Ctrl_obj)(unsigned char *buffer,unsigned char obj);
//void  (*DeserializeBuffer)(unsigned char *buffer,unsigned char arg[]);	

//}protocol_t;

void float_to_uchar(float data,unsigned char *uchar_data);
void SerializeBuffer(unsigned char *buffer,unsigned char Datahead_Byte,unsigned char obj,float data[]);


static unsigned char Deserialize_Ctrl_dir(unsigned char *buffer,unsigned char dir);
static unsigned char Deserialize_Ctrl_obj(unsigned char *buffer,unsigned char obj);
void  DeserializeBuffer(unsigned char *buffer,unsigned char arg[]);	

#endif



