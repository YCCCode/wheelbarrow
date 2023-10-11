

#include "CAMERA.h"

/** 图像原始数据存放 */
unsigned char Image_Data[IMAGEH][IMAGEW];

/** 压缩后之后用于存放屏幕显示数据  */
unsigned char Image_Use[LCDH][LCDW];

/** 二值化后用于OLED显示的数据 */
uint8 Pixle[LCDH][LCDW]={0};



// An highlighted block
/*---------------------------------------------------------------
 【函    数】Bin_Image_Filter
 【功    能】过滤噪点
 【参    数】无
 【返 回 值】无
 【注意事项】
 ----------------------------------------------------------------*/
void Pixle_Filter (void)
{
    sint16 nr; //行
    sint16 nc; //列

    for (nr = 1; nr < LCDH - 9; nr++)
    {
        for (nc = 1; nc < LCDW - 1; nc = nc + 1)
        {
            if ((Pixle[nr][nc] == 0)
                    && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] > 700))
            {
                Pixle[nr][nc] = 255;
            }
            else if ((Pixle[nr][nc] == 255)
                    && (Pixle[nr - 1][nc] + Pixle[nr + 1][nc] + Pixle[nr][nc + 1] + Pixle[nr][nc - 1] < 300))
            {
                Pixle[nr][nc] = 0;
            }
        }
    }
}

/**
  * @brief    获取需要使用的图像数组
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      Get_Use_Image();
  *
  * @date     2019/6/25 星期二
  */
void Get_Use_Image(void)
{

    short i = 0, j = 0, row = 0, line = 0;

    for (i = 120-1; i >=0 ; i-=2)          //神眼高 120 / 2  = 60，
    {
        for (j = 188-1; j>=0; j-=1)     //神眼宽188 / 2  = 94，
        {
            Image_Use[row][line] = mt9v03x_image[i][j]; // j (188-160)/2=14
            line++;
        }
        line = 0;
        row++;
    }

}

/*!
  * @brief    二值化
  *
  * @param    mode  ：    0：使用大津法阈值      1：使用平均阈值      2: sobel 算子改进型  手动阈值    3：sobel 算子改进型   动态阈值
  *
  * @return   无
  *
  * @note     无
  *
  * @see      Get_01_Value(0); //使用大津法二值化
  *
  * @date     2020/5/15 星期二
  */
void Get_01_Value(unsigned char mode)
{
    int i = 0,j = 0;
    int Threshold = 0;
    unsigned long  tv=0;


    if(mode == 0)
    {
        Threshold = GetOSTU(Image_Use);//大津法阈值
    }
    if(mode == 1)
    {
      //累加
        for(i = 0; i <LCDH; i++)
        {
            for(j = 0; j <LCDW; j++)
            {
                tv+=Image_Use[i][j];   //累加
            }
        }
        Threshold=tv/LCDH/LCDW;        //求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
        Threshold=Threshold + 0;      //此处阈值设置，根据环境的光线来设定
    }
    else if(mode == 2)
    {
        Threshold = 80;               //手动调节阈值
        sobel(Image_Use, Pixle,GetOSTU(Image_Use)*0.9);

        return;

    }
    else if(mode == 3)
    {
        sobelAutoThreshold(Image_Use, Pixle);  //动态调节阈值
        return;
    }

    /* 二值化 */
    for(i = 0; i < LCDH; i++)
    {
        for(j = 0; j < LCDW; j++)
        {
              if(Image_Use[i][j] >Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
                Pixle[i][j] =255;
              else
                Pixle[i][j] =0;
        }
    }
}

/*!
  * @brief    大津法求阈值大小
  *
  * @param    tmImage ： 图像数据
  *
  * @return   阈值
  *
  * @note     参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
  * @note     https://www.cnblogs.com/moon1992/p/5092726.html
  * @note     https://www.cnblogs.com/zhonghuasong/p/7250540.html
  * @note     Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
  * @note     1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
  * @note     2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
  * @note     3) i表示分类的阈值，也即一个灰度级，从0开始迭代    1
  * @note     4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
  * @note     5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
  * @note     6) i++；转到4)，直到i为256时结束迭代
  * @note     7) 将最大g相应的i值作为图像的全局阈值
  * @note     缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
  * @note     解决光照不均匀  https://blog.csdn.net/kk55guang2/article/details/78475414
  * @note     https://blog.csdn.net/kk55guang2/article/details/78490069
  * @note     https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
  *
  * @see      GetOSTU(Image_Use);//大津法阈值
  *
  * @date     2019/6/25 星期二
  */
int GetOSTU(unsigned char tmImage[LCDH][LCDW])
{
    signed short i,j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelIntegralBack = 0;
    unsigned long PixelIntegral = 0;
    signed long PixelIntegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //初始化灰度直方图

    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            HistoGram[tmImage[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];   //前景像素点数
        PixelFore = Amount - PixelBack;         //背景像素点数
        OmegaBack = (float)PixelBack / Amount;//前景像素百分比
        OmegaFore = (float)PixelFore / Amount;//背景像素百分比
        PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
        MicroBack = (float)PixelIntegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float)PixelIntegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}




/*!
  * @brief    基于soble边沿检测算子的一种边沿检测
  *
  * @param    imageIn    输入数组
  *           imageOut   输出数组      保存的二值化后的边沿信息
  *           Threshold  阈值
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void sobel(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW], unsigned char Threshold)
{
    /** 卷积核大小 */
    int KERNEL_SIZE = 3;
    int xStart = KERNEL_SIZE/2;
    int xEnd   = LCDW - KERNEL_SIZE/2;
    int yStart = KERNEL_SIZE/2;
    int yEnd   = LCDH - KERNEL_SIZE/2;
    int i, j, k;
    int temp[4];
    for(i = yStart; i < yEnd; i++)
    {
        for(j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imageIn[i-1][j-1] + (int)imageIn[i-1][j+1]     //{{-1, 0, 1},
                      -(int)imageIn[i][j-1]   +(int)imageIn[i][j+1]        // {-1, 0, 1},
                      -(int)imageIn[i+1][j-1] + (int)imageIn[i+1][j+1];    // {-1, 0, 1}};

            temp[1] = -(int)imageIn[i-1][j-1] + (int)imageIn[i+1][j-1]     //{{-1, -1, -1},
                      -(int)imageIn[i-1][j]   + (int)imageIn[i+1][j]       // { 0,  0,  0},
                      -(int)imageIn[i-1][j+1] + (int)imageIn[i+1][j+1];    // { 1,  1,  1}};


            temp[2] = -(int)imageIn[i-1][j]   + (int)imageIn[i][j-1]       //  0, -1, -1
                      -(int)imageIn[i][j+1]   + (int)imageIn[i+1][j]       //  1,  0, -1
                      -(int)imageIn[i-1][j+1] + (int)imageIn[i+1][j-1];    //  1,  1,  0

            temp[3] = -(int)imageIn[i-1][j]   + (int)imageIn[i][j+1]       // -1, -1,  0
                      -(int)imageIn[i][j-1]   + (int)imageIn[i+1][j]       // -1,  0,  1
                      -(int)imageIn[i-1][j-1] + (int)imageIn[i+1][j+1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            if(temp[0] > Threshold)
            {
                imageOut[i][j] = 0;
            }
            else
            {
                imageOut[i][j] = 255;
            }
        }
    }
}



/*!
  * @brief    基于soble边沿检测算子的一种自动阈值边沿检测
  *
  * @param    imageIn    输入数组
  *           imageOut   输出数组      保存的二值化后的边沿信息
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void sobelAutoThreshold(unsigned char imageIn[LCDH][LCDW], unsigned char imageOut[LCDH][LCDW])
{
    /** 卷积核大小 */
    int KERNEL_SIZE = 3;
    int xStart = 0;
    int xEnd   = LCDW;
    int yStart = 0;
    int yEnd   = LCDH;
    int i, j, k;
    int temp[4];
    for(i = yStart; i < yEnd; i++)
    {
        for(j = xStart; j < xEnd; j++)
        {
            /* 计算不同方向梯度幅值  */
            temp[0] = -(int)imageIn[i-1][j-1] + (int)imageIn[i-1][j+1]     //{{-1, 0, 1},
                      -(int)imageIn[i][j-1]   + (int)imageIn[i][j+1]       // {-1, 0, 1},
                      -(int)imageIn[i+1][j-1] + (int)imageIn[i+1][j+1];    // {-1, 0, 1}};

            temp[1] = -(int)imageIn[i-1][j-1] + (int)imageIn[i+1][j-1]     //{{-1, -1, -1},
                      -(int)imageIn[i-1][j]   + (int)imageIn[i+1][j]       // { 0,  0,  0},
                      -(int)imageIn[i-1][j+1] + (int)imageIn[i+1][j+1];    // { 1,  1,  1}};


            temp[2] = -(int)imageIn[i-1][j]   + (int)imageIn[i][j-1]       //  0, -1, -1
                      -(int)imageIn[i][j+1]   + (int)imageIn[i+1][j]       //  1,  0, -1
                      -(int)imageIn[i-1][j+1] + (int)imageIn[i+1][j-1];    //  1,  1,  0

            temp[3] = -(int)imageIn[i-1][j]   + (int)imageIn[i][j+1]       // -1, -1,  0
                      -(int)imageIn[i][j-1]   + (int)imageIn[i+1][j]       // -1,  0,  1
                      -(int)imageIn[i-1][j-1] + (int)imageIn[i+1][j+1];    //  0,  1,  1

            temp[0] = abs(temp[0]);
            temp[1] = abs(temp[1]);
            temp[2] = abs(temp[2]);
            temp[3] = abs(temp[3]);

            /* 找出梯度幅值最大值  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            /* 使用像素点邻域内像素点之和的一定比例    作为阈值  */
            temp[3] = (int)imageIn[i-1][j-1]   + (int)imageIn[i-1][j] +  (int)imageIn[i-1][j+1]
                     +(int)imageIn[i  ][j-1]   + (int)imageIn[i  ][j] +  (int)imageIn[i  ][j+1]
                     +(int)imageIn[i+1][j-1]   + (int)imageIn[i+1][j] +  (int)imageIn[i+1][j+1];

            if(temp[0] > temp[3]/9.0f)
            {
                imageOut[i][j] = 0;
            //  wireless_uart_send_byte(0);
            }
            else
            {
                imageOut[i][j] = 255;
            //  wireless_uart_send_byte(255);
            }


        }
    }
}
