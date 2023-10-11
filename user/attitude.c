#include "attitude.h"
#include <math.h>

#define delta_T     0.005f  //5ms计算一次
#define PI        3.1415926f

float I_ex, I_ey, I_ez;  // 误差积分

quater_param_t Q_info = {1, 0, 0,0};  // 全局四元数
euler_param_t eulerAngle; //欧拉角

icm_param_t icm_data;
gyro_param_t GyroOffset;

bool GyroOffset_init = 0;

float param_Kp = 0.012;   // 加速度计的收敛速率比例增益
float param_Ki = 0.002f;   //陀螺仪收敛速率的积分增益 0.004


float fast_sqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *) &y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *) &i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}


float pitchAngle, angle_dot;
float Q_angle=0.001;// 过程噪声的协方差
float Q_gyro=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle=0.5;// 测量噪声的协方差 既测量偏差
float dt=0.005;//
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter(float Accel,float Gyro)
{
    pitchAngle+=(Gyro - Q_bias) * dt; //先验估计
    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分

    Pdot[1]=-PP[1][1];
    Pdot[2]=-PP[1][1];
    Pdot[3]=Q_gyro;
    PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分
    PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - pitchAngle;  //zk-先验估计

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //后验估计误差协方差
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    pitchAngle   += K_0 * Angle_err;  //后验估计
    Q_bias  += K_1 * Angle_err;  //后验估计
    angle_dot   = Gyro - Q_bias;     //输出值(后验估计)的微分=角速度
}




float Q_angle_2=0.001;// 过程噪声的协方差
float Q_gyro_2=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle_2=0.5;// 测量噪声的协方差 既测量偏差
float dt_2=0.005;//

float rollAngle, angle_dot_2;
char  C_0_2 = 1;
float Q_bias_2, Angle_err_2;
float PCt_0_2, PCt_1_2, E_2;
float K_0_2, K_1_2, t_0_2, t_1_2;
float Pdot_2[4] ={0,0,0,0};
float PP_2[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
函数功能：简易卡尔曼滤波
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter_2(float Accel,float Gyro)
{
    rollAngle+=(Gyro - Q_bias_2) * dt_2; //先验估计
    Pdot_2[0]=Q_angle_2 - PP_2[0][1] - PP_2[1][0]; // Pk-先验估计误差协方差的微分

    Pdot_2[1]=-PP_2[1][1];
    Pdot_2[2]=-PP_2[1][1];
    Pdot_2[3]=Q_gyro_2;
    PP_2[0][0] += Pdot_2[0] * dt_2;   // Pk-先验估计误差协方差微分的积分
    PP_2[0][1] += Pdot_2[1] * dt_2;   // =先验估计误差协方差
    PP_2[1][0] += Pdot_2[2] * dt_2;
    PP_2[1][1] += Pdot_2[3] * dt_2;

    Angle_err_2 = Accel - rollAngle;  //zk-先验估计

    PCt_0_2 = C_0_2 * PP_2[0][0];
    PCt_1_2 = C_0_2 * PP_2[1][0];

    E_2 = R_angle_2 + C_0_2 * PCt_0_2;

    K_0_2 = PCt_0_2 / E_2;
    K_1_2 = PCt_1_2 / E_2;

    t_0_2 = PCt_0_2;
    t_1_2 = C_0_2 * PP_2[0][1];

    PP_2[0][0] -= K_0_2 * t_0_2;       //后验估计误差协方差
    PP_2[0][1] -= K_0_2 * t_1_2;
    PP_2[1][0] -= K_1_2 * t_0_2;
    PP_2[1][1] -= K_1_2 * t_1_2;

    rollAngle   += K_0_2 * Angle_err_2;  //后验估计
    Q_bias_2  += K_1_2 * Angle_err_2;  //后验估计
    angle_dot_2   = Gyro - Q_bias_2;     //输出值(后验估计)的微分=角速度
}



float Q_angle_3=0.001;// 过程噪声的协方差
float Q_gyro_3=0.003;//0.003 过程噪声的协方差 过程噪声的协方差为一个一行两列矩阵
float R_angle_3=0.5;// 测量噪声的协方差 既测量偏差
float dt_3=0.005;//


float yawAngle, angle_dot_3;
char  C_0_3 = 1;
float Q_bias_3, Angle_err_3;
float PCt_0_3, PCt_1_3, E_3;
float K_0_3, K_1_3,t_0_3, t_1_3;
float Pdot_3[4] ={0,0,0,0};
float PP_3[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
函数功能：简易卡尔曼滤波 偏航角
入口参数：加速度、角速度
返回  值：无
**************************************************************************/
void Kalman_Filter_3(float Accel,float Gyro)
{
    yawAngle+=(Gyro - Q_bias_3) * dt; //先验估计
    Pdot_3[0]=Q_angle - PP_3[0][1] - PP_3[1][0]; // Pk-先验估计误差协方差的微分

    Pdot_3[1]=-PP_3[1][1];
    Pdot_3[2]=-PP_3[1][1];
    Pdot_3[3]=Q_gyro_3;
    PP_3[0][0] += Pdot_3[0] * dt_3;   // Pk-先验估计误差协方差微分的积分
    PP_3[0][1] += Pdot_3[1] * dt_3;   // =先验估计误差协方差
    PP_3[1][0] += Pdot_3[2] * dt_3;
    PP_3[1][1] += Pdot_3[3] * dt_3;

    Angle_err_3 = Accel - yawAngle;  //zk-先验估计

    PCt_0_3 = C_0_3 * PP_3[0][0];
    PCt_1_3 = C_0_3 * PP_3[1][0];

    E_3 = R_angle_3 + C_0_3 * PCt_0_3;

    K_0_3 = PCt_0_3 / E_3;
    K_1_3 = PCt_1_3 / E_3;

    t_0_3 = PCt_0_3;
    t_1_3 = C_0_3 * PP_3[0][1];

    PP_3[0][0] -= K_0_3 * t_0_3;       //后验估计误差协方差
    PP_3[0][1] -= K_0_3 * t_1_3;
    PP_3[1][0] -= K_1_3 * t_0_3;
    PP_3[1][1] -= K_1_3 * t_1_3;

    yawAngle   += K_0_3 * Angle_err_3;  //后验估计
    Q_bias_3  += K_1_3 * Angle_err_3;  //后验估计
    angle_dot_3   = Gyro - Q_bias_3;    //输出值(后验估计)的微分=角速度
}




#define alpha           0.3f

//转化为实际物理值
void ICM_getValues() {


    icm_data.acc_x = ((float) imu660ra_acc[0])  / 8192 ;
    icm_data.acc_y = ((float) imu660ra_acc[1])  /8192  ;
    icm_data.acc_z = ((float) imu660ra_acc[2])  / 8192 ;


    icm_data.gyro_x = (float) imu660ra_gyro[0]  / 32.8f;
    icm_data.gyro_y = (float) imu660ra_gyro[1]  / 32.8f;
    icm_data.gyro_z = (float) imu660ra_gyro[2]  / 32.8f;
}



float roll_acc=0,pitch_acc=0,yaw_acc=0;

/*转换成欧拉角*/
void ICM_getEulerianAngles(void) {

    //采集陀螺仪数据
    imu660ra_get_gyro();
    imu660ra_get_acc();

    ICM_getValues();


    roll_acc =atan2(-icm_data.acc_y,-icm_data.acc_z)*180/PI;
    pitch_acc=atan2(icm_data.acc_x,-icm_data.acc_z)*180/PI;
  //  yaw_acc=atan2(icm_data.acc_x,icm_data.acc_y)*180/PI;

    yawAngle= (yawAngle+icm_data.gyro_z*0.005);
    Kalman_Filter(roll_acc, icm_data.gyro_x); //计算倾角  roll
    Kalman_Filter_2(pitch_acc, icm_data.gyro_y); //计算倾角  pitch
  //  Kalman_Filter_3(yaw_acc,icm_data.gyro_z);
}

