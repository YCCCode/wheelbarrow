//7 18  1:37   �������

#include "zf_common_headfile.h"
#include "balance.h"
#include "ano.h"
#include "attitude.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



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

//��ص�ѹ
#define VOLTAGE_PORT    ADC0_CH8_A8 // ������˿�

#define KEY1                    (P23_1)
#define KEY2                    (P22_0)
#define KEY3                    (P22_1)
#define KEY4                    (P22_2)


#define     PID_WAY     1   //1Ϊ��pid  0Ϊ��pid


#define ENCODER_1                   (TIM4_ENCODER)
#define ENCODER_1_A                 (TIM4_ENCODER_CH1_P02_8)
#define ENCODER_1_B                 (TIM4_ENCODER_CH2_P00_9)

//-------------------------------------------------------------------------------------------


#define DIN_ON      gpio_set_level(LEFT_FLYWHEEL_BRAKE, 1),gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 1);// ������ɲ���ߣ�
#define DIN_OFF     gpio_set_level(RIGHT_FLYWHEEL_BRAKE, 0),gpio_set_level(LEFT_FLYWHEEL_BRAKE, 0);         // ֹͣ��ɲ���ߣ�



#define BUZZER_PIN              (P11_11)



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
int  Start_Flag=0;                    //������־
unsigned char  show_flag=0;                     //��ʾ��־
unsigned char  delay_30,delay_flag;             //30�����־
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
    round_L_Out,    //3:������
    round_R,        //4:�һ���
    cross,          //5:ʮ��
    zebra           //6:������
}Element;


Status_car status_car = idle;                   //С��״̬ һ��ʼΪ����״̬
//Element element = nothing;                      //����Ԫ�� һ��ʼΪĬ��

int count_size,count_sum;
int count_A,count_B,count_C,count_D,count_E,count_F,count_G;            //������
int Flag_A,Flag_B,Flag_C,Flag_D,Flag_E,Flag_F,Flag_G,Flag_H,Flag_I;     //��־λ

int Motor_A,Motor_B,Motor_C,Motor_D;                                  //���PWM����

int Encoder_A,Encoder_B,Encoder_C,Encoder_D;                          //���������������
int32_t Encoder_C_Sum;                                                     //�н�������������ۼ�ֵ

int PWM_R,PWMA_accel,PWMB_accel,PWMC_accel;                       //PWM�м���

                                                        //��ص�ѹ


float Roll_error=0,Pitch_error=0,Yaw_error=0;
float error_Camera, error_Blance;       //����ͷ�͵�����
float error_of_CameraOrBalance;         //ת�����

//                 6200                                    45
float R_Balance_KP=-000,  R_Balance_KI=0   ,  R_Balance_KD=60  ;          //AB����������4500 15 275
float Y_Balance_KP=0  ,  Y_Balance_KI=0.0  ,  Y_Balance_KD=0  ;      //AB�����ת�Ƕ�
float R_Velocity_KP=0  ,  R_Velocity_KI=00   ;                               //AB����ٶȻ�

                    //-190  220                          -0.1  -0.28
float P_Balance_KP=0, P_Balance_KI=0,P_Balance_KD=0;                //C���ǰ�����
//float P_Balance_KP=420,P_Balance_KI=0.5,P_Balance_KD=8;
                    //3
float P_Velocity_KP=0,P_Velocity_KI=0;                             //C����ٶȻ�

float Yaw_control_1,Yaw_control_2 = 0;
float Yaw_mark;
int Move_distance=0;



//�н����

#define XJDIR                 (P21_4)
#define XJPWM                 (ATOM0_CH3_P21_5)

//**********************************************************************************************
//���ú�  Pitch_Zero=1.35  ������ Pitch_Zero=3.82
float Roll_Zero/*, Roll_Zero1=5.63*/;      //Roll_Zero1����¼ֵ
float Pitch_Zero/*,Pitch_Zero1=5.63*/;     //Pitch_Zero1:
float Yaw_Zero;                     //XY��Ƕ���㣬���е�йأ�Ӱ���ȶ���
float Roll_Zero_py_K=0,pitch_Zero_py_K=0,roll_zero_py=0,pitch_zero_py=0;
const float r_wheel = 0.55;
float target_speed_C=0;

int keyn=0;
int foutPwm = 0;


uint8 mscount=0;

int LeftEnable=1,RightEnable=1;

void FlyWheelinit(){
    pwm_init (LEFT_FLYWHEEL_PWM, 17000, FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                  // ��ʼ��������PWM�ź�
    gpio_init(LEFT_FLYWHEEL_DIR, GPO, FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                              // ��ʼ��������DIR�ź�
    gpio_init(LEFT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                  // ��ʼ��������ɲ���ź�


    pwm_init (RIGHT_FLYWHEEL_PWM, 17000, FLYWHEEL_DUTY_LEVEL == 0 ? PWM_DUTY_MAX : 0);                // ��ʼ���Ҳ����PWM�ź�
    gpio_init(RIGHT_FLYWHEEL_DIR, GPO, FLYWHEEL_CLOCKWISE, GPO_PUSH_PULL);                            // ��ʼ���Ҳ����DIR�ź�
    gpio_init(RIGHT_FLYWHEEL_BRAKE, GPO, 1, GPO_PUSH_PULL);                                                 // ��ʼ���Ҳ����ɲ���ź�


}


int16 left_flywheel_encoder_data = 0;                                 // �����ֱ�����ֵ
int16 right_flywheel_encoder_data = 0;                                // �Ҳ���ֱ�����ֵ


void MotorCtrl3W(int a,int b,int c);
int  isReady=0;
float a=1.23,b=2.45,c=3;



int pwmdir=0;
float speedpwm=0,lastCMotor=0;
int PWM_Y_FLAG=30;
float ACC_Y=0;
int flag_reduce=0;

float* changeVar[100]={0};


uint8 temdebugre[33]={0};
uint8 debugrebuff[33]={0};
int curpos=0;
uint8 debugrecnt=0;
//uint8_t txbuff[11]={0};
uint8_t resize = 0;
int16_t tem=0;

void wirelessDebugInit();

int datalen=0;
float Voltage=0.0,PWM_Y=0;
float maxpitch_py=0;
uint8 debugtxbuff[32];
uint8 temtemrebuff[32]={0};
float debugcarstate=0;

int32 datavalue=0,negative=1,cunzaixiaosu=0;
float cusu=1;
void wirelessDebug(){

    datalen=wireless_uart_read_buff(temdebugre, 32);


    if (datalen==0)
        return;


    int ww2=30;
      while(ww2--){

          gpio_toggle_level(BUZZER_PIN);
          system_delay_ms(10);
      }
    for(int i=0;i<datalen;i++){
          if(temtemrebuff[i]=='#' || temtemrebuff[i]=='&'){
              for(int j=i;j<datalen;j++){
                  temdebugre[curpos++]=temtemrebuff[j];
              }
              for(int j=0;j<i;j++){
                  temdebugre[curpos++]=temtemrebuff[j];
              }
          }

      }



    if(temdebugre[0]=='#'){
        debugrecnt=0;
    }else if(temdebugre[0]=='&' && temdebugre[datalen-1]=='!'){
        // �ҵ���һ�����ֵ���ʼλ��
        int a=0, b=0;
        for(int i=1;i<datalen-1;i++){
            if(temdebugre[i]=='*'){
                if(temdebugre[1]=='-'){negative=-1;

                }negative=1;

                for(int j=1;j<i;j++){
                    if(temdebugre[j]=='-')continue;
                    a=a*10+(temdebugre[j]-'0');
                }
                a=negative*a;


                if(temdebugre[i+1]=='-'){negative=-1;

                }negative=1;

                for(int j=1+i;j<datalen-1;j++){
                    if(temdebugre[j]=='-')continue;
                    b=b*10+(temdebugre[j]-'0');
                }
                b=negative*b;

                target_speed_C = a/2.0;

                ACC_Y = b/100.0;
                return;
            }

        }




        return;
    }
    for(int i=0;i<datalen;i++){
                debugrebuff[debugrecnt++]=temdebugre[i];
    }
    if(temdebugre[datalen-1]!='!'){
        return;
    }
    int datasize=debugrecnt;
    debugrecnt=0;
    datavalue=0;

    if(datasize>4){
        cusu=1,cunzaixiaosu=0;
        for(int i=3;i<datasize-1;i++){
            if(i==3){
                if(debugrebuff[3]=='-'){negative=-1;continue;}
                else negative=1;

            }
            if(debugrebuff[i]=='.'){
                cunzaixiaosu=1;
                continue;
            }
            if(cunzaixiaosu==1)cusu=cusu*10;
            datavalue=datavalue * 10 +debugrebuff[i] -'0';

        }

        int dede = (debugrebuff[1]-'0')*10+debugrebuff[2]-'0';
        *changeVar[dede]=negative* datavalue/cusu;

        int ww=30;
        while(ww--){

            gpio_toggle_level(BUZZER_PIN);
            system_delay_ms(10);
        }
    }

}


//pid
struct _pid{
    float err;                  //����ƫ��ֵ
    float err_last;             //������һ��ƫ��ֵ
    float err_sum;             //
    float Kp;                   //������������֡�΢��ϵ��
    float Ki;
    float Kd;
    float pwm;
}pid_gyro= {0  ,0  ,0  ,/*P*/8.3  ,/*i*/0.1  ,/*D*/0 ,0  },
 pid_angle={0  ,0  ,0  ,/*P*/350,/*i*/0   ,/*D*/0.12   ,0  },
 pid_speed={0  ,0  ,0  ,/*P*/-0.039   ,/*i*/-0.002 ,/*D*/0   ,0  },

 pid_gyro_C= {0  ,0  ,0  ,/*P*/300 ,/*i*/0  ,/*D*/8  ,0  },
 pid_angle_C={0  ,0  ,0  ,/*P*/0,/*i*/0   ,/*D*/0   ,0  },
 pid_speed_C={0  ,0  ,0  ,/*P*/-0.0078,/*i*/-0.001   ,/*D*/0   ,0  },
 pid_acc_c={0  ,0  ,0  ,/*P*/-0,/*i*/0   ,/*D*/0   ,0  },
yaw_y={0  ,0  ,0  ,/*P*/-10,/*i*/0   ,/*D*/0   ,0  };

float cnt_angle=0,cnt_speed=0,cur_pwm=0,cur_pwm2=0,cur_pwm_C=0,cur_pwm2_C=0,cnt_gyro=0;
float cur_set_speed=0,maxacc=0;
//roll_zero_py=0,pitch_zero_py=0;
float backv1=0,backv2=0,backv3=0,backv4=0;
float Ypwm_P=0,Ypwm_I=0,Ypwm_D=0;
char str[100];
int XJ_PWM_C=0;

extern float camerr;
void SendVofa(){
    sprintf(str,"%s:%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n","acc",
            camerr,Encoder_C/1.0,Encoder_A/1.0,Encoder_B/1.0,PWM_Y,pid_speed.pwm,
            pitch_zero_py,roll_zero_py,Motor_A/1.0,Motor_B/1.0,Motor_C/1.0,
            pitchAngle,rollAngle,Voltage,pid_acc_c.pwm,pid_acc_c.err,
            ACC_Y,Ypwm_P,Ypwm_I,Ypwm_D,backv4,
            pid_acc_c.pwm);
    wireless_uart_send_string(str);


//uart_write_string(UART_2, str);

}
float balpwm=400;

float minPWM_Y=0,k_cam=1,k_xie=10;
float maxPWM_Y = 2,ypwmjiange=30,mj_PY=0,max_C_acc=0,setYpwm=0,max_Y_gyro=300,max_err_ypwm=30;
extern float turn_r,turn_xie;

int encode_speed_C=0;
float C_speed_k=0.1;
//
void chansuInit(){
//    pid_gyro.Kp
//    &pid_gyro.Ki
//    pid_angle.Kp,
//    pid_angle.Ki,
//    pid_speed.Kp,
//    pid_speed.Ki,
//
    pid_gyro_C.Kp=270;
    C_speed_k=-0.003;
//    pid_gyro_C.Ki,
//    pid_angle_C.Kp,
//    pid_angle_C.Kd,
//    pid_speed_C.Kp,
//
//    pid_speed_C.Ki,
    Pitch_Zero=0.95;  //1.35
    Roll_Zero=6.5;    //6.5
//    Start_Flag,
//



    pid_gyro_C.Kd=12.2;
//    pid_angle_C.Ki,
//
//    pid_speed_C.Kd,
//    PWM_Y,

    //ת��

    Y_Balance_KP=10;
    Y_Balance_KI=0.2;
    Y_Balance_KD=800;
//        Y_Balance_KP=13;
//        Y_Balance_KI=1;
//        Y_Balance_KD=1100;
//

//
    pitch_Zero_py_K=0.01;   //0.005
//    Roll_Zero_py_K
  //  target_speed_C=-410;  //400  430
//    balpwm,
//    pid_acc_c.Kp,
//    pid_acc_c.Ki,
//    pid_acc_c.Kd,
//    maxacc,
//    maxPWM_Y,
    minPWM_Y=50;
//    ypwmjiange,
    k_cam=0.7;//0.7
//    k_xie,
//    mj_PY,
//    max_C_acc,
//    setYpwm,
    max_err_ypwm=99000;
    max_Y_gyro=3000;
    encode_speed_C=00;
    maxpitch_py=3;
    speedChange(0);


}
int acc_yunxu=0;
int showstate=0;
int dl1a_distance=0;
int curmode=0;
int expwm=1000;
//0����ģʽ  1����ģʽ
void speedChange(int mode){

    dl1a_get_distance();
    if(dl1a_finsh_flag == 1)
    {
        dl1a_finsh_flag = 0;
        dl1a_distance= dl1a_distance_mm;
    }




 if(mode ==0){
     curmode=0;
     encode_speed_C=-200;
     pid_gyro_C.Kd=6.9;
     target_speed_C=-410;
     k_cam=0.8;

     if(acc_yunxu==1){
         encode_speed_C=-200;
         pid_gyro_C.Kd=7;
         target_speed_C=-610;
         k_cam=0.8;
     }

 }else if(mode == 1){  //����  ���������������ٶ�  �رն�̬���
     curmode=1;
     k_cam=0.7;
     encode_speed_C=-450;
     Roll_Zero=8;
     pid_gyro_C.Kd=12.2;
 }else if(mode == 2){  //׼���׶�
     curmode=2;
     pid_gyro_C.Kd=7.2;
     target_speed_C=-0;
     encode_speed_C=0;
     k_cam=0;
 }else if(mode == 4){ //����״̬
     curmode=4;
     encode_speed_C=-200;
     pid_gyro_C.Kd=7;
     target_speed_C=-610;
     k_cam=0.8;
 }




}
float maxrollzero=8.5;
void wirelessDebugInit(){
    float* setchangeVar[]={&pid_gyro.Kp,&pid_gyro.Ki,&pid_angle.Kp,&pid_angle.Ki,&pid_speed.Kp,&pid_speed.Ki,
            &pid_gyro_C.Kp,     &pid_gyro_C.Ki, &pid_angle_C.Kp,    &pid_angle_C.Kd,    &pid_speed_C.Kp,
            &pid_speed_C.Ki,    &Pitch_Zero,    &Roll_Zero,         &Start_Flag,        &a,
            &pid_gyro.Kd,       &pid_angle.Ki,  &pid_speed.Kd,      &pid_gyro_C.Kd,     &pid_angle_C.Ki,
            &pid_speed_C.Kd,    &PWM_Y,         &Y_Balance_KP,      &Y_Balance_KI,      &Y_Balance_KD,
            &pitch_Zero_py_K,   &Roll_Zero_py_K,&target_speed_C,    &balpwm,            &pid_acc_c.Kp,
            &pid_acc_c.Ki,      &pid_acc_c.Kd,  &maxacc,            &maxPWM_Y,          &minPWM_Y,
            &ypwmjiange,        &k_cam,         &k_xie,             &mj_PY,             &max_C_acc,
            &setYpwm,           &max_err_ypwm,  &max_Y_gyro,        &maxpitch_py

    };
    for(int i=0;i<sizeof(setchangeVar)/sizeof(setchangeVar[0]);i++){
        changeVar[i]=setchangeVar[i];
    }

}
extern int cross_turn;


int t1flag=0,t2flag=0,t3flag=0;

int t1time=0,t2time=0,t3time=0;

void speed_C(){
    key_scanner();
    if(t1flag==1){
        if(encode_speed_C<0){
            encode_speed_C+=10;
        }
        t1time++;
    }else{
        t1time=0;
    }
    if(t2flag==1){
        t2time++;
    }else{
        t2time=0;
    }
    if(t3flag==1){
        t3time++;
    }else{
        t3time=0;
    }

    float err=target_speed_C-Encoder_C;
if(Start_Flag==0 || curmode==1)return;
    err = constrain_float(err *C_speed_k, -0.1, 0.1);
    Roll_Zero +=err ;
    Roll_Zero= constrain_float(Roll_Zero, -5,maxrollzero);

}
uint8 txbuff[20]={0};
int disp = 0;
uint8 key2_long_press_flag=0,key1_long_press_flag=0,key3_long_press_flag=0,key4_long_press_flag=0;
extern int circlestate;
extern uint8 goflag;
extern int go_over_dis,go_angle,go_angle_zero;
extern float duanru_len;
int debugmode = 1; //1����ģʽ  2������ģʽ 3�رհ�����
void key_handle(){
//------------------------------------KEY1-----------------------------
    if(key_get_state(KEY_1)==KEY_LONG_PRESS){


        if(debugmode==0){
            debugmode=1;
        }else{
            debugmode=0;
        }


       key1_long_press_flag=1;
       key_clear_state(KEY_1);
   }
    else  if(key_get_state(KEY_1)==KEY_SHORT_PRESS ){
        if(key1_long_press_flag==0){
            //�̰�Ч��


            if(Start_Flag==0 || curmode==3){
                Start_Flag=2;
                speedChange(0);
            }else if(Start_Flag==2)Start_Flag=0;


        }else{
            key1_long_press_flag=0;
        }
        key_clear_state(KEY_1);
    }

//------------------------------------KEY2-----------------------------
    if(key_get_state(KEY_2)==KEY_LONG_PRESS){

        maxrollzero+=0.1;

       key2_long_press_flag=1;
       key_clear_state(KEY_2);
   }else  if(key_get_state(KEY_2)==KEY_SHORT_PRESS ){
        if(key2_long_press_flag==0){


        maxrollzero-=0.1;




        }else{
            key2_long_press_flag=0;
        }
        key_clear_state(KEY_2);
    }



//------------------------------------KEY3-----------------------------
    if(key_get_state(KEY_3)==KEY_LONG_PRESS){

        duanru_len+=1;

       key3_long_press_flag=1;
       key_clear_state(KEY_3);
   }else  if(key_get_state(KEY_3)==KEY_SHORT_PRESS ){
        if(key3_long_press_flag==0){


            duanru_len-=1;


        }else{
            key3_long_press_flag=0;
        }
        key_clear_state(KEY_3);
    }


//------------------------------------KEY4-----------------------------
    if(key_get_state(KEY_4)==KEY_LONG_PRESS){

        Pitch_Zero+=0.1;


       key4_long_press_flag=1;
       key_clear_state(KEY_4);
   }else  if(key_get_state(KEY_4)==KEY_SHORT_PRESS ){
        if(key4_long_press_flag==0){

            Pitch_Zero-=0.1;


        }else{
            key4_long_press_flag=0;
        }
        key_clear_state(KEY_4);
    }



}


void Balance_init(){
   // speedChange(0);

    gpio_init(XJDIR, GPO, GPIO_HIGH, GPO_PUSH_PULL);                              // GPIO ��ʼ��Ϊ��� Ĭ�����������
    pwm_init(XJPWM, 5000, 0);

  wirelessDebugInit();
    encoder_dir_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);

    int t=0;
 // wireless_uart_init();

    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);                        // ��ʼ�� BUZZER_PIN ��� Ĭ�ϵ͵�ƽ �������ģʽ

   // tft180_init();
    *changeVar[15]=32.1;
//    int16_t senddata[10]={0};
//
//    int8_t rebuff[10]={0};

    isReady = 1;
    dl1a_init();


    int reuarttime=0;
   // uart_init(UART_2, 112500,UART2_TX_P10_5 ,UART2_RX_P10_6 );
    //wireless_uart_send_string("start");

    chansuInit();

    while (TRUE)
        {
        reuarttime++;
        if(reuarttime>=3){
            reuarttime=0;

            //if(Start_Flag!=0)
//            SendVofa();
//            wirelessDebug();

//            txbuff[0]=(((int16)(rollAngle*100)))&0xFF;
//            txbuff[1]=((int16)(rollAngle*100)>>8)&0xFF;
//            txbuff[2]=(((int16)(Roll_Zero*100)))&0xFF;
//            txbuff[3]=((int16)(Roll_Zero*100)>>8)&0xFF;
////            txbuff[4]=(((int16)cur_pwm2*100))&0xFF;
////            txbuff[5]=(((int16)cur_pwm2*100)>>8)&0xFF;
////            txbuff[6]=((int16)(cur_pwm*100))&0xFF;
////            txbuff[7]=((int16)(cur_pwm*100)>>8)&0xFF;
////            txbuff[8]=((int16)(PWM_R*100))&0xFF;
////            txbuff[9]=((int16)(PWM_R*100)>>8)&0xFF;
//            ToNimin(0xF1, txbuff, 4);
//
//
//            txbuff[0]=(((int16)(cur_pwm_C*100)))&0xFF;
//            txbuff[1]=((int16)(cur_pwm_C*100)>>8)&0xFF;
//            txbuff[2]=(((int16)(Motor_C*100)))&0xFF;
//            txbuff[3]=((int16)(Motor_C*100)>>8)&0xFF;
//            txbuff[4]=(((int16)(PWM_R*100)))&0xFF;
//            txbuff[5]=((int16)(PWM_R*100)>>8)&0xFF;
//            ToNimin(0xF2, txbuff, 6);

    //        pid_speed.Kp * pid_speed.err+pid_speed.err_sum*pid_speed.Ki+pid_speed.Kd*(pid_speed.err-pid_speed.err_last);//+ pid_speed.Kd * (pid_speed.err - pid_speed.err_last);

        }
    //    dl1a_get_distance();
       //     wireless_uart_read_buff(rebuff, 1);

          // wireless_uart_read_buff(rebuff, 1);

           // ToNimin(0xF1,senddata ,sizeof(senddata[0])*3);


//           //��������ʾ
//            tft180_show_float(0, 0, pitchAngle,5, 2);
//
//            tft180_show_float(0, 15, rollAngle,5, 2);
//           tft180_show_float(0, 30, a,5, 2);
        key_handle();

            if(t>300){
                t=0;

                Voltage=adc_convert(VOLTAGE_PORT)*11*3.3/4096.0;
                if(Voltage<21.8 && Voltage>10){
                    int ww=200;
                    while(ww--){

                        gpio_toggle_level(BUZZER_PIN);
                        system_delay_ms(5);
                    }
                }


            }else t++;

            /******************״̬������***********************************************************************************/
//            switch(status_car)
//            {
//                case idle:          //����״̬
//                    num++;
//
//                    if(key_get_state(KEY_1)==KEY_SHORT_PRESS || key_get_state(KEY_1)==KEY_LONG_PRESS){
//
//                        status_car = garage_out;
//                        Encoder_C_Sum = 0;
//                        num=0;
//
//                        key_clear_state(KEY_1);
//                    }
//                    break;
//                case garage_out:    //����״̬  (�ò��� ֱ����)
//                    status_car = run;
//                    break;
//                case run:           //ѭ��״̬
//
//                    if(key_get_state(KEY_2)==KEY_SHORT_PRESS || key_get_state(KEY_2)==KEY_LONG_PRESS) //��������
//                    {
//                        status_car = idle;
//                        Encoder_C_Sum = 0;
//                        key_clear_state(KEY_2);
//                    }
//                    break;
//                case garage_in:     //���״̬
//                    break;
//                default: while(1);
//
//            }

            system_delay_ms(5);

        }

}


//���ú�  Pitch_Zero=1.35  ������ Pitch_Zero=3.82
float Roll_Zero=5.41/*, Roll_Zero1=5.63*/;      //Roll_Zero1����¼ֵ
float Pitch_Zero=1.08/*,Pitch_Zero1=5.63*/;     //Pitch_Zero1:
float Yaw_Zero=0.0;                     //XY��Ƕ���㣬���е�йأ�Ӱ���ȶ���

//float Roll_Zero_py_K=0,pitch_Zero_py_K=0,roll_zero_py=0,pitch_zero_py=0;


int Y_PWM=0;

float Encoder_C2=0;
float Pid_speed_C(float set){
    Encoder_C2 = Encoder_C;
   // pid_angle_C.err=Encoder2;

    pid_speed_C.err=set-(Encoder_C2);
    pid_speed_C.err_sum+=pid_speed_C.err;
    pid_speed_C.err_sum = constrain_float(pid_speed_C.err_sum, -300, 300);//�����޷�
    pid_speed_C.pwm = pid_speed_C.Kp * pid_speed_C.err+pid_speed_C.err_sum*pid_speed_C.Ki+pid_speed_C.Kd*(pid_speed_C.err-pid_speed_C.err_last);//+ pid_speed_C.Kd * (pid_speed_C.err - pid_speed_C.err_last);
    pid_speed_C.err_last=pid_speed_C.err;
    if(Start_Flag==0)pid_speed_C.err_sum=0,pid_speed_C.err=0,pid_speed_C.err_last=0;

    if(pid_speed_C.pwm>8)pid_speed_C.pwm=8;
    if(pid_speed_C.pwm<-8)pid_speed_C.pwm=-8;
    return pid_speed_C.pwm;
}


//�ٶ�  ֻpi����
float encoder,encoder_least;
float Pid_speed(float set){
//    encoder_least=(Encoder_A-Encoder_B);
//    encoder*=0.7;
//    encoder+=encoder_least*0.3;
    encoder=Encoder_A-Encoder_B;
    pid_speed.err=set-encoder;

    pid_speed.err_sum+=pid_speed.err;
    pid_speed.err_sum = constrain_float(pid_speed.err_sum, -0.5, 0.5);//�����޷�
    pid_speed.pwm = pid_speed.Kp * pid_speed.err+pid_speed.err_sum*pid_speed.Ki+pid_speed.Kd*(pid_speed.err-pid_speed.err_last);//+ pid_speed.Kd * (pid_speed.err - pid_speed.err_last);
    pid_speed.err_last=pid_speed.err;
    pid_speed.pwm=constrain_float(pid_speed.pwm, -3, 3);
    if(Start_Flag==0)pid_speed.err_sum=0,pid_speed.err=0,pid_speed.err_last=0;
    return pid_speed.pwm;
}

float Pid_acc_C(float set){
    pid_acc_c.err=set-Encoder_C;

    pid_acc_c.err_sum+=pid_acc_c.err;
    pid_acc_c.err_sum=constrain_float(pid_acc_c.err_sum, -maxacc, maxacc);//�����޷�
  //  pid_acc_c.err_sum=pid_acc_c.err_sum=constrain_float(pid_acc_c.err_sum, -maxaccsum, maxaccsum);//�����޷�
   // pid_acc_c.err_sum = constrain_float(pid_acc_c.err_sum, -maxacc, maxacc);//�����޷�
    pid_acc_c.pwm = pid_acc_c.Kp * pid_acc_c.err+pid_acc_c.err_sum*pid_acc_c.Ki+pid_acc_c.Kd*(pid_acc_c.err-pid_acc_c.err_last);//+ pid_acc_c.Kd * (pid_acc_c.err - pid_acc_c.err_last);
    pid_acc_c.err_last=pid_acc_c.err;
    if(Start_Flag==0)pid_acc_c.err_sum=0,pid_acc_c.err=0,pid_acc_c.err_last=0,pid_acc_c.pwm=0;
    return pid_acc_c.pwm;
}
float imu660ra_gyro_last=0;
float Pid_gyro(float set){
    pid_gyro.err=set-(imu660ra_gyro[0]);
    imu660ra_gyro_last=imu660ra_gyro[0];
    pid_gyro.err_sum+=pid_gyro.err;
    pid_gyro.err_sum = constrain_float(pid_gyro.err_sum, -1, 1);//�����޷�
    pid_gyro.pwm = pid_gyro.Kp * pid_gyro.err+pid_gyro.err_sum*pid_gyro.Ki+pid_gyro.Kd*(pid_gyro.err-pid_gyro.err_last);//+ pid_gyro.Kd * (pid_gyro.err - pid_gyro.err_last);
    backv1=pid_gyro.err;
    backv2=imu660ra_gyro[0];
    pid_gyro.err_last=pid_gyro.err;
    if(Start_Flag==0)pid_gyro.err_sum=0,pid_gyro.err=0,pid_gyro.err_last=0;
    return pid_gyro.pwm;
}
float Pid_gyro_C(float set){
   // roll_zero_py = Roll_Zero_py_K* PWM_Y*Encoder_C*Encoder_C/10000.0;
   pid_gyro_C.err=(rollAngle-Roll_Zero - set-roll_zero_py);
  //  pid_gyro_C.err_sum+=pid_gyro_C.err;
  //  pid_gyro_C.err_sum = constrain_float(pid_gyro_C.err_sum, -2000, 2000);//�����޷�
    pid_gyro_C.pwm = pid_gyro_C.Kp *pid_gyro_C.err +pid_gyro_C.Kd*(imu660ra_gyro[1]/10)+200*(pid_gyro_C.err-pid_gyro_C.err_last);//+ pid_gyro_C.Kd * (pid_gyro_C.err - pid_gyro_C.err_last);
    pid_gyro_C.err_last=pid_gyro_C.err;

    if(Start_Flag==0)pid_gyro_C.err_sum=0,pid_gyro_C.err=0,pid_gyro_C.err_last=0;

    pid_gyro_C.pwm=constrain_float(pid_gyro_C.pwm, -2500, 2500);

    return pid_gyro_C.pwm;
}

//�Ƕ�
float Pid_angle(float set){
 //   pitch_zero_py = pitch_Zero_py_K* PWM_Y*Encoder_C*Encoder_C/100000.0;
 //   pitch_zero_py = constrain_float(pitch_zero_py, -2, 2);
    pid_angle.err=set-(pitchAngle-Pitch_Zero-pitch_zero_py);
    pid_angle.err_sum+=pid_angle.err;
    pid_angle.err_sum = constrain_float(pid_angle.err_sum, -4000, 4000);//�����޷�
    pid_angle.pwm = pid_angle.Kp * pid_angle.err+pid_angle.err_sum*pid_angle.Ki+pid_angle.Kd*(pid_angle.err-pid_angle.err_last);//+ pid_angle.Kd * (pid_angle.err - pid_angle.err_last);
    pid_angle.err_last=pid_angle.err;
    pid_angle.pwm=constrain_float(pid_angle.pwm, -8000, 8000);
    if(Start_Flag==0)pid_angle.pwm=0,pid_angle.err_sum=0,pid_angle.err=0,pid_angle.err_last=0;
    return pid_angle.pwm;
}
//�Ƕ�
float Pid_angle_C(float set){

  //  pid_angle_C.err=set-(rollAngle-Roll_Zero);
  //  pid_angle_C.err_sum+=pid_angle_C.err;
 //   pid_angle_C.err_sum = constrain_float(pid_angle_C.err_sum, -2000, 2000);//�����޷�
 //   pid_angle_C.pwm = pid_angle_C.Kp * pid_angle_C.err+pid_angle_C.err_sum*pid_angle_C.Ki+pid_angle_C.Kd*(pid_angle_C.err-pid_angle_C.err_last);//+ pid_angle_C.Kd * (pid_angle_C.err - pid_angle_C.err_last);
 //   pid_angle_C.err_last=pid_angle_C.err;

    float Encoder_Least2;
    static float Encoder2;
    Encoder_Least2= Encoder_C;
    Encoder2 *= 0.7;            //һ�׻����˲�
    Encoder2 += Encoder_Least2*0.3;
    pid_angle_C.err=Encoder2;

    pid_angle_C.err_sum+=pid_angle_C.err;
    pid_angle_C.err_sum=constrain_float(pid_angle_C.err_sum, 10, 10);
    pid_angle_C.pwm = pid_angle_C.Kp*pid_angle_C.err + pid_angle_C.Ki* pid_angle_C.err_sum;
   // pid_angle_C.pwm=constrain_float(pid_angle_C.pwm, -1200, 1200);
    return pid_angle_C.pwm;
}
int carstate=1;
int t5=0;
//-----------------------
void balance(){

    cnt_gyro++;
    cnt_speed++;
    cnt_angle++;
    flag_reduce++;
    PWM_Y_FLAG++;

    if(PID_WAY==1){

        if(PWM_Y_FLAG>=50 ){
            PWM_Y_FLAG=0;


//            if(curmode != 4){
                Y_PWM = Y_balance_Control(camerr*k_cam, 0,imu660ra_gyro[2]/100)+expwm;
//            }else if(curmode == 4){
//                Y_PWM =  yaw_y.pwm;
//            }




                if(curmode==0){



//                    if(pitch_zero_py<0.5 && pitch_zero_py>-0.5){
//
            if(abs(Encoder_A)-abs(Encoder_B)>0){
                                if(Encoder_A>0){
                                    pitch_zero_py=pitch_Zero_py_K*(Encoder_A-Encoder_B);
                                }else{
                                    pitch_zero_py=-pitch_Zero_py_K*(Encoder_A-Encoder_B);
                                }

                            }else if(abs(Encoder_B)-abs(Encoder_A)>0){
                                if(Encoder_B>0){
                                    pitch_zero_py=pitch_Zero_py_K*(Encoder_B-Encoder_A);
                                }else{
                                    pitch_zero_py=-pitch_Zero_py_K*(Encoder_B-Encoder_A);
                                }


                            }

            pitch_zero_py=constrain_float(pitch_zero_py, -0.6, -0.6);


                    }


//                pitch_zero_py+=-(Encoder_C*Encoder_C*imu660ra_gyro[2]*camerr)/9000000000.0;
//                pitch_zero_py=constrain_float(pitch_zero_py, -maxpitch_py, maxpitch_py);
        }

//        if(abs(PWM_Y)>2200){
//            carstate=0;
//            ACC_Y=0;
//            Roll_Zero=2.39;
//        }else if(abs(PWM_Y)<000 && carstate==0){
//            Roll_Zero=5;
//            carstate=1;
//        }


     //   PWM_Y= constrain_float(PWM_Y, -2200, 2200);


        if(flag_reduce>=500){
            flag_reduce=0;

            if(abs(Y_PWM)<50){
                Y_PWM=0;
            }else if(Y_PWM>minPWM_Y )Y_PWM =Y_PWM-minPWM_Y;
            else
            {
                Y_PWM =Y_PWM+minPWM_Y;

            }


        }






        if(cnt_speed>=20){
            cnt_speed=0;






            Encoder_A =encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);  //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
            Encoder_B =encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);   //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
            Encoder_C = encoder_get_count(ENCODER_1);   //�н���� ĸ���ϱ�����3��С��ǰ��Ϊ��ֵ
           //
            Encoder_C_Sum += Encoder_C;                     //�н�����������ۼ�
           // speedpwm=Pid_acc_C(target_speed_C);
            cur_pwm2=Pid_speed(0);
            backv4=cur_pwm2;
           //cur_pwm2_C=Pid_speed_C(0);
            encoder_clear_count(ENCODER_1);
            encoder_clear_count(LEFT_FLYWHEEL_ENCODER_INDEX);
            encoder_clear_count(RIGHT_FLYWHEEL_ENCODER_INDEX);

        }


        if(cnt_angle>=8){
            cnt_angle=0;



//

            speedChange(curmode);
//
//            if(curmode==0){
//                pitch_zero_py=-(Encoder_C*Encoder_C*camerr)/8000000.0;
//            }
           // pitch_zero_py=constrain_float(pitch_zero_py, -0.5, 0.5);

           cur_pwm=Pid_angle(cur_pwm2);
           backv3=cur_pwm;
//            if(maxacc<abs(target_speed_C-cur_set_speed)){
//                if(target_speed_C>cur_set_speed){
//                    cur_set_speed+=maxacc;
//                }else{
//                    cur_set_speed-=maxacc;
//                }
//
//            }else{
//                cur_set_speed=target_speed_C;
//            }
           cur_pwm_C=Pid_speed_C(encode_speed_C);
            //cur_pwm_C=Pid_angle_C(0);

        }
        if(cnt_gyro>=2){
            cnt_gyro=0;






            Motor_C = Pid_gyro_C(cur_pwm_C);


//            if(abs(max_C_acc)<=abs(Motor_C-lastCMotor)){
//                    if(Motor_C-lastCMotor>0){
//                        Motor_C=lastCMotor+max_C_acc;
//                    }else{
//
//                        Motor_C=lastCMotor-max_C_acc;
//                    }
//
//
//            }


             PWM_R =  Pid_gyro(cur_pwm);

        }





    }








//        backv3=PWM_Y;
//   //     PWM_Y = constrain_float(PWM_Y, -maxPWM_Y, maxPWM_Y);
//
//    }else{
//        if(PWM_Y>0 && PWM_Y>minPWM_Y)PWM_Y=PWM_Y-minPWM_Y;
//        else if(PWM_Y<0 && PWM_Y<-minPWM_Y)PWM_Y+=minPWM_Y;
//    }


//   PWM_Y += Y_balance_Control(error_Camera, 0,imu660ra_gyro[2]);
//   PWM_Y=constrain_float(PWM_Y, -maxPWM_Y, maxPWM_Y);


    if(Motor_A>Motor_B){
        if(Y_PWM>0){
            Motor_A = (short)+PWM_R+ 0.8*Y_PWM;
            Motor_B = (short)-PWM_R + 1.2*Y_PWM;
        }else{
            Motor_A = (short)+PWM_R+ 1.2*Y_PWM;
            Motor_B = (short)-PWM_R + 0.8*Y_PWM;
        }
    }else{
        if(Y_PWM>0){
            Motor_A = (short)+PWM_R+ 1.2*Y_PWM;
            Motor_B = (short)-PWM_R + 0.8*Y_PWM;

        }else{
            Motor_A = (short)+PWM_R+ 0.8*Y_PWM;
            Motor_B = (short)-PWM_R + 1.2*Y_PWM;
        }


    }

//      Motor_A = (short)+PWM_R+ Y_PWM;// Y_PWM;                               //���տ�����
//
//
//
//
//       Motor_B = (short)-PWM_R + Y_PWM;   //���տ�����


//t5++;
//if(t5>=0 && t5<5){
//    Motor_A = (short)+PWM_R;// Y_PWM;                               //���տ�����
//        Motor_B = (short)-PWM_R+ Y_PWM ;   //���տ�����
//
//}else if(t5>=5 ){
//
//    Motor_A = (short)+PWM_R+ Y_PWM;// Y_PWM;                               //���տ�����
//   Motor_B = (short)-PWM_R ;   //���տ�����
//   if(t5>=10){
//       t5=0;
//   }
//}


       if(fabs(rollAngle)>40 || fabs(pitchAngle)>20)Start_Flag=0;  //ˤ�����

    Motor_A = constrain_short(Motor_A, -9800, 9800);                     //PWM�޷�
    Motor_B =constrain_short(Motor_B, -9800, 9800);                     //PWM�޷�
    Motor_C = constrain_short(Motor_C, -3700, 3700);                       //PWM�޷�
    //Start_Flag=2;
    if(Start_Flag==0)
    {
        if(curmode==3){

        }else{
            DIN_OFF;                                        //ɲ��
           MotorCtrl3W(0,0,0);                             //���� ���ֶ�ֹͣ
           Y_PWM=0;
        }

    }
    else if(Start_Flag==1)
    {
        DIN_OFF;                                        //ɲ��
        MotorCtrl3W(0,0,Motor_C);                       //����ɲ�� ��������
    }
    else if(Start_Flag==2)
    {

        DIN_ON;                                         //����
        MotorCtrl3W(Motor_A, Motor_B,Motor_C);         //���� ���ֶ�����
    }
    else if(Start_Flag==3)
    {
        DIN_ON;                                         //����
        MotorCtrl3W(Motor_A, Motor_B,0);         //���� ���ֶ�����
    }
}





/*************************************************************************
���غ���
**************************************************************************/
int time=0;
float hand_gyro=0;
float C_gyro=0;
int EA[5]={0},EB[5]={0},EC=0;
void balance_2(void)
{

    if(isReady==0)return;

    time=0;
    EA[0]=EA[1];
    EA[1]=EA[2];
    EA[2]=EA[3];
    EA[3]=EA[4];
    EA[4]= encoder_get_count(LEFT_FLYWHEEL_ENCODER_INDEX);

    EB[0]=EB[1];
    EB[1]=EB[2];
    EB[2]=EB[3];
    EB[3]=EB[4];
    EB[4]= encoder_get_count(RIGHT_FLYWHEEL_ENCODER_INDEX);


/*****************������Ϣ�ɼ�***************************************************************************/
    Encoder_A = EA[0]+EA[1]+EA[2]+EA[3]+EA[4];   //���� ĸ���ϱ�����1��С��ǰ��Ϊ��ֵ
    Encoder_B = EB[0]+EB[1]+EB[2]+EB[3]+EB[4];   //�ҵ�� ĸ���ϱ�����2��С��ǰ��Ϊ��ֵ
    Encoder_C = encoder_get_count(ENCODER_1);   //�н���� ĸ���ϱ�����3��С��ǰ��Ϊ��ֵ

    Encoder_C_Sum += Encoder_C;                     //�н�����������ۼ�

    encoder_clear_count(ENCODER_1);
    encoder_clear_count(LEFT_FLYWHEEL_ENCODER_INDEX);
    encoder_clear_count(RIGHT_FLYWHEEL_ENCODER_INDEX);

/*************************************Ԫ���ж�*****************************************************************/

/*************************************���*********************************************************************/

/************************����ֱ���Լ�ת������*********************************************************************************************************************************/
    hand_gyro = imu660ra_gyro[1]*0.85 + hand_gyro*0.15;   //�˲�

    PWM_R = R_balance_Control(pitchAngle, &Pitch_Zero,hand_gyro);      //A B��������������
    if(++Flag_I==30)
    {

    //    PWM_Y = Y_balance_Control(error_of_CameraOrBalance, 0,imu660ra_gyro[2]/100);     //A B�������Z��ת��
        Flag_I=0;
    }

    PWMA_accel = Velocity_Control_A(Encoder_A);
   // PWMB_accel = Velocity_Control_B(Encoder_B);
    Motor_A = (short)+PWM_R;//+ PWMA_accel+PWMB_accel; //+ PWM_Y                                //���տ�����
    Motor_B = (short)-PWM_R;// + PWMB_accel+PWMA_accel;  //- PWM_Y                              //���տ�����

    PWMC_accel = Velocity_Control_C(Encoder_C);                             //C����ٶȻ�������

    C_gyro = imu660ra_gyro[0]*0.75+ C_gyro*0.25;   //�˲�
    //Motor_C = -P_balance_Control(rollAngle, Roll_Zero, C_gyro) + PWMC_accel;    //C�������ǰ�����

/*************************����******************************************************************************************************************************/
    Roll_error = rollAngle - Roll_Zero;
    Pitch_error = pitchAngle - Pitch_Zero;
//    �����ض��Ƕ�ֹͣ����
    //if(fabs(rollAngle)>15 || fabs(pitchAngle)>15){ Motor_A =0,Motor_B =0,Motor_C =0,Start_Flag=0;DIN_OFF;}

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
        MotorCtrl3W(Motor_A, Motor_B,Motor_C);         //���� ���ֶ�����
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
    error = constrain_float(error, -220, 220);                            //�����޷�
    PWM=R_Balance_KP*Bias + R_Balance_KI*error + Gyro*R_Balance_KD/10;   //��ȡ������ֵ
    PWM = constrain_float(PWM, -10000, 10000);                            //����޷�

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
    Gyro = constrain_float(Gyro, -max_Y_gyro, max_Y_gyro);                            //ȡֵ�޷�
    Bias=Angle-Angle_Zero;                                               //��ȡƫ�ƫ��ֵ��Ҫ���¼��㣬��Ȼ�������䣩
    error+=Bias;                                                         //ƫ���ۻ�
    error = constrain_float(error, -max_err_ypwm, max_err_ypwm);                            //�����޷�
    if(k_cam==0) Gyro=0;
    PWM=Y_Balance_KP*Bias + Y_Balance_KI*error + Gyro*Y_Balance_KD/10;   //��ȡ������ֵ
    Ypwm_P=Y_Balance_KP*Bias,Ypwm_D= Gyro*Y_Balance_KD/10,Ypwm_I=Y_Balance_KI*error;
    if(Start_Flag==0) PWM=0,error=0;                                     //ֹͣʱ��������
    if(carstate==0)PWM=0,error=0;


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

//    Encoder_Least = (float)encoder- (float)Move_distance;                                  //�ٶ��˲�
//    Encoder *= 0.7;                                                            //һ�׵�ͨ�˲���
//    Encoder += Encoder_Least*0.3;                                              //һ�׵�ͨ�˲���
    Encoder=(float)encoder- (float)Move_distance;
    Encoder_Integral += Encoder - Move_distance;                             //���ֳ�λ��
    Encoder_Integral = constrain_float(Encoder_Integral, -1000, 1000);        //�����޷�
    Velocity = Encoder * P_Velocity_KP + Encoder_Integral * P_Velocity_KI;   //��ȡ������ֵ
//    Roll_Zero = Roll_Zero1 - Move_distance/500;
    if(Start_Flag==0) Encoder_Integral=0,Encoder=0,Velocity=0;               //ֹͣʱ��������
    return Velocity;
}

void MotorCtrl3W(int a,int b,int c){
    if(a<0)gpio_set_level(LEFT_FLYWHEEL_DIR, FLYWHEEL_CLOCKWISE);
    else gpio_set_level(LEFT_FLYWHEEL_DIR, !FLYWHEEL_CLOCKWISE);
    if(b<0)gpio_set_level(RIGHT_FLYWHEEL_DIR, FLYWHEEL_CLOCKWISE);
    else gpio_set_level(RIGHT_FLYWHEEL_DIR,!FLYWHEEL_CLOCKWISE);
    if(c<0)gpio_set_level(XJDIR, GPIO_LOW  );
    else gpio_set_level(XJDIR,GPIO_HIGH ),c=10000-func_abs(c);

//gpio_set_level(XJDIR, GPIO_HIGH);
    pwm_set_duty(XJPWM,func_abs(c));  //c���ռ�ձ�6250~10000  10000 0v  5000��Ӧ9v
    pwm_set_duty(LEFT_FLYWHEEL_PWM,PWM_DUTY_MAX - func_abs(a) );
    pwm_set_duty(RIGHT_FLYWHEEL_PWM,  PWM_DUTY_MAX - func_abs(b));
}







