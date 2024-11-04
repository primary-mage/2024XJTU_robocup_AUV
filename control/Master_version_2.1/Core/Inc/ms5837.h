#ifndef __MS5837_H_
#define __MS5837_H_

#include "stm32f4xx_hal.h"
#include "i2c.h"
void MS5837_init(void);
void MS5837_Getdata(void);

extern uint32_t Pressure;
extern uint32_t Depth;
extern uint32_t D1_Pres,D2_Temp;
extern uint32_t Cal_C[7];	
#endif
