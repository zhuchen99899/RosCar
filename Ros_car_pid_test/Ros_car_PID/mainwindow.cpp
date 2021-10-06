#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"
#include<iostream>

extern "C"{
#include "crc16.h"

}



/********服务器相关宏********/
#define server "192.168.43.152"
//#define server "127.0.0.1"
#define port 8090






MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QByteArray a;
    ui->setupUi(this);
    setWindowTitle("ROS-Car-set");



    Qchar_init();
    tcpClient_init();
    QgamepadConnect();


    //mysignals
    connect(this,SIGNAL(GamePAD_Control(int)),this,SLOT(GamePadVal_changed(int)));
    connect(this,SIGNAL(GamePAD_Control_base(int)),this,SLOT(GamePadVal_base_changed(int)));
    //连接LCD显示槽
        connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),ui->lcdNumber,SLOT(display(int)));
        connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
    //手柄调速信号槽
        connect(ui->horizontalSlider_3,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider3_valueChanged(int)));
    //连接pushButton信号槽
        connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(QPushButton_clicked())); //电机方向1
        connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(QPushButton1_clicked()));//电机方向2
        connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(QPushButton4_clicked()));//Qchart暂停
        connect(ui->pushButton_14,SIGNAL(clicked()),this,SLOT(QPushButton14_clicked()));//Qchart接收
        connect(ui->pushButton_15,SIGNAL(clicked()),this,SLOT(QPushButton15_clicked()));//Qchart清空
        connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(QPushButton3_clicked()));//发送PID设置
        connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(QPushButton6_clicked()));//方向左
        connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(QPushButton7_clicked()));//方向上
        connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(QPushButton8_clicked()));//方向下
        connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(QPushButton9_clicked()));//方向右
   //连接doublespin 槽 ，slider槽
       connect(ui->doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(slotDoubleSpinbox_slider()));
       connect(ui->horizontalSlider_2, SIGNAL(valueChanged(int)), this, SLOT(slotslider_DoubleSpinBox()));

   //TCP连接信号槽
        connect(tcpClient, SIGNAL(readyRead()), this, SLOT(readMessage()) );//当有消息接受时会触发接收

   //ComBox电机对象选择信号槽
        connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)) ,this,SLOT(Target_Changed()) );

    //Qchart 中Checkbox 电机1速度stateChanged 信号槽
        connect(ui->checkBox,SIGNAL(stateChanged(int)),this,SLOT(slotcheckbox_changed(int)));
        connect(ui->checkBox_2,SIGNAL(stateChanged(int)),this,SLOT(slotcheckbox2_changed(int)));

}
/****************窗口析构函数*********************/
MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::RealtimeDataSlot()
{

    qreal x = chart.plotArea().width() / m_axis.tickCount();//x 代表的窗口横坐标方向滚动的区域大小
    qreal y = (m_axis.max() - m_axis.min()) / m_axis.tickCount();
          m_x += y;

          m_y1 = Receive_Speed.Motor1_Speed;
          m_y2 = Receive_Speed.Motor2_speed;

          m_series.append(m_x, m_y1);
          m_series2.append(m_x,m_y2);

          m_series.setUseOpenGL(true); //opengl 加速
          m_series2.setUseOpenGL(true);//opengl 加速

          if(m_x>(m_axis.max() - m_axis.min())-1) //当数据占满窗口(1的余量，可以看起末端数据)
          chart.scroll(x,0);//x 的单位不是横坐标的单位，而是窗口像素




}




/**************Qchar初始化函数*******************/
void MainWindow::Qchar_init()
{


    ui->setupUi(this);

    connect(&m_timer,SIGNAL(timeout()),this,SLOT(RealtimeDataSlot()));
    m_timer.setInterval(500);
    m_x=0;
    m_y1=0;
    m_y2=0;
//    chart.setTheme(QChart::ChartThemeBlueCerulean);//设置系统主题
    chart.setTitle("电机速度曲线");//设置标题
    chart.setTitleFont(QFont("微软雅黑",10));

/*电机1曲线*/

    QPen darkRed(Qt::darkRed);
    darkRed.setWidth(2);
    m_series.setPen(darkRed);
    m_series.append(m_x, m_y1);
    m_series.setName("Motor1");

/*电机2曲线*/
    QPen darkBlue(Qt::darkBlue);
    darkBlue.setWidth(2);
    m_series2.setPen(darkBlue);
    m_series2.append(m_x, m_y2);
    m_series2.setName("Motor2");


    chart.addSeries(&m_series);
    chart.addSeries(&m_series2);

    chart.createDefaultAxes();
    chart.setAxisX(&m_axis,&m_series);
    chart.setAxisX(&m_axis,&m_series2);
    m_axis.setTickCount(5);
    chart.axisX()->setRange(0,50);
    chart.axisY()->setRange(-10, 100);



    //隐藏背景网格线
//    chart.axisX()->setGridLineVisible(false);
//    chart.axisY()->setGridLineVisible(false);
//    chart.legend()->setVisible(true);              //图例显示



    chart.axisY()->setTitleBrush(Qt::blue);
    chart.axisY()->setTitleText("速度");

  auto *chartView = new ChartView(&chart);//使用自定义ChartView




    chartView->setRenderHint(QPainter::Antialiasing);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(chartView, 0, 0);
    ui->widget->setLayout(baseLayout);

//    ui->widget->setChart(&chart); //显示到QWidget控件


    m_timer.start();






}
/**************TCP连接初始化函数*******************/
void MainWindow::tcpClient_init(){

    tcpClient = new QTcpSocket(this);   //实例化tcpClient
    tcpClient->abort(); //取消原有链接
    tcpClient->connectToHost(server,port);
    if (tcpClient->waitForConnected(1000))  // 连接成功则进入if{}
      {
        qDebug()<<"连接成功";
      }
    else
      {

        qDebug()<<"连接失败";
      }

}


/***************TCP接收槽函数*********************/
void MainWindow::readMessage()
{

    uchar datatype;
    QByteArray message;
    float Speed[2];

    // 将接收到的数据存放到变量中
    message=tcpClient->readAll();
    qint64 tcp_buffsize=(qint64)message.size();
    // 显示接收到的数据*/
#if printTcp
        qDebug() << message.toHex()<<endl;
#endif

    /*解帧头部，并进行CRC 校验和长度校验*/
    datatype=P->Receive_data(message,tcp_buffsize);

    switch (datatype) {
    case data_err:
           Speed[0]=0.000;
           Speed[1]=0.000;
        break;
    case data_speed://速度帧
        P->Receive_Data_Speed(Speed,message);


#if printTcp
        qDebug() <<"电机1"<<Speed[0]<<endl;
        qDebug() <<"电机2"<<Speed[1]<<endl;
#endif

        break;
    case data_Line_speed:
        break;
    case data_Angular_speed:
        break;
    default:
        break;
    }

Receive_Speed.Motor1_Speed=Speed[0];
Receive_Speed.Motor2_speed=Speed[1];
///*******接收消息后更新Qchart************/
//line->append(i,Speed);
//chart->addSeries(line);



}




/****************切换方向按钮1*********************/
void MainWindow::QPushButton_clicked()
{
    QByteArray senf_buffer;//缓存需要发送的数据

    senf_buffer=P->Control_direction(senf_buffer,target,Motor_dir1);
#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);


}


/**************切换方向按钮2************************/
void MainWindow::QPushButton1_clicked()
{

     QByteArray senf_buffer;//缓存需要发送的数据

     senf_buffer=P->Control_direction(senf_buffer,target,Motor_dir2);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);
}

/*PWM滚动调改变槽函数*/
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QByteArray senf_buffer;//缓存需要发送的数据

    senf_buffer=P->Control_PWM(senf_buffer,target,value);
#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);
}

/*PID发送槽*/
void MainWindow::QPushButton3_clicked(){

QString strP, strI, strD;
QByteArray senf_buffer;//缓存需要发送的数据

float f_P,f_I,f_D;
PID PID_send;

strP=ui->lineEdit->text();
strI=ui->lineEdit_2->text();
strD=ui->lineEdit_3->text();

f_P=strP.toFloat();
f_I=strI.toFloat();
f_D=strD.toFloat();


PID_send.P=f_P;
PID_send.I=f_I;
PID_send.D=f_D;

senf_buffer=P->Control_set_PID(senf_buffer,target,PID_send);
#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);

}

/*slider 与 spinbox槽*/
void MainWindow::slotDoubleSpinbox_slider()
{

    ui->horizontalSlider_2->setValue((int)(ui->doubleSpinBox->value()*100));
}

void MainWindow::slotslider_DoubleSpinBox()
{
    float Motor_speed[2];
    QByteArray senf_buffer;//缓存需要发送的数据
    ui->doubleSpinBox->setValue((double)(ui->horizontalSlider_2->value())/100);
    Motor_speed[0]=(float)(ui->doubleSpinBox->value());
    Motor_speed[1]=0x00;
#if printTcp
    qDebug()<<"speed"<<Motor_speed[0]<<endl;
#endif

    senf_buffer=P->Control_Speed(senf_buffer,target,Motor_speed);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);
}



/*停止刷新Qchart*/
void MainWindow::QPushButton4_clicked(){


m_timer.stop();
}

/*继续刷新Qchart*/
void MainWindow::QPushButton14_clicked(){

m_timer.start();

}

//清除qchart1 数据
void MainWindow::QPushButton15_clicked(){
m_series.clear();

}

//checkbox1
void MainWindow::slotcheckbox_changed(int state){
    if (state == Qt::Checked) // "选中"
    {
        //电机1checkbox选中
        m_series.setVisible(true);

    }
    else if(state == Qt::PartiallyChecked) // "半选"
    {


    }
    else // 未选中 - Qt::Unchecked
    {
        //电机1checkbox未选中
        m_series.setVisible(false);
    }


}
//清除checkbox2
void MainWindow::slotcheckbox2_changed(int state){
    if (state == Qt::Checked) // "选中"
    {
        //电机2checkbox选中
        m_series2.setVisible(true);

    }
    else if(state == Qt::PartiallyChecked) // "半选"
    {


    }
    else // 未选中 - Qt::Unchecked
    {
        //电机2checkbox未选中
        m_series2.setVisible(false);
    }


}





/*连接手柄信号槽*/
void MainWindow::QgamepadConnect(){



          m_gamepad = new QGamepad(0, this);

          connect(m_gamepad, &QGamepad::buttonR1Changed, this, [=](double value){
              GamePAD_getBtnVaule(1, value);
          });
          connect(m_gamepad, &QGamepad::buttonR2Changed, this, [=](double value){
              GamePAD_getBtnVaule(2, value);
          });

          connect(m_gamepad, &QGamepad::buttonL1Changed, this, [=](double value){
              GamePAD_getBtnVaule(3, value);
          });
          connect(m_gamepad,&QGamepad::buttonL2Changed, this, [=](double value){
              GamePAD_getBtnVaule(4, value);
          });
          connect(m_gamepad, &QGamepad::buttonUpChanged, this, [=](double value){
              GamePAD_getBtnVaule(5, value);
          });
          connect(m_gamepad, &QGamepad::buttonDownChanged, this, [=](double value){
              GamePAD_getBtnVaule(6, value);
          });
          connect(m_gamepad, &QGamepad::buttonRightChanged, this, [=](double value){
              GamePAD_getBtnVaule(7, value);
          });
          connect(m_gamepad, &QGamepad::buttonLeftChanged, this, [=](double value){
              GamePAD_getBtnVaule(8, value);
          });

          connect(m_gamepad, &QGamepad::axisRightXChanged, this, [=](double value){
                  GamePAD_getBtnVaule(9, value);
          });
          connect(m_gamepad, &QGamepad::axisRightYChanged, this, [=](double value){
              GamePAD_getBtnVaule(10, value);
          });
          connect(m_gamepad, &QGamepad::axisLeftXChanged, this, [=](double value){
              GamePAD_getBtnVaule(11, value);
          });
          connect(m_gamepad, &QGamepad::axisLeftYChanged, this, [=](double value){
              GamePAD_getBtnVaule(12, value);
          });

}

//手柄控制时电机速度
void MainWindow::on_horizontalSlider3_valueChanged(int value)
{
qgameSpeed=value;


}

/*获取手柄数值*/
void MainWindow::GamePAD_getBtnVaule(int btnName, double value){

    float Motor_speed[2];

    QByteArray senf_buffer;//缓存需要发送的数据

    QByteArray senf_bufferdir;//缓存需要发送的数据


    int res; //状态


    switch (btnName) {


    case 1://R1

     if (value==1){//按下
    qDebug()<<"R1"<<endl;
    senf_bufferdir=P->Control_direction(senf_buffer,Motor1,Motor_dir1);
    tcpClient->write(senf_bufferdir);}
     else{

     }

    break;

    case 2://R2
    if (value==1){//按下
    qDebug()<<"R2"<<endl;
    senf_bufferdir=P->Control_direction(senf_buffer,Motor1,Motor_dir2);
    tcpClient->write(senf_bufferdir);
    }
    else{

    }
    break;

    case 3://L1
    if (value==1){//按下
    qDebug()<<"L1"<<endl;
    senf_bufferdir=P->Control_direction(senf_buffer,Motor2,Motor_dir1);
    tcpClient->write(senf_bufferdir);

    }
    else{

    }
    break;

    case 4://L2
    if (value==1){//按下
    qDebug()<<"L2"<<endl;
    senf_bufferdir=P->Control_direction(senf_buffer,Motor2,Motor_dir2);
    tcpClient->write(senf_bufferdir);

    }
    else{


    }
    break;


    case 5: //上
    if (value==1){//按下

        qgameSpeed=qgameSpeed+5;
        if(qgameSpeed>=75) qgameSpeed=75;
        qDebug()<<qgameSpeed<<endl;
//        Motor_speed[0]=qgameSpeed;
//        Motor_speed[1]=qgameSpeed;
//        senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
//        tcpClient->write(senf_buffer);
    }
    else{


    }
    break;


    case 6: //下
    if (value==1){//按下

        qgameSpeed=qgameSpeed-5;
        if(qgameSpeed<=0) qgameSpeed=0;
        qDebug()<<qgameSpeed<<endl;
//        Motor_speed[0]=qgameSpeed;
//        Motor_speed[1]=qgameSpeed;
//        senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
//        tcpClient->write(senf_buffer);
    }
    else{


    }
    break;

    case 7: //左 停止
    if (value==1){//按下
        qDebug()<<"停止"<<endl;
        qgameSpeed=0;
        Motor_speed[0]=0;
        Motor_speed[1]=0;
        senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
        tcpClient->write(senf_buffer);
        gamepad_resL_last=0;
        gamepad_resR_last=0;
    }
    else{


    }
    break;

    case 8: //右 停止

    if (value==1){//按下
        qDebug()<<"停止"<<endl;
        qgameSpeed=0;
        Motor_speed[0]=0;
        Motor_speed[1]=0;
        senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
        tcpClient->write(senf_buffer);
        gamepad_resL_last=0;
        gamepad_resR_last=0;
    }
    else{


    }
    break;


    //右摇杆X
    case 9:
        if(value < 0)
        {

            right_X=value;
//             qDebug()<<"right_Xvalue:"<<value<<endl;

        }
        else if(value > 0)
        {

              right_X=value;
//              qDebug()<<"right_Xvalue:"<<value<<endl;
        }
        else
        {

            right_X=value;
//            qDebug()<<"right_Xvalue:"<<value<<endl;
        }
        break;


        //右摇杆Y
    case 10:
//        qDebug()<<"Y:"<<value<<endl;
//        qDebug()<<"X:"<<right_X<<endl;
        if(value < 0) //上半轴
        {

            if(value<=-0.8)
            {
             qDebug()<<"前直行:"<<qgameSpeed<<endl;
             res=1;
             emit GamePAD_Control_base(res);

            }
            else if(value>=-0.2)
            {



                if (right_X<=-0.8)
                {
                  qDebug()<<"左转向:"<<qgameSpeed<<endl;
                  res=2;
                  emit GamePAD_Control_base(res);


                }
                else if(right_X>=0.8)
                {

                    qDebug()<<"右转向"<<qgameSpeed<<endl;
                    res=3;
                    emit GamePAD_Control_base(res);



                }


            }


        }



        else if(value > 0)  //下半轴
        {

           //qDebug()<<"右摇杆Y>0,the value is:"<<value<<endl;
           if(value>=0.8)
           {


               qDebug()<<"后直行:"<<qgameSpeed<<endl;
               res=4;
               emit GamePAD_Control_base(res);


           }
           else if(value<=0.2)
           {



                if (right_X<=-0.8)
                {
                  qDebug()<<"左转向:"<<qgameSpeed<<endl;
                  res=2;
                  emit GamePAD_Control_base(res);


                }
                else if(right_X>=0.8)
                {

                    qDebug()<<"右转向"<<qgameSpeed<<endl;
                    res=3;
                    emit GamePAD_Control_base(res);

                }



           }





        }


        else //按键释放状态 发送静止速度
        {
            QEventLoop eventloop;

            QTimer::singleShot(50, &eventloop, SLOT(quit()));
            eventloop.exec();
            qDebug()<<"停止"<<qgameSpeed<<endl;
            Motor_speed[0]=0;
            Motor_speed[1]=0;
            senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
            tcpClient->write(senf_buffer);
            gamepad_resL_last=0;
            gamepad_resR_last=0;

        }
        break;

       //左摇杆X
       case 11:
        if(value < 0)
        {

            left_X=value;
//             qDebug()<<"right_Xvalue:"<<value<<endl;

        }
        else if(value > 0)
        {

            left_X=value;
//              qDebug()<<"right_Xvalue:"<<value<<endl;
        }
        else
        {

            left_X=value;
//            qDebug()<<"right_Xvalue:"<<value<<endl;
        }
        break;
        //右摇杆Y


       break;

       //左摇杆Y
       case 12:
        if(value < 0) //上半轴
        {

            if(value<=-0.8)
            {
              qDebug()<<"前直行:"<<qgameSpeed<<endl;
             res=1;
             emit GamePAD_Control(res);


            }
            else if(value>=-0.2)
            {



                if (left_X<=-0.8)
                {
                  qDebug()<<"左转向:"<<qgameSpeed<<endl;
                  res=2;
                  emit GamePAD_Control(res);


                }
                else if(left_X>=0.8)
                {

                    qDebug()<<"右转向"<<qgameSpeed<<endl;
                    res=3;
                    emit GamePAD_Control(res);



                }


            }


        }



        else if(value > 0)  //下半轴
        {

           //qDebug()<<"右摇杆Y>0,the value is:"<<value<<endl;
           if(value>=0.8)
           {

               qDebug()<<"后直行:"<<qgameSpeed<<endl;
               res=4;
               emit GamePAD_Control(res);



           }
           else if(value<=0.2)
           {



                if (left_X<=-0.8)
                {
                  qDebug()<<"左转向:"<<qgameSpeed<<endl;
                  res=2;
                  emit GamePAD_Control(res);


                }
                else if(left_X>=0.8)
                {

                    qDebug()<<"右转向"<<qgameSpeed<<endl;
                    res=3;
                    emit GamePAD_Control(res);

                }



           }





        }


        else //按键释放状态 发送静止速度
        {
            QEventLoop eventloop;

            QTimer::singleShot(50, &eventloop, SLOT(quit()));
            eventloop.exec();
            qDebug()<<"停止"<<qgameSpeed<<endl;
            Motor_speed[0]=0;
            Motor_speed[1]=0;
            senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
            tcpClient->write(senf_buffer);
            gamepad_resL_last=0;
            gamepad_resR_last=0;

        }


       break;

    default:
        break;
    }


}

//gamepad 摇杆左
void MainWindow::GamePadVal_changed(int res){

    float Motor_speed[2];
    uchar Motor_dir[2];
    QByteArray senf_buffer;//缓存需要发送的数据

    QByteArray senf_bufferdir;//缓存需要发送的数据

    switch (res) {
    case 1:
        if(gamepad_resL_last!=res)
           {
            gamepad_resL_last=res;
            qDebug()<<"发送小车方向网络指令:"<<gamepad_resL_last<<endl;
                         Motor_speed[0]=qgameSpeed;
                         Motor_speed[1]=qgameSpeed;

                         Motor_dir[0]=Motor_dir2;
                         Motor_dir[1]=Motor_dir2;
                         senf_buffer=P->Control_SpeedAndDir(senf_buffer,each,Motor_speed,Motor_dir);
                            #if printTcp
                                qDebug()<<senf_buffer.toHex()<<endl;
                            #endif
                         tcpClient->write(senf_buffer);
            }
        break;

    case 2:
        if(gamepad_resL_last!=res)
           {
            gamepad_resL_last=res;
            qDebug()<<"发送小车方向网络指令:"<<gamepad_resL_last<<endl;
                              Motor_speed[0]=qgameSpeed;
                              Motor_speed[1]=0;
                              senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
                                #if printTcp
                                    qDebug()<<senf_buffer.toHex()<<endl;
                                #endif
                              tcpClient->write(senf_buffer);
            }
        break;

    case 3:
        if(gamepad_resL_last!=res)
           {
            gamepad_resL_last=res;
            qDebug()<<"发送小车方向网络指令:"<<gamepad_resL_last<<endl;
                                Motor_speed[0]=0;
                                Motor_speed[1]=qgameSpeed;
                                senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
                                #if printTcp
                                    qDebug()<<senf_buffer.toHex()<<endl;
                                #endif
                                tcpClient->write(senf_buffer);
            }
        break;

    case 4:
        if(gamepad_resL_last!=res)
           {
            gamepad_resL_last=res;
            qDebug()<<"发送小车方向网络指令:"<<gamepad_resL_last<<endl;
                        Motor_speed[0]=qgameSpeed;
                        Motor_speed[1]=qgameSpeed;

                        Motor_dir[0]=Motor_dir1;
                        Motor_dir[1]=Motor_dir1;
                        senf_buffer=P->Control_SpeedAndDir(senf_buffer,each,Motor_speed,Motor_dir);
                        #if printTcp
                            qDebug()<<senf_buffer.toHex()<<endl;
                        #endif
                        tcpClient->write(senf_buffer);
            }
        break;
    default:
    break;
    }


}

//摇杆右
void MainWindow::GamePadVal_base_changed(int res){
    float Motor_speed[2];
    uchar Motor_dir[2];
    QByteArray senf_buffer;//缓存需要发送的数据

    switch (res) {
    case 1:
        if(gamepad_resR_last!=res)
           {
            gamepad_resR_last=res;
            qDebug()<<"发送小车原地网络指令:"<<gamepad_resR_last<<endl;
                         Motor_speed[0]=qgameSpeed;
                         Motor_speed[1]=qgameSpeed;

                         senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
                            #if printTcp
                                qDebug()<<senf_buffer.toHex()<<endl;
                            #endif
                         tcpClient->write(senf_buffer);
            }
        break;

    case 2:
        if(gamepad_resR_last!=res)
           {
            gamepad_resR_last=res;
            qDebug()<<"发送小车原地网络指令:"<<gamepad_resR_last<<endl;
                              Motor_speed[0]=qgameSpeed;
                              Motor_speed[1]=qgameSpeed;
                              Motor_dir[0]=Motor_dir2;
                              Motor_dir[1]=Motor_dir1;

                              senf_buffer=P->Control_SpeedAndDir(senf_buffer,each,Motor_speed,Motor_dir);
                                #if printTcp
                                    qDebug()<<senf_buffer.toHex()<<endl;
                                #endif
                              tcpClient->write(senf_buffer);

            }
        break;

    case 3:
        if(gamepad_resR_last!=res)
           {
            gamepad_resR_last=res;
            qDebug()<<"发送小车原地网络指令:"<<gamepad_resR_last<<endl;
                                Motor_speed[0]=qgameSpeed;
                                Motor_speed[1]=qgameSpeed;
                                Motor_dir[0]=Motor_dir1;
                                Motor_dir[1]=Motor_dir2;

                                senf_buffer=P->Control_SpeedAndDir(senf_buffer,each,Motor_speed,Motor_dir);
                                #if printTcp
                                    qDebug()<<senf_buffer.toHex()<<endl;
                                #endif
                                tcpClient->write(senf_buffer);

            }
        break;

    case 4:
        if(gamepad_resR_last!=res)
           {
            gamepad_resR_last=res;
            qDebug()<<"发送小车原地网络指令:"<<gamepad_resR_last<<endl;
                        Motor_speed[0]=qgameSpeed;
                        Motor_speed[1]=qgameSpeed;
                        senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);
                        #if printTcp
                            qDebug()<<senf_buffer.toHex()<<endl;
                        #endif
                        tcpClient->write(senf_buffer);
            }
        break;
    default:
    break;
    }


}





/*左方向*/
void MainWindow::QPushButton6_clicked()
{

    float Motor_speed[2];
    QByteArray senf_buffer;//缓存需要发送的数据
    Motor_speed[0]=75.0;
    Motor_speed[1]=0.0;
#if printTcp
    qDebug()<<"M1_speed"<<Motor_speed[0]<<endl;
    qDebug()<<"M2_speed"<<Motor_speed[1]<<endl;
#endif

    senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);

}

/*右方向*/
void MainWindow::QPushButton9_clicked()
{

    float Motor_speed[2];
    QByteArray senf_buffer;//缓存需要发送的数据
    Motor_speed[0]=0.0;
    Motor_speed[1]=75.0;
#if printTcp
    qDebug()<<"M1_speed"<<Motor_speed[0]<<endl;
    qDebug()<<"M2_speed"<<Motor_speed[1]<<endl;
#endif

    senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_buffer);

}

/*上方向*/
void MainWindow::QPushButton7_clicked()
{

    float Motor_speed[2];
    QByteArray senf_buffer;//缓存需要发送的数据

    QByteArray senf_bufferdir;//缓存需要发送的数据

    senf_bufferdir=P->Control_direction(senf_buffer,both,Motor_dir2);
#if printTcp
    qDebug()<<senf_bufferdir.toHex()<<endl;
#endif
    //TCP发送




    Motor_speed[0]=75.0;
    Motor_speed[1]=75.0;
#if printTcp
    qDebug()<<"M1_speed"<<Motor_speed[0]<<endl;
    qDebug()<<"M2_speed"<<Motor_speed[1]<<endl;
#endif

    senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_bufferdir);
    QEventLoop eventloop;

    QTimer::singleShot(50, &eventloop, SLOT(quit()));
    eventloop.exec();

    tcpClient->write(senf_buffer);

}

/*下方向*/
void MainWindow::QPushButton8_clicked()
{

    float Motor_speed[2];
    QByteArray senf_buffer;//缓存需要发送的数据

    QByteArray senf_bufferdir;//缓存需要发送的数据

    senf_bufferdir=P->Control_direction(senf_buffer,both,Motor_dir1);
#if printTcp
    qDebug()<<senf_bufferdir.toHex()<<endl;
#endif
    //TCP发送


    Motor_speed[0]=75.0;
    Motor_speed[1]=75.0;
#if printTcp
    qDebug()<<"M1_speed"<<Motor_speed[0]<<endl;
    qDebug()<<"M2_speed"<<Motor_speed[1]<<endl;
#endif

    senf_buffer=P->Control_Speed(senf_buffer,each,Motor_speed);

#if printTcp
    qDebug()<<senf_buffer.toHex()<<endl;
#endif
    //TCP发送
    tcpClient->write(senf_bufferdir);
    QEventLoop eventloop;

    QTimer::singleShot(50, &eventloop, SLOT(quit()));
    eventloop.exec();
    tcpClient->write(senf_buffer);

}


void MainWindow::Target_Changed(){

if(ui->comboBox->currentIndex()==0) target=Motor1;
else if(ui->comboBox->currentIndex()==1) target=Motor2;
else if(ui->comboBox->currentIndex()==2) target=both;
qDebug()<<hex<<target<<endl;

}





