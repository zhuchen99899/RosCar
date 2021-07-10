#ifndef CAR_CONFIG_H
#define CAR_CONFIG_H

/********调试级别*************/
#define printTcp 1

/********Debug模式************/
#define Debug 0


//首字节
#define data_speed          0x00
#define control_dir         0x01
#define control_PWM         0x02
#define control_speed       0x03
#define control_PID         0x04
#define data_Line_speed     0x05
#define data_Angular_speed  0x06
#define control_speedAnddir 0x07

#define data_err           0xff //数据帧出错
/********控制模式相关宏*********/

#define both 0x00
#define each 0x03

/********电机控制对象宏*********/

#define Motor1 0x01
#define Motor2 0x02


/********电机方向控制相关宏*********/

#define Motor_dir1 0x01
#define Motor_dir2 0x02


/********电机PWM控制相关宏*********/



/*******电机PID控制相关**********/
typedef struct set_PID{
 float P;
 float I;
 float D;
}PID;



#endif

