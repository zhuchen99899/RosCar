/*****十六进制自拟定协议****/
#include "protocol.h"





/*转换float型数据为uchar数据
@data 输入的float类型数据
@uchar_data 返回四个字节的uchar型数组，注意不能使用局部，局部变量指针调用函数后就会被销毁
*/

void float_to_uchar(float data,unsigned char *uchar_data)
{

union float_trans trans;
trans.f=data;
uchar_data[0]=trans.uch[3];
uchar_data[1]=trans.uch[2];
uchar_data[2]=trans.uch[1];
uchar_data[3]=trans.uch[0];
	
}



/*转换uchar型数据为float数据
@uchar_data 输入的uchar类型数据
@return 返回3个数量float型数组，注意不能使用局部，局部变量指针调用函数后就会被销毁

*/

void uchar_to_float_speed(unsigned char obj,unsigned char *uchar_data,float *data)
{
	
union float_trans_rec trans;
	int i=0;
//trans.f=data;
	
	
if((obj==Motor1)||(obj==Motor2)||(obj==Both)){
	
		for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[3-i];
		}
	
		data[0]=trans.f;
	}
	
	
	else{
	 //分别控制
				for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[3-i];
		}
	
		data[0]=trans.f;
		
				for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[7-i];
		}
		data[1]=trans.f;
		
//				for(i=0;i<=3;i++){
//			trans.uch[i]=uchar_data[11-i];
//		}
//		
//		data[2]=trans.f;
	}
}

/*转换uchar型数据为float数据
@uchar_data 输入的uchar类型数据
@return 返回3个数量float型数组，注意不能使用局部，局部变量指针调用函数后就会被销毁

*/

void uchar_to_float_PID(unsigned char obj,unsigned char *uchar_data,float *data)
{
	
union float_trans_rec trans;
	int i=0;
//trans.f=data;
	
	
if((obj==Motor1)||(obj==Motor2)||(obj==Both)){
	
				for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[3-i];
		}
	
		data[0]=trans.f;
		
				for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[7-i];
		}
		data[1]=trans.f;
		
				for(i=0;i<=3;i++){
			trans.uch[i]=uchar_data[11-i];
		}
		
		data[2]=trans.f;
	}
	
	
	else{
		
		
		
	 //分别控制

	}
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

				CRC16_uch(buffer,buffersize-1,crc_res);//去除一帧最后两个CRC校验位计算CRC16值
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			
			case Motor2:
				buffer[1]=residue_length;
				buffer[2]=Motor2;
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

				CRC16_uch(buffer,buffersize-1,crc_res);//去除一帧最后两个CRC校验位计算CRC16值
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			case Both:

				buffer[1]=residue_length;
				buffer[2]=Both;
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

				CRC16_uch(buffer,buffersize-1,crc_res);//去除一帧最后两个CRC校验位计算CRC16值
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
				break;
			case Each:
				buffer[1]=residue_length;
				buffer[2]=Each;
				//转化float类型数据
				float_to_uchar(data[0],uchar_data);
			
				for(i=0;i<=3;i++){
					
				buffer[3+i]=uchar_data[i];
				}

				float_to_uchar(data[1],uchar_data);
				for(i=0;i<=3;i++){
					
				buffer[7+i]=uchar_data[i];
				}
				for(i=11;i<=14;i++)
				{
				buffer[i]=0x00;
				}

				CRC16_uch(buffer,buffersize-1,crc_res);//去除一帧最后两个CRC校验位计算CRC16值
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

/*解帧PID负载
@buffer 帧指针
@PID 数组 根据控制对象不同
*/
static void Deserialize_Ctrl_PID(unsigned char *buffer,unsigned char PID[])
{
	int i=0;
	if((buffer[2]==Both)||(buffer[2]==Motor1)||(buffer[2]==Motor2)){

		for(i=0;i<=11;i++)
		{
		//取出PID数值
		PID[i]=buffer[3+i];
		}
		

	}
	else{//无each
	
	
	}
		

}

/*解帧中的speed负载
@buffer 帧指针
@speed 数组 根据控制对象不同
*/
static void Deserialize_Ctrl_speed(unsigned char *buffer,unsigned char speed[])
{
	int i;
	switch(buffer[2]){
		case Both:
			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];
		break;
		case Motor1:

			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];

		break;
		
		case Motor2:
			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];
		break;
		case Each:
			for(i=0;i<=7;i++){
			speed[i]=buffer[3+i];
			}					
		break;
			
		default:
		break;
	}



}


/*解帧中的speed 和 dir 负载
@buffer 帧指针
@speed 数组 根据控制对象不同
@dir   数组 根据控制对象不同
*/
static void Deserialize_Ctrl_speedAnddir(unsigned char *buffer,unsigned char speed[],unsigned char dir[])
{
	int i;
	switch(buffer[2]){
		case Both:
			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];
			dir[0]=buffer[7];
		break;
		
		case Motor1:

			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];
			dir[0]=buffer[7];
		break;
		
		case Motor2:
			speed[0]=buffer[3];
			speed[1]=buffer[4];
			speed[2]=buffer[5];
			speed[3]=buffer[6];
			dir[0]=buffer[7];
		break;
		
		case Each:
			for(i=0;i<=7;i++){
			speed[i]=buffer[3+i];
			}					
			dir[0]=buffer[11];
			dir[1]=buffer[12];
		break;
			
		default:
		break;
	}



}





/*解帧中的PWM负载
@buffer 帧指针
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
arg[4]PWM幅度百分比  // 方向  
arg[4~12] 负载

*/

void  DeserializeBuffer(unsigned char *buffer,unsigned char arg[],float data[])	
{

	unsigned long crc_res;
	unsigned long crc_combine_2byte;
	unsigned char obj;
	unsigned char dir;
	unsigned char pwm_val;
	unsigned char ctrl_speed[12]; 
	unsigned char ctrl_PID[12];
	unsigned char ctrl_dir[2];
	
	if(buffer[1]==residue_length)//首先校验剩余长度
	{
	
					arg[0]=length_true;
					/**********校验CRC(成功后解包)***********/
					crc_res=CRC16(buffer,buffersize-1);//去除一帧最后两个CRC校验位计算CRC16值
					crc_combine_2byte=Combine_CRC_2Byte(buffer[15],buffer[16]);
//							pr_warn_pure("打印CRC:%x\r\n",crc_res);
//							pr_warn_pure("打印CRC组合:%x\r\n",crc_combine_2byte);
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
											arg[2]=Control_speed;
											arg[3]=Deserialize_Ctrl_obj(buffer,obj);
											Deserialize_Ctrl_speed(buffer,ctrl_speed);
											uchar_to_float_speed(arg[3],ctrl_speed,data);
				
											

									
									break;
									
									case Control_PID://PID控制报文
										arg[2]=Control_PID;
										arg[3]=Deserialize_Ctrl_obj(buffer,obj);
										Deserialize_Ctrl_PID(buffer,ctrl_PID);
										uchar_to_float_PID(arg[3],ctrl_PID,data);
									
									break;
									

									case Data_Line_speed: 
										
									break;
									
									case Data_Angular_speed:
										
									break;
									
									case Control_speedAnddir:
										arg[2]=Control_speedAnddir;
										arg[3]=Deserialize_Ctrl_obj(buffer,obj);
										
										Deserialize_Ctrl_speedAnddir(buffer,ctrl_speed,ctrl_dir);	
										arg[4]=ctrl_dir[0];
										arg[5]=ctrl_dir[1];
										uchar_to_float_speed(arg[3],ctrl_speed,data);
									
									
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


