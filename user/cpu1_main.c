/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          cpu1_main
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.8.0
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "ImageProcess.h"
#include "CAMERA.h"
#include "image.h"
#include "zf_device_tft180.h"
#pragma section all "cpu1_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU1的RAM中


// 工程导入到软件之后，应该选中工程然后点击refresh刷新一下之后再编译
// 工程默认设置为关闭优化，可以自己右击工程选择properties->C/C++ Build->Setting
// 然后在右侧的窗口中找到C/C++ Compiler->Optimization->Optimization level处设置优化等级
// 一般默认新建立的工程都会默认开2级优化，因此大家也可以设置为2级优化

// 对于TC系列默认是不支持中断嵌套的，希望支持中断嵌套需要在中断内使用 enableInterrupts(); 来开启中断嵌套
// 简单点说实际上进入中断后TC系列的硬件自动调用了 disableInterrupts(); 来拒绝响应任何的中断，因此需要我们自己手动调用 enableInterrupts(); 来开启中断的响应。
int bwthreshold=30;

// **************************** 代码区域 ****************************

#define MAX_COL 80
#define Bin_Image Pixle
extern uint8 Pixle[LCDH][LCDW];

int OFFSET0, OFFSET1, OFFSET2, OFFSET3;
sint16 temp0_L = 0, temp0_R = 0;        //临时数值
sint16 temp1_L = 0, temp1_R = 0;        //临时数值
sint16 temp2_L = 0, temp2_R = 0;        //临时数值
extern float error_Camera;
extern float pitchAngle;
extern float rollAngle;
extern int Start_Flag;
//extern float error_Camera;
void Seek_Road (void);
extern float PWM_Y,yawAngle,yaw_acc,ACC_Y;
extern float Voltage;
extern float turn_r;
extern float turn_xie;
extern float pitch_zero_py,Roll_Zero;
extern int Encoder_A,Encoder_B;
void core1_main(void)
{
    disable_Watchdog();                     // 关闭看门狗
    interrupt_global_enable(0);             // 打开全局中断
    // 此处编写用户代码 例如外设初始化代码等


  // tft180_set_dir(TFT180_CROSSWISE);                                           // 需要先横屏 不然显示不下

    tft180_init();

    cpu_wait_event_ready();                 // 等待所有核心初始化完毕

    while(1)
   {
       if(mt9v03x_init())
           tft180_show_string(0, 16, "mt9v03x reinit.");
       else
           break;
       system_delay_ms(1000);                                                  // 闪灯表示异常
   }
 //    此处编写用户代码 例如外设初始化代码等
    tft180_show_string(0, 16, "mt9v03x1111.");
    int t1=0;


    extern float mj_PY;
//    tft180_show_string(0,85 ,"r");
//    tft180_show_string(0, 70, "l");
//
//    tft180_show_string(0, 100, "u");
//    tft180_show_string(0, 115, "c" );
//    tft180_show_string(0, 130, "r");
 float ppp=0;
 int waya=0;
 int t2=0;
 extern float Pitch_Zero;
 extern int debugmode;
// wireless_uart_init();
 //uart_init(UART_2, 115200, UART2_TX_P10_5, UART2_RX_P10_6);
// gogogo(-310000,0);
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
        t1++,t2++;


        if(mt9v03x_finish_flag)
       {
            mt9v03x_finish_flag = 0;

//            tft180_show_gray_image(0, 0, mt9v03x_image, 120, 180, 128, 160, 0);

            Get_Use_Image();
            Get_01_Value(3);
            Pixle_Filter();
            if(debugmode)
            tft180_show_binary_image(0, 0, Pixle, LCDW, LCDH, LCDW, LCDH);

//            uart_write_byte_wait(UART_2, 0xFC);
//
//            uart_write_byte_wait(UART_2, 0xCF);
//            for(int i=LCDH-1;i>=0;i--){
//                for(int j=LCDW-1;j>=0;j--){
//                    uart_write_byte_wait(UART_2, Pixle[i][j]);
//                                }
//            }
//            uart_write_byte_wait(UART_2, 0xCF);
//            uart_write_byte_wait(UART_2,0xFC);


            MyImageProcess();
           if(t2>30){
               t2=0;

//                   tft180_show_int(0,70, num_left_turn, 4);
//                 tft180_show_int(60,70 , num_right_turn, 4);

               tft180_show_float(80, 60, Roll_Zero,2, 1);
               tft180_show_float(40, 60, Pitch_Zero,2, 1);

               tft180_show_float(0, 75, pitchAngle,4, 2);
//
               tft180_show_float(60, 75, rollAngle,4, 2);
//  tft180_show_float(0, 60, camerr,4,2);

               //       tft180_show_int(0, 90,left_turn_up[1], 3);
               //       tft180_show_int(60,90, left_turn_up[0], 3);


//               tft180_show_int(10, 100, num_up_turn, 4);
//               tft180_show_float(0,90 , Voltage, 2,1);
               ppp+=0.01;
//
           //    tft180_show_float(80, 105, ppp,2, 2);



           }






       //    Seek_Road();



     ///      error_Camera = (float)(OFFSET0*15/10 + OFFSET1*9/10 + OFFSET2*6/10)+mj_PY;


         // tft180_show_int(0, 0, (int)error_Camera, 5);
       }



        // 此处编写需要循环执行的代码
    }
}



/***************************************************************************
 *                                                                          *
 *  函数名称：Seek_Road(void)                                           *
 *  功能说明：寻找白色区域偏差值                                            *
 *  参数说明：无                                                            *
 *  函数返回：值的大小                                                      *
 *  修改时间：2017-07-16                                                    *
 *  备    注：以中间为0，左侧减一，右侧加一，数值代表1的面积                *
 *            计算区域从第一行开始到倒数第二行结束。                        *
 *            如果面积为负数，数值越大说明越偏左边；                        *
 *            如果面积为正数，数值越大说明越偏右边。                        *
 ***************************************************************************/
void Seek_Road (void)
{
    sint16 nr; //行
    sint16 nc; //列
    sint16 temp = 0; //临时数值
    sint16 temp_L = 0, temp_R = 0; //临时数值
    sint16 temp2 = 0; //临时数值
    //for(nr=1; nr<MAX_ROW-1; nr++)
    temp = 0;
/*****************************8-24**********************************/
    for (nr = 52; nr < 60; nr++)
    {
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_L;
            }else{
                --temp_L;
            }
        }
        for (nc = MAX_COL / 2; nc < MAX_COL; nc++)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_R;
            }else{
                --temp_R;
            }
        }
    }
    temp0_L = temp_L;
    temp0_R = temp_R;
    temp_L = 0;
    temp_R = 0;
    OFFSET0 = temp0_R - temp0_L;

    for (nr = 44; nr < 52; nr++)
    {
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_L;
            }else{
                --temp_L;
            }
        }
        for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_R;
            }else{
                --temp_R;
            }
        }
    }
    temp1_L = temp_L;
    temp1_R = temp_R;
    temp_L = 0;
    temp_R = 0;
    OFFSET1 = temp1_R - temp1_L;

    for (nr = 36; nr < 44; nr++)
    {
        for (nc = 0; nc < MAX_COL / 2; nc = nc + 1)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_L;
            }else{
                --temp_L;
            }
        }
        for (nc = MAX_COL / 2; nc < MAX_COL; nc = nc + 1)
        {
            if (Bin_Image[nr][nc]==1)
            {
                ++temp_R;
            }else{
                --temp_R;
            }
        }
    }
    temp2_L = temp_L;
    temp2_R = temp_R;
    temp_L = 0;
    temp_R = 0;
    OFFSET2 = temp2_R - temp2_L;

}




#pragma section all restore
