/*****ʮ���������ⶨЭ��****/
#include "protocol.h"

/***������ṹת��floatΪ���ֽ���  OR ���ֽ���ת��float****/
union float_trans{
	float f;
	unsigned char uch[4];
};
	




/*ת��float������Ϊuchar����
@data �����float��������
@return �����ĸ��ֽڵ�uchar�����飬ע�ⲻ��ʹ�þֲ����ֲ�����ָ����ú�����ͻᱻ����
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

				CRC16_uch(buffer,14,crc_res);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
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

/*��֡�е�PWM����
@buffer ָ֡��
@dir �����
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
arg[4]PWM���Ȱٷֱ�

*/

void  DeserializeBuffer(unsigned char *buffer,unsigned char arg[])	
{
	unsigned long crc_res;
	unsigned long crc_combine_2byte;
	unsigned char obj;
	unsigned char dir;
	unsigned char pwm_val;
	
	
	if(buffer[1]==residue_length)//����У��ʣ�೤��
	{
	
					arg[0]=length_true;
					/**********У��CRC(�ɹ�����)***********/
					crc_res=CRC16(buffer,buffersize-2);//ȥ��һ֡�������CRCУ��λ����CRC16ֵ
					crc_combine_2byte=Combine_CRC_2Byte(buffer[15],buffer[16]);
				////			pr_warn_pure("��ӡCRC:%x\r\n",crc_ret);
				////			pr_warn_pure("��ӡCRC���:%x\r\n",crc_2byte);
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
									break;
									
									case Control_PID:
									break;
									

									case Data_Line_speed: 
									break;
									
									case Data_Angular_speed:
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


