/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC3xxxA核心板
【编    写】chiusir
【E-mail】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.3版本
【Target 】 TC3xxxA
【Crystal】 20.000Mhz
【SYS PLL】 300MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了RIF为TC3xxxA独有外，其它的代码兼容TC3xx
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include "zf_common_headfile.h"
#include "balance_2.h"
#include "ano.h"
#include "attitude.h"

/*******************
以笛卡尔坐标系为参考
Roll为绕X轴旋转角，
Pitch为绕Y轴旋转角，
Yaw为绕Z轴旋转角。
*******************/
#define LeftFlyWheel 0
#define RightFlyWheel 1


//飞轮
#define FLYWHEEL_CLOCKWISE              (1                      )     // 左侧飞轮顺时针旋转方向
#define FLYWHEEL_DUTY_LEVEL             (0                      )     // 左侧飞轮占空比有效电平

#define LEFT_FLYWHEEL_PWM               (ATOM0_CH7_P02_7        )     // 左侧飞轮PWM控制引脚
#define LEFT_FLYWHEEL_DIR               (P02_6                  )     // 左侧飞轮方向控制引脚
#define LEFT_FLYWHEEL_BRAKE             (P11_3                  )     // 左侧飞轮刹车控制引脚
#define LEFT_FLYWHEEL_ENCODER_INDEX     (TIM5_ENCODER          )     // 左侧飞轮编码器编号
#define LEFT_FLYWHEEL_ENCODER_CH1       (TIM5_ENCODER_CH1_P10_3)     // 左侧飞轮编码器通道1
#define LEFT_FLYWHEEL_ENCODER_CH2       (TIM5_ENCODER_CH2_P10_1)     // 左侧飞轮编码器通道2

#define RIGHT_FLYWHEEL_PWM              (ATOM0_CH5_P02_5        )     // 右侧飞轮PWM控制引脚
#define RIGHT_FLYWHEEL_DIR              (P02_4                  )     // 右侧飞轮方向控制引脚
#define RIGHT_FLYWHEEL_BRAKE            (P11_2                  )     // 右侧飞轮刹车控制引脚
#define RIGHT_FLYWHEEL_ENCODER_INDEX    (TIM2_ENCODER          )     // 右侧飞轮编码器编号
#define RIGHT_FLYWHEEL_ENCODER_CH1      (TIM2_ENCODER_CH1_P33_7)     // 右侧飞轮编码器通道1
#define RIGHT_FLYWHEEL_ENCODER_CH2      (TIM2_ENCODER_CH2_P33_6)     // 右侧飞轮编码器通道2

#define     zero_p      -3




#define DIN_ON      gpio_set_level(LEFT_FLYWHEEL_BRAKE, 1),gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 1);// 启动（刹车线）
#define DIN_OFF     gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 0),gpio_set_level(LEFT_FLYWHEEL_BRAKE, 0);         // 停止（刹车线）

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
unsigned char  Start_Flag=0;                    //启动标志
unsigned char  show_flag=0;                     //显示标志
unsigned char  delay_30,delay_flag;             //30毫秒标志

Status_car status_car = idle;                   //小车状态 一开始为空闲状态
Element element = nothing;                      //赛道元素 一开始为默认

int count_size,count_sum;
int count_A,count_B,count_C,count_D,count_E,count_F,count_G;            //计数器
int Flag_A,Flag_B,Flag_C,Flag_D,Flag_E,Flag_F,Flag_G,Flag_H,Flag_I;     //标志位

short Motor_A,Motor_B,Motor_C,Motor_D;                                  //电机PWM变量

short Encoder_A,Encoder_B,Encoder_C,Encoder_D;                          //编码器的脉冲计数
int32_t Encoder_C_Sum;                                                     //行进电机编码器的累计值

int PWM_R,PWM_Y,PWMA_accel,PWMB_accel,PWMC_accel;                       //PWM中间量

int Voltage;                                                            //电池电压

//静置后  Pitch_Zero=1.35  不静置 Pitch_Zero=3.82
float Roll_Zero=5.63, Roll_Zero1=5.63;      //Roll_Zero1：记录值
float Pitch_Zero=1.10,Pitch_Zero1=5.63;     //Pitch_Zero1:
float Yaw_Zero=0.0;                     //XY轴角度零点，与机械有关，影响稳定性

float Roll_error=0,Pitch_error=0,Yaw_error=0;
float error_Camera, error_Blance;       //摄像头和电磁误差
float error_of_CameraOrBalance;         //转向环误差
//extern int OFFSET0, OFFSET1, OFFSET2;   //面积法三段误差值 8-23 24-39 40-56
//extern sint16 temp0_L, temp0_R;        //临时数值
//extern sint16 temp1_L, temp1_R;        //临时数值
//extern sint16 temp2_L, temp2_R;        //临时数值


float R_Balance_KP=7000 ,  R_Balance_KI=80   ,  R_Balance_KD=450  ;          //AB电机左右倾角4500 15 275
float Y_Balance_KP=-1.05  ,  Y_Balance_KI=30.0  ,  Y_Balance_KD=-25.0  ;      //AB电机旋转角度
float R_Velocity_KP=00  ,  R_Velocity_KI=0   ;                               //AB电机速度环

float P_Balance_KP=152.5, P_Balance_KI=1.2,P_Balance_KD=19.5;                //C电机前后倾角
//float P_Balance_KP=420,P_Balance_KI=0.5,P_Balance_KD=8;
float P_Velocity_KP=9.15,P_Velocity_KI=0.155;                                //C电机速度环

float Yaw_control_1,Yaw_control_2 = 0;
float Yaw_mark;
int Move_distance=0;

/*************************************************************************
主控函数
**************************************************************************/
void Balance_2(void)
{
    static int num;
/*****************基本信息采集***************************************************************************/
    Encoder_A = encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);   //左电机 母板上编码器1，小车前进为负值
    Encoder_B = encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);   //右电机 母板上编码器2，小车前进为正值
  //  Encoder_C = ENC_GetCounter(ENC2_InPut_P33_7);   //行进电机 母板上编码器3，小车前进为正值
    Encoder_C_Sum += Encoder_C;                     //行进电机编码器累加
                                  //获取姿态信息

/******************状态机控制***********************************************************************************/
    switch(status_car)
    {
        case idle:          //空闲状态
            num++;


            if(key_get_state(KEY_1)==KEY_SHORT_PRESS || key_get_state(KEY_1)==KEY_LONG_PRESS){

                status_car = garage_out;
                Encoder_C_Sum = 0;
                num=0;


                key_clear_state(KEY_1);
            }
            break;
        case garage_out:    //出库状态  (用不到 直接走)
            status_car = run;
            break;
        case run:           //循迹状态
/*************************************元素判断*****************************************************************/

/*************************************误差*********************************************************************/
            if(key_get_state(KEY_2)==KEY_SHORT_PRESS || key_get_state(KEY_2)==KEY_LONG_PRESS) //按键出库
            {
                status_car = idle;
                Encoder_C_Sum = 0;
                key_clear_state(KEY_2);
            }
            break;
        case garage_in:     //入库状态
            break;
        default: while(1);

    }
/************************保持直立以及转向运算*********************************************************************************************************************************/
    PWM_R = R_balance_Control(eulerAngle.pitch, &Pitch_Zero,imu660ra_gyro[1]);                    //A B电机控制左右倾角
    if(++Flag_I==30)
    {

        PWM_Y = Y_balance_Control(error_of_CameraOrBalance, 0,imu660ra_gyro[2]);     //A B电机控制Z轴转动
        Flag_I=0;
    }
    Motor_A = (short)+PWM_R + PWM_Y + PWMA_accel;                                //最终控制量
    Motor_B = (short)-PWM_R + PWM_Y + PWMA_accel;                                //最终控制量

    PWMC_accel = Velocity_Control_C(Encoder_C);                             //C电机速度环正反馈
    Motor_C = -P_balance_Control(eulerAngle.roll, Roll_Zero, imu660ra_gyro[0]) + PWMC_accel;    //C电机控制前后倾角

/*************************保护******************************************************************************************************************************/
    Roll_error = eulerAngle.roll - Roll_Zero;
    Pitch_error = eulerAngle.pitch - Pitch_Zero;
    //超过特定角度停止运行
    if(fabs(Roll_error)>15 || fabs(Pitch_error)>15){ Motor_A =0,Motor_B =0,Motor_C =0,Start_Flag=0;DIN_OFF;}

/***********************电机限幅*****************************************************************************************************************************/
    Motor_A = constrain_short(Motor_A, -10000, 10000);                     //PWM限幅
    Motor_B = constrain_short(Motor_B, -10000, 10000);                     //PWM限幅
    Motor_C = constrain_short(Motor_C, -3500, 3500);                       //PWM限幅

/************************电机控制****************************************************************************************************************************/
    if(Start_Flag==0)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,0);                             //飞轮 独轮都停止
    }
    else if(Start_Flag==1)
    {
        DIN_OFF;                                        //刹车
        MotorCtrl3W(0,0,Motor_C);                       //飞轮刹车 独轮启动
    }
    else if(Start_Flag==2)
    {
        DIN_ON;                                         //启动
        MotorCtrl3W(Motor_A, Motor_B,/* Motor_C*/0);         //飞轮 独轮都启动
    }

    if(delay_flag==1)
    {
        if(++delay_30==6)   delay_30=0,delay_flag=0;    //给主函数提供30ms的精准延时
    }
}

/**************************************************************************
X轴平衡PID控制,角度环
**************************************************************************/
float R_balance_Control(float Angle,float *Angle_Zero,float Gyro)
{
//    static unsigned int n;                         //计数，用来改变零点
    float PWM,Bias;
    static float error;
    Bias=Angle-*Angle_Zero;                                              //获取偏差
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -120, 120);                            //积分限幅
    PWM=R_Balance_KP*Bias + R_Balance_KI*error + Gyro*R_Balance_KD/10;   //获取最终数值
    PWM = constrain_float(PWM, -9000, 9000);                            //输出限幅

    if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零
    return PWM;
}
/**************************************************************************
Y轴平衡PID控制,角度环
**************************************************************************/
float P_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Bias=Angle-Angle_Zero;                                               //获取偏差
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -30, 30);                            //积分限幅
    PWM=P_Balance_KP*Bias + P_Balance_KI*error + Gyro*P_Balance_KD/10;   //获取最终数值
    if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零
    return PWM;
}
/**************************************************************************
Z轴平衡PID控制,角度环
**************************************************************************/
float Y_balance_Control(float Angle,float Angle_Zero,float Gyro)
{
    float PWM,Bias;
    static float error;
    Gyro = constrain_float(Gyro, -3000, 3000);                            //取值限幅
    Bias=Angle-Angle_Zero;                                               //获取偏差（偏差值需要重新计算，不然会有跳变）
    error+=Bias;                                                         //偏差累积
    error = constrain_float(error, -30, 30);                            //积分限幅
    PWM=Y_Balance_KP*Bias + Y_Balance_KI*error + Gyro*Y_Balance_KD/10;   //获取最终数值
    if(Start_Flag==0) PWM=0,error=0;                                     //停止时参数清零

    return PWM;
}

/**************************************************************************
速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_A(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //速度滤波
    Encoder *= 0.7;                                                              //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                                //一阶低通滤波器
    Encoder_Integral += Encoder;                                               //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //积分限幅
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //获取最终数值
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //停止时参数清零
    return Velocity;
}

/**************************************************************************
速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_B(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder;                                                   //速度滤波
    Encoder *= 0.7;                                                              //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                                //一阶低通滤波器
    Encoder_Integral += Encoder;                                               //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -2600, 2600);        //积分限幅
    Velocity = Encoder * R_Velocity_KP + Encoder_Integral * R_Velocity_KI/100; //获取最终数值
    Velocity = constrain_float(Velocity, -5000, 5000);
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;                 //停止时参数清零
    return Velocity;
}

/**************************************************************************
C电机速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_C(int encoder)
{
    static float Encoder,Encoder_Integral;
    float Velocity,Encoder_Least;

    Encoder_Least = (float)encoder- (float)Move_distance;                                  //速度滤波
    Encoder *= 0.7;                                                            //一阶低通滤波器
    Encoder += Encoder_Least*0.3;                                              //一阶低通滤波器
    Encoder_Integral += Encoder - Move_distance;                             //积分出位移
    Encoder_Integral = constrain_float(Encoder_Integral, -1000, 1000);        //积分限幅
    Velocity = Encoder * P_Velocity_KP + Encoder_Integral * P_Velocity_KI;   //获取最终数值
//    Roll_Zero = Roll_Zero1 - Move_distance/500;
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;               //停止时参数清零
    return Velocity;
}


