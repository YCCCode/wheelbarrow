/*
 * ano.h
 *
 *  Created on: 2023Äê2ÔÂ26ÈÕ
 *      Author: Administrator
 */

#ifndef USER_ANO_H_

#define USER_ANO_H_

#include "zf_common_headfile.h"


void ToNimin(uint8 fun,uint8 *data,uint8 len);
void IMUToNimin(int16 aacx,int16 aacy,int16 aacz,int16 gyrox,int16 gyroy,int16 gyroz);
void IMUAngleToNimin(short ROL,short PIT,short YAW);






#endif /* USER_ANO_H_ */
