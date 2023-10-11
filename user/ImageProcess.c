
#include "ImageProcess.h"
#include "CAMERA.h"
#include "zf_common_headfile.h"

extern float error_Camera;

uint8 ImageSide[LCDH][2];     //左右边线数组 0左 1右
uint8 UpSide[LCDW];           //上边线数组
uint8 num_left_lost = 0;      //左侧丢线个数
uint8 num_right_lost = 0;     //右侧丢线个数
uint8 num_all_lost = 0;       //左右同行丢线个数
uint8 num_up_turn = 0;        //上边线转折点个数
uint8 num_left_turn = 0;      //左边线转折点个数
uint8 num_right_turn = 0;     //右边线转折点个数
uint8 num_black_white = 0;    //黑白相间个数

uint8 count = 1;              //斑马线次数
/**  @brief    转向误差  */
sint16 g_sSteeringError = 0;
/**  @brief    丢线标志位  */
uint8 g_ucIsNoSide = 0;

/**  @brief    主跑行  */
uint8 ROAD_MAIN_ROW     = 35;

/**  @brief    使用起始行  */
uint8 ROAD_START_ROW   =  59;

/**  @brief    使用结束行  */
uint8 ROAD_END_ROW   =  5;


/**  @brief    环岛标志位  */
FlagRoundabout g_ucFlagRoundabout = NO;

/**  @brief    十字标志位  */
uint8 g_ucFlagCross  = 0;

/**  @brief    斑马线标志位  */
uint8 g_ucFlagZebra  = 0;

/**  @brief    T型岔口标志位  */
uint8 g_ucFlagT = 0;

game_status game = no_start;    //比赛状态






/*************************************************************************
 *  函数名称：void ImageProcess(void)
 *  功能说明：图像数据处理
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2022年12月9日
 *  备    注：CCU61_CH0中断使用的服务函数
 *************************************************************************/
void ImageProcess (void)
{

   // LQ_DMP_Read();
//    uint8 i;
    //计算领跑行前10（20-30）行的曲率，以此来改变舵机的P 0为中间 -70< drv_err/10 <70
//    for(i = 20; i <30; i++)
//        drv_err += (ImageSide[i][0] + ImageSide[i][1] - LCDW);  //20-29行误差累计值
//    if(abs(drv_err/10)<15)      //根据计算值重新调整舵机比例系数   (此处一般不做修改)
//        Servo_P = 13;
//    else if(abs(drv_err/10)>15)
//        Servo_P = 19;

////////////////////////////////////////////////////////出库 不出库不执行后面的元素判定 （拨码开关波到上方为左转出库 拨到下方为右转出库）//////////////////////////////////////////////////////////////////////////////////////////////////////
//    if(game == no_start)    //出库（出库时 速度、角度、编码器累计值 三者需要搭配好）
//    {
//        if(KEY_Read(DSW0) == 1)     //拨码开关 切换出库方向（拨码开关在ON时向左转出库）
//            ServoDuty = 70;         //出库转弯角度（一般不做修改）
//        else
//            ServoDuty = -70;
//        Target_Speed = 2500;        //出库速度  （一般不做修改）
//
//        if(RAllPulse > 2500)        //行进路程大于2500后进入循迹阶段   此值可以修改
//            game = start;
//
//        if(ServoDuty>80) ServoDuty=80;                     //偏差限幅
//        if(ServoDuty<-80) ServoDuty=-80;                   //偏差限幅
//        ServoCtrl(Servo_Center_Mid-ServoDuty);             //舵机打角
//    }

// 元素处理程序从此处开始 先从环岛开始 T字 岔路口 斑马线依次一个函数一个函数的追
///////////////////////////////////////////////////开始循迹////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(game == start)
    {
        /**************************************斑马线***********************************************************************************************************************/
         if(g_ucFlagRoundabout == 0 && g_ucFlagCross == 0 && g_ucFlagZebra == 0)
         {
             //检测斑马线
//             RoadIsZebra(ImageSide, &g_ucFlagZebra);
         }
         if(g_ucFlagZebra)
         {
             //斑马线处理（第一次左或者右补线，与出库方向决定左补线还是右补线，第二次则入库）
//             ZebraProcess(ImageSide, &g_ucFlagZebra);
         }

        /********************************环岛**********************************************************************************************************************************/
         if(g_ucFlagRoundabout == NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
         {
             // 检测环岛
//             RoadIsRoundabout(UpdowmSide, Bin_Image, ImageSide);
         }
        if(g_ucFlagRoundabout != NO)
         {
            //环岛处理，分为左环岛和右环岛 检测T型口 （环岛与T型口一开始十分相像且一开始的处理方式也一直，所以放在一起检测，度过第一阶段后再判断是环岛还是T字）
//            RoundaboutProcess(Bin_Image, ImageSide, UpdowmSide);
         }

        /********************************T形路口***************************************************************************************************************************/

        /*************以下注释******************************/
        /******T形路口不在这个检测，放到环岛处理中检测， 因为T形路口一开始与环岛检测相同且处理方式也相同（都是补线不往左或者右转），后面不同（上线有突起则为环岛，单调则为T形口）***/
//        if(g_ucFlagRoundabout == NO && g_ucFlagFork == Y_NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
//        {
//            //检查T字
//            RoadIsT(UpdowmSide, ImageSide, &g_ucFlagT);
//            if(g_ucFlagT)
//                RAllPulse = 0;
//        }
        /**************以上注释*************************************/

        if(g_ucFlagT)
        {
            //T字处理，分为左T字和右T字（左T字：整个T字一直在左转 右T字：整个T字一直在右转）
//            TProcess(Bin_Image, UpdowmSide, ImageSide, &g_ucFlagT);
        }

         /*********************************Y形路口************************************************************************************************************************/
         if(g_ucFlagRoundabout == NO && g_ucFlagZebra == 0 && g_ucFlagT == 0)
           {
//               RoadIsFork(UpdowmSide, ImageSide);
           }

         /* 根据主跑行，求取舵机偏差 */
         g_sSteeringError = ImageSide[ROAD_MAIN_ROW][0] + ImageSide[ROAD_MAIN_ROW][1] - LCDW;

    }

////////////////////////////////////////////////////////////入库结束/////////////////////////////////////////////////////////////////////////////////////////////
//    if(game == over)    //入库（同出库一样）
//    {
//        if(RAllPulse < 2500)            //行程小于2500时入库（大于2500表示入库完毕 此处可以修改，以实际情况为准）
//        {
//            if(KEY_Read(DSW0) == 1)     //入库与出库转向相同都与拨码开关有关（左转出库则左转入库，右转出库则右转入库）
//                ServoDuty = -70;
//            else                        //入库与出库转向相同
//                ServoDuty = 70;
//            Target_Speed = 2000;        //入库速度值（可修改）
//        }
//        else                            //行程大于2500时表示入库完毕则停车
//        {
//            Target_Speed = 0;
//            ServoDuty = 0;
//        }
//        if(ServoDuty>80) ServoDuty=80;                     //偏差限幅
//        if(ServoDuty<-80) ServoDuty=-80;                   //偏差限幅
//        ServoCtrl(Servo_Center_Mid-ServoDuty);             //舵机打角
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
 *  函数名称：void TFTSPI_BinRoadSide(uint8)
 *  功能说明：
 *  参数说明：imageOut：边线数组
 *  函数返回：无
 *  修改时间：2022年3月30日
 *  备    注：CCU61_CH0使用的中断服务函数
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
 *  函数名称：void TFTSPI_BinRoad_UpdownSide(uint8)
 *  功能说明：
 *  参数说明：imageOut :边线数组
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：CCU61_CH0使用的中断服务函数
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
*  函数名称：ImageGetSide
*  功能说明：获取左右边线
*  参数说明:            imageInput  ：边缘图像数组
*                     imageOut    ：左右边线数组
*  函数返回：0
*  修改时间：2022/6/23 星期四
*  备   注：不要改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#define MAX_LOST_COUNT 10



uint8 ImageGetSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2])
{

    num_left_lost = 0;      //左侧丢线个数清零
    num_right_lost = 0;     //右侧丢线个数清零
    num_all_lost = 0;       //左右都丢线清零

    //判断 ROAD_START_ROW行 是不是丢线
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

    // 离车头近的行 寻找边线 59-5
    for(i = ROAD_START_ROW-1; i > ROAD_END_ROW; i--)
    {


            //   根据边界连续特性 寻找边界
            for(j = (imageOut[i+1][0]+ imageOut[i+1][1])>>1; j > 0; j--)
            {
                if(j >= LCDW-1)   j=LCDW-1;
                if(!imageInput[i][j])   //黑色是0 为边线
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
//        // 如果左边界 即将超出中线 则检查是否右丢线
//        if(imageOut[i][0] > (LCDW/2 - 5) && imageOut[i][1] >  (LCDW - 5))
//        {
//            //      右丢线处理
//            imageOut[i][1]=LCDW-1;
//
//            RoadNoSideProcess(imageInput, imageOut, 2, i);
//            return 1;
//        }
//
//       //  如果右边界 即将超出中线 则检查是否左丢线
//        if(imageOut[i][1] < (LCDW/2 + 5) && imageOut[i][0] < 5)
//        {
//            //  左丢线处理
//         //   RoadNoSideProcess(imageInput, imageOut, 1, i);
//        //    return 2;
//            imageOut[i][0]=0;
//
//        }
//    }


   return 0;


}

/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  函数名称：UpdownSideGet
*  功能说明：获取上边线
*  参数说明:            imageInput  ：边缘图像数组
*                     imageOut    ：上下边线数组
*  函数返回：0
*  修改时间：2022/6/23 星期四
*  备   注：不要改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDW])
{
    uint8 i = 0, j = 0;
    uint8 last = LCDH/2;
    uint8 Haf_W = LCDW/2;
    uint8 cnt = LCDH;

    memset(imageOut, 0, sizeof(imageOut));
    imageOut[Haf_W] = 0;
    /* 用中线比较近的行 判断图像中间是不是有上边线 */
    for(i = last; i >= 0; i--)
    {
        if(imageInput[i][Haf_W])
        {
            imageOut[Haf_W] = i;
            break;
        }
    }

    /* 中线往左 寻找边线 */
    for(i = Haf_W-1; i > 0; i--)
    {
        imageOut[i] = 0;
        cnt = imageOut[i+1] + 5;
        if(cnt >= LCDH-1) cnt = LCDH-1;
        /* 根据边界连续特性 寻找边界 */
        for(j = cnt; j > 0; j--)
        {
            if(!imageInput[j][i])
            {
                imageOut[i] = j;
                break;
            }
        }
    }
    /*中线往右 寻找边线*/
    for(i = Haf_W+1; i < LCDW-1; i++)
        {
            imageOut[i] = 0;
            cnt = imageOut[i+1] + 5;
            if(cnt >= LCDH-1) cnt = LCDH-1;

            /* 根据边界连续特性 寻找边界 */
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
*  函数名称：RoadIsNoSide
*  功能说明：判断是左右否丢线
*  参数说明：    imageInput  ：二进制图像数组
*             imageOut    ：边线数组[0]:左  [1]：右
*             lineIndex   ：指定行
*  函数返回：0：没有丢线   1:左边丢线  2：右边丢线  3： 左右都丢线   4：错误
*  修改时间：2022年6月24日
*  备   注：guo
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 RoadIsNoSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 lineIndex)
{
    uint8 state = 0;
    uint8 i = 0;
    static uint8 last = LCDW/2;   //图像中间位置

    imageOut[lineIndex][0] = 0;     //左右边线位置
    imageOut[lineIndex][1] = LCDW-1;  //
    /* 图像的起始行的左边部分 判断是否丢线 */
    for(i = last; i > 1; i--)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][0] = i;
            break;
        }
    }
    if(i == 1)      //上面循环执行到最后也没有退出则判定为丢线
        state = 1;  //左边界丢线

    for(i = last; i < 93; i++)
    {
        if(!imageInput[lineIndex][i])
        {
            imageOut[lineIndex][1] = i;
            break;
        }
    }
    if(i == 93)//上面循环执行到最后也没有退出则判定为丢线
    {
        /* 左右边界丢线 */
        if(state == 1)
            state = 3;
        /* 右边界丢线 */
        else
            state = 2;
    }
    if(state == 0 && imageOut[lineIndex][1] <= imageOut[lineIndex][0])    //如果没有丢线但是 左边的边线 大于 右边的边线
        state = 4;
    return state;
}
/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
*  函数名称：RoadNoSideProcess
*  功能说明：右丢线丢线处理
*  参数说明：    imageInput  ：二进制图像数组
*             imageOut    ：边线数组[0]:左  [1]：右
*             mode        : 左或者右
*             lineIndex   ：指定行
*  函数返回：无
*  修改时间：2022年6月24日
*  备   注：guo
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
*  函数名称：TurnNumSideGet
*  功能说明：获取边线的转折点个数
*  参数说明:无
*  函数返回：0
*  修改时间：2022/6/23 星期四
*  备   注：不要改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
uint8 TurnNumSideGet(void)
{
    char i;
    char err_front = 0, err_back = 0;

    num_up_turn=0;
    num_left_turn=0;
    num_right_turn=0;

    //计算上边线有多少个折点 10-80
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
    //计算左边线有多少个折点 从55-20
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
    //计算右边线有多少个折点 从55-20
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
