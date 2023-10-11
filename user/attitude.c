#include "attitude.h"
#include <math.h>

#define delta_T     0.005f  //5ms����һ��
#define PI        3.1415926f

float I_ex, I_ey, I_ez;  // ������

quater_param_t Q_info = {1, 0, 0,0};  // ȫ����Ԫ��
euler_param_t eulerAngle; //ŷ����

icm_param_t icm_data;
gyro_param_t GyroOffset;

bool GyroOffset_init = 0;

float param_Kp = 0.012;   // ���ٶȼƵ��������ʱ�������
float param_Ki = 0.002f;   //�������������ʵĻ������� 0.004


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
float Q_angle=0.001;// ����������Э����
float Q_gyro=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle=0.5;// ����������Э���� �Ȳ���ƫ��
float dt=0.005;//
char  C_0 = 1;
float Q_bias, Angle_err;
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
�������ܣ����׿������˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Kalman_Filter(float Accel,float Gyro)
{
    pitchAngle+=(Gyro - Q_bias) * dt; //�������
    Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

    Pdot[1]=-PP[1][1];
    Pdot[2]=-PP[1][1];
    Pdot[3]=Q_gyro;
    PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
    PP[0][1] += Pdot[1] * dt;   // =����������Э����
    PP[1][0] += Pdot[2] * dt;
    PP[1][1] += Pdot[3] * dt;

    Angle_err = Accel - pitchAngle;  //zk-�������

    PCt_0 = C_0 * PP[0][0];
    PCt_1 = C_0 * PP[1][0];

    E = R_angle + C_0 * PCt_0;

    K_0 = PCt_0 / E;
    K_1 = PCt_1 / E;

    t_0 = PCt_0;
    t_1 = C_0 * PP[0][1];

    PP[0][0] -= K_0 * t_0;       //����������Э����
    PP[0][1] -= K_0 * t_1;
    PP[1][0] -= K_1 * t_0;
    PP[1][1] -= K_1 * t_1;

    pitchAngle   += K_0 * Angle_err;  //�������
    Q_bias  += K_1 * Angle_err;  //�������
    angle_dot   = Gyro - Q_bias;     //���ֵ(�������)��΢��=���ٶ�
}




float Q_angle_2=0.001;// ����������Э����
float Q_gyro_2=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle_2=0.5;// ����������Э���� �Ȳ���ƫ��
float dt_2=0.005;//

float rollAngle, angle_dot_2;
char  C_0_2 = 1;
float Q_bias_2, Angle_err_2;
float PCt_0_2, PCt_1_2, E_2;
float K_0_2, K_1_2, t_0_2, t_1_2;
float Pdot_2[4] ={0,0,0,0};
float PP_2[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
�������ܣ����׿������˲�
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Kalman_Filter_2(float Accel,float Gyro)
{
    rollAngle+=(Gyro - Q_bias_2) * dt_2; //�������
    Pdot_2[0]=Q_angle_2 - PP_2[0][1] - PP_2[1][0]; // Pk-����������Э�����΢��

    Pdot_2[1]=-PP_2[1][1];
    Pdot_2[2]=-PP_2[1][1];
    Pdot_2[3]=Q_gyro_2;
    PP_2[0][0] += Pdot_2[0] * dt_2;   // Pk-����������Э����΢�ֵĻ���
    PP_2[0][1] += Pdot_2[1] * dt_2;   // =����������Э����
    PP_2[1][0] += Pdot_2[2] * dt_2;
    PP_2[1][1] += Pdot_2[3] * dt_2;

    Angle_err_2 = Accel - rollAngle;  //zk-�������

    PCt_0_2 = C_0_2 * PP_2[0][0];
    PCt_1_2 = C_0_2 * PP_2[1][0];

    E_2 = R_angle_2 + C_0_2 * PCt_0_2;

    K_0_2 = PCt_0_2 / E_2;
    K_1_2 = PCt_1_2 / E_2;

    t_0_2 = PCt_0_2;
    t_1_2 = C_0_2 * PP_2[0][1];

    PP_2[0][0] -= K_0_2 * t_0_2;       //����������Э����
    PP_2[0][1] -= K_0_2 * t_1_2;
    PP_2[1][0] -= K_1_2 * t_0_2;
    PP_2[1][1] -= K_1_2 * t_1_2;

    rollAngle   += K_0_2 * Angle_err_2;  //�������
    Q_bias_2  += K_1_2 * Angle_err_2;  //�������
    angle_dot_2   = Gyro - Q_bias_2;     //���ֵ(�������)��΢��=���ٶ�
}



float Q_angle_3=0.001;// ����������Э����
float Q_gyro_3=0.003;//0.003 ����������Э���� ����������Э����Ϊһ��һ�����о���
float R_angle_3=0.5;// ����������Э���� �Ȳ���ƫ��
float dt_3=0.005;//


float yawAngle, angle_dot_3;
char  C_0_3 = 1;
float Q_bias_3, Angle_err_3;
float PCt_0_3, PCt_1_3, E_3;
float K_0_3, K_1_3,t_0_3, t_1_3;
float Pdot_3[4] ={0,0,0,0};
float PP_3[2][2] = { { 1, 0 },{ 0, 1 } };

/**************************************************************************
�������ܣ����׿������˲� ƫ����
��ڲ��������ٶȡ����ٶ�
����  ֵ����
**************************************************************************/
void Kalman_Filter_3(float Accel,float Gyro)
{
    yawAngle+=(Gyro - Q_bias_3) * dt; //�������
    Pdot_3[0]=Q_angle - PP_3[0][1] - PP_3[1][0]; // Pk-����������Э�����΢��

    Pdot_3[1]=-PP_3[1][1];
    Pdot_3[2]=-PP_3[1][1];
    Pdot_3[3]=Q_gyro_3;
    PP_3[0][0] += Pdot_3[0] * dt_3;   // Pk-����������Э����΢�ֵĻ���
    PP_3[0][1] += Pdot_3[1] * dt_3;   // =����������Э����
    PP_3[1][0] += Pdot_3[2] * dt_3;
    PP_3[1][1] += Pdot_3[3] * dt_3;

    Angle_err_3 = Accel - yawAngle;  //zk-�������

    PCt_0_3 = C_0_3 * PP_3[0][0];
    PCt_1_3 = C_0_3 * PP_3[1][0];

    E_3 = R_angle_3 + C_0_3 * PCt_0_3;

    K_0_3 = PCt_0_3 / E_3;
    K_1_3 = PCt_1_3 / E_3;

    t_0_3 = PCt_0_3;
    t_1_3 = C_0_3 * PP_3[0][1];

    PP_3[0][0] -= K_0_3 * t_0_3;       //����������Э����
    PP_3[0][1] -= K_0_3 * t_1_3;
    PP_3[1][0] -= K_1_3 * t_0_3;
    PP_3[1][1] -= K_1_3 * t_1_3;

    yawAngle   += K_0_3 * Angle_err_3;  //�������
    Q_bias_3  += K_1_3 * Angle_err_3;  //�������
    angle_dot_3   = Gyro - Q_bias_3;    //���ֵ(�������)��΢��=���ٶ�
}




#define alpha           0.3f

//ת��Ϊʵ������ֵ
void ICM_getValues() {


    icm_data.acc_x = ((float) imu660ra_acc[0])  / 8192 ;
    icm_data.acc_y = ((float) imu660ra_acc[1])  /8192  ;
    icm_data.acc_z = ((float) imu660ra_acc[2])  / 8192 ;


    icm_data.gyro_x = (float) imu660ra_gyro[0]  / 32.8f;
    icm_data.gyro_y = (float) imu660ra_gyro[1]  / 32.8f;
    icm_data.gyro_z = (float) imu660ra_gyro[2]  / 32.8f;
}



float roll_acc=0,pitch_acc=0,yaw_acc=0;

/*ת����ŷ����*/
void ICM_getEulerianAngles(void) {

    //�ɼ�����������
    imu660ra_get_gyro();
    imu660ra_get_acc();

    ICM_getValues();


    roll_acc =atan2(-icm_data.acc_y,-icm_data.acc_z)*180/PI;
    pitch_acc=atan2(icm_data.acc_x,-icm_data.acc_z)*180/PI;
  //  yaw_acc=atan2(icm_data.acc_x,icm_data.acc_y)*180/PI;

    yawAngle= (yawAngle+icm_data.gyro_z*0.005);
    Kalman_Filter(roll_acc, icm_data.gyro_x); //�������  roll
    Kalman_Filter_2(pitch_acc, icm_data.gyro_y); //�������  pitch
  //  Kalman_Filter_3(yaw_acc,icm_data.gyro_z);
}

