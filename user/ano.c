#include "ano.h"


/**
  *  ���ܣ��������ݸ�������λ����V2.6��
  *  ��ڲ�����fun,�����֣�0xA0~0xAF
  *            data�����ݻ����������28���ֽ�
  *            len��data���ݳ���
  *  ����ֵ����
  *  ע�����ݸ�ʽ��0x88+FUN+LEN+DATA+SUM
  */
void ToNimin(uint8 fun,uint8 *data,uint8 len)
{
    uint8 BUFF[30]={0};
    int i;
    uint8 sumcheck = 0;
    uint8 addcheck = 0;
    uint8 _cnt=0;
    BUFF[_cnt++]=0xAA;//֡ͷ
    BUFF[_cnt++]=0xFF;//Ŀ���ַ
    BUFF[_cnt++]=fun;//������

    BUFF[_cnt++]=len;//���ݳ���
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




/**  �Զ���֡0xA1
  *  ���ܣ����ͼ��ٶȴ������������Ǵ��������ݸ�������λ����V2.6��
  *  ��ڲ�����aacx,aacy,aacz��xyz��������ļ��ٶ�ֵ
  *            gyrox,gyroy,gyroz��xyz���������������ֵ
  *  ����ֵ����
  *  ע�����ݸ�ʽ��0x88+FUN+LEN+DATA+SUM
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

/**  �ɿ���ʾ֡
  *  ���ܣ��ϱ���������̬���ݸ���λ��
  *  ��ڲ�����aacx,aacy,aacz��xyz��������ļ��ٶ�ֵ
  *            gyrox,gyroy,gyroz��xyz���������������ֵ
  *            yaw,ƫ���ǣ���λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
  *            roll,����ǣ���λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
  *            pitch,�����ǣ���λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
  *  ����ֵ����
  *  ע�����ݸ�ʽ��0x88+0xAF+0x1C+ ACC DATA + GYRO DATA + MAG DATA + ANGLE DATA(roll/pitch/yaw) +0x00+0x00+0x00+0x00+SUM
  *      ANGLE��roll��pitch����Ϊʵ��ֵ����100�Ժ�õ�������ֵ��yawΪ����10�Ժ�õ�������ֵ
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
