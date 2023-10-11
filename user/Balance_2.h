/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年10月28日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】AURIX Development Studio1.2.2及以上版本
【Target 】 TC264DA/TC264D
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________
基于iLLD_1_0_1_11_0底层程序,

使用例程的时候，建议采用没有空格的英文路径，
除了CIF为TC264DA独有外，其它的代码兼容TC264D
本库默认初始化了EMEM：512K，如果用户使用TC264D，注释掉EMEM_InitConfig()初始化函数。
工程下\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c第164行左右。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_BALANCE_H_
#define _LQ_BALANCE_H_



typedef enum {
    idle = 0,       //0:空闲-原地直立
    garage_out,     //1:出库
    run,            //2:正常循迹
    garage_in       //3:入库
}Status_car;

typedef enum {
    nothing=0,      //0:平常路段
    straight,       //1:直道
    round_L,        //2:左环岛
    round_L_Out,      //3:出环岛
    round_R,        //4:右环岛
    cross,          //5:十字
    zebra           //6:斑马线
}Element;
extern Element element;                         //赛道元素状态
extern Status_car status_car;                   //小车的状态：出库 循迹 入库

extern float error_Camera, error_Blance;       //摄像头和电磁误差

void Balance_car(void);
void Key(void);
extern volatile char mutexCpu0TFTIsOk;         // CPU1 0占用/1释放 TFT
extern int Target_Speed;
void Balance(void);
float yijielvbo_P(float angle_m,float gyro_m);
/**************************************************************************
Y轴平衡PID控制,角度环
**************************************************************************/
float X_balance_Control(float Angle,float Angle_Zero,float Gyro);
/**************************************************************************
速度PI控制,速度正反馈环
**************************************************************************/
float Velocity_Control_A(int encoder);
#endif /* SRC_APPSW_TRICORE_MAIN_LQ_SMARTCAR_H_ */


