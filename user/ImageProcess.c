
#include "ImageProcess.h"
#include "CAMERA.h"
#include "zf_common_headfile.h"

extern float error_Camera;

uint8 ImageSide[LCDH][2];     //���ұ������� 0�� 1��
uint8 UpSide[LCDW];           //�ϱ�������
uint8 num_left_lost = 0;      //��ඪ�߸���
uint8 num_right_lost = 0;     //�Ҳඪ�߸���
uint8 num_all_lost = 0;       //����ͬ�ж��߸���
uint8 num_up_turn = 0;        //�ϱ���ת�۵����
uint8 num_left_turn = 0;      //�����ת�۵����
uint8 num_right_turn = 0;     //�ұ���ת�۵����
uint8 num_black_white = 0;    //�ڰ�������

uint8 count = 1;              //�����ߴ���
/**  @brief    ת�����  */
sint16 g_sSteeringError = 0;
/**  @brief    ���߱�־λ  */
uint8 g_ucIsNoSide = 0;

/**  @brief    ������  */
uint8 ROAD_MAIN_ROW     = 35;

/**  @brief    ʹ����ʼ��  */
uint8 ROAD_START_ROW   =  59;

/**  @brief    ʹ�ý�����  */
uint8 ROAD_END_ROW   =  5;


/**  @brief    ������־λ  */
FlagRoundabout g_ucFlagRoundabout = NO;

/**  @brief    ʮ�ֱ�־λ  */
uint8 g_ucFlagCross  = 0;

/**  @brief    �����߱�־λ  */
uint8 g_ucFlagZebra  = 0;

/**  @brief    T�Ͳ�ڱ�־λ  */
uint8 g_ucFlagT = 0;

game_status game = no_start;    //����״̬






/*************************************************************************
 *  �������ƣ�void ImageProcess(void)
 *  ����˵����ͼ�����ݴ���
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2022��12��9��
 *  ��    ע��CCU61_CH0�ж�ʹ�õķ�����
 *************************************************************************/
void ImageProcess (void)
{

   // LQ_DMP_Read();
//    uint8 i;
    //����������ǰ10��20-30���е����ʣ��Դ����ı�����P 0Ϊ�м� -70< drv_err/10 <70
//    for(i = 20; i <30; i++)
//        drv_err += (ImageSide[i][0] + ImageSide[i][1] - LCDW);  //20-29������ۼ�ֵ
//    if(abs(drv_err/10)<15)      //���ݼ���ֵ���µ����������ϵ��   (�˴�һ�㲻���޸�)
//        Servo_P = 13;
//    else if(abs(drv_err/10)>15)
//        Servo_P = 19;

////////////////////////////////////////////////////////���� �����ⲻִ�к����Ԫ���ж� �����뿪�ز����Ϸ�Ϊ��ת���� �����·�Ϊ��ת���⣩//////////////////////////////////////////////////////////////////////////////////////////////////////
//    if(game == no_start)    //���⣨����ʱ �ٶȡ��Ƕȡ��������ۼ�ֵ ������Ҫ����ã�
//    {
//        if(KEY_Read(DSW0) == 1)     //���뿪�� �л����ⷽ�򣨲��뿪����ONʱ����ת���⣩
//            ServoDuty = 70;         //����ת��Ƕȣ�һ�㲻���޸ģ�
//        else
//            ServoDuty = -70;
//        Target_Speed = 2500;        //�����ٶ�  ��һ�㲻���޸ģ�
//
//        if(RAllPulse > 2500)        //�н�·�̴���2500�����ѭ���׶�   ��ֵ�����޸�
//            game = start;
//
//        if(ServoDuty>80) ServoDuty=80;                     //ƫ���޷�
//        if(ServoDuty<-80) ServoDuty=-80;                   //ƫ���޷�
//        ServoCtrl(Servo_Center_Mid-ServoDuty);             //������
//    }

// Ԫ�ش������Ӵ˴���ʼ �ȴӻ�����ʼ T�� ��·�� ����������һ������һ��������׷
///////////////////////////////////////////////////��ʼѭ��////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(game == start)
    {
        /**************************************������***********************************************************************************************************************/
         if(g_ucFlagRoundabout == 0 && g_ucFlagCross == 0 && g_ucFlagZebra == 0)
         {
             //��������
//             RoadIsZebra(ImageSide, &g_ucFlagZebra);
         }
         if(g_ucFlagZebra)
         {
             //�����ߴ�����һ��������Ҳ��ߣ�����ⷽ��������߻����Ҳ��ߣ��ڶ�������⣩
//             ZebraProcess(ImageSide, &g_ucFlagZebra);
         }

        /********************************����**********************************************************************************************************************************/
         if(g_ucFlagRoundabout == NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
         {
             // ��⻷��
//             RoadIsRoundabout(UpdowmSide, Bin_Image, ImageSide);
         }
        if(g_ucFlagRoundabout != NO)
         {
            //����������Ϊ�󻷵����һ��� ���T�Ϳ� ��������T�Ϳ�һ��ʼʮ��������һ��ʼ�Ĵ���ʽҲһֱ�����Է���һ���⣬�ȹ���һ�׶κ����ж��ǻ�������T�֣�
//            RoundaboutProcess(Bin_Image, ImageSide, UpdowmSide);
         }

        /********************************T��·��***************************************************************************************************************************/

        /*************����ע��******************************/
        /******T��·�ڲ��������⣬�ŵ����������м�⣬ ��ΪT��·��һ��ʼ�뻷�������ͬ�Ҵ���ʽҲ��ͬ�����ǲ��߲����������ת�������治ͬ��������ͻ����Ϊ������������ΪT�οڣ�***/
//        if(g_ucFlagRoundabout == NO && g_ucFlagFork == Y_NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
//        {
//            //���T��
//            RoadIsT(UpdowmSide, ImageSide, &g_ucFlagT);
//            if(g_ucFlagT)
//                RAllPulse = 0;
//        }
        /**************����ע��*************************************/

        if(g_ucFlagT)
        {
            //T�ִ�����Ϊ��T�ֺ���T�֣���T�֣�����T��һֱ����ת ��T�֣�����T��һֱ����ת��
//            TProcess(Bin_Image, UpdowmSide, ImageSide, &g_ucFlagT);
        }

         /*********************************Y��·��************************************************************************************************************************/
         if(g_ucFlagRoundabout == NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
           {
//               RoadIsFork(UpdowmSide, ImageSide);
           }

         /* ���������У���ȡ���ƫ�� */
         g_sSteeringError = ImageSide[ROAD_MAIN_ROW][0] + ImageSide[ROAD_MAIN_ROW][1] - LCDW;

    }

////////////////////////////////////////////////////////////������/////////////////////////////////////////////////////////////////////////////////////////////
//    if(game == over)    //��⣨ͬ����һ����
//    {
//        if(RAllPulse < 2500)            //�г�С��2500ʱ��⣨����2500��ʾ������ �˴������޸ģ���ʵ�����Ϊ׼��
//        {
//            if(KEY_Read(DSW0) == 1)     //��������ת����ͬ���벦�뿪���йأ���ת��������ת��⣬��ת��������ת��⣩
//                ServoDuty = -70;
//            else                        //��������ת����ͬ
//                ServoDuty = 70;
//            Target_Speed = 2000;        //����ٶ�ֵ�����޸ģ�
//        }
//        else                            //�г̴���2500ʱ��ʾ��������ͣ��
//        {
//            Target_Speed = 0;
//            ServoDuty = 0;
//        }
//        if(ServoDuty>80) ServoDuty=80;                     //ƫ���޷�
//        if(ServoDuty<-80) ServoDuty=-80;                   //ƫ���޷�
//        ServoCtrl(Servo_Center_Mid-ServoDuty);             //������
//    }
}





float calc_radius(float x1, float y1, float x2, float y2, float x3, float y3) {
    float X,Y, a, b, c, g, e, f;
       e = 2 * (x2 - x1);
       f = 2 * (y2 - y1);
       g = x2*x2 - x1*x1 + y2*y2 - y1*y1;
       a = 2 * (x3 - x2);
       b = 2 * (y3 - y2);
       c = x3*x3 - x2*x2 + y3*y3 - y2*y2;
       X = (g*b - c*f) / (e*b - a*f);
       Y = (a*g - c*e) / (a*f - b*e);
      float r= sqrt((X-x1)*(X-x1)+(Y-y1)*(Y-y1));
      if(r>70)r=0;
      return r;
}

/*************************************************************************
 *  �������ƣ�void TFTSPI_BinRoadSide(uint8)
 *  ����˵����
 *  ����˵����imageOut����������
 *  �������أ���
 *  �޸�ʱ�䣺2022��3��30��
 *  ��    ע��CCU61_CH0ʹ�õ��жϷ�����
 *************************************************************************/
float cammida=0,cammidb=0,cammidc=0;
float turn_r=0;
float turn_xie=0,lasterror_Camera=0;
extern float k_xie;

void TFTSPI_BinRoadSide(uint8 imageOut[LCDH][2])
{
    uint8 i = 0;

    for(i = 0; i < ROAD_START_ROW; i++)
    {

        tft180_draw_point(imageOut[i][0]/2, i, RGB565_YELLOW);

        tft180_draw_point(imageOut[i][1]/2, i, RGB565_YELLOW);

        if(i==ROAD_START_ROW-2) {

            cammida=LCDW/2 - (imageOut[i][1]+imageOut[i][0])/2-k_xie;

            error_Camera = 3*cammida+2*cammidb+cammidc;
            if(abs(error_Camera-lasterror_Camera)>250){
                error_Camera=lasterror_Camera;
                return;
            }
//
           // error_Camera= constrain_float(error_Camera, -120.0, 120.0);

            lasterror_Camera=error_Camera;
        //    turn_r =calc_radius (cammida,i,cammidb,ROAD_START_ROW-20,cammidc,ROAD_START_ROW-42);

       //     turn_xie = (cammida-cammidb)/18;

            tft180_draw_point((imageOut[i][1]+imageOut[i][0])>>2, i, RGB565_YELLOW);
            continue;


        }else if(i==ROAD_START_ROW-14){

            cammidb= LCDW/2 -(imageOut[i][1]+imageOut[i][0])/2-k_xie;
            tft180_draw_point((imageOut[i][1]+imageOut[i][0])>>2, i, RGB565_YELLOW);
            continue;

        }else if(i==ROAD_START_ROW-32){


            cammidc=LCDW/2 - (imageOut[i][1]+imageOut[i][0])/2-k_xie;
            tft180_draw_point((imageOut[i][1]+imageOut[i][0])>>2, i, RGB565_YELLOW);
            continue;
        }else{


            tft180_draw_point((imageOut[i][1]+imageOut[i][0])>>2, i, RGB565_BLUE);
        }





    }
}
/*************************************************************************
 *  �������ƣ�void TFTSPI_BinRoad_UpdownSide(uint8)
 *  ����˵����
 *  ����˵����imageOut :��������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע��CCU61_CH0ʹ�õ��жϷ�����
 *************************************************************************/
void TFTSPI_BinRoad_UpdownSide(uint8 imageOut[LCDW])
{
    uint8 i = 0;

    for(i = 0; i < LCDW; i++)
    {
        tft180_draw_point(i, imageOut[i], RGB565_BLUE);

    }
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  �������ƣ�ImageGetSide
*  ����˵������ȡ���ұ���
*  ����˵��:            imageInput  ����Եͼ������
*                     imageOut    �����ұ�������
*  �������أ�0
*  �޸�ʱ�䣺2022/6/23 ������
*  ��   ע����Ҫ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#define MAX_LOST_COUNT 10



uint8 ImageGetSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2])
{

    num_left_lost = 0;      //��ඪ�߸�������
    num_right_lost = 0;     //�Ҳඪ�߸�������
    num_all_lost = 0;       //���Ҷ���������

    //�ж� ROAD_START_ROW�� �ǲ��Ƕ���
  //  RoadIsNoSide(imageInput, imageOut, ROAD_START_ROW);


    //
    uint8_t i, j;
       for (j = LCDW/2-1; j >=0; j--) {
           if (imageInput[ROAD_START_ROW][j]==black_pixel) {
               imageOut[ROAD_START_ROW][0] = j;
               break;

           }
       }

       for (j = LCDW/2; j < LCDW; j++) {
           if (imageInput[ROAD_START_ROW][j]==black_pixel) {
               imageOut[ROAD_START_ROW][1] = j;
               break;
           }
       }

    // �복ͷ������ Ѱ�ұ��� 59-5
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {


            //   ���ݱ߽��������� Ѱ�ұ߽�
            for(j = (imageOut[i+1][0]+ imageOut[i+1][1])>>1; j > 0; j--)
            {
                if(j >= LCDW-1)   j=LCDW-1;
                if(!imageInput[i][j])   //��ɫ��0 Ϊ����
                {
                    imageOut[i][0] = j;
                    break;
                }
                else
                    imageOut[i][0] = 0;
            }
            if(imageOut[i][0] == 0)
                num_left_lost++;



            for(j = (imageOut[i+1][0]+ imageOut[i+1][1])>>1; j < LCDW-1; j++)
            {
                if(j<0)j=0;
                if(!imageInput[i][j])
                {
                    imageOut[i][1] = j;
                    break;
                }else
                    imageOut[i][1] = LCDW-1;




            if(imageOut[i][1] == LCDW-1){
                num_right_lost++;
                if(imageOut[i][0] == 0)
                    num_all_lost++;
            }
        }

    }


//
//        // �����߽� ������������ �����Ƿ��Ҷ���
//        if(imageOut[i][0] > (LCDW/2 - 5) && imageOut[i][1] >  (LCDW - 5))
//        {
//            //      �Ҷ��ߴ���
//            imageOut[i][1]=LCDW-1;
//
//            RoadNoSideProcess(imageInput, imageOut, 2, i);
//            return 1;
//        }
//
//       //  ����ұ߽� ������������ �����Ƿ�����
//        if(imageOut[i][1] < (LCDW/2 + 5) && imageOut[i][0] < 5)
//        {
//            //  ���ߴ���
//         //   RoadNoSideProcess(imageInput, imageOut, 1, i);
//        //    return 2;
//            imageOut[i][0]=0;
//
//        }
//    }


   return 0;


}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  �������ƣ�UpdownSideGet
*  ����˵������ȡ�ϱ���
*  ����˵��:            imageInput  ����Եͼ������
*                     imageOut    �����±�������
*  �������أ�0
*  �޸�ʱ�䣺2022/6/23 ������
*  ��   ע����Ҫ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDW])
{
    uint8 i = 0, j = 0;
    uint8 last = LCDH/2;
    uint8 Haf_W = LCDW/2;
    uint8 cnt = LCDH;

    memset(imageOut, 0, sizeof(imageOut));
    imageOut[Haf_W] = 0;
    /* �����߱ȽϽ����� �ж�ͼ���м��ǲ������ϱ��� */
    for(i = last; i >= 0; i--)
    {
        if(imageInput[i][Haf_W])
        {
            imageOut[Haf_W] = i;
            break;
        }
    }

    /* �������� Ѱ�ұ��� */
    for(i = Haf_W-1; i > 0; i--)
    {
        imageOut[i] = 0;
        cnt = imageOut[i+1] + 5;
        if(cnt >= LCDH-1) cnt = LCDH-1;
        /* ���ݱ߽��������� Ѱ�ұ߽� */
        for(j = cnt; j > 0; j--)
        {
            if(!imageInput[j][i])
            {
                imageOut[i] = j;
                break;
            }
        }
    }
    /*�������� Ѱ�ұ���*/
    for(i = Haf_W+1; i < LCDW-1; i++)
        {
            imageOut[i] = 0;
            cnt = imageOut[i+1] + 5;
            if(cnt >= LCDH-1) cnt = LCDH-1;

            /* ���ݱ߽��������� Ѱ�ұ߽� */
            for(j = cnt; j > 0; j--)
            {
                if(!imageInput[j][i])
                {
                    imageOut[i] = j;
                    break;
                }
            }
        }
    return 0;
}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  �������ƣ�RoadIsNoSide
*  ����˵�����ж������ҷ���
*  ����˵����    imageInput  ��������ͼ������
*             imageOut    ����������[0]:��  [1]����
*             lineIndex   ��ָ����
*  �������أ�0��û�ж���   1:��߶���  2���ұ߶���  3�� ���Ҷ�����   4������
*  �޸�ʱ�䣺2022��6��24��
*  ��   ע��guo
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 RoadIsNoSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 lineIndex)
{
    uint8 state = 0;
    uint8 i = 0;
    static uint8 last = LCDW/2;   //ͼ���м�λ��

    imageOut[lineIndex][0] = 0;     //���ұ���λ��
    imageOut[lineIndex][1] = LCDW-1;  //
    /* ͼ�����ʼ�е���߲��� �ж��Ƿ��� */
    for(i = last; i > 1; i--)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][0] = i;
            break;
        }
    }
    if(i == 1)      //����ѭ��ִ�е����Ҳû���˳����ж�Ϊ����
        state = 1;  //��߽綪��

    for(i = last; i < 93; i++)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][1] = i;
            break;
        }
    }
    if(i == 93)//����ѭ��ִ�е����Ҳû���˳����ж�Ϊ����
    {
        /* ���ұ߽綪�� */
        if(state == 1)
            state = 3;
        /* �ұ߽綪�� */
        else
            state = 2;
    }
    if(state == 0 && imageOut[lineIndex][1] <= imageOut[lineIndex][0])    //���û�ж��ߵ��� ��ߵı��� ���� �ұߵı���
        state = 4;
    return state;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  �������ƣ�RoadNoSideProcess
*  ����˵�����Ҷ��߶��ߴ���
*  ����˵����    imageInput  ��������ͼ������
*             imageOut    ����������[0]:��  [1]����
*             mode        : �������
*             lineIndex   ��ָ����
*  �������أ���
*  �޸�ʱ�䣺2022��6��24��
*  ��   ע��guo
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
void RoadNoSideProcess(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 mode, uint8 lineIndex)
{
    uint8 i = 0, j = 0, count = 0;

    switch(mode)
    {
      case 1:
        for(i = imageOut[lineIndex][1]; i > 1; i--)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = 0;
                    imageOut[lineIndex - count][1] = i;
                    break;
                }
            }
        }
        break;

      case 2:
        for(i = imageOut[lineIndex][0]; i < 93; i++)
        {
            count++;
            for(j = lineIndex; j > ROAD_END_ROW && lineIndex > count; j--)
            {
                if(imageInput[j][i])
                {
                    imageOut[lineIndex - count][0] = i;
                    imageOut[lineIndex - count][1] = 93;
                    break;
                }

            }
        }
        break;
    }
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  �������ƣ�TurnNumSideGet
*  ����˵������ȡ���ߵ�ת�۵����
*  ����˵��:��
*  �������أ�0
*  �޸�ʱ�䣺2022/6/23 ������
*  ��   ע����Ҫ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 TurnNumSideGet(void)
{
    char i;
    char err_front = 0, err_back = 0;

    num_up_turn=0;
    num_left_turn=0;
    num_right_turn=0;

    //�����ϱ����ж��ٸ��۵� 10-80
    for(i = 10; i < 80; i++)
    {
        err_front= UpSide[i-1]-UpSide[i];

        while(UpSide[i] == UpSide[i+1])
        {
            i++;
        }
        err_back= UpSide[i]-UpSide[i+1];


        if(err_front * err_back < 0)
            num_up_turn++;

    }
    //����������ж��ٸ��۵� ��55-20
    for(i = 50; i > 15; i-=2)
    {
        err_front= ImageSide[i+1][0]-ImageSide[i][0];

        while(ImageSide[i][0] == ImageSide[i-1][0])
        {
            i--;
        }
        err_back= ImageSide[i][0]-ImageSide[i-1][0];


        if(err_front * err_back < 0){
            num_left_turn++;
        }


    }
    //�����ұ����ж��ٸ��۵� ��55-20
    for(i = 50; i > 15; i-=2)
    {
        err_front= ImageSide[i+1][1]-ImageSide[i][1];

        while(ImageSide[i][1] == ImageSide[i-1][1])
        {
            i--;
        }
        err_back= ImageSide[i][1]-ImageSide[i-1][1];

        if(err_front * err_back < 0)
            num_right_turn++;
    }
    return 0;
}
