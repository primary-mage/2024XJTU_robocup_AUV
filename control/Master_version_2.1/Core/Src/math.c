#include "math.h"

uint16_t abs(int16_t x) {
    if (x < 0) {
        return (uint16_t)(-x);
    } else {
        return (uint16_t)x;
    }
}



float absf(float x) {
    if (x < 0) {
        return -x;
    } else {
        return x;
    }
}


uint16_t  contain(uint16_t a,uint16_t b)
{
  if (a > b) {
        return b;
    } else {
        return a;
    }
}


float  containf(float a,float b)
{
  if (a > b) {
        return b;
    } else {
        return a;
    }
}


 uint16_t Moto_Constrain(float motor)
{
    if(motor > MAXTHROTTLE_LIMIT) return MAXTHROTTLE_LIMIT;
    if(motor < MAXTHROTTLE_LIMIT_INV) return MAXTHROTTLE_LIMIT_INV;
//		if(motor <MINTHROTTLE+COMMANDER_MINCHECK_RELATIVE && motor > MINTHROTTLE+COMMANDER_MINCHECK_RELATIVE_INV) 
//			return MINTHROTTLE;
		
    else return motor;
}

