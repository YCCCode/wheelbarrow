#include "ano.h"


/**
  *  功能：发送数据给匿名上位机（V2.6）
  *  入口参数：fun,功能字，0xA0~0xAF
  *            data，数据缓存区，最多28个字节
  *            len，data数据长度
  *  返回值：无
  *  注：数据格式：0x88+FUN+LEN+DATA+SUM
  */
void ToNimin(uint8 fun,uint8 *data,uint8 len)
{
    uint8 BUFF[30]={0};
    int i;
    uint8 sumcheck = 0;
    uint8 addcheck = 0;
    uint8 _cnt=0;
    BUFF[_cnt++]=0xAA;//帧头
    BUFF[_cnt++]=0xFF;//目标地址
    BUFF[_cnt++]=fun;//功能码

    BUFF[_cnt++]=len;//数据长度
    for(i=0;i<len;i++)
        BUFF[_cnt++]=data[i];
    for(i=0;i<BUFF[3]+4;i++)
    {
        sumcheck+=BUFF[i];
        addcheck+=sumcheck;
    }
    BUFF[_cnt++]=sumcheck;
    BUFF[_cnt++]=addcheck;

//    for(i=0;i<_cnt;i++)
        uart_write_buffer(UART_2, BUFF, _cnt);
//        wireless_uart_send_byte(BUFF[i]);


}




/**  自定义帧0xA1
  *  功能：发送加速度传感器和陀螺仪传感器数据给匿名上位机（V2.6）
  *  入口参数：aacx,aacy,aacz：xyz三个方向的加速度值
  *            gyrox,gyroy,gyroz：xyz三个方向的陀螺仪值
  *  返回值：无
  *  注：数据格式：0x88+FUN+LEN+DATA+SUM
  */
void IMUToNimin(int16 aacx,int16 aacy,int16 aacz,int16 gyrox,int16 gyroy,int16 gyroz)
{
      uint8 buf[18]={0};
      buf[0]=(aacx>>8)&0xFF;
      buf[1]=aacx&0xFF;
      buf[2]=(aacy>>8)&0xFF;
      buf[3]=aacy&0xFF;
      buf[4]=(aacz>>8)&0xFF;
      buf[5]=aacz&0xFF;
      buf[6]=(gyrox>>8)&0xFF;
      buf[7]=gyrox&0xFF;
      buf[8]=(gyroy>>8)&0xFF;
      buf[9]=gyroy&0xFF;
      buf[10]=(gyroz>>8)&0xFF;
      buf[11]=gyroz&0xFF;
      ToNimin(0x02,buf,18);
//    buf[0]=(aacx>>8)&0xFF;
//    buf[1]=aacx&0xFF;
//    buf[2]=(aacy>>8)&0xFF;
//    buf[3]=aacy&0xFF;
//    buf[4]=(aacz>>8)&0xFF;
//    buf[5]=aacz&0xFF;
//
//    buf[6]=(gyrox>>8)&0xFF;
//    buf[7]=gyrox&0xFF;
//    buf[8]=(gyroy>>8)&0xFF;
//    buf[9]=gyroy&0xFF;
//    buf[10]=(gyroz>>8)&0xFF;
//    buf[11]=gyroz&0xFF;

}

/**  飞控显示帧
  *  功能：上报解算后的姿态数据给上位机
  *  入口参数：aacx,aacy,aacz：xyz三个方向的加速度值
  *            gyrox,gyroy,gyroz：xyz三个方向的陀螺仪值
  *            yaw,偏航角，单位为0.1度 0 -> 3600  对应 0 -> 360.0度
  *            roll,横滚角，单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
  *            pitch,俯仰角，单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
  *  返回值：无
  *  注：数据格式：0x88+0xAF+0x1C+ ACC DATA + GYRO DATA + MAG DATA + ANGLE DATA(roll/pitch/yaw) +0x00+0x00+0x00+0x00+SUM
  *      ANGLE的roll和pitch数据为实际值乘以100以后得到的整数值，yaw为乘以10以后得到的整数值
  */
void IMUAngleToNimin(short ROL,short PIT,short YAW)
{
    uint8 buf[16]={0};


    buf[0]=(ROL>>8)&0xFF;
    buf[1]=ROL&0xFF;
    buf[2]=(PIT>>8)&0xFF;
    buf[3]=PIT&0xFF;
    buf[4]=(YAW>>8)&0xFF;
    buf[5]=YAW&0xFF;
    ToNimin(0x01,buf,16);


}
