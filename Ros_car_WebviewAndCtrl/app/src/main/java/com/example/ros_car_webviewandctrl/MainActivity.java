package com.example.ros_car_webviewandctrl;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.support.v4.app.INotificationSideChannel;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.WindowManager;
import android.widget.SeekBar;
import android.widget.Toast;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.OutputStream;
import java.net.Socket;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Timer;
import java.util.TimerTask;


import static java.util.stream.Stream.concat;

public class MainActivity extends AppCompatActivity {

    ControlView mControlView;

    float float_data = 0.0F;

    private SeekBar ID_SeekBar_0;
    private WebView webview;
    private static int rocking_rate_last;//摇杆状态历史变量
    Socket socket=null;
//    private String ipAddress="192.168.43.157";
private String ipAddress="192.168.43.152";
    private  int port=8090;
    public  static Handler mhandler;
    public static int rocking_state; //传递给tcp的摇杆状态变量(经过是否重复检验)

    String data_test="test_data";
    private OutputStream outputStream=null;
    private  Send_Thread send_Thread=null;

    private static final String TAG = "MainActivity";

    //协议相关


    byte [] buffer_send_with_crc16= new byte[17];


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);



        webview = (WebView) findViewById(R.id.webview);
        load();
        ID_SeekBar_0 = findViewById(R.id.ID_SeekBar_0);//寻找xml中真正的id

        //摇杆 (单独开启一个线程)
        mControlView = (ControlView)findViewById(R.id.control_view);
        mControlView = new ControlView(MainActivity.this);

        //TCP连接线程
        Connect_Thread connect_Thread = new Connect_Thread();
        connect_Thread.start();



        //获取摇杆线程中传来的数据
        mhandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
//  这边有个BUG,摇杆控件一开始摇杆时候会出现先是出现错误的方向，然后才能到正确的方向。可能和摇杆的底层逻辑有关
                if(msg.arg1!=rocking_rate_last) {
                    switch (String.valueOf(msg.arg1)) {
                        case "0":
//                            Toast.makeText(MainActivity.this, "原点", Toast.LENGTH_SHORT).show();
//                        Log.d(TAG,"原点");
                            rocking_state=0;
                            //开启发送线程
                            send_Thread= new Send_Thread(rocking_state);
                            send_Thread.start();
                            break;
                        case "1":
//                            Toast.makeText(MainActivity.this, "前", Toast.LENGTH_SHORT).show();
//                        Log.d(TAG,"前");
                            rocking_state=1;
                            send_Thread = new Send_Thread(rocking_state);
                            send_Thread.start();
                            break;
                        case "2":
//                            Toast.makeText(MainActivity.this, "后", Toast.LENGTH_SHORT).show();
//                        Log.d(TAG,"后");
                            rocking_state=2;
                            send_Thread = new Send_Thread(rocking_state);
                            send_Thread.start();
                            break;
                        case "3":
//                            Toast.makeText(MainActivity.this, "左", Toast.LENGTH_SHORT).show();
//                        Log.d(TAG,"左");
                            rocking_state=3;
                            send_Thread = new Send_Thread(rocking_state);
                            send_Thread.start();
                            break;
                        case "4":
//                            Toast.makeText(MainActivity.this, "右", Toast.LENGTH_SHORT).show();
//                        Log.d(TAG,"右");
                            rocking_state=4;
                            send_Thread = new Send_Thread(rocking_state);
                            send_Thread.start();
                            break;
                    }
                }
                rocking_rate_last=msg.arg1;

            }
        };




        /***********SeekBar数值改变方法*********/

        ID_SeekBar_0.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener()

                                                {



                                                    public void onProgressChanged (SeekBar seekBar, final int progress,
                                                                                   boolean fromUser){
                                                        // TODO Auto-generated method stub
                                                        if (ID_SeekBar_0.getId() == R.id.ID_SeekBar_0) {
                                                            Log.d(TAG,"float_data="+progress);

                                                            float_data=(float) progress;
                                                        } else {


                                                        }
                                                    }


                                                    public void onStartTrackingTouch (SeekBar seekBar){
                                                        // TODO Auto-generated method stub

                                                    }

                                                    @Override
                                                    public void onStopTrackingTouch(SeekBar seekBar) {

                                                    }

                                                }
        );

    }//Oncreat构造;



    //析构

    @Override
    protected void onDestroy() {
        super.onDestroy();


        //处理 黑屏重新点亮后surfaceview重新启动了线程
    }


    /*
     *********************tcp客户端************************
     */
    //连接线程
    class Connect_Thread extends Thread//继承Thread
    {
        public void run()//重写run方法
        {
            try
            {
                if (socket == null)
                {
                    //用InetAddress方法获取ip地址
                    Log.d(TAG,"连接TCP ");
                    socket = new Socket(ipAddress, port);//创建连接地址和端口
                    //在创建完连接后启动接收线程
                    //Receive_Thread receive_Thread = new Receive_Thread();
                    //receive_Thread.start();
                }
            }
            catch (Exception e)
            {
                e.printStackTrace();
            }
        }
    }


    class Send_Thread extends Thread
    {

        TCP_car_app car_tcp =new TCP_car_app();
        String msg;

        String Strchange;
        String rocking_bar_data;

        public Send_Thread(int msg){
            this.msg=String.valueOf(msg);
        }
        public void run()
        {
            try {
                //组协议帧

                //由于没有再嵌入式中做帧粘连冗余处理，此处放置协议帧粘连
                try{
                    Thread.sleep(20);
                }catch(InterruptedException e){
                    e.printStackTrace();
                }


                outputStream = socket.getOutputStream();



                switch (msg) {
                    case "1":
                        Log.d(TAG,"发送前进协议帧");
                        rocking_bar_data="forward";
                        buffer_send_with_crc16=car_tcp.Framing(rocking_bar_data,float_data);
//                        printHexString(Strchange,buffer_send_with_crc16);

                        outputStream.write(buffer_send_with_crc16);
                        break;
                    case "2":
                        Log.d(TAG,"发送后退协议帧");
                        rocking_bar_data="back";
                        buffer_send_with_crc16=car_tcp.Framing(rocking_bar_data,float_data);
//                        printHexString(Strchange,buffer_send_with_crc16);
                        outputStream.write(buffer_send_with_crc16);
                        break;
                    case "3":
                        Log.d(TAG,"发送左协议帧");
                        rocking_bar_data="left";
                        buffer_send_with_crc16=car_tcp.Framing(rocking_bar_data,float_data);
//                        printHexString(Strchange,buffer_send_with_crc16);
                        outputStream.write(buffer_send_with_crc16);
                        break;
                    case "4":
                        Log.d(TAG,"发送右协议帧");
                        rocking_bar_data="right";
                        buffer_send_with_crc16=car_tcp.Framing(rocking_bar_data,float_data);
//                        printHexString(Strchange,buffer_send_with_crc16);
                        outputStream.write(buffer_send_with_crc16);
                        break;
                    case "0":
                        Log.d(TAG,"发送原点议帧");
                        rocking_bar_data="origin";
                        buffer_send_with_crc16=car_tcp.Framing(rocking_bar_data,float_data);
//                        printHexString(Strchange,buffer_send_with_crc16);
                        outputStream.write(buffer_send_with_crc16);
                        break;



                }

                //发送一指令
//                outputStream.write(msg.getBytes());

            } catch (Exception e) {

                e.printStackTrace();
            }
        }

        /**********方法*********************/
        //byte 拼接
        //System.arraycopy()方法
        public byte[] byteMerger(byte[] bt1, byte[] bt2){
            byte[] bt3 = new byte[bt1.length+bt2.length];
            System.arraycopy(bt1, 0, bt3, 0, bt1.length);
            System.arraycopy(bt2, 0, bt3, bt1.length, bt2.length);
            return bt3;
        }


        //数值反传
        private byte[] dataValueRollback(byte[] data) {
            ArrayList<Byte> al = new ArrayList<Byte>();
            for (int i = data.length - 1; i >= 0; i--) {
                al.add(data[i]);
            }

            byte[] buffer = new byte[al.size()];
            for (int i = 0; i <= buffer.length - 1; i++) {
                buffer[i] = al.get(i);
            }
            return buffer;
        }

        /**
         * 将指定byte数组以16进制的形式打印到控制台
         *
         * @param hint
         *            String
         * @param b
         *            byte[]
         * @return void
         */
        public  void printHexString(String hint, byte[] b)
        {
            System.out.print(hint);
            for (int i = 0; i < b.length; i++)
            {
                String hex = Integer.toHexString(b[i] & 0xFF);
                if (hex.length() == 1)
                {
                    hex = '0' + hex;
                }
                System.out.print(hex.toUpperCase() + " ");
            }
            System.out.println("");
        }



        /***float转byte***/
        public  byte[] getByteArrayInt(int i) {
            byte[] b = new byte[4];
            b[0] = (byte) ((i & 0xff000000) >> 24);
            b[1] = (byte) ((i & 0x00ff0000) >> 16);
            b[2] = (byte) ((i & 0x0000ff00) >> 8);
            b[3] = (byte)  (i & 0x000000ff);
            return b;
        }
        public  byte[] getByteArray(float f) {
            int Intbits = Float.floatToIntBits(f);//将float里面的二进制串解释为int整数
            return getByteArrayInt(Intbits);
        }


    }





    /*webview 加载网页页面*/
    private void load() {
        WebSettings webviewSettings = webview.getSettings();
        webviewSettings.setJavaScriptEnabled(true);
        webviewSettings.setJavaScriptEnabled(true);
        webviewSettings.setJavaScriptCanOpenWindowsAutomatically(true);
        webviewSettings.setAllowFileAccess(true);// 设置允许访问文件数据
        webviewSettings.setSupportZoom(true);
        webviewSettings.setBuiltInZoomControls(true);
        webviewSettings.setJavaScriptCanOpenWindowsAutomatically(true);
        webviewSettings.setCacheMode(WebSettings.LOAD_CACHE_ELSE_NETWORK);
        webviewSettings.setDomStorageEnabled(true);
        webviewSettings.setDatabaseEnabled(true);

        //设置载入页面自适应手机屏幕，居中显示

        webviewSettings.setUseWideViewPort(true);
        webviewSettings.setLoadWithOverviewMode(true);
//        webview.setWebViewClient(new WebViewClient());

        webview.setWebChromeClient(new WebChromeClient());
        webview.loadUrl("http://192.168.43.111:8081/");
//        webview.loadUrl("https://www.baidu.com/");
    }




}