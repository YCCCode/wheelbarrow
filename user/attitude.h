#ifndef __ATTITUDE_H
#define __ATTITUDE_H

#include "zf_common_headfile.h"

typedef struct {
    float gyro_x;
    float gyro_y;
    float gyro_z;
    float acc_x;
    float acc_y;
    float acc_z;
} icm_param_t;


typedef struct {
    float q0;
    float q1;
    float q2;
    float q3;
} quater_param_t;


typedef struct {
    float pitch;    //俯仰角
    float roll;     //偏航角
    float yaw;       //翻滚角
} euler_param_t;


typedef struct {
    float Xdata;
    float Ydata;
    float Zdata;
} gyro_param_t;

//extern euler_param_t eulerAngle;




extern icm_param_t icm_data;
extern float pitchAngle,rollAngle,yawAngle;
extern float Accel_Angle;
extern float roll_acc;
extern float pitch_acc,yaw_acc;

void gyroOffset_init(void);  //零漂初始化

float fast_sqrt(float x);

void ICM_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az);  ////互补滤波

void ICM_getValues();   //转化为实际物理值

void ICM_getEulerianAngles(void);  /*把四元数转换成欧拉角*/

#endif
