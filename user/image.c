//23  7  21 12  28
// 718  1 54  �������
#include "CAMERA.h"
#include "zf_common_headfile.h"
#include "string.h"
#include "balance.h"

#define Pixels Pixle

#define ADC_CHANNEL1            (ADC0_CH13_A13)
#define ADC_CHANNEL2            (ADC0_CH12_A12)
#define ADC_CHANNEL3            (ADC0_CH11_A11)
#define ADC_CHANNEL4            (ADC0_CH10_A10)


uint8 channel_index = 0;
adc_channel_enum channel_list[4] =
{
    ADC_CHANNEL1, ADC_CHANNEL2, ADC_CHANNEL3, ADC_CHANNEL4
};

int type=0;

int cross_turn = 0; //0��ת  1�м� 2��ת
int cir_turn=0;

float mid_py=0;
extern int dl1a_distance;
extern uint8 Pixle[LCDH][LCDW];
extern float yawAngle;
#define CROSSROAD 2

int crossstate=0;//0��״̬  1׼������  2 ����״̬  3����״̬
int cirstate=0;//0��   1.׼������  2.

#define HUAN 3

int rightflag[60]={0};
int leftflag[60]={0};


int gamemode =1;
int left_turn_up[60]={0};
int right_turn_down[60]={0};
int left_turn_down[60]={0};
int right_turn_up[60]={0};


uint8 leftdownflag=0,rightdownflag=0,leftupflag=0,rightupflag=0;

uint8 shortflag=0,trueshortflag=0;


int R_black[60]={0};
int L_black[60]={0};
int LCenter[60]={0};


int zhongold=LCDW/2;
int rightold=LCDW-1;
int leftold=0;

#define HU_DIAN 5
#define TU_BIAN 6



int shortwrong;
float ksecond=0,kfirst=0,parameterA=0,parameterB=0,kerror=0;


//��������
void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2);


void find_leftup_point(int start_point, int end_point, int RoadName);

void find_leftdown_point(int start_point, int end_point, int RoadName);

void find_rightup_point(int start_point, int end_point, int RoadName);

void find_rightdown_point(int start_point, int end_point, int RoadName);


float My_Abs(float a,float b);

int panduanfuhao(float x, float y);
void leftlinefuzhi(float A, float B, int start_point, int end_point);
void R_blackfuzhi(float A, float B, int start_point, int end_point);
void zhongxianfuzhi(int start_point, int end_point);

float GetAngle(float a,float b,float c,float ya,float yb,float yc);

void DebugMes(char  mes[]){
    wireless_uart_send_string(mes);
}


uint8 special_err_flag=0;
float special_err=0;

int carfree=1;

//void DebugMes(int mes){
//    wireless_uart_send_byte(mes);
//}
int CheckMidChange(){
    int maxv=0;
    for(int i=5;i<=4;i++){
        if(abs(LCenter[i]-LCenter[i-1])>=5){
            return i;
        }
    }
    return 0;
}


int My_int_Abs(int a, int b)//������֮�����ֵ�ĸ�����
{

    if ((a - b) > 0)
        return ((int)(a - b));
    else return ((int)(b - a));
}
void findborder(){



}

int oldfirst=LCDW/2;
int leftlost=0,rightlost=0,rea_leftlost=0,rea_rightlost=0,rea_leftlost_flag=0,rea_rightlost_flag=0;
int overflag=0;

int leftlost_low=0;
int leftlost_high=0;
int rightlost_low=0;
int rightlost_high=0;

int leftmaxjump;
int rightmaxjump;

int R_b_max=0,L_b_min=185,L_b_min_y,R_b_max_y,center_flag,centermaxjump;

int highhang=60;
void zhongxian(){
    rea_leftlost=0;
    rea_rightlost=0;
    rea_leftlost_flag=0;
    rea_rightlost_flag=0;

    center_flag=0;
    leftlost=0;
    rightlost=0;
    overflag=0;
    highhang=50;
    leftlost_low=60;
    leftlost_high=0;
    rightlost_low=60;
    rightlost_high=0;
    leftmaxjump=0;
    rightmaxjump=0;
    R_b_max=0;
    R_b_max_y=0;
    L_b_min=185;
    centermaxjump=0;
    L_b_min_y=0;
    for (int hang = 1; hang <= 50; hang++)
    {
        if(hang==1){

            for (  int lie = oldfirst; lie >= 1; lie--)
            {

                if (lie >= (int)186) lie = 185;
                if ( Pixels[hang][ lie] == 0 && Pixels[hang][ lie+1] == 255 ) //�ڰ�                              ����255
                {

                    R_black[hang] = (lie + 1);

                    rightold = (int)(lie + 1);
                    rightflag[hang] = 1;
                    break;
                }
                else
                {

                    R_black[hang] = 0;
                    rightflag[hang] = 0;
                }
            }
            for (int lie = oldfirst; lie < 186; lie++)
            {
                if (lie == 0) lie = 1;
                if ( Pixels[hang][ lie] == 0  && Pixels[hang][ lie-1] == 255)
                {

                    L_black[hang] =(lie - 1);
                    leftold = (int)(lie - 1);
                    leftflag[hang] = 1;
                    break;
                }
                else
                {
                    L_black[hang] = 185;
                    leftflag[hang] = 0;
                }
            }
            zhongold = (int)((L_black[hang] + R_black[hang])>>1);
            if(L_black[hang]==185){
                leftlost++;

            }else{
                rea_leftlost_flag=1;
            }
            if(R_black[hang]<=2){
                rightlost++;
            }else{
                rea_rightlost_flag=1;
            }
            continue;

        }


        if(Pixels[hang][ zhongold] == 0 && overflag==0){
            overflag=1;
            highhang=hang;
        }


        for (  int lie = zhongold; lie >= 1; lie--)
        {
            if(overflag){
                R_black[hang] = 0;
                break;
            }

            if (lie >= (int)185) lie = 184;
            if ( Pixels[hang][ lie] ==0 && Pixels[hang][ lie+1] == 255 ) //�ڰ�                              ����255
            {

                R_black[hang] = (lie + 1);
                rightold = (int)(lie + 1);
                rightflag[hang] = 1;
                break;
            }
            else
            {

                R_black[hang] = 0;
                rightflag[hang] = 0;
            }
        }
        for (int lie = zhongold; lie < 185; lie++)
        {
            if(overflag){
                L_black[hang] = 185;
                break;
            }

            if (lie == 0) lie = 1;
            if ( Pixels[hang][ lie] == 0  && Pixels[hang][ lie-1] == 255)
            {

                L_black[hang] =(lie - 1);
                leftold = (int)(lie - 1);
                leftflag[hang] = 1;
                break;
            }
            else
            {
                L_black[hang] = 185;
                leftflag[hang] = 0;
            }
        }
        zhongold = (int)((L_black[hang] + R_black[hang])>>1);
        LCenter[hang]=zhongold;



        if(L_black[hang]==185 && hang<=45 && hang<highhang ){
            leftlost++;
            if(rea_leftlost_flag==1){
                rea_leftlost++;
                if(hang>leftlost_high)leftlost_high=hang;
                if(hang<leftlost_low)leftlost_low=hang;
            }

        }else{
            rea_leftlost_flag=1;
        }
        if(R_black[hang]<=2  && hang<=45 && hang<highhang){
            rightlost++;
            if(rea_rightlost_flag==1){
                rea_rightlost++;
                if(hang>rightlost_high)rightlost_high=hang;
                if(hang<rightlost_low)rightlost_low=hang;
            }

        }else{
            rea_rightlost_flag=1;


        }

        if(hang==5){
            oldfirst=zhongold;
        }



        if(hang>=2 && hang <=40 && hang<highhang){
            if(abs(R_black[hang]-R_black[hang-1])>rightmaxjump){
                           rightmaxjump=abs(R_black[hang]-R_black[hang-1]);
                    }

            if(abs(L_black[hang]-L_black[hang-1])>leftmaxjump){
                           leftmaxjump=abs(L_black[hang]-L_black[hang-1]);
            }

            if(R_b_max< R_black[hang] ){
                R_b_max=R_black[hang];
                R_b_max_y=hang;
            }
            if(L_b_min>L_black[hang]){
                L_b_min=L_black[hang];
                L_b_min_y=hang;
            }


        }

    }






}

float camerr=0,result=0,cammid_py=0;

uint8 zangstate=0,zangstate1flag=0,zangstate2flag=0;

void GetCamErr(){
    if(center_flag==0){
        for (int hang = 0; hang <= 50; hang++)
                {

                        LCenter[hang]=(int)((L_black[hang] + R_black[hang])>>1);
                }
    }



    if(special_err_flag==0)
        result =(LCenter[2] + LCenter[8] + LCenter[15] + LCenter[22] +LCenter[30]-470)/2.6 ;
    else result = special_err;
//
//    result =0.3*camerr+(3* LCenter[6] + 2*LCenter[12] + LCenter[18] +0.8* LCenter[24] + 0.5*LCenter[30] -773)*0.7;

    if(result>=50)result=50;
    else if(result<=-50)result=-50;
    camerr=result;
}


void showBigPoint(int x,int y,int color){

    tft180_draw_point(x/2, y, color);
    if((x/2+1<93))tft180_draw_point(x/2+1, y, color);
    if((x/2-1>0))tft180_draw_point(x/2-1, y, color);
    if((y+1<59))tft180_draw_point(x/2, y+1, color);
    if((y-1>0))tft180_draw_point(x/2, y-1, color);


}

void showSide(){
    for(int i=0;i<60;i++){

        tft180_draw_point(L_black[i]/2, i, RGB565_PINK);
        if(L_black[i]/2-1>0) tft180_draw_point(L_black[i]/2-1, i, RGB565_PINK);
        tft180_draw_point(R_black[i]/2, i, RGB565_YELLOW);

        if(R_black[i]/2+1<94) tft180_draw_point(R_black[i]/2+1, i, RGB565_YELLOW);

        tft180_draw_point((L_black[i]+R_black[i])>>2, i, RGB565_BLACK);

    }






}
double dot_product(double ux, double uy, double vx, double vy) {
    return ux * vx + uy * vy;
}

double cross_product(double ux, double uy, double vx, double vy) {
    return ux * vy - uy * vx;
}

float GetAngle(float a,float b,float c,float ya,float yb,float yc){
       double ux = a - b;
       double uy = ya - yb;
       double vx = c - b;
       double vy = yc - yb;
       double u_length = sqrt(ux * ux + uy * uy);
       double v_length = sqrt(vx * vx + vy * vy);
       double cos_angle = dot_product(ux, uy, vx, vy) / (u_length * v_length);
       double direction = cross_product(a - b, ya - yb, c - b, yc - yb);
       if (direction-1>0)return -acos(cos_angle) * 180 / 3.141592;
       else{
           return acos(cos_angle) * 180 / 3.141592;
       }

}

int sumerr=0;
uint8 acc_flag=0;
extern float target_speed_C;
void straightjudge(){
    sumerr=0;
    if(carfree!=1){
        speedChange(0);
        return;
    }
    if(highhang>=48 && rea_leftlost==0 && rea_rightlost==0){
        for(int i=3;i<=48;i++){
               sumerr+=(LCenter[i]-LCenter[i-1])*(LCenter[i]-LCenter[i-1]);


           }
        if(sumerr<22){
            target_speed_C=-610;
            acc_flag=1;
            return;
        }else{
            acc_flag=0;
            target_speed_C=-410;
        }


    }else{
        acc_flag=0;
        target_speed_C=-410;
    }




//
//    shortwrong = 0;
//    advanced_regression(0, 2, 8, 14, 20);
//    kfirst = parameterB;
//
//    advanced_regression(0, 25, 29, 34, 50);
//    ksecond = parameterB;
//
//    if (My_Abs(0, kfirst) >= 0.7 || My_Abs(0, ksecond) >= 0.7) { shortwrong = 1; }
//    kerror = My_Abs(kfirst, ksecond);
//
//    if (kerror <= 0.2) shortflag = shortflag + 1;
//    if (kerror > 0.2) { shortflag = 0; }
//    if (shortwrong == 1) { shortflag = 0; }
//    if (shortflag >= 2) { shortflag = 2; trueshortflag = 1; }
//    if (shortflag < 2) trueshortflag = 0;


}

void R_laxian(int ax,int ay,int bx,int by){//180 40  0 0  k=40
    float k=0;
    if(ay-by!=0)k=(ax-bx)/(ay-by);

    for(int i=0;i<ay-by;i++)
    {
        R_black[by+i] = bx+k*i;
    }

}


void L_laxian(int ax,int ay,int bx,int by){//180 40  0 0  k=40
    float k=0;
    if(ay-by!=0)k=(ax-bx)/(ay-by);

    for(int i=0;i<ay-by;i++)
    {
        L_black[by+i] = bx+k*i;
    }

}

void m_laxian(int ax,int ay,int bx,int by){//180 40  0 0  k=40
    float k=0;
    if(ay-by!=0)k=(ax-bx)/(ay-by);

    for(int i=0;i<ay-by;i++)
    {
        LCenter[by+i] = bx+k*i;
    }

}

extern struct _pid{
    float err;                  //����ƫ��ֵ
    float err_last;             //������һ��ƫ��ֵ
    float err_sum;             //
    float Kp;                   //������������֡�΢��ϵ��
    float Ki;
    float Kd;
    float pwm;
};


extern struct _pid yaw_y;

uint8 goflag=0;
int go_over_dis=0,go_angle=0,go_angle_zero=0;
extern int Encoder_C_Sum,Encoder_A,Encoder_B;

void gogogo(){

//    yaw_y.err=go_angle_zero+go_angle-yawAngle;
//    yaw_y.pwm = yaw_y.Kp* yaw_y.err;
    if(goflag==1){
        special_err=-2*(go_angle-yawAngle);
//        special_err =
        special_err_flag=1;
        if(Encoder_C_Sum<go_over_dis){
            goflag=0;
            special_err_flag=0;
           // speedChange(0);
        }
    }
}

extern int t2flag;
extern int t2time;
void cross_state_check(){
    static ax,ay,bx,by;
    find_rightdown_point(5, 30, CROSSROAD);
    find_leftdown_point(5, 30, CROSSROAD);
    find_leftup_point(20, 50, CROSSROAD);
    find_rightup_point(20, 50, CROSSROAD);
    if(carfree==1){



        if(leftupflag ==1 && leftdownflag == 1 && rightupflag==1 && rightdownflag==1 &&left_turn_up[1]>94  &&  right_turn_up[1]<94
            ){
            if(rea_leftlost>5 && rea_rightlost>5 ){
                if(left_turn_up[1]<left_turn_down[1]
                          && left_turn_up[1]>right_turn_up[1]&& right_turn_up[1]>right_turn_down[1] ){


                    if(R_black[ (right_turn_up[0]+ right_turn_down[0])>>1]<=2 && L_black[ (left_turn_up[0]+ left_turn_down[0])>>1]==185){

                        crossstate=1;
                                carfree=0;
                    }
                }


            }





//            if(cross_turn==0){
//               // R_laxian(left_turn_up[1], left_turn_up[0], right_turn_down[1], right_turn_down[0]);
//            }else if(cross_turn==1){
//                L_laxian(right_turn_up[1], right_turn_up[0], right_turn_down[1], right_turn_down[0]);
//                R_laxian(right_turn_up[1], right_turn_up[0], right_turn_down[1], right_turn_down[0]);
//            }else if(cross_turn==2){
//                L_laxian(right_turn_up[1], right_turn_up[0], left_turn_down[1], left_turn_down[0]);
//
//            }
        }
    }else if(crossstate==1){
            if(cross_turn==0){
                if(leftlost_low==60 ){
                        special_err=30;
                        special_err_flag=1;
                        t2flag=1;
                        crossstate=2;

                }
//
//                if(t2time>=20 && t2flag==1){
//                     t2flag=0;
//                     special_err_flag=0;
//                     //crossstate=2;
//
//                     rightdownflag=0;
//                     leftupflag=0;
//                     rightupflag=0;
//                     leftdownflag=0;
//                     crossstate=0;
//                     carfree=1;
//
//
//                }

            }

//             //  R_laxian(left_turn_up[1], left_turn_up[0], right_turn_down[1], right_turn_down[0]);
//           }else if(cross_turn==1){
//               L_laxian(right_turn_up[1], right_turn_up[0], right_turn_down[1], right_turn_down[0]);
//               R_laxian(right_turn_up[1], right_turn_up[0], right_turn_down[1], right_turn_down[0]);
//           }else if(cross_turn==2){
//               L_laxian(right_turn_up[1], right_turn_up[0], left_turn_down[1], left_turn_down[0]);
//
//           }





            //������ʮ��
//            if(cross_turn==2){
//                if(L_black[1]>=185){
//                    crossstate=2;
//                }
//            }else if(cross_turn==0){
//                for(int i=1;i<=15;i++){
//                    if(R_black[i]>2){
//
//                        return;
//                    }
//                }
//
//
//
//            }


    }else if(crossstate==2){

        if(t2time>=20 && t2flag==1){
               t2flag=0;
               special_err_flag=0;
               crossstate=0;
               carfree=1;

        }
//
//        rightdownflag=0;
//        leftupflag=0;
//        rightupflag=0;
//        leftdownflag=0;
//


//        if(cross_turn==2){
//            if(L_black[10]<175){
//               crossstate=0;
//               carfree=1;
//           }
//        }else if(cross_turn==0){
//            if(R_black[10]>10){
//               crossstate=0;
//               carfree=1;
//           }
//        }else if(cross_turn==1){
//            if(R_black[10]>=10 &&L_black[10]<175){
//                crossstate=0;
//                carfree=1;
//
//            }
//        }

    }
    return;

//    if(crossstate==0){
//
//        find_rightdown_point(6, 45, CROSSROAD);
//        find_leftdown_point(6, 45, CROSSROAD);
//
//        if(rightdownflag==1 && leftdownflag==1){
//            crossstate=1;
//        }
//    }else if(crossstate==1 ){
//        find_rightdown_point(4, 45, CROSSROAD);
//        find_leftdown_point(4, 45, CROSSROAD);
//        find_leftup_point(6, 45, CROSSROAD);
//        find_rightup_point(6, 45, CROSSROAD);
//
//
//        if(cross_turn==0){
//            if(rightdownflag==1){
//
//
//            }
//        }
//
//    }



}



float maxlinek=0;

void ChecklineK(int mode) {
    double slope = 0.0; // б��
    double prev_slope = 0.0; // ǰһ�����б��
    int i;
    for (i = 6; i <= 35; i+=3) {

        slope = (double)(3) / (double)(R_black[i] - R_black[i-3]); // ����б��
        if (i > 6 && fabs(slope - prev_slope) > maxlinek) { // ���б����ǰһ�����б�ʵĲ���������б�����
            maxlinek=My_Abs(slope, prev_slope);
        }
        prev_slope = slope; // ����ǰһ�����б��
    }
}


void crossway(){
    cross_state_check();



//    find_rightdown_point(6, 45, CROSSROAD);
//    find_leftdown_point(6, 45, CROSSROAD);
//    find_leftup_point(6, 45, CROSSROAD);
//   find_rightup_point(6, 45, CROSSROAD);


//    if(leftupflag==1 && rightdownflag==1){
//        R_laxian(left_turn_up[1],left_turn_up[0],right_turn_down[1],right_turn_down[0]);
//    }
//    else if (leftdownflag == 0 && rightdownflag == 0 && leftupflag == 1 && rightupflag == 1)
//   {
//     advanced_regression(1, left_turn_up[0], left_turn_up[0] + 1, left_turn_up[0] + 3, left_turn_up[0] + 4);
//     leftk = parameterB;
//
//     advanced_regression(2, right_turn_up[0], right_turn_up[0] + 1, right_turn_up[0] + 3, right_turn_up[0] + 4);
//     rightk = parameterB;
//
//     int fanhui = panduanfuhao(leftk, rightk);
//       if (fanhui == 0)
//       {
//       advanced_regression(1, left_turn_up[0], left_turn_up[0] + 1, left_turn_up[0] + 3, left_turn_up[0] + 4);
//       leftlinefuzhi(parameterA, parameterB, 0, (int)(left_turn_up[0] + 1));
//       advanced_regression(2, right_turn_up[0], right_turn_up[0] + 1, right_turn_up[0] + 3, right_turn_up[0] + 4);
//       R_blackfuzhi(parameterA, parameterB, 0, (int)(right_turn_up[0] + 1));
//       zhongxianfuzhi(0, 69);
//       }
//   }





}


int cir_rightupx=0,cir_rightupy=0;
int findcirleft(){
    int bigval=0,i;
    for(i=1;i<=20;i++){
        if(R_black[i]>bigval)bigval=R_black[i];

    }

    if(bigval>5){
        cir_rightupx=R_black[i];
        cir_rightupy=i;


        return i;
    }
    else return 0;


}




//�������ұ������ֵ�ı߽�����������ĳֵ
uint8 point1flag=0;
uint8 point1x,point1y;
void find_point_1(){

    point1flag=0;

//
//    for(int i=5;i<=50;i++){
//        if( R_black[i]-R_black[i+2]>=5){
//            point1x=R_black[i];
//            point1y=i;
//            point1flag=1;
//
//            showBigPoint(point1x,point1y,RGB565_BLUE);
//            return;
//        }
//    }
//    return;

    int maxv=0,h=0;
    for(int i=2;i<=55;i++){
        if(R_black[i]>maxv){
            h=i;
            maxv=R_black[i];
        }
    }

    if(h<=40 && h<rightlost_low){
        point1flag=1;

        point1x=R_black[h];
        point1y=h;
        showBigPoint(point1x,point1y,RGB565_BLUE);
    }

}
int circlestate=0
        ; //0δ���룬1��⵽׼������  2.����   3.��⵽ǰ��2��   4.�ս�ȥ  5.��⵽�㲹���� 6��⵽��2�㲹����
int truecirflag=0;  //ȷ���ҵ�Բ��������ϵ�
int cirleftx=0,cirlefty=0;
int circlex=0,clrcley=0;

int cirflag=0;
float duanru_len=180;


void circleway(){
    //�һ������ұߴ���10  ���0     �������� ����
    if(carfree==1 && right_turn_down)
    if(crossstate==0 && circlestate==0  && 0){
        cirflag=0;
        if(rightmaxjump<=2 && leftmaxjump>=18 && leftmaxjump<=60){
            cirflag=1;
            for(int i =5;i<=40;i++){
                if(L_black[i]- L_black[i-4]<0 && L_black[i]-L_black[i+1]<=-18 && L_black[i-1]-L_black[i-3]<0){
                    //�ҵ�����Բ���Ľ���ʱ����¹յ�

                    cirflag=2;
                    for(int j=i+5;j<=45;j++){
                        if(Pixle[j][L_black[i]]==255){
                            cirflag=3;
                            if(Pixle[j+2][L_black[i]]==0 || Pixle[j+3][L_black[i]]==0){
                                cirflag=4;
                                showBigPoint(L_black[i], i, RGB565_BLUE);
                               circlestate=1;

                               return;

                            }
                        }
                    }



                }else{


                }
            }


        }

//        if(truecirflag==1){
//
//            L_laxian(cirleftx,cirlefty, left_turn_down[1], left_turn_down[0]);
//
//        }

//        if(leftdownflag==1){
//            find_leftup_point(20,50, HUAN);
//            if(left_turn_down[1]-left_turn_up[1]>30 && rightlost<=1 && carfree==1 && CheckMidChange()==1){
//
//                truecirflag=1;
//                cirleftx=left_turn_up[1];
//                cirlefty=left_turn_up[0];
//                circlestate=1;
//                carfree=0;
//
//            }
//            L_laxian(left_turn_up[1], left_turn_up[0], left_turn_down[1], left_turn_down[0]);
//
//
//        }

    }else if(circlestate==1){  //��⵽�˻�û����

        for(int i=5;i<=15;i++){
            if(L_black[i]!=185){
                L_laxian(left_turn_up[1], left_turn_up[0], left_turn_down[1], left_turn_down[0]);
                return;
            }
        }
        L_laxian(left_turn_up[1], left_turn_up[0], left_turn_down[1], left_turn_down[0]);
        circlestate=2;



    }else if(circlestate==2){  //������

        for(int i=5;i<=15;i++){
            if(L_black[i]==185){
                L_laxian(left_turn_up[1], left_turn_up[0], left_turn_down[1], left_turn_down[0]);
                return;
            }
        }
        L_laxian(left_turn_up[1], left_turn_up[0], left_turn_down[1], left_turn_down[0]);
        circlestate=3;
    }else if(circlestate==3){   //ԲȦ���


        for(int i=5;i<=15;i++){
            if( L_black[i]<=180){
                //���û����
                return;
            }
        }
        //��߶���
//        R_laxian(140, 50,20,0);

        special_err_flag=1;
        special_err=15;
        circlestate=4;


    }else if(circlestate==4){ //��


//���߽�ȥ  ���ұ�����ʧ����һ�׶�

        for(int i=12;i<=18;i++){
            if(  R_black[i]>=4){
//                R_laxian(140, 50,20,0);
                return;
            }
        }

//        R_laxian(140, 50,20,0);
        //�ұ߶���
        circlestate=5;

    }else if(circlestate==5){


     //�ұ߶���״̬���������߻ظ����½׶�

        for(int i=12;i<=18;i++){
            if( R_black[i]<=4){
//                R_laxian(140, 50,20,0);
                return;
            }
        }

        special_err_flag=0;
        circlestate=7;
        //����û����

    }else if(circlestate==6){






        //��������״̬���ȳ��ڽ���һ�׶�

        //if(leftlost>7 && rightlost>10){

//            find_point_1();
//            if(point1flag==1){
//                if(R_b_max_y!=0){
//                    R_laxian(185, leftlost_high, point1x, point1y);
//                }
//
//                circlestate=7;
//            }
//




//            for(int i=5;i<=30;i++){
//
//                if(R_black[i+1]<=2 && R_black[i]>=20){
//                    R_laxian(188, 50, R_black[i], i);
//                    circlestate=7;
//                    circlex=R_black[i];
//                    clrcley=i;
//                    return;
//                }
//
//            }


    //    }

//        if(findcirleft()==0 || (leftlost<10) || rightlost<10)
//            return;



    }else if(circlestate==7){


        //Բ������ת�����ұ�����ʧ����һ�׶�

//        for(int i=3;i<=15;i++){
//            if(R_black[i]>5){
//                R_laxian(180, 50, right_turn_down[1], right_turn_down[0]);
//                return;
//            }
//        }

        if(rightmaxjump<=2 && leftlost>=4 && rightlost<=1){
            m_laxian(LCenter[highhang], highhang, LCenter[5], 5);
            circlestate=9;
        }


        for(int i=5;i<=35;i++){

            if(R_black[i]-R_black[i-2]>4 && R_black[i]- R_black[i+2]>=0){
//                for(int j=i+1;j<=45;j++){
//                    R_black[j]=R_black[i];
//                }
                R_laxian(185, 50, R_black[i], i);
                return;
            }else{
                R_laxian(185/2, 50, 5, 2);
            }



        }

//
//        if(R_black[8]<5  &&  LCenter[highhang]>125){   //�ұ���б�ʿ�����
//
////                     special_err=17;
//            circlestate=8;
//            return;
//        }

    //  R_laxian(180, 50, point1x, point1y);
       return;


    }else if(circlestate==8){

        //�ұ߶���
        if(R_black[8]>5){
            special_err_flag=0;
            center_flag=1;
            m_laxian(LCenter[highhang], highhang, LCenter[5], 5);
            circlestate=9;
           // special_err=15;
        }else{
//            special_err_flag=0;
//            circlestate=9;
        }





    }else if(circlestate==9){




        for(int i=1;i<=10;i++){
            if(L_black[i+1]-L_black[i]<-15 && L_black[i]==185 && L_black[i+2]-L_black[i+1]<=0){
                circlestate=0;
                carfree=1;
                center_flag=0;
                return;
            }
        }
        m_laxian(LCenter[highhang], highhang, LCenter[5], 5);




    }

}
uint8 shangpoflag=0,xiawanpoflag=0;
uint8 xiapoflag=0;
extern int showstate,t1time,t1flag;

int shangpostate=0;

void ruku(){

}

int ban_ma_count=0;
uint8 ban_ma_flag=0,banmastate=0;

void ban_ma_check(){
    int count=0;
    uint8 last=0;
//    if(rightdownflag==1){


    if(carfree==0 || banmastate==2)return;
    for(int i=30;i<=150;i+=2){
        if(Pixle[28][i]!=last){
            count++;
        }
        last=Pixle[28][i];
    }


    if(banmastate==1){
        if(t2time>=10){
            special_err_flag=0;
            t2flag=0;
            banmastate=2;
        }
    }

//    }

    ban_ma_count=count;

    if(ban_ma_count>=15 &&  rea_rightlost>=10 && rea_leftlost==0){
        ban_ma_flag=1;
          if(rea_leftlost>0 && rea_rightlost==0){
             //�����
              showstate = 1;


          }else if( rea_rightlost>0 && rea_leftlost==0 ){
              //�ұ����
              showstate=2;

              special_err=-10;
              special_err_flag=1;
              t2flag=1;
              banmastate=1;
          }
    }

}

uint8 linenum=0;
uint8 up_black[190]={0};
int ttx=0,tty=0;
uint8 up_st_x=0,up_st_y=0,up_max_len=0,up_end_x=0,up_end_y=0;


//  wireless_uart_send_buff(up_black,181);
//                if(up_black[i]-up_black[i-1]<=1 && up_black[i]-up_black[i-1]>=-1){
//                    if(curnum==0){
//                        up_st_x=i;
//                        up_st_y=j;
//                        curnum++;
//
//                    }else{
//
//                        curnum++;
//                    }
//                    if(up_max_len<curnum){
//                        up_end_x=i;
//                        up_end_y=j;
//                        up_max_len=curnum;
//                    }
//
//                }else{
//                    curnum=0;
//                }

uint8 up_wanzen_flag=0;
void sao_xian_up(){
    linenum=0;
    up_max_len=0;
    up_wanzen_flag=1;
    uint8 curnum=0;
    int lasterr=99,curerr=0;
//    for(int i=1;i<=50;i++){
//        if(Pixle[i][1]==0){
//            up_black[1]=i;
//            break;
//        }
//    }

    for(int i =R_black[2]+1;i<L_black[2]-1;i+=1){
        up_black[i]=60;
        for(int j=1;j<=highhang+5;j++){
            if(j<=0)j=1;
            if(Pixle[j][i]==0){
                up_black[i]=j;
                break;
            }
        }

        if(up_black[i]==60){
            up_wanzen_flag=0;
            ttx = i;
        }


    }

}


extern int Start_Flag;

uint8 overduanway=0;
uint8 tixing_flag=0;

int errsum=0;

uint8 special_zongxian_flag=0;
uint8 duanruflag=0;
uint8 duanrustate=0;
uint8 oldmidx=0,duanru_dis=0;
void duanru(){

    if(duanruflag==0 && tixing_flag==1 ){
        duanrustate=1;
        carfree=0;
    }


    if(duanrustate==1){

        if(highhang<=30 ){
            oldmidx=(LCenter[5]+LCenter[10]+LCenter[15]+LCenter[20])/4;
            duanrustate=2;
            return;
        }


    }

    if(duanrustate==2){
        center_flag=1;
        duanru_dis=0;
        for(int i=1;i<=40;i++){
            if(Pixle[i][oldmidx]==0 && duanru_dis==0){
                duanru_dis=i;

            }
            LCenter[i]=oldmidx;
        }

        if(duanru_dis<10){
            go_over_dis=-duanru_len*1500+Encoder_C_Sum;
            go_angle=yawAngle;
            goflag=1;

            duanrustate=3;
            return;




        }


    }

    if(duanrustate==3){
        if(goflag==0){
            center_flag=0;
            carfree=1;
            duanrustate=4;

}

    }


}
void ti_xing_kou_check(){
    tixing_flag=0;
    if(rea_leftlost==0 && rea_rightlost==0 ){
        if(highhang < 45){
            if(R_black[highhang-5]>5 && L_black[highhang-5]<185){
                   sao_xian_up();
                   if(up_wanzen_flag==1 ){
                       errsum=0;
                       for(int i=80;i<=110;i++){
                           errsum+=abs(up_black[i]-up_black[i-1])*abs(up_black[i]-up_black[i-1]);
                       }

                       if(errsum<=3){
                           tixing_flag=1;


                           if(ban_ma_flag==1 && duanrustate!=0 && carfree==1){

                             //�����յ�
                             Start_Flag=0;
                             return;


                           }




                   }


//                int x=0,y=0;
//                for(int i=5;i<=180;i++){
//
//
//
//                }



            }
        }




    }


}

}






void zang_ai_wu(){


    if(dl1a_distance<630&& dl1a_distance>550 && rea_leftlost==0 && rea_rightlost==0 && tixing_flag==1){
        if(  zangstate==0 && carfree==1){
            carfree=0;
            zangstate=1;
        }

    }

        if(zangstate==1){
                if(zangstate1flag==0){
                    zangstate1flag=1;

                    go_over_dis=-73700+Encoder_C_Sum;
                    go_angle=yawAngle-60;
                    goflag=1;

                }

                if(goflag==0){

                    zangstate2flag=1;
                    go_over_dis=-94700+Encoder_C_Sum;
                    go_angle=yawAngle+60;
                    zangstate=2;
                    goflag=1;
                }
                return;
            }



        if(zangstate==2){

               if(goflag==0){
//                   zangstate1flag=3;
//                   carfree=1;

                   go_over_dis=-23700+Encoder_C_Sum;
                   go_angle=yawAngle+35;

                   goflag=1;
                   zangstate=3;
                   return;

               }

            }

        if(zangstate==3){
            if(goflag==0){


                carfree=1;
                zangstate=4;

            }


        }

}










//
//void shangpo(){
//    if(leftlost<=1 && rightlost<=1 && L_black[40]-R_black[40]>=80 &&L_black[8]-R_black[8] >=150 && L_black[8]-R_black[8] <=176 && L_black[40]-R_black[40]<=110 && carfree == 1 && shangpoflag==0 && leftmaxjump<=2 && rightmaxjump<=2){
//        advanced_regression(0, 2, 25, 30, 48);
//        if(parameterB<=0.3 && parameterB>=-0.3){
//
//            advanced_regression(1, 5, 25, 30, 48);
//            float k1 = parameterB;
//            if(k1<0 && k1>-1.2){
//
//                advanced_regression(2, 5, 25, 30, 48);
//                if(parameterB>0 && parameterB<1.2){
//                    carfree=0;
//                    shangpoflag=1;
//                    speedChange(1);
//                }
//
//            }
//
//        }
//
//
//    }
//
//    else if(shangpoflag==1 && L_black[8]-R_black[8]<=115 && xiapoflag==0){
//        xiapoflag=1;
//        speedChange(2);
//    }else if(xiapoflag==1 && L_black[8]-R_black[8]>135 && xiawanpoflag==0){
//        xiawanpoflag=1;
//        xiapoflag=0;
//        showstate=0;
//    }else if(xiawanpoflag==1){
//        if(t1time>=10){
//            t1flag=0;
//            xiawanpoflag=0;
//            shangpoflag=0;
//
//            carfree=1;
//            speedChange(0);
//        }else{
//            t1flag=1;
//        }
//
//
//    }
//}
extern int disp;

extern int Encoder_C,Motor_A,Motor_B,Encoder_C_Sum,debugmode;
extern float yawAngle;
extern int showstate,Y_PWM,curmode,Start_Flag;
extern float pitch_zero_py,maxrollzero;
//ͼ����ʼ
int t=0;


void MyImageProcess()
{

   if(type!=1&&type!=2)
    {
       zhongxian();  //����ɨ��

   //    shangpo();
           ti_xing_kou_check();
           gogogo();

           zang_ai_wu();

           duanru();
           crossway();
           ban_ma_check();
           //circleway();
       //    straightjudge();



           GetCamErr();


   //    showBigPoint(R_black[30],30,RGB565_BLUE);
  //     R_laxian(left_turn_up[1],left_turn_up[0],0,0);
        if(debugmode){

            showSide();
            if(leftupflag==1)
                       showBigPoint(left_turn_up[1],left_turn_up[0],RGB565_BLUE);
                   if(rightdownflag==1)
                       showBigPoint(right_turn_down[1],right_turn_down[0],RGB565_BLUE);
                   if(rightupflag==1)
                          showBigPoint(right_turn_up[1],right_turn_up[0],RGB565_BLUE);
                   if(leftdownflag==1)
                          showBigPoint(left_turn_down[1],left_turn_down[0],RGB565_BLUE);


        }

//       showBigPoint(L_black[30],30,RGB565_RED);



      // float ang=GetAngle(R_black[30-3], R_black[30], R_black[30+2], 27, 30, 32);


       t++;
       if(t>3){
           t=0;


           tft180_show_int(94,0, leftupflag, 1);
                 tft180_show_int(104,0, rightupflag, 1);
                 tft180_show_int(94, 15, leftdownflag, 1);
                 tft180_show_int(104,15,rightdownflag, 1);
                 tft180_show_float(0, 60, camerr,4,2);




                 //75����   gpio_get_level

                 tft180_show_int(0, 90,Motor_A, 4);

                 tft180_show_int(60,90, Motor_B, 4);

//                 tft180_show_int(0, 105,leftlost_low, 2);

//                 tft180_show_int(0, 105,highhang, 3);


               //  tft180_show_float(60, 105,  ,2,1);

          //       tft180_show_int(60, 105,L_black[10]-R_black[10] ,4);


//                 tft180_show_float(0, 120,pitch_zero_py,2,2);

                 tft180_show_int(0,120,zangstate,1);
                 tft180_show_int(10,120,tixing_flag,1);
                 tft180_show_int(20,120,ban_ma_flag,1);
                 tft180_show_int(30,120,crossstate,1);
                 tft180_show_int(40,120,duanrustate,1);
                 tft180_show_int(65,120,carfree,1);
//                 tft180_show_int(75,120,,1);
//



                 tft180_show_int(80,120,Y_PWM,4);
//
//


                 tft180_show_int(0, 135,duanru_len ,3);
//                 tft180_show_int(20, 135,rea_rightlost,2);
                 tft180_show_float(50,135, maxrollzero,2,1);
//                 tft180_show_int(80,135, Encoder_C_Sum,2);


//                 tft180_show_int(0, 135,rea_leftlost ,2);
//                 tft180_show_int(20, 135,rea_rightlost,2);
//                 tft180_show_int(50,135, highhang,2);
//                 tft180_show_int(80,135, Encoder_C_Sum,2);

//                 tft180_show_int(40, 135,showstate,1);
//                 tft180_show_int(60, 135,cirflag,1);

            //     tft180_show_float(75, 135,kfirst,2,1);

           //      tft180_show_float(75, 120,kfirst,2,1);

       }


//       tft180_show_int(40,105,R_black[29], 3);
//
//       tft180_show_int(0, 105,R_black[30], 3);
//       tft180_show_int(60,105,R_black[29], 3);

//       continuepanduan();  //����������
//       continuepanduanforleft();
//       cumulants();
//       huandaofuzhu();  //��������
//       cakkuandu();  //������
//       //calend();
//       podaojudge();  //�µ�  ��Ҫ����
//       mynewleftku();
//       juge_left_succesive_and_if_k_limit();  //�һ���  ��
//       juge_left_succesive_and_if_k_limitforleft();  //�󻷵� ��
//       threeroadfuzhu();
//
//       buzhongxian4();  //б��ʮ�� ��
//       buzhongxian2();//����ʮ�� ��
//       buzhongxian2_2();
//       mysmalls();  //��
//       findmiddle();
//       //bushizidan();//���뵥�㲹ʮ�� ��
//
//       threeroad();
//       findwanguaidian();
//       wenhaowan();
//       longstraight();
//       shortst();
//
//       everyjishu();
//       yanxu();
    }
}

//ͼ�������

float sumUp = 0;
float sumDown = 0;
void advanced_regression(int type, int startline1, int endline1, int startline2, int endline2)
{
    int i = 0;
    int sumlines1 = endline1 - startline1;
    int sumlines2 = endline2 - startline2;
    int sumX = 0;
    int sumY = 0;
    float averageX = 0;
    float averageY = 0;

    if (type == 0)  //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += LCenter[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += LCenter[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (LCenter[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
        {
            sumUp += (LCenter[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;

    }
    else if (type == 1)     //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += L_black[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += L_black[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (L_black[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
        {
            sumUp += (L_black[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
    else if (type == 2)         //�������
    {
        /**����sumX sumY**/
        for (i = startline1; i < endline1; i++)
        {
            sumX += i;
            sumY += R_black[i];
        }
        for (i = startline2; i < endline2; i++)
        {
            sumX += i;
            sumY += R_black[i];
        }
        averageX = (float)(sumX / (sumlines1 + sumlines2));     //x��ƽ��ֵ
        averageY = (float)(sumY / (sumlines1 + sumlines2));     //y��ƽ��ֵ
        for (i = startline1; i < endline1; i++)
        {
            sumUp += (R_black[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        for (i = startline2; i < endline2; i++)
        {
            sumUp += (R_black[i] - averageY) * (i - averageX);
            sumDown += (i - averageX) * (i - averageX);
        }
        if (sumDown == 0) parameterB = 0;
        else parameterB = sumUp / sumDown;
        parameterA = averageY - parameterB * averageX;
    }
}


float My_Abs(float a, float b)//������֮�����ֵ�ĸ�����
{

    if ((a - b) > 0)
        return ((float)(a - b));
    else return ((float)(b - a));
}



//����ʮ���ҹյ㿪ʼ
int leftuplost=0;

void find_leftup_point(int start_point, int end_point, int RoadName)
{
    int j;
    if (RoadName == CROSSROAD)
    {
        leftupflag=0;
        if (end_point >= 45) end_point = 45;


        // for (j = start_point; j <=end_point; j++)
        for (j = end_point; j >= start_point; j--)
        {
//            if (L_black[j] >  60 && L_black[j]<170 && L_black[j-5]>180)

           //     float ang =GetAngle(L_black[j-3],L_black[j],L_black[j+2],j-3,j,j+2);
                if (
//                        ang>=-155 && ang<=-135

                        L_black[j]-L_black[j-1]<10 && L_black[j+1]-L_black[j]<=0 &&
                        L_black[j]-L_black[j-1]<L_black[j-1]-L_black[j-2] && L_black[j]-L_black[j-1]<L_black[j+1]-L_black[j]


                 )
                {
                    int record;
                    record = L_black[j - 1];
                    left_turn_up[0] = (int)(j );//��������û�е�0��
                   left_turn_up[1] = L_black[j];
                   leftupflag=1;


                   return;

    //
    //                if ((j - 1) > 15 && record < 186)
    //                {
    //                    left_turn_up[0] = (int)(j - 1);//��������û�е�0��
    //                    left_turn_up[1] = L_black[j - 1];
    //                    if (left_turn_up[1] < 186)
    //                    {
    //                        leftupflag = 1;
    //                        //SetText("�ҵ����Ϲյ�2�� " + left_turn_up[0] + "  " + left_turn_up[1]);
    //                        break;
    //                    }
    //                }
    //                if ((j - 1) <= 15)
    //                {
    //                    left_turn_up[0] = (int)(j - 1);//��������û�е�0��
    //                    left_turn_up[1] = L_black[j - 1];
    //                    if (left_turn_up[1] < 186)
    //                    {
    //                        leftupflag = 1;
    //                        //SetText("�ҵ����Ϲյ�2�� " + left_turn_up[0] + "  " + left_turn_up[1]);
    //                        break;
    //                    }
    //                }
                }

            //�ڶ���
    //        if (L_black[j] > 0 && j >= 35)
    //        {
    //            if (L_black[j - 2] - L_black[j - 1] >= 30 && My_Abs(L_black[j], L_black[j - 1]) <= 10 && My_Abs(L_black[j + 1], L_black[j]) <= 10 && (L_black[j - 2] > 150 || L_black[j - 3] > 150 || L_black[j - 4] > 150 || L_black[j - 5] > 150 || L_black[j - 6] > 150 || L_black[j - 3] - L_black[j - 1] >= 30)
    //             && leftflag[j - 1] == 1 && leftflag[j] == 1 && leftflag[j + 1] == 1)
    //            {
    //                int record;
    //                record = L_black[j - 1];
    //                if ((j - 1) > 15 && record < 180)
    //                {
    //                    left_turn_up[0] = (int)(j - 1);//��������û�е�0��
    //                    left_turn_up[1] = L_black[j - 1];
    //                    if (left_turn_up[1] < 182)
    //                    {
    //                        leftupflag = 1;
    //                        //SetText("�ҵ��������Ϲյ�2�� " + left_turn_up[0] + "  " + left_turn_up[1]);
    //                        break;
    //                    }
    //                }
    //                if ((j - 1) <= 15)
    //                {
    //                    left_turn_up[0] = (int)(j - 1);//��������û�е�0��
    //                    left_turn_up[1] = L_black[j - 1];
    //                    if (left_turn_up[1] < 182)
    //                    {
    //                        leftupflag = 1;
                          //  SetText("�ҵ��������Ϲյ�2�� " + left_turn_up[0] + "  " + left_turn_up[1]);
    //                        break;
    //                    }
    //                }
    //            }right_turn_up[0]=right_turn_down[0]+i;
    //        }

        }


//        if(leftupflag==1 && leftuplost>=0)leftuplost++;
//        if(leftuplost>10 && leftupflag==1 ){
//            for(int i=1;i<=40;i++)
//            {
//                if(Pixle[5][i]==255){
//                    leftuplost=0;
//                    leftupflag=0;
//                }
//            }
//
//
//        }


    }else if(RoadName==HUAN){

        leftupflag=0;
       if (end_point >= 45) end_point = 45;

        if(leftdownflag==1){
      //      DebugMes("���µ㣬���ϵ�");
            int yp=left_turn_down[0];
            int xp=left_turn_down[1];
            for(int i=yp+1;i<=end_point;i++){
                if(Pixels[i][xp]==255){

                    //�ҵ�Բ���ײ�,��������
                    for(int j=1;j<=3;j++){
                        if(Pixels[i+j][xp]==0){
                            yp = i+j-1;

                            for(int k=0;k<30;k++){
                                if(Pixle[yp][xp-1]==255){
                                    xp--;
                                    continue;
                                }
                                else if(Pixle[yp+1][xp]==255){
                                    yp++;
                                    continue;
                                }
                                else{
                                    left_turn_up[1]=xp;
                                    left_turn_up[0]=yp;
                                    leftupflag=1;
                                    return;
                                }


                            }



                            return;
//                            for(int k=0;k<=50;k++){
//                                if(Pixels[i+j-1][yp+k]==255)continue;
//                                else if(Pixels[i+j+1][yp+k]==255)
//                            }


                        }
                    }
                }
            }
        }



    }
}

void find_rightup_point(int start_point, int end_point, int RoadName)
{
    int j;
    if (RoadName == CROSSROAD)
    {
        rightupflag=0;

        for (j = end_point; j >= start_point; j--)
        {


            if (R_black[j]-R_black[j-1]>10 && R_black[j+1]-R_black[j]>=0 && R_black[j]-R_black[j-1]>R_black[j-1]-R_black[j-2] && R_black[j]-R_black[j-1]>R_black[j+1]-R_black[j] )
           {
                    int record;
                    int recordhang;
//                    int i;
//                    int rightduan = 0;
//                    record = R_black[j - 1];
//                    recordhang = j - 1;
                    right_turn_up[0] = (int)(j );
                    right_turn_up[1] = R_black[j ];
                    rightupflag = 1;

                        break;



            }

        }
    }
}
void find_leftdown_point(int start_point, int end_point, int RoadName)
{
    int j;
    if (RoadName == CROSSROAD || RoadName==HUAN)
    {
        leftdownflag=0;
        for (j = start_point; j <= end_point; j++)
        {
            if(j<6)j=6;

            if (L_black[j] >90 &&  L_black[j+4]>180 && L_black[j-5]-L_black[j]>0 &&L_black[j+1]- L_black[j]>0 && L_black[j-3] - L_black[j-1]>=0 && L_black[j+1]<= L_black[j+2])  //����Ϊ4�У�ͼ����
            {
         //       float ang =GetAngle(L_black[j-3],L_black[j],L_black[j+2],j-3,j,j+2);
//               if ( ang<=-65&& ang>=-85){
                    int record = 0;
                    int i;
                    record = j;
                    left_turn_down[0] = (int)(record);//��������û�е�0��
                    left_turn_down[1] = L_black[record]; //��ס������
                    leftdownflag = 1;
                    return;

                    //SetText("�ҵ����¹յ�2�� " + left_turn_down[0] + "  " + left_turn_down[1]);
                    break; //�ҵ�ͻ���֮�������

            }

        }
    }
}





void find_rightdown_point(int start_point, int end_point, int RoadName)//�õ��յ�������� int
{
    int j;

    if (RoadName == CROSSROAD)   //����ʮ��
    {
        rightdownflag=0;
        for (j = 5; j <= 40; j++)
        {

            if (
                    ( R_black[j]-R_black[j+3]>0 && R_black[j] - R_black[j-3]>0) || (R_black[j]-R_black[j+2]>30)
//                R_black[j]-R_black[j-2]>=1 && R_black[j+2]<=0 &&
//                    R_black[j] <120 &&  R_black[j+1]==185 &&  R_black[j-1]
                                    ) //����Ϊ4�У�ͼ����
            {
            //    float ang =GetAngle(R_black[j-3],R_black[j],R_black[j+2],j-3,j,j+2);
//                   if ( ang>=65&& ang<=85){

                       int record = 0;
                      int i;
                      record = j;
                      right_turn_down[0] = (int)(record);//��������û�е�0��
                     right_turn_down[1] = R_black[record];
                     rightdownflag = 1;
                      //SetText("�ҵ����¹յ�2�� " + right_turn_down[0] + "  " + right_turn_down[1]);
                      break;                //��ס�����������


//                   }

            }

        }
    }else if (RoadName==HUAN){

        rightdownflag=0;
        for (j = start_point; j <= end_point; j++)
               {

                   if (R_black[j] <80 && R_black[j]-R_black[j+1]>5 && R_black[j]-R_black[j-2]>=0 && R_black[j]-R_black[j+3]>0)  //����Ϊ4�У�ͼ����
                   {

                       right_turn_down[1]=R_black[j];
                       right_turn_down[0]=j;
                       rightdownflag=1;


                             break;                //��ס�����������


                          }

                   }

   }else if(RoadName==HU_DIAN){
       rightdownflag=0;
       for (j = start_point; j <= end_point; j++)
              {

                  if (R_black[j]-R_black[j+1]>0 && R_black[j-1]-R_black[j-2]>=0 && R_black[j+1]-R_black[j+2]>=0 && R_black[j]-R_black[j+3]>0 && R_black[j-1]-R_black[j-2]>0)  //����Ϊ4�У�ͼ����
                  {

                      right_turn_down[1]=R_black[j];
                      right_turn_down[0]=j;
                      rightdownflag=1;


                            break;                //��ס�����������


                         }

                  }
   }



}
//����ʮ���ҹյ����


int panduanfuhao(float x, float y)
{
    int a;
    a = 0;
    if (x < 0 && y < 0) a = 1;
    if (x >= 0 && y >= 0) a = 1;
    return a;
}
void leftlinefuzhi(float A, float B, int start_point, int end_point)
{
    int m;
    for (m = start_point; m <= end_point; m++)
    {
        if ((B * m + A) >= 255) L_black[m] = 255;
        if ((B * m + A) <= 0) L_black[m] = 0;
        else if (0 < (B * m + A) && (B * m + A) < 255) L_black[m] = (B * m + A);
    }
}

//m��ʾy����
void R_blackfuzhi(float A, float B, int start_point, int end_point)
{
    int m;
    for (m = start_point; m <= end_point; m++)
    {
        if ((B * m + A) < 0) { R_black[m] = 0; }
        else if ((B * m + A) >= 0 && (B * m + A) <= 255) R_black[m] =(B * m + A);
        if ((B * m + A) >= 255) R_black[m] = 255;
    }
}
void zhongxianfuzhi(int start_point, int end_point)
{
    int m;
    for (m = start_point; m <= end_point; m++)
    {
        LCenter[m] = (L_black[m] / 2 + R_black[m] / 2);
    }
}


