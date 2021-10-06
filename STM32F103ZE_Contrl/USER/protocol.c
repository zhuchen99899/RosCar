/*****ʮ���������ⶨЭ��****/
#include "protocol.h"





/*ת��float������Ϊuchar����
@data �����float��������
@uchar_data �����ĸ��ֽڵ�uchar�����飬ע�ⲻ��ʹ�þֲ����ֲ�����ָ����ú�����ͻᱻ����
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



/*ת��uchar������Ϊfloat����
@uchar_data �����uchar��������
@return ����3������float�����飬ע�ⲻ��ʹ�þֲ����ֲ�����ָ����ú�����ͻᱻ����

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
	 //�ֱ����
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

/*ת��uchar������Ϊfloat����
@uchar_data �����uchar��������
@return ����3������float�����飬ע�ⲻ��ʹ�þֲ����ֲ�����ָ����ú�����ͻᱻ����

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
		
		
		
	 //�ֱ����

	}
}




/********************��֡����*****************************/
/*�������֡
@Datahead_Byte ����ָ���ֽ�
@obj ���ƶ���
@data float��������
*/
void SerializeBuffer(unsigned char *buffer,unsigned char Datahead_Byte,unsigned char obj,float data[])
{
	
unsigned char	uchar_data[4];
int i;
unsigned char crc_res[2];
switch(Datahead_Byte)
{
	case Data_speed://�����ٶ�����
		
		buffer[0]=Data_speed;
		switch(obj){
			case Motor1:
				buffer[1]=residue_length;
				buffer[2]=Motor1;
				//ת��float��������
				float_to_uchar(data[0],uchar_data);
				buffer[3]=uchar_data[0];
				buffer[4]=uchar_data[1];
				buffer[5]=uchar_data[2];
				buffer[6]=uchar_data[3];

				for(i=7;i<=14;i++)
				{
				buffer[i]=0x00;
				}

				CRC16_uch(buffer,buffersize-1,crc_res);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			
			case Motor2:
				buffer[1]=residue_length;
				buffer[2]=Motor2;
				//ת��float��������
				float_to_uchar(data[0],uchar_data);
				buffer[3]=uchar_data[0];
				buffer[4]=uchar_data[1];
				buffer[5]=uchar_data[2];
				buffer[6]=uchar_data[3];

				for(i=7;i<=14;i++)
				{
				buffer[i]=0x00;
				}

				CRC16_uch(buffer,buffersize-1,crc_res);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			case Both:

				buffer[1]=residue_length;
				buffer[2]=Both;
				//ת��float��������
				float_to_uchar(data[0],uchar_data);
				buffer[3]=uchar_data[0];
				buffer[4]=uchar_data[1];
				buffer[5]=uchar_data[2];
				buffer[6]=uchar_data[3];

				for(i=7;i<=14;i++)
				{
				buffer[i]=0x00;
				}

				CRC16_uch(buffer,buffersize-1,crc_res);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
				break;
			case Each:
				buffer[1]=residue_length;
				buffer[2]=Each;
				//ת��float��������
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

				CRC16_uch(buffer,buffersize-1,crc_res);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
				buffer[15]=crc_res[0];
				buffer[16]=crc_res[1];
			break;
			default:
			break;
		}//���ƶ���
	
			
		break;
	case Data_Line_speed:
		break;
	case Data_Angular_speed:
		break;
	default:
		break;
}//ָ���ֽ�



}


/*****************��֡����*****************************/

/*��֡PID����
@buffer ָ֡��
@PID ���� ���ݿ��ƶ���ͬ
*/
static void Deserialize_Ctrl_PID(unsigned char *buffer,unsigned char PID[])
{
	int i=0;
	if((buffer[2]==Both)||(buffer[2]==Motor1)||(buffer[2]==Motor2)){

		for(i=0;i<=11;i++)
		{
		//ȡ��PID��ֵ
		PID[i]=buffer[3+i];
		}
		

	}
	else{//��each
	
	
	}
		

}

/*��֡�е�speed����
@buffer ָ֡��
@speed ���� ���ݿ��ƶ���ͬ
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


/*��֡�е�speed �� dir ����
@buffer ָ֡��
@speed ���� ���ݿ��ƶ���ͬ
@dir   ���� ���ݿ��ƶ���ͬ
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





/*��֡�е�PWM����
@buffer ָ֡��
@PWM ��ֵ ���ٷ�����Ҫ����100��
*/
static unsigned char Deserialize_Ctrl_PWM(unsigned char *buffer,unsigned char PWM_val)
{
	PWM_val=buffer[3];
	return PWM_val;
	
}





/*��֡�еķ�����
@buffer ָ֡��
@dir �����
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


/*��֡�еĶ����ֽ�
@buffer ָ֡��
@obj���� ��

*/

static unsigned char Deserialize_Ctrl_obj(unsigned char *buffer,unsigned char obj)
{

	
	switch(buffer[2]){
		case Motor1://���1����
		obj=Motor1;
		break;
		
		case Motor2://���2����
		obj=Motor2;
		break;
		
		case Both://ͬʱ����
		obj=Both;
		break;

		
		case Each://�ֱ����
		obj=Each;
		break;
		
		default:
    break;
	}
		
	return obj;
	

}



/*������֡ ���洢��֡����
@buffer:Ϊ����֡
@argΪ�������:
arg[0]Ϊʣ�೤��У���־λ
arg[1]ΪCRCУ���־λ
arg[2]Ϊ����/ָ������ֽ� ��־λ
arg[3]Ϊ���ƶ����ֽ� ��־λ
arg[4]PWM���Ȱٷֱ�  // ����  
arg[4~12] ����

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
	
	if(buffer[1]==residue_length)//����У��ʣ�೤��
	{
	
					arg[0]=length_true;
					/**********У��CRC(�ɹ�����)***********/
					crc_res=CRC16(buffer,buffersize-1);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
					crc_combine_2byte=Combine_CRC_2Byte(buffer[15],buffer[16]);
//							pr_warn_pure("��ӡCRC:%x\r\n",crc_res);
//							pr_warn_pure("��ӡCRC���:%x\r\n",crc_combine_2byte);
					if(crc_res == crc_combine_2byte){

					arg[1]=CRC_success;

					
								
								switch(buffer[0]){
									case Data_speed://�����ٶȱ���
									break;
									
									
									case Control_direction://�ٶȿ��Ʊ���
										
											arg[2]=Control_direction;//д���arg���� 
											arg[3]=Deserialize_Ctrl_obj(buffer,obj);			
											arg[4]=Deserialize_Ctrl_dir(buffer,dir);


											
											
									break;
									
									
									case Control_PWM://PWM���Ʊ���
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
									
									case Control_PID://PID���Ʊ���
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
									
						
						
						}//����/ָ��
					
					}//CRCУ��ɹ�
					else{
					
					arg[1]=CRC_fail;
					}//CRCУ��ʧ��
					
	
	
	}//ʣ�೤��У��
	else{
	arg[0]=length_err;
		
	}
	
}


