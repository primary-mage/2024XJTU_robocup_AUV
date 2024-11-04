#ifndef __RC_H
#define __RC_H

#include "pid.h"
#include "math.h"
#include "structconfig.h"

void imu229_Translate(uint8_t data[],FLOAT_Angle *Angle,FLOAT_Angle *AngleRate,FLOAT_xyz *Gry);
void Raspberry_Translate(uint8_t data[],CMD_TYPE *CMD);
void Raspberry_Get(void);
void GetBits(uint8_t data);
void imu229_Get(void);
void MS5837_Get(void);
#endif
