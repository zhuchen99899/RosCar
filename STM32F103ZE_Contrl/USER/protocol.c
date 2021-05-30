/*****十六进制自拟定协议****/
#include "protocol.h"

/***联合体结构转换float为四字节数  OR 四字节数转换float****/
union float_trans{
	float f;
	unsigned char uch[4];
};
	




/*转换float型数据为uchar数据
@data 输入的float类型数据
@return 返回四个字节的uchar型数组，注意不能使用局部，局部变量指针调用函数后就会被销毁
*/

void float_to_uchar(float data,unsigned char *uchar_data)
{

union float_trans trans;
trans.f=data;
uchar_data[0]=trans.uch[0];
uchar_data[1]=trans.uch[1];
uchar_data[2]=trans.uch[2];
uchar_data[3]=trans.uch[3];
	

	
	
}





/********************组帧函数*****************************/
/*组合数据帧
@Datahead_Byte 数据指令字节
@obj 控制对象
@data float类型数据
*/
void SerializeBuffer(unsigned char *buffer,unsigned char Datahead_Byte,unsigned char obj,float data[])
{
	
unsigned char	uchar_data[4];
int i;
unsigned char crc_res[2];
switch(Datahead_Byte)
{
	case Data_speed://发送速度数据
		
		buffer[0]=Data_speed;
		switch(obj){
			case Motor1:
				buffer[1]=residue_length;
				buffer[2]=Motor1;
				//转化float类型数据
				float_to_uchar(data[0],uchar_data);
				buffer[3]=uchar_data[0];
				buffer[4]=uchar_data[1];
				buffer[5]=uchar_data[2];
				buffer[6]=uchar_data[3];

				for(i=7;i<=14;i++)
				{
				buffer[i]=0x00;
				}

				CRC16_uch(buffer,14,crc_res);//去除一帧最后两个CRC校验位计算CRC16值
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			

			default:
			break;
		}//控制对象
	
		break;
	case Data_Line_speed:
		break;
	case Data_Angular_speed:
		break;
	default:
		break;
}//指定字节



}


/*****************解帧函数*****************************/

/*解帧中的PWM负载
@buffer 帧指针
@dir 方向宏
@PWM 数值 （百分数需要除以100）
*/
static unsigned char Deserialize_Ctrl_PWM(unsigned char *buffer,unsigned char PWM_val)
{
	PWM_val=buffer[3];
	return PWM_val;
	
}





/*解帧中的方向负载
@buffer 帧指针
@dir 方向宏
*/

static unsigned char Deserialize_Ctrl_dir(unsigned char *buffer,unsigned char dir)
{

	switch(buffer[3])
	{
		case Motor_dir1:
			dir=Motor_dir1;
			break;
		case Motor_dir2:
			dir=Motor_dir2;
			break;
		default:
			break;

	}
	
	return dir;

}


/*解帧中的对象字节
@buffer 帧指针
@obj对象 宏

*/

static unsigned char Deserialize_Ctrl_obj(unsigned char *buffer,unsigned char obj)
{

	
	switch(buffer[2]){
		case Motor1://电机1控制
		obj=Motor1;
		break;
		
		case Motor2://电机2控制
		obj=Motor2;
		break;
		
		case Both://同时控制
		obj=Both;
		break;

		
		case Each://分别控制
		obj=Each;
		break;
		
		default:
    break;
	}
		
	return obj;
	

}

/*解数据帧 并存储解帧参数
@buffer:为数据帧
@arg为解包参数:
arg[0]为剩余长度校验标志位
arg[1]为CRC校验标志位
arg[2]为数据/指令控制字节 标志位
arg[3]为控制对象字节 标志位
arg[4]PWM幅度百分比

*/

void  DeserializeBuffer(unsigned char *buffer,unsigned char arg[])	
{
	unsigned long crc_res;
	unsigned long crc_combine_2byte;
	unsigned char obj;
	unsigned char dir;
	unsigned char pwm_val;
	
	
	if(buffer[1]==residue_length)//首先校验剩余长度
	{
	
					arg[0]=length_true;
					/**********校验CRC(成功后解包)***********/
					crc_res=CRC16(buffer,buffersize-2);//去除一帧最后两个CRC校验位计算CRC16值
					crc_combine_2byte=Combine_CRC_2Byte(buffer[15],buffer[16]);
				////			pr_warn_pure("打印CRC:%x\r\n",crc_ret);
				////			pr_warn_pure("打印CRC组合:%x\r\n",crc_2byte);
					if(crc_res == crc_combine_2byte){

					arg[1]=CRC_success;

					
								
								switch(buffer[0]){
									case Data_speed://数据速度报文
									break;
									
									
									case Control_direction://速度控制报文
										
											arg[2]=Control_direction;//写解包arg参数 
											arg[3]=Deserialize_Ctrl_obj(buffer,obj);			
											arg[4]=Deserialize_Ctrl_dir(buffer,dir);

											
											
									break;
									
									
									case Control_PWM://PWM控制报文
											arg[2]=Control_PWM;
											arg[3]=Deserialize_Ctrl_obj(buffer,obj);
											arg[4]=Deserialize_Ctrl_PWM(buffer,pwm_val);
									
									break;
									
									case Control_speed:
									break;
									
									case Control_PID:
									break;
									

									case Data_Line_speed: 
									break;
									
									case Data_Angular_speed:
									break;
									
									default:
									break;
									
						
						
						}//数据/指令
					
					}//CRC校验成功
					else{
					
					arg[1]=CRC_fail;
					}//CRC校验失败
					
	
	
	}//剩余长度校验
	else{
	arg[0]=length_err;
		
	}
	
}


