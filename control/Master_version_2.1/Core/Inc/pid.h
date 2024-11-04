#ifndef   _PID_H_
#define   _PID_H_

#include "structconfig.h"
#include "math.h"

void PidParameter_init(void);
void PID_Postion_Cal(PID_TYPE*PID,float target,float measure);

#endif
