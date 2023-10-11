

#include "CAMERA.h"

/** ͼ��ԭʼ���ݴ�� */
unsigned char Image_Data[IMAGEH][IMAGEW];

/** ѹ����֮�����ڴ����Ļ��ʾ����  */
unsigned char Image_Use[LCDH][LCDW];

/** ��ֵ��������OLED��ʾ������ */
uint8 Pixle[LCDH][LCDW]={0};



// An highlighted block
/*---------------------------------------------------------------
 ����    ����Bin_Image_Filter
 ����    �ܡ��������
 ����    ������
 ���� �� ֵ����
 ��ע�����
 ----------------------------------------------------------------*/
void Pixle_Filter (void)
{
    sint16 nr; //��
    sint16 nc; //��

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
  * @brief    ��ȡ��Ҫʹ�õ�ͼ������
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      Get_Use_Image();
  *
  * @date     2019/6/25 ���ڶ�
  */
void Get_Use_Image(void)
{

    short i = 0, j = 0, row = 0, line = 0;

    for (i = 120-1; i >=0 ; i-=2)          //���۸� 120 / 2  = 60��
    {
        for (j = 188-1; j>=0; j-=1)     //���ۿ�188 / 2  = 94��
        {
            Image_Use[row][line] = mt9v03x_image[i][j]; // j (188-160)/2=14
            line++;
        }
        line = 0;
        row++;
    }

}

/*!
  * @brief    ��ֵ��
  *
  * @param    mode  ��    0��ʹ�ô����ֵ      1��ʹ��ƽ����ֵ      2: sobel ���ӸĽ���  �ֶ���ֵ    3��sobel ���ӸĽ���   ��̬��ֵ
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      Get_01_Value(0); //ʹ�ô�򷨶�ֵ��
  *
  * @date     2020/5/15 ���ڶ�
  */
void Get_01_Value(unsigned char mode)
{
    int i = 0,j = 0;
    int Threshold = 0;
    unsigned long  tv=0;


    if(mode == 0)
    {
        Threshold = GetOSTU(Image_Use);//�����ֵ
    }
    if(mode == 1)
    {
      //�ۼ�
        for(i = 0; i <LCDH; i++)
        {
            for(j = 0; j <LCDW; j++)
            {
                tv+=Image_Use[i][j];   //�ۼ�
            }
        }
        Threshold=tv/LCDH/LCDW;        //��ƽ��ֵ,����Խ��ԽС��ȫ��Լ35��������ĻԼ160��һ������´�Լ100
        Threshold=Threshold + 0;      //�˴���ֵ���ã����ݻ����Ĺ������趨
    }
    else if(mode == 2)
    {
        Threshold = 80;               //�ֶ�������ֵ
        sobel(Image_Use, Pixle,GetOSTU(Image_Use)*0.9);

        return;

    }
    else if(mode == 3)
    {
        sobelAutoThreshold(Image_Use, Pixle);  //��̬������ֵ
        return;
    }

    /* ��ֵ�� */
    for(i = 0; i < LCDH; i++)
    {
        for(j = 0; j < LCDW; j++)
        {
              if(Image_Use[i][j] >Threshold) //��ֵԽ����ʾ������Խ�࣬��ǳ��ͼ��Ҳ����ʾ����
                Pixle[i][j] =255;
              else
                Pixle[i][j] =0;
        }
    }
}

/*!
  * @brief    �������ֵ��С
  *
  * @param    tmImage �� ͼ������
  *
  * @return   ��ֵ
  *
  * @note     �ο���https://blog.csdn.net/zyzhangyue/article/details/45841255
  * @note     https://www.cnblogs.com/moon1992/p/5092726.html
  * @note     https://www.cnblogs.com/zhonghuasong/p/7250540.html
  * @note     Ostu������������������ͨ��ͳ������ͼ���ֱ��ͼ������ʵ��ȫ����ֵT���Զ�ѡȡ�����㷨����Ϊ��
  * @note     1) �ȼ���ͼ���ֱ��ͼ������ͼ�����е����ص㰴��0~255��256��bin��ͳ������ÿ��bin�����ص�����
  * @note     2) ��һ��ֱ��ͼ��Ҳ����ÿ��bin�����ص����������ܵ����ص�
  * @note     3) i��ʾ�������ֵ��Ҳ��һ���Ҷȼ�����0��ʼ����    1
  * @note     4) ͨ����һ����ֱ��ͼ��ͳ��0~i �Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ���ǰ������) ��ռ����ͼ��ı���w0����ͳ��ǰ�����ص�ƽ���Ҷ�u0��ͳ��i~255�Ҷȼ�������(��������ֵ�ڴ˷�Χ�����ؽ�����������) ��ռ����ͼ��ı���w1����ͳ�Ʊ������ص�ƽ���Ҷ�u1��
  * @note     5) ����ǰ�����غͱ������صķ��� g = w0*w1*(u0-u1) (u0-u1)
  * @note     6) i++��ת��4)��ֱ��iΪ256ʱ��������
  * @note     7) �����g��Ӧ��iֵ��Ϊͼ���ȫ����ֵ
  * @note     ȱ��:OSTU�㷨�ڴ�����ղ����ȵ�ͼ���ʱ��Ч�������Բ��ã���Ϊ���õ���ȫ��������Ϣ��
  * @note     ������ղ�����  https://blog.csdn.net/kk55guang2/article/details/78475414
  * @note     https://blog.csdn.net/kk55guang2/article/details/78490069
  * @note     https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
  *
  * @see      GetOSTU(Image_Use);//�����ֵ
  *
  * @date     2019/6/25 ���ڶ�
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
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    signed short MinValue, MaxValue;
    signed short Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)  HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for (j = 0; j < LCDH; j++)
    {
        for (i = 0; i < LCDW; i++)
        {
            HistoGram[tmImage[j][i]]++; //ͳ�ƻҶȼ���ÿ������������ͼ���еĸ���
        }
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)     return MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)  return MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  ��������

    PixelIntegral = 0;
    for (j = MinValue; j <= MaxValue; j++)
    {
        PixelIntegral += HistoGram[j] * j;//�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++)
    {
        PixelBack = PixelBack + HistoGram[j];   //ǰ�����ص���
        PixelFore = Amount - PixelBack;         //�������ص���
        OmegaBack = (float)PixelBack / Amount;//ǰ�����ذٷֱ�
        OmegaFore = (float)PixelFore / Amount;//�������ذٷֱ�
        PixelIntegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelIntegralFore = PixelIntegral - PixelIntegralBack;//�����Ҷ�ֵ
        MicroBack = (float)PixelIntegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float)PixelIntegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
        {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //���������ֵ;
}




/*!
  * @brief    ����soble���ؼ�����ӵ�һ�ֱ��ؼ��
  *
  * @param    imageIn    ��������
  *           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
  *           Threshold  ��ֵ
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
    /** ����˴�С */
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
            /* ���㲻ͬ�����ݶȷ�ֵ  */
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

            /* �ҳ��ݶȷ�ֵ���ֵ  */
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
  * @brief    ����soble���ؼ�����ӵ�һ���Զ���ֵ���ؼ��
  *
  * @param    imageIn    ��������
  *           imageOut   �������      ����Ķ�ֵ����ı�����Ϣ
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
    /** ����˴�С */
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
            /* ���㲻ͬ�����ݶȷ�ֵ  */
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

            /* �ҳ��ݶȷ�ֵ���ֵ  */
            for(k = 1; k < 4; k++)
            {
                if(temp[0] < temp[k])
                {
                    temp[0] = temp[k];
                }
            }

            /* ʹ�����ص����������ص�֮�͵�һ������    ��Ϊ��ֵ  */
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
