package com.example.ros_car_webviewandctrl;

public class TCP_car_app {
    Crc16_modbus CRC_CAL= new Crc16_modbus();




    /****组发送帧*****/
public byte[] Framing(String direction,float speed){


    byte [] Header_byteArray=new byte[3];
    byte [] float_to_byte=new byte[4];
    byte [] dir_to_byte=new byte[1];
    byte [] dir_to_byte2=new byte[2];
    byte [] zero_buffer1=new byte[7];
    byte [] zero_buffer2=new byte[2];
    byte [] zero_buffer3=new byte[12];
    byte [] Send_buffer_without_crc16 = new byte[15];

    byte [] CRC_result_byte =new byte[2];
    byte [] Send_buffer_with_crc16 = new byte[17];

        switch (direction)
        {
            case "forward":
                //写入头部
                Header_byteArray[0]=0x07;
                Header_byteArray[1]=0x0F;
                Header_byteArray[2]=0x00;
                //写入速度方向 并合并
                float_to_byte=getByteArray(speed);
                Send_buffer_without_crc16=byteMerger(Header_byteArray,float_to_byte);
                dir_to_byte[0]=0x02;

                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,dir_to_byte);
                //填充剩余长度 并合并
                for(int i=0;i<=6;i++) {
                    zero_buffer1[i] = 0x00;
                }

                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,zero_buffer1);
                //CRC校验计算值并合并
                CRC_result_byte=CRC_CAL.getCRC(Send_buffer_without_crc16);
                Send_buffer_with_crc16=byteMerger(Send_buffer_without_crc16,CRC_result_byte);

                break;

            case "back":
                //写入头部
                Header_byteArray[0]=0x07;
                Header_byteArray[1]=0x0F;
                Header_byteArray[2]=0x00;
                //写入速度方向 并合并
                float_to_byte=getByteArray(speed);
                Send_buffer_without_crc16=byteMerger(Header_byteArray,float_to_byte);
                dir_to_byte[0]=0x01;
                //填充剩余长度 并合并
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,dir_to_byte);

                for(int i=0;i<=6;i++) {
                    zero_buffer1[i] = 0x00;
                }

                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,zero_buffer1);
                //CRC校验计算值并合并
                CRC_result_byte=CRC_CAL.getCRC(Send_buffer_without_crc16);
                Send_buffer_with_crc16=byteMerger(Send_buffer_without_crc16,CRC_result_byte);

                break;

            case "left":
                //写入头部
                Header_byteArray[0]=0x07;
                Header_byteArray[1]=0x0F;
                Header_byteArray[2]=0x03;

                //写入速度方向，并合并
                float_to_byte=getByteArray(speed);
                Send_buffer_without_crc16=byteMerger(Header_byteArray,float_to_byte);
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,float_to_byte);
                dir_to_byte2[0]=0x02;
                dir_to_byte2[1]=0x01;

                //
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,dir_to_byte2);


                for(int i=0;i<=1;i++) {
                    zero_buffer2[i] = 0x00;
                }
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,zero_buffer2);



                //CRC校验计算值并合并
                CRC_result_byte=CRC_CAL.getCRC(Send_buffer_without_crc16);
                Send_buffer_with_crc16=byteMerger(Send_buffer_without_crc16,CRC_result_byte);

                break;

            case "right":
                //写入头部
                Header_byteArray[0]=0x07;
                Header_byteArray[1]=0x0F;
                Header_byteArray[2]=0x03;

                //写入速度方向，并合并
                float_to_byte=getByteArray(speed);
                Send_buffer_without_crc16=byteMerger(Header_byteArray,float_to_byte);
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,float_to_byte);
                dir_to_byte2[0]=0x01;
                dir_to_byte2[1]=0x02;

                //
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,dir_to_byte2);


                for(int i=0;i<=1;i++) {
                    zero_buffer2[i] = 0x00;
                }
                Send_buffer_without_crc16=byteMerger(Send_buffer_without_crc16,zero_buffer2);



                //CRC校验计算值并合并
                CRC_result_byte=CRC_CAL.getCRC(Send_buffer_without_crc16);
                Send_buffer_with_crc16=byteMerger(Send_buffer_without_crc16,CRC_result_byte);

                break;

            case "origin":
                //写入头部
                Header_byteArray[0]=0x03;
                Header_byteArray[1]=0x0F;
                Header_byteArray[2]=0x00;

                //填充剩余长度 并合并
                for(int i=0;i<=11;i++) {
                    zero_buffer3[i] = 0x00;
                }
                Send_buffer_without_crc16=byteMerger(Header_byteArray,zero_buffer3);
                //CRC校验计算值并合并

                CRC_result_byte=CRC_CAL.getCRC(Send_buffer_without_crc16);
                Send_buffer_with_crc16=byteMerger(Send_buffer_without_crc16,CRC_result_byte);

                break;

        }


return Send_buffer_with_crc16;
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

    /**********方法*********************/
    //byte 拼接
    //System.arraycopy()方法
    public byte[] byteMerger(byte[] bt1, byte[] bt2){
        byte[] bt3 = new byte[bt1.length+bt2.length];
        System.arraycopy(bt1, 0, bt3, 0, bt1.length);
        System.arraycopy(bt2, 0, bt3, bt1.length, bt2.length);
        return bt3;
    }



}
