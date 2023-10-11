/*!
  * @file     LQ_ImageProcess.h
  *
  * @brief    
  *
  * @company  �����������ܿƼ�
  *
  * @author   LQ-005
  *
  * @note     Tab�� 4���ո�
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2022��12��9��
  */ 

#ifndef IMAGEPROCESS_H_
#define IMAGEPROCESS_H_

#include "CAMERA.h"
#define Bin_Image Pixle
typedef enum
{
    NO=0,
    FlagRoundabout_R,
    FlagRoundabout_R_1,
    FlagRoundabout_R_2,
    FlagRoundabout_R_3,
    FlagRoundabout_L,
    FlagRoundabout_L_1,
    FlagRoundabout_L_2,
    FlagRoundabout_L_3
}FlagRoundabout;
typedef enum
{

    no_start=0,
    start,
    over
}game_status;
extern uint8 ImageSide[LCDH][2];     //���ұ������� 0�� 1��
extern uint8 UpSide[LCDW];    //���±������� 0�� 1��
extern uint8 num_left_turn;      //�����ת�۵����
extern uint8 num_right_turn;     //�ұ���ת�۵����
extern uint8 num_up_turn;

void ImageProcess(void);
void TFTSPI_BinRoadSide(uint8 imageOut[LCDH][2]);
void TFTSPI_BinRoad_UpdownSide(uint8 imageOut[LCDW]);
uint8 ImageGetSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2]);
uint8 UpdownSideGet(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDW]);
uint8 RoadIsNoSide(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 lineIndex);
void RoadNoSideProcess(uint8 imageInput[LCDH][LCDW], uint8 imageOut[LCDH][2], uint8 mode, uint8 lineIndex);
uint8 TurnNumSideGet(void);


#endif /* SRC_APPSW_TRICORE_USER_LQ_IMAGEPROCESS_H_ */
