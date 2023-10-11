/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC3xxxA���İ�
����    д��chiusir
��E-mail��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.3�汾
��Target �� TC3xxxA
��Crystal�� 20.000Mhz
��SYS PLL�� 300MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����RIFΪTC3xxxA�����⣬�����Ĵ������TC3xx
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "zf_common_headfile.h"
#include "balance_2.h"
#include "ano.h"
#include "attitude.h"

/*******************
�Եѿ�������ϵΪ�ο�
RollΪ��X����ת�ǣ�
PitchΪ��Y����ת�ǣ�
YawΪ��Z����ת�ǡ�
*******************/
#define LeftFlyWheel 0
#define RightFlyWheel 1


//����
#define FLYWHEEL_CLOCKWISE              (1                      )     // ������˳ʱ����ת����
#define FLYWHEEL_DUTY_LEVEL             (0                      )     // ������ռ�ձ���Ч��ƽ

#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // ������PWM��������
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // �����ַ����������
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // ������ɲ����������
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // �����ֱ��������
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // �����ֱ�����ͨ��1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // �����ֱ�����ͨ��2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // �Ҳ����PWM��������
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // �Ҳ���ַ����������
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // �Ҳ����ɲ����������
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // �Ҳ���ֱ��������
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // �Ҳ���ֱ�����ͨ��1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // �Ҳ���ֱ�����ͨ��2

#define     zero_p      -3




#define DIN_ON      gpio_set_level(LEFT_FLYWHEEL_BRAKE, 1),gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 1);// ������ɲ���ߣ�
#define DIN_OFF     gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 0),gpio_set_level(LEFT_FLYWHEEL_BRAKE, 0);         // ֹͣ��ɲ���ߣ�

float R_balance_Control(float Angle,float *Angle_Zero,float Gyro);
float P_balance_Control(float Angle,float Angle_Zero,float Gyro);
float Y_balance_Control(float Angle,float Angle_Zero,float Gyro);
//float Yaw_balance_Control(float Gyro,float Gyro_control);
float Velocity_Control_A(int encoder);
float Velocity_Control_B(int encoder);
float Velocity_Control_C(int encoder);
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

short constrain_short(short amt, short low, short high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}
//void Get_distance(void);
//void Ultrasonic_module(void);
unsigned char  Start_Flag=0;                    //������־
unsigned char  show_flag=0;                     //��ʾ��־
unsigned char  delay_30,delay_flag;             //30�����־

Status_car status_car = idle;                   //С��״̬ һ��ʼΪ����״̬
Element element = nothing;                      //����Ԫ�� һ��ʼΪĬ��

int count_size,count_sum;
int count_A,count_B,count_C,count_D,count_E,count_F,count_G;            //������
int Flag_A,Flag_B,Flag_C,Flag_D,Flag_E,Flag_F,Flag_G,Flag_H,Flag_I;     //��־λ

short Motor_A,Motor_B,Motor_C,Motor_D;                                  //���PWM����

short Encoder_A,Encoder_B,Encoder_C,Encoder_D;                          //���������������
int32_t Encoder_C_Sum;                                                     //�н�������������ۼ�ֵ

int PWM_R,PWM_Y,PWMA_accel,PWMB_accel,PWMC_accel;                       //PWM�м���

int Voltage;                                                            //��ص�ѹ

//���ú�  Pitch_Zero=1.35  ������ Pitch_Zero=3.82
float Roll_Zero=5.63, Roll_Zero1=5.63;      //Roll_Zero1����¼ֵ
float Pitch_Zero=1.10,Pitch_Zero1=5.63;     //Pitch_Zero1:
float Yaw_Zero=0.0;                     //XY��Ƕ���㣬���е�йأ�Ӱ���ȶ���

float Roll_error=0,Pitch_error=0,Yaw_error=0;
float error_Camera, error_Blance;       //����ͷ�͵�����
float error_of_CameraOrBalance;         //ת�����
//extern int OFFSET0, OFFSET1, OFFSET2;   //������������ֵ 8-23 24-39 40-56
//extern sint16 temp0_L, temp0_R;        //��ʱ��ֵ
//extern sint16 temp1_L, temp1_R;        //��ʱ��ֵ
//extern sint16 temp2_L, temp2_R;        //��ʱ��ֵ


float R_Balance_KP=7000 ,  R_Balance_KI=80   ,  R_Balance_KD=450  ;          //AB����������4500 15 275
float Y_Balance_KP=-1.05  ,  Y_Balance_KI=30.0  ,  Y_Balance_KD=-25.0  ;      //AB�����ת�Ƕ�
float R_Velocity_KP=00  ,  R_Velocity_KI=0   ;                               //AB����ٶȻ�

float P_Balance_KP=152.5, P_Balance_KI=1.2,P_Balance_KD=19.5;                //C���ǰ�����
//float P_Balance_KP=420,P_Balance_KI=0.5,P_Balance_KD=8;
float P_Velocity_KP=9.15,P_Velocity_KI=0.155;                                //C����ٶȻ�

float Yaw_control_1,Yaw_control_2 = 0;
float Yaw_mark;
int Move_distance=0;

/*************************************************************************
���غ���
**************************************************************************/
void Balance_2(void)
{
    static int num;
/*****************������Ϣ�ɼ�***************************************************************************/
    Encoder_A = encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);   //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    Encoder_B = encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);   //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
  //  Encoder_C = ENC_GetCounter(ENC2_InPut_P33_7);   //�н���� ĸ���ϱ�����3��С��ǰ��Ϊ��ֵ
    Encoder_C_Sum += Encoder_C;                     //�н�����������ۼ�
                                  //��ȡ��̬��Ϣ

/******************״̬������***********************************************************************************/
    switch(status_car)
    {
        case idle:          //����״̬
            num++;


            if(key_get_state(KEY_1)==KEY_SHORT_PRESS || key_get_state(KEY_1)==KEY_LONG_PRESS){

                status_car = garage_out;
                Encoder_C_Sum = 0;
                num=0;


                key_clear_state(KEY_1);
            }
            break;
        case garage_out:    //����״̬  (�ò��� ֱ����)
            status_car = run;
            break;
        case run:           //ѭ��״̬
/*************************************Ԫ���ж�*****************************************************************/

/*************************************���*********************************************************************/
            if(key_get_state(KEY_2)==KEY_SHORT_PRESS || key_get_state(KEY_2)==KEY_LONG_PRESS) //��������
            {
                status_car = idle;
                Encoder_C_Sum = 0;
                key_clear_state(KEY_2);
            }
            break;
        case garage_in:     //���״̬
            break;
        default: while(1);

    }
/************************����ֱ���Լ�ת������*********************************************************************************************************************************/
    PWM_R = R_balance_Control(eulerAngle.pitch, &Pitch_Zero,imu660ra_gyro[1]);                    //A B��������������
    if(++Flag_I==30)
    {

        PWM_Y = Y_balance_Control(error_of_CameraOrBalance, 0,imu660ra_gyro[2]);     //A B�������Z��ת��
        Flag_I=0;
    }
    Motor_A = (short)+PWM_R + PWM_Y + PWMA_accel;                                //���տ�����
    Motor_B = (short)-PWM_R + PWM_Y + PWMA_accel;                                //���տ�����

    PWMC_accel = Velocity_Control_C(Encoder_C);                             //C����ٶȻ�������
    Motor_C = -P_balance_Control(eulerAngle.roll, Roll_Zero, imu660ra_gyro[0]) + PWMC_accel;    //C�������ǰ�����

/*************************����******************************************************************************************************************************/
    Roll_error = eulerAngle.roll - Roll_Zero;
    Pitch_error = eulerAngle.pitch - Pitch_Zero;
    //�����ض��Ƕ�ֹͣ����
    if(fabs(Roll_error)>15 || fabs(Pitch_error)>15){ Motor_A =0,Motor_B =0,Motor_C =0,Start_Flag=0;DIN_OFF;}

/***********************����޷�*****************************************************************************************************************************/
    Motor_A = constrain_short(Motor_A, -10000, 10000);                     //PWM�޷�
    Motor_B = constrain_short(Motor_B, -10000, 10000);                     //PWM�޷�
    Motor_C = constrain_short(Motor_C, -3500, 3500);                       //PWM�޷�

/************************�������****************************************************************************************************************************/
    if(Start_Flag==0)
    {
        DIN_OFF;                                        //ɲ��
        MotorCtrl3W(0,0,0);                             //���� ���ֶ�ֹͣ
    }
    else if(Start_Flag==1)
    {
        DIN_OFF;                                        //ɲ��
        MotorCtrl3W(0,0,Motor_C);                       //����ɲ�� ��������
    }
    else if(Start_Flag==2)
    {
        DIN_ON;                                         //����
        MotorCtrl3W(Motor_A, Motor_B,/* Motor_C*/0);         //���� ���ֶ�����
    }

    if(delay_flag==1)
    {
        if(++delay_30==6)   delay_30=0,delay_flag=0;    //���������ṩ30ms�ľ�׼��ʱ
    }
}

/**************************************************************************
X��ƽ��PID����,�ǶȻ�
**************************************************************************/
float R_balance_Control(float Angle,float *Angle_Zero,float Gyro)
{
//    static unsigned int n;                         //�����������ı����
    float PWM,Bias;
    static float error;
    Bias=Angle-*Angle_Zero;                                              //��ȡƫ��
    error+=Bias;                                                         //ƫ���ۻ�
    error = constrain_float(error, -120, 120);                            //�����޷�
    PWM=R_Balance_KP*Bias + R_Balance_KI*error + Gyro*R_Balance_KD/10;   //��ȡ������ֵ
    PWM = constrain_float(PWM, -9000, 9000);                            //����޷�

    if(Start_Flag==0) PWM=0,error=0;                                     //ֹͣʱ��������
    return PWM;
}
/**************************************************************************
Y��ƽ��PID����,�ǶȻ�
**************************************************************************/
float P_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Bias=Angle-Angle_Zero;                                               //��ȡƫ��
    error+=Bias;                                                         //ƫ���ۻ�
    error = constrain_float(error, -30, 30);                            //�����޷�
    PWM=P_Balance_KP*Bias + P_Balance_KI*error + Gyro*P_Balance_KD/10;   //��ȡ������ֵ
    if(Start_Flag==0) PWM=0,error=0;                                     //ֹͣʱ��������
    return PWM;
}
/**************************************************************************
Z��ƽ��PID����,�ǶȻ�
**************************************************************************/
float Y_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Gyro = constrain_float(Gyro, -3000, 3000);                            //ȡֵ�޷�
    Bias=Angle-Angle_Zero;                                               //��ȡƫ�ƫ��ֵ��Ҫ���¼��㣬��Ȼ�������䣩
    error+=Bias;                                                         //ƫ���ۻ�
    error = constrain_float(error, -30, 30);                            //�����޷�
    PWM=Y_Balance_KP*Bias + Y_Balance_KI*error + Gyro*Y_Balance_KD/10;   //��ȡ������ֵ
    if(Start_Flag==0) PWM=0,error=0;                                     //ֹͣʱ��������

    return PWM;
}

/**************************************************************************
�ٶ�PI����,�ٶ���������
**************************************************************************/
float Velocity_Control_A(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //�ٶ��˲�
    Encoder *= 0.7;                                                              //һ�׵�ͨ�˲���
    Encoder += Encoder_Least*0.3;                                                //һ�׵�ͨ�˲���
    Encoder_Integral += Encoder;                                               //���ֳ�λ��
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //�����޷�
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //��ȡ������ֵ
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //ֹͣʱ��������
    return Velocity;
}

/**************************************************************************
�ٶ�PI����,�ٶ���������
**************************************************************************/
float Velocity_Control_B(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //�ٶ��˲�
    Encoder *= 0.7;                                                              //һ�׵�ͨ�˲���
    Encoder += Encoder_Least*0.3;                                                //һ�׵�ͨ�˲���
    Encoder_Integral += Encoder;                                               //���ֳ�λ��
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //�����޷�
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //��ȡ������ֵ
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //ֹͣʱ��������
    return Velocity;
}

/**************************************************************************
C����ٶ�PI����,�ٶ���������
**************************************************************************/
float Velocity_Control_C(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder- (float)Move_distance;                                  //�ٶ��˲�
    Encoder *= 0.7;                                                            //һ�׵�ͨ�˲���
    Encoder += Encoder_Least*0.3;                                              //һ�׵�ͨ�˲���
    Encoder_Integral += Encoder - Move_distance;                             //���ֳ�λ��
    Encoder_Integral = constrain_float(Encoder_Integral, -1000, 1000);        //�����޷�
    Velocity = Encoder * P_Velocity_KP + Encoder_Integral * P_Velocity_KI;   //��ȡ������ֵ
//    Roll_Zero = Roll_Zero1 - Move_distance/500;
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;               //ֹͣʱ��������
    return Velocity;
}


