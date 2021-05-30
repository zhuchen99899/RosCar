#ifndef __PROTOCOL_H	
#define __PROTOCOL_H	 
#include "Car_config.h"
#include "crc16.h"
#include "sys.h"
#include "config.h"
#include "FreeRTOS.h"

/************自拟定通讯协议相关*******************/
#define sendbuffsize 17 //发送协议数组大小
#define buffersize 16  //协议固定长度 由于上位机可能速度很快，造成帧粘连，丢弃缓冲区中粘连长度


#define CRC_success        0x01
#define CRC_fail 					 0x00


//首字节
#define Data_speed         0x00
#define Control_direction  0x01
#define Control_PWM        0x02
#define Control_speed      0x03
#define Control_PID        0x04
#define Data_Line_speed    0x05
#define Data_Angular_speed 0x06

//剩余长度字节
#define residue_length     0x0f

#define length_true        0x01
#define length_err         0x00


//控制对象字节 
#define Both               0x00
#define Each               0x03
#define Motor1             0x01
#define Motor2             0x02

//电机方向控制相关宏

#define Motor_dir1         0x01
#define Motor_dir2         0x02

//电机PWM控制相关宏
#define control_PWM        0x02





////面向对象封装
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



