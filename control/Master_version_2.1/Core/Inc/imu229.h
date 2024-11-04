#ifndef _IMU229_H
#define _IMU229_H
#include "structconfig.h"


extern uint8_t imu229_Data[200];
void imu229_Translate(uint8_t data[],FLOAT_Angle *Angle,FLOAT_Angle *AngleRate,FLOAT_xyz *Gry);
void imu229_Get(void);

#endif
