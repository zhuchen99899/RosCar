#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "qbytearray.h"
#include "car_config.h"

#include <QDebug>

extern "C"{
#include "crc16.h"
}
class protocol
{
public: union float_trans_forM1_rec_speed{

        float f;
        unsigned char uch[4];
    };

       union float_trans_forM1_PID{
       float f;
       unsigned char uch[4];
    };
       union float_trans_forM1_ctrl_speed{
       float f;
       unsigned char uch[4];
    };

public:
    protocol();
    ~protocol();

    /*方向控制报文方法*/
    QByteArray Control_direction(QByteArray block,uchar motor,uchar dir);
    /*pwm控制报文方法*/
    QByteArray Control_PWM(QByteArray block,uchar motor,int PWM_percentage);
    QByteArray Control_set_PID(QByteArray block,uchar motor,PID PID_data);
    QByteArray Control_Speed(QByteArray block,uchar motor,float speed_data[]);
    QByteArray Control_SpeedAndDir(QByteArray block,uchar motor,float speed_data[],uchar dir[]);
    /*接收一帧数据，筛选数据类型，并进行CRC校验和长度校验*/
    uchar Receive_data(QByteArray block,qint64 tcp_readsize);
    /*校验后,按照数据包头类型，选择解帧方法*/
    /*解速度帧数据*/
    void Receive_Data_Speed(float speed_data[],QByteArray block);

private:
    /*CRC16——MODBUS 校验计算*/
    ushort CRC16_calculate(QByteArray block);
     /*CRC16校验结果转换 ushort 型 ->转为两个字节 uchar*/
    void CRC16_trans(uchar *CRC_out_uchar,ushort CRC16_res);
    /*取字节流最后两位组成CRC校验值*/
    ushort CRC16_comb(QByteArray block,qint64 tcp_buffsize);
};

#endif // PROTOCOL_H
