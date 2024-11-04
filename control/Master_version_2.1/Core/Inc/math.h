#ifndef __MATH_H
#define __MATH_H
#include "stm32f4xx_hal.h"
#include "control.h"

uint16_t  abs(int16_t  x);
float absf(float x);
uint16_t  contain(uint16_t a,uint16_t b);
float  containf(float a,float b);
uint16_t Moto_Constrain(float motor);

#endif
