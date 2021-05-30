#ifndef __CAR_CONFIG_H	
#define __CAR_CONFIG_H	 

/********电机/编码器相关相关******************/

//#define  circle_period  1560  //电机转动一圈编码器脉冲数= 编码器线数* 电机减速比 *频率倍数


#define  Encoder1_period 65535
#define  Motor_Reduction_ratio  30  //电机减速比
#define  Encoder1_n_lines 13  //编码器线数
#define  period_mode   4      //四倍频模式
#define  period_rate   4      //四倍频倍率为4
#define  Perimeter 0.11932 // 小车车轮周长



#define Car_Wifi_Receive_OFF  1

#define Car_Wifi_Send_OFF  1


#endif



