#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QMainWindow>
#include <QTcpSocket>
#include <QtCharts/QChartGlobal>
#include "protocol.h"
#include <QGamepad>
#include <QChart>   //类似于画笔
#include <QChartView>   //类似于画布
#include <QSplineSeries>  // 类 Series 是用来添加数据的对象（可以理解为一个集合）
#include <QValueAxis>//用来自定义图表中的X，Y坐标轴
#include <QTimer>
#include <QPen>


#include <chartview.h>

using namespace QtCharts;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    typedef struct
    {
    float Motor1_Speed;
    float Motor2_speed;

    }Speed_Data;



    uchar target=Motor1;


    double right_X=0;
    double left_X=0;
    float qgameSpeed=0.0;

signals:
    GamePAD_Control(int);
    GamePAD_Control_base(int);
public slots:
    void GamePadVal_changed(int res);
    void GamePadVal_base_changed(int res);
    //方向按钮1
    void QPushButton_clicked();
    //方向按钮2
    void QPushButton1_clicked();
    //Qchart暂停
    void QPushButton4_clicked();
    //方向左
    void QPushButton6_clicked();
    //方向上
    void QPushButton7_clicked();
    //方向下
    void QPushButton8_clicked();
    //方向右
    void QPushButton9_clicked();
    //Qchart接收
    void QPushButton14_clicked();
    //Qchart清空
    void QPushButton15_clicked();
    //checkbox1 改变槽
    void slotcheckbox_changed(int state);
    //checkbox2 改变槽
    void slotcheckbox2_changed(int state);
    //PID发送槽
    void QPushButton3_clicked();
    //PWMSlider控件数值变动槽
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider3_valueChanged(int);
    //double_spin和 slider 槽
    void slotDoubleSpinbox_slider();
    void slotslider_DoubleSpinBox();
    void readMessage();//tcp读取数据
    void RealtimeDataSlot();//qchart刷新
    void Target_Changed();//下拉框改变目标
    void GamePAD_getBtnVaule(int btnName, double value);//获取手柄数值

private: //私有成员

    void Qchar_init();
    void tcpClient_init();
    void QgamepadConnect();

    Ui::MainWindow *ui;
    QTcpSocket *tcpClient; //Tcp客户端对象
    protocol *P = new protocol;

    QGamepad *m_gamepad;
    QTimer m_timer;
    QChart chart;
    QSplineSeries m_series;
    QSplineSeries m_series2;
    QStringList m_titles;
    QValueAxis m_axis;
    QValueAxis m_axisy;
    qreal m_step;
    qreal m_x;
    qreal m_y1;
    qreal m_y2;

    Speed_Data Receive_Speed;
    int gamepad_resL_last=0;
    int gamepad_resR_last=0;
};




#endif // MAINWINDOW_H
