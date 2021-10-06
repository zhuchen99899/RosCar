#include "protocol.h"


/*构造函数*/
protocol::protocol()
{



}
/*析构函数*/
protocol::~protocol()

{


}

/*方向控制报文方法
@block 报文缓冲区 ;QbyteArray 类型字节流
@mode  报文模式 ;QString类型 "single" 为控制单电机,"both"为控制双电机同时工作,"each"为分别控制报文
@dir   控制电机的方向;uchar类型 以宏定义为传入参数
@motor 控制的电机对象:uchar类型 以宏定义为传入参数
*/
QByteArray protocol::Control_direction(QByteArray block,uchar motor,uchar dir)
{
     uchar CRC_out[1];
     ushort CRC_res;
     block.resize(16);
     block[0]=control_dir;
     block[1]=0x0F;

    switch (motor) {
    case Motor1://控制单电机模式
         block[2]= Motor1;
        if (dir==Motor_dir1)   block[3]=Motor_dir1;
        if (dir==Motor_dir2)   block[3]=Motor_dir2;

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif

        break;
    case Motor2:
        block[2]=Motor2;
        if (dir==Motor_dir1)   block[3]=Motor_dir1;
        if (dir==Motor_dir2)   block[3]=Motor_dir2;

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif
        break;
    case both://同步控制电机模式
        block[2]=both;     
        if (dir==Motor_dir1)   block[3]=Motor_dir1;
        if (dir==Motor_dir2)   block[3]=Motor_dir2;

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif
        break;

    case each://同时控制电机模式
        break;

    default:
        break;
    }


    return block;

}

/*PWM控制报文方法
@block 报文缓冲区 ;QbyteArray 类型字节流
@mode  报文模式 ;QString类型 "single" 为控制单电机,"both"为控制双电机同时工作,"each"为分别控制报文
@dir   控制电机的方向;uchar类型 以宏定义为传入参数
@motor 控制的电机对象:uchar类型 以宏定义为传入参数
*/
QByteArray protocol::Control_PWM(QByteArray block,uchar motor,int PWM_percentage)
{
    uchar CRC_out[1];
    ushort CRC_res;
    QByteArray PWMper;
    unsigned char * PWMper_uchar;

    block.resize(16);
    block[0]=control_PWM; //控制指令字节
    block[1]=0x0F;//剩余长度

    switch (motor) {
    case Motor1://控制单电机模式
        block[2]= Motor1;
        PWMper.append(PWM_percentage);
        PWMper_uchar=reinterpret_cast<unsigned char*>(PWMper.data());
        block[3]=PWMper_uchar[0];

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif

        break;

    case Motor2:
        block[2]=Motor2;
        PWMper.append(PWM_percentage);
        PWMper_uchar=reinterpret_cast<unsigned char*>(PWMper.data());
        block[3]=PWMper_uchar[0];

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif
        break;

    case both://同步控制电机模式
        block[2]=both;
        PWMper.append(PWM_percentage);
        PWMper_uchar=reinterpret_cast<unsigned char*>(PWMper.data());
        block[3]=PWMper_uchar[0];

        for(int i=4;i<=16;i++)
        {
        block[i]=0x00;
        }
        /*CRC校验计算*/
        CRC_res=CRC16_calculate(block);
        /*CRC转换*/
        CRC16_trans(CRC_out,CRC_res);
        /*去除字节流最后两个字节*/
        block.chop(2);
        /*加入CRC检验字节*/
        block.append(CRC_out[0]);
        block.append(CRC_out[1]);
#if  Debug
       qDebug()<<hex<<CRC_out[0]<<endl;
       qDebug()<<hex<<CRC_out[1]<<endl;
       qDebug()<<hex<<CRC_res<<endl;
       qDebug()<<block.toHex()<<endl;
#endif

        break;


    case each://同时控制电机模式
        break;

    default:
        break;
    }


    return block;


}

/*发送速度值
*@block 发送缓冲字节流
*@motor 控制对象
*@speed_val 速度值
*/
QByteArray protocol::Control_Speed(QByteArray block,uchar motor,float speed_data[]){

    ushort CRC_res;
    uchar CRC_out[1];
    float_trans_forM1_ctrl_speed trans;
    block.resize(16);
    block[0]=control_speed; //控制指令字节
    block[1]=0x0F;//剩余长度
    switch (motor) {
        case Motor1:block[2]=Motor1;
                trans.f=speed_data[0];
                block[3]=trans.uch[3];
                block[4]=trans.uch[2];
                block[5]=trans.uch[1];
                block[6]=trans.uch[0];
                for(int i=7;i<=16;i++)
                {
                block[i]=0x00;
                }

                /*CRC校验计算*/
                CRC_res=CRC16_calculate(block);
                /*CRC转换*/
                CRC16_trans(CRC_out,CRC_res);
                /*去除字节流最后两个字节*/
                block.chop(2);
                /*加入CRC检验字节*/
                block.append(CRC_out[0]);
                block.append(CRC_out[1]);
        #if  Debug
               qDebug()<<hex<<CRC_out[0]<<endl;
               qDebug()<<hex<<CRC_out[1]<<endl;
               qDebug()<<hex<<CRC_res<<endl;
               qDebug()<<block.toHex()<<endl;
        #endif
        break;

        case Motor2:
                    block[2]=Motor2;
                    trans.f=speed_data[0];
                    block[3]=trans.uch[3];
                    block[4]=trans.uch[2];
                    block[5]=trans.uch[1];
                    block[6]=trans.uch[0];
                    for(int i=7;i<=16;i++)
                    {
                    block[i]=0x00;
                    }

                    /*CRC校验计算*/
                    CRC_res=CRC16_calculate(block);
                    /*CRC转换*/
                    CRC16_trans(CRC_out,CRC_res);
                    /*去除字节流最后两个字节*/
                    block.chop(2);
                    /*加入CRC检验字节*/
                    block.append(CRC_out[0]);
                    block.append(CRC_out[1]);
            #if  Debug
                   qDebug()<<hex<<CRC_out[0]<<endl;
                   qDebug()<<hex<<CRC_out[1]<<endl;
                   qDebug()<<hex<<CRC_res<<endl;
                   qDebug()<<block.toHex()<<endl;
            #endif
        break;

            case both:
                        block[2]=both;
                        trans.f=speed_data[0];
                        block[3]=trans.uch[3];
                        block[4]=trans.uch[2];
                        block[5]=trans.uch[1];
                        block[6]=trans.uch[0];
                        for(int i=7;i<=16;i++)
                        {
                        block[i]=0x00;
                        }

                        /*CRC校验计算*/
                        CRC_res=CRC16_calculate(block);
                        qDebug()<<block<<endl;
                        qDebug()<<block.length()<<endl;
                        qDebug()<<CRC_res<<endl;
                        /*CRC转换*/
                        CRC16_trans(CRC_out,CRC_res);
                        /*去除字节流最后两个字节*/
                        block.chop(2);
                        /*加入CRC检验字节*/
                        block.append(CRC_out[0]);
                        block.append(CRC_out[1]);
                #if  Debug
                       qDebug()<<hex<<CRC_out[0]<<endl;
                       qDebug()<<hex<<CRC_out[1]<<endl;
                       qDebug()<<hex<<CRC_res<<endl;
                       qDebug()<<block.toHex()<<endl;
                #endif
                break;
        case each:
                block[2]=each;
                trans.f=speed_data[0];
                block[3]=trans.uch[3];
                block[4]=trans.uch[2];
                block[5]=trans.uch[1];
                block[6]=trans.uch[0];

                trans.f=speed_data[1];
                block[7]=trans.uch[3];
                block[8]=trans.uch[2];
                block[9]=trans.uch[1];
                block[10]=trans.uch[0];
                for(int i=11;i<=16;i++)
                {
                block[i]=0x00;
                }

                /*CRC校验计算*/
                CRC_res=CRC16_calculate(block);
                /*CRC转换*/
                CRC16_trans(CRC_out,CRC_res);
                /*去除字节流最后两个字节*/
                block.chop(2);
                /*加入CRC检验字节*/
                block.append(CRC_out[0]);
                block.append(CRC_out[1]);
        #if  Debug
               qDebug()<<hex<<CRC_out[0]<<endl;
               qDebug()<<hex<<CRC_out[1]<<endl;
               qDebug()<<hex<<CRC_res<<endl;
               qDebug()<<block.toHex()<<endl;
        #endif
            break;

        default:
            break;
    }

    return block;
}






/*发送设置的PID参数
*@block 发送缓冲字节流
*@motor 控制对象
*@PID参数结构体
*/
 QByteArray protocol::Control_set_PID(QByteArray block,uchar motor,PID PID_data){

 ushort CRC_res;
 uchar CRC_out[1];
 float_trans_forM1_PID trans;
     block.resize(16);
     block[0]=control_PID; //控制指令字节
     block[1]=0x0F;//剩余长度
     switch (motor) {
     case Motor1://控制单电机模式
         block[2]= Motor1;
         /*写入F值*/
         trans.f=PID_data.P;
         block[3]=trans.uch[3];
         block[4]=trans.uch[2];
         block[5]=trans.uch[1];
         block[6]=trans.uch[0];
         /*写入I值*/
         trans.f=PID_data.I;
         block[7]=trans.uch[3];
         block[8]=trans.uch[2];
         block[9]=trans.uch[1];
         block[10]=trans.uch[0];
         /*写入D值*/
         trans.f=PID_data.D;
         block[11]=trans.uch[3];
         block[12]=trans.uch[2];
         block[13]=trans.uch[1];
         block[14]=trans.uch[0];
         for(int i=15;i<=16;i++)
         {
         block[i]=0x00;
         }
         /*CRC校验计算*/
         CRC_res=CRC16_calculate(block);
         /*CRC转换*/
         CRC16_trans(CRC_out,CRC_res);
         /*去除字节流最后两个字节*/
         block.chop(2);
         /*加入CRC检验字节*/
         block.append(CRC_out[0]);
         block.append(CRC_out[1]);
 #if  Debug
        qDebug()<<hex<<CRC_out[0]<<endl;
        qDebug()<<hex<<CRC_out[1]<<endl;
        qDebug()<<hex<<CRC_res<<endl;
        qDebug()<<block.toHex()<<endl;
 #endif

         break;
     case Motor2://控制单电机模式
         block[2]= Motor2;
         /*写入F值*/
         trans.f=PID_data.P;
         block[3]=trans.uch[3];
         block[4]=trans.uch[2];
         block[5]=trans.uch[1];
         block[6]=trans.uch[0];
         /*写入I值*/
         trans.f=PID_data.I;
         block[7]=trans.uch[3];
         block[8]=trans.uch[2];
         block[9]=trans.uch[1];
         block[10]=trans.uch[0];
         /*写入D值*/
         trans.f=PID_data.D;
         block[11]=trans.uch[3];
         block[12]=trans.uch[2];
         block[13]=trans.uch[1];
         block[14]=trans.uch[0];
         for(int i=15;i<=16;i++)
         {
         block[i]=0x00;
         }
         /*CRC校验计算*/
         CRC_res=CRC16_calculate(block);
         /*CRC转换*/
         CRC16_trans(CRC_out,CRC_res);
         /*去除字节流最后两个字节*/
         block.chop(2);
         /*加入CRC检验字节*/
         block.append(CRC_out[0]);
         block.append(CRC_out[1]);
 #if  Debug
        qDebug()<<hex<<CRC_out[0]<<endl;
        qDebug()<<hex<<CRC_out[1]<<endl;
        qDebug()<<hex<<CRC_res<<endl;
        qDebug()<<block.toHex()<<endl;
 #endif
         break;
     case both://同步控制电机模式
         block[2]= both;
         /*写入F值*/
         trans.f=PID_data.P;
         block[3]=trans.uch[3];
         block[4]=trans.uch[2];
         block[5]=trans.uch[1];
         block[6]=trans.uch[0];
         /*写入I值*/
         trans.f=PID_data.I;
         block[7]=trans.uch[3];
         block[8]=trans.uch[2];
         block[9]=trans.uch[1];
         block[10]=trans.uch[0];
         /*写入D值*/
         trans.f=PID_data.D;
         block[11]=trans.uch[3];
         block[12]=trans.uch[2];
         block[13]=trans.uch[1];
         block[14]=trans.uch[0];
         for(int i=15;i<=16;i++)
         {
         block[i]=0x00;
         }
         /*CRC校验计算*/
         CRC_res=CRC16_calculate(block);
         /*CRC转换*/
         CRC16_trans(CRC_out,CRC_res);
         /*去除字节流最后两个字节*/
         block.chop(2);
         /*加入CRC检验字节*/
         block.append(CRC_out[0]);
         block.append(CRC_out[1]);
 #if  Debug
        qDebug()<<hex<<CRC_out[0]<<endl;
        qDebug()<<hex<<CRC_out[1]<<endl;
        qDebug()<<hex<<CRC_res<<endl;
        qDebug()<<block.toHex()<<endl;
 #endif

         break;
        //无each模式
     default:
         break;
     }




    return block;

 }


/*
发送速度与方向设定
*/
QByteArray protocol::Control_SpeedAndDir(QByteArray block, uchar motor, float speed_data[], uchar dir[])
{
ushort CRC_res;
uchar CRC_out[1];
float_trans_forM1_ctrl_speed trans;
block.resize(16);
block[0]=control_speedAnddir; //控制指令字节
block[1]=0x0F;//剩余长度
switch (motor) {
    case Motor1:block[2]=Motor1;
            trans.f=speed_data[0];
            block[3]=trans.uch[3];
            block[4]=trans.uch[2];
            block[5]=trans.uch[1];
            block[6]=trans.uch[0];
            block[7]=dir[0];
            for(int i=8;i<=16;i++)
            {
            block[i]=0x00;
            }

            /*CRC校验计算*/
            CRC_res=CRC16_calculate(block);
            /*CRC转换*/
            CRC16_trans(CRC_out,CRC_res);
            /*去除字节流最后两个字节*/
            block.chop(2);
            /*加入CRC检验字节*/
            block.append(CRC_out[0]);
            block.append(CRC_out[1]);
    #if  Debug
           qDebug()<<hex<<CRC_out[0]<<endl;
           qDebug()<<hex<<CRC_out[1]<<endl;
           qDebug()<<hex<<CRC_res<<endl;
           qDebug()<<block.toHex()<<endl;
    #endif
    break;

    case Motor2:
                block[2]=Motor2;
                trans.f=speed_data[0];
                block[3]=trans.uch[3];
                block[4]=trans.uch[2];
                block[5]=trans.uch[1];
                block[6]=trans.uch[0];
                block[7]=dir[0];
                for(int i=8;i<=16;i++)
                {
                block[i]=0x00;
                }

                /*CRC校验计算*/
                CRC_res=CRC16_calculate(block);
                /*CRC转换*/
                CRC16_trans(CRC_out,CRC_res);
                /*去除字节流最后两个字节*/
                block.chop(2);
                /*加入CRC检验字节*/
                block.append(CRC_out[0]);
                block.append(CRC_out[1]);
        #if  Debug
               qDebug()<<hex<<CRC_out[0]<<endl;
               qDebug()<<hex<<CRC_out[1]<<endl;
               qDebug()<<hex<<CRC_res<<endl;
               qDebug()<<block.toHex()<<endl;
        #endif
    break;

        case both:
                    block[2]=both;
                    trans.f=speed_data[0];
                    block[3]=trans.uch[3];
                    block[4]=trans.uch[2];
                    block[5]=trans.uch[1];
                    block[6]=trans.uch[0];
                    block[7]=dir[0];
                    for(int i=8;i<=16;i++)
                    {
                    block[i]=0x00;
                    }

                    /*CRC校验计算*/
                    CRC_res=CRC16_calculate(block);
                    qDebug()<<block<<endl;
                    qDebug()<<block.length()<<endl;
                    qDebug()<<CRC_res<<endl;
                    /*CRC转换*/
                    CRC16_trans(CRC_out,CRC_res);
                    /*去除字节流最后两个字节*/
                    block.chop(2);
                    /*加入CRC检验字节*/
                    block.append(CRC_out[0]);
                    block.append(CRC_out[1]);
            #if  Debug
                   qDebug()<<hex<<CRC_out[0]<<endl;
                   qDebug()<<hex<<CRC_out[1]<<endl;
                   qDebug()<<hex<<CRC_res<<endl;
                   qDebug()<<block.toHex()<<endl;
            #endif
            break;
    case each:
            block[2]=each;
            trans.f=speed_data[0];
            block[3]=trans.uch[3];
            block[4]=trans.uch[2];
            block[5]=trans.uch[1];
            block[6]=trans.uch[0];

            trans.f=speed_data[1];
            block[7]=trans.uch[3];
            block[8]=trans.uch[2];
            block[9]=trans.uch[1];
            block[10]=trans.uch[0];
            block[11]=dir[0];
            block[12]=dir[1];
            for(int i=13;i<=16;i++)
            {
            block[i]=0x00;
            }

            /*CRC校验计算*/
            CRC_res=CRC16_calculate(block);
            /*CRC转换*/
            CRC16_trans(CRC_out,CRC_res);
            /*去除字节流最后两个字节*/
            block.chop(2);
            /*加入CRC检验字节*/
            block.append(CRC_out[0]);
            block.append(CRC_out[1]);
    #if  Debug
           qDebug()<<hex<<CRC_out[0]<<endl;
           qDebug()<<hex<<CRC_out[1]<<endl;
           qDebug()<<hex<<CRC_res<<endl;
           qDebug()<<block.toHex()<<endl;
    #endif
        break;

    default:
        break;
}

return block;
}
/*
接收一帧数据
//筛选数据类型
@block  输入字节流
*/
uchar protocol::Receive_data(QByteArray block,qint64 tcp_readsize)
{
    uchar datatype;
    ulong crc_res;
    ulong crc_combine_2byte;

    if ((0x0f==block[1])&&tcp_readsize==17){//数据长度校验
        crc_res=CRC16_calculate(block);
#if  Debug
        qDebug() <<hex<<crc_res<<endl;
#endif

        crc_combine_2byte =CRC16_comb(block,tcp_readsize);

#if  Debug
        qDebug() <<hex<<crc_combine_2byte<<endl;
#endif

        if(crc_res==crc_combine_2byte){

            switch (block[0]) {
            case data_speed:
                datatype=data_speed;
                break;
            case data_Line_speed:
                datatype=data_Line_speed;
                break;
            case data_Angular_speed:
                datatype=data_Angular_speed;
                break;
            default:
                break;
            }
        }//CRC 校验
        else{

        qDebug()<<"crc校验失败"<<endl;


        datatype=0xff;
        }
   }//帧长度校验

   else
   {

     qDebug()<<"数据帧长度出错"<<endl;
    tcp_readsize=17;

     /*获取前17字节*/

    for(int i=0;i<=16;i++)
    {
       block[i]=block[i];
    }

    crc_res=CRC16_calculate(block);
#if  Debug
    qDebug() <<hex<<crc_res<<endl;
#endif

    crc_combine_2byte =CRC16_comb(block,tcp_readsize);

#if  Debug
    qDebug() <<hex<<crc_combine_2byte<<endl;
#endif

    if(crc_res==crc_combine_2byte){

        switch (block[0]) {
        case data_speed:
            datatype=data_speed;
            break;
        case data_Line_speed:
            datatype=data_Line_speed;
            break;
        case data_Angular_speed:
            datatype=data_Angular_speed;
            break;
        default:
            break;
        }
    }//CRC 校验
    else{

    qDebug()<<"crc校验失败"<<endl;


    datatype=0xff;
    }

   }
    return datatype;

}


/*解速度帧数据
@block 字节流
data_head
*/
void protocol::Receive_Data_Speed(float speed_data[],QByteArray block)
{
    float_trans_forM1_rec_speed trans;


/*控制对象字节*/
    switch (block[2]) {
    case both:
        trans.uch[0]=block[6];
        trans.uch[1]=block[5];
        trans.uch[2]=block[4];
        trans.uch[3]=block[3];
        speed_data[0]=trans.f;
        speed_data[1]=0.0;

        break;
    case Motor1://电机1
        trans.uch[0]=block[6];
        trans.uch[1]=block[5];
        trans.uch[2]=block[4];
        trans.uch[3]=block[3];
        speed_data[0]=trans.f;
        speed_data[1]=0.0;

        break;

    case Motor2:
        trans.uch[0]=block[6];
        trans.uch[1]=block[5];
        trans.uch[2]=block[4];
        trans.uch[3]=block[3];
        speed_data[0]=trans.f;
        speed_data[1]=0.0;
        break;
    case each:
        trans.uch[0]=block[6];
        trans.uch[1]=block[5];
        trans.uch[2]=block[4];
        trans.uch[3]=block[3];
        speed_data[0]=trans.f;
        trans.uch[0]=block[10];
        trans.uch[1]=block[9];
        trans.uch[2]=block[8];
        trans.uch[3]=block[7];
        speed_data[1]=trans.f;


        break;
    default:
        break;
    }




}





/*CRC16——MODBUS 校验计算
@QByteArray block 所需校验字节流(固定校验字节流前14位)
*/
 ushort protocol::CRC16_calculate(QByteArray block)
{

 uchar *buffer;
 ushort CRC16_res;

 buffer= reinterpret_cast<unsigned char*>(block.data());
 CRC16_res=CRC16_Modbus(buffer,15);

 return CRC16_res;
}

 /*CRC16校验结果转换 ushort 型 ->转为两个字节 uchar
 @CRC16_res :输入CRC计算结果(两字节)
 @CRC_out_uchar:类型转换存储结果数组
 */
void protocol::CRC16_trans(uchar *CRC_out_uchar,ushort CRC16_res)
{
uchar CRC16_u8_low;
uchar CRC16_u8_high;
CRC16_u8_low=CRC16_res&0xFF;
CRC16_u8_high=(CRC16_res>>8)&0xFF;

CRC_out_uchar[0]=CRC16_u8_high;
CRC_out_uchar[1]=CRC16_u8_low;
}

/*取字节流最后两位组成CRC校验值
@block :字节流
@qint64 : tcp 缓冲区大小
*/
ushort protocol::CRC16_comb(QByteArray block,qint64 tcp_buffsize)
{
    uchar *buffer;
    ushort CRC16_comb;

    buffer= reinterpret_cast<unsigned char*>(block.data());
    CRC16_comb=Combine_CRC_2Byte(buffer[tcp_buffsize-2],buffer[tcp_buffsize-1]);

    return CRC16_comb;

}


