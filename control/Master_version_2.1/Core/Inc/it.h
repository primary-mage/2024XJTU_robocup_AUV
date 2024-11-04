#ifndef __IT_H
#define __IT_H

#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "imu229.h"            
#include "structconfig.h"  
#include "ms5837.h"
#include "rc.h"  
#include "stm32f4xx_hal.h"
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef*huart,uint16_t Size);
extern uint8_t MS5837_Scan;

#endif
