/*********************************************************************************************************************
* TC264 Opensourec Library ����TC264 ��Դ�⣩��һ�����ڹٷ� SDK �ӿڵĵ�������Դ��
* Copyright (c) 2022 SEEKFREE ��ɿƼ�
*
* ���ļ��� TC264 ��Դ���һ����
*
* TC264 ��Դ�� ��������
* �����Ը��������������ᷢ���� GPL��GNU General Public License���� GNUͨ�ù������֤��������
* �� GPL �ĵ�3�棨�� GPL3.0������ѡ��ģ��κκ����İ汾�����·�����/���޸���
*
* ����Դ��ķ�����ϣ�����ܷ������ã�����δ�������κεı�֤
* ����û�������������Ի��ʺ��ض���;�ı�֤
* ����ϸ����μ� GPL
*
* ��Ӧ�����յ�����Դ���ͬʱ�յ�һ�� GPL �ĸ���
* ���û�У������<https://www.gnu.org/licenses/>
*
* ����ע����
* ����Դ��ʹ�� GPL3.0 ��Դ���֤Э�� �����������Ϊ���İ汾
* �������Ӣ�İ��� libraries/doc �ļ����µ� GPL3_permission_statement.txt �ļ���
* ���֤������ libraries �ļ����� �����ļ����µ� LICENSE �ļ�
* ��ӭ��λʹ�ò����������� ���޸�����ʱ���뱣����ɿƼ��İ�Ȩ����������������
*
* �ļ�����          cpu1_main
* ��˾����          �ɶ���ɿƼ����޹�˾
* �汾��Ϣ          �鿴 libraries/doc �ļ����� version �ļ� �汾˵��
* ��������          ADS v1.8.0
* ����ƽ̨          TC264D
* ��������          https://seekfree.taobao.com/
*
* �޸ļ�¼
* ����              ����                ��ע
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#include "zf_common_headfile.h"
#include "ImageProcess.h"
#include "CAMERA.h"
#include "image.h"
#include "zf_device_tft180.h"
#pragma section all "cpu1_dsram"
// ���������#pragma section all restore���֮���ȫ�ֱ���������CPU1��RAM��


// ���̵��뵽���֮��Ӧ��ѡ�й���Ȼ����refreshˢ��һ��֮���ٱ���
// ����Ĭ������Ϊ�ر��Ż��������Լ��һ�����ѡ��properties->C/C++ Build->Setting
// Ȼ�����Ҳ�Ĵ������ҵ�C/C++ Compiler->Optimization->Optimization level�������Ż��ȼ�
// һ��Ĭ���½����Ĺ��̶���Ĭ�Ͽ�2���Ż�����˴��Ҳ��������Ϊ2���Ż�

// ����TCϵ��Ĭ���ǲ�֧���ж�Ƕ�׵ģ�ϣ��֧���ж�Ƕ����Ҫ���ж���ʹ�� enableInterrupts(); �������ж�Ƕ��
// �򵥵�˵ʵ���Ͻ����жϺ�TCϵ�е�Ӳ���Զ������� disableInterrupts(); ���ܾ���Ӧ�κε��жϣ������Ҫ�����Լ��ֶ����� enableInterrupts(); �������жϵ���Ӧ��
int bwthreshold=30;

// **************************** �������� ****************************

#define MAX_COL 80
#define Bin_Image Pixle
extern uint8 Pixle[LCDH][LCDW];

int OFFSET0, OFFSET1, OFFSET2, OFFSET3;
sint16 temp0_L = 0, temp0_R = 0;        //��ʱ��ֵ
sint16 temp1_L = 0, temp1_R = 0;        //��ʱ��ֵ
sint16 temp2_L = 0, temp2_R = 0;        //��ʱ��ֵ
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
    disable_Watchdog();                     // �رտ��Ź�
    interrupt_global_enable(0);             // ��ȫ���ж�
    // �˴���д�û����� ���������ʼ�������


  // tft180_set_dir(TFT180_CROSSWISE);                                           // ��Ҫ�Ⱥ��� ��Ȼ��ʾ����

    tft180_init();

    cpu_wait_event_ready();                 // �ȴ����к��ĳ�ʼ�����

    while(1)
   {
       if(mt9v03x_init())
           tft180_show_string(0, 16, "mt9v03x reinit.");
       else
           break;
       system_delay_ms(1000);                                                  // ���Ʊ�ʾ�쳣
   }
 //    �˴���д�û����� ���������ʼ�������
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
        // �˴���д��Ҫѭ��ִ�еĴ���
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



        // �˴���д��Ҫѭ��ִ�еĴ���
    }
}



/***************************************************************************
 *                                                                          *
 *  �������ƣ�Seek_Road(void)                                           *
 *  ����˵����Ѱ�Ұ�ɫ����ƫ��ֵ                                            *
 *  ����˵������                                                            *
 *  �������أ�ֵ�Ĵ�С                                                      *
 *  �޸�ʱ�䣺2017-07-16                                                    *
 *  ��    ע�����м�Ϊ0������һ���Ҳ��һ����ֵ����1�����                *
 *            ��������ӵ�һ�п�ʼ�������ڶ��н�����                        *
 *            ������Ϊ��������ֵԽ��˵��Խƫ��ߣ�                        *
 *            ������Ϊ��������ֵԽ��˵��Խƫ�ұߡ�                        *
 ***************************************************************************/
void Seek_Road (void)
{
    sint16 nr; //��
    sint16 nc; //��
    sint16 temp = 0; //��ʱ��ֵ
    sint16 temp_L = 0, temp_R = 0; //��ʱ��ֵ
    sint16 temp2 = 0; //��ʱ��ֵ
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
