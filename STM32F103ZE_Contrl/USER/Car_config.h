#ifndef __CAR_CONFIG_H	
#define __CAR_CONFIG_H	 

/********电机/编码器相关相关******************/

//#define  circle_period  1560  //电机转动一圈编码器脉冲数= 编码器线数* 电机减速比 *频率倍数


#define  Encoder1_period 65535
#define  Motor_Reduction_ratio  30  //电机减速比
#define  Encoder1_n_lines 13  //编码器线数
#define  period_mode   4      //四倍频模式
#define  period_rate   4      //四倍频倍率为4
#define  Perimeter 11.932 // 小车车轮周长 cm


/* 启用 WIFI小车*/
#define Car_Wifi_Receive_OFF  0
#define Car_Wifi_Send_OFF  0
#define Car_PWM_ctrl  0  //小车PWM控制，开启后所有闭环控制失效



#define Car_ctrl_speed_loop  1
#define Car_ctrl_length_loop 0
#define Car_ctrl_force_loop  0
#define Car_ctrl_dirc_inquiry 0





#endif



