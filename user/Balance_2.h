/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_BALANCE_H_
#define _LQ_BALANCE_H_



typedef enum {
    idle = 0,       //0:����-ԭ��ֱ��
    garage_out,     //1:����
    run,            //2:����ѭ��
    garage_in       //3:���
}Status_car;

typedef enum {
    nothing=0,      //0:ƽ��·��
    straight,       //1:ֱ��
    round_L,        //2:�󻷵�
    round_L_Out,      //3:������
    round_R,        //4:�һ���
    cross,          //5:ʮ��
    zebra           //6:������
}Element;
extern Element element;                         //����Ԫ��״̬
extern Status_car status_car;                   //С����״̬������ ѭ�� ���

extern float error_Camera, error_Blance;       //����ͷ�͵�����

void Balance_car(void);
void Key(void);
extern volatile char mutexCpu0TFTIsOk;         // CPU1 0ռ��/1�ͷ� TFT
extern int Target_Speed;
void Balance(void);
float yijielvbo_P(float angle_m,float gyro_m);
/**************************************************************************
Y��ƽ��PID����,�ǶȻ�
**************************************************************************/
float X_balance_Control(float Angle,float Angle_Zero,float Gyro);
/**************************************************************************
�ٶ�PI����,�ٶ���������
**************************************************************************/
float Velocity_Control_A(int encoder);
#endif /* SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_ */


