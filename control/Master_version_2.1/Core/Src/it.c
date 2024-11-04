#include "it.h"

uint8_t PID_Scan= 0;
uint8_t MS5837_Scan= 0;
uint8_t RC_Scan= 0;
 
 
 
 
 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    	static uint16_t ms2 = 0,ms5 = 0,ms10 = 0,ms100 = 0,ms200 = 0,ms2000 = 0; //·ÖÆµÏµÊý
    if (htim == (&htim14))
    {
			ms2++;
			ms5++;
			ms10++;
			ms100++;
			ms200++;
			ms2000++;
			
		if(ms2 >= 2)//500Hz
		{
			ms2 = 0;
			PID_Scan = 1;
		}
			
		if(ms2000 >= 2000)//500Hz
		{
			ms2000 = 0;
			MS5837_Scan = 1;
		}
      
    }
		
}
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef*huart,uint16_t Size)
	{
		
		 if (huart == &huart6)
		 {
				imu229_Get();
  			HAL_UARTEx_ReceiveToIdle_DMA(&huart6, imu229_Data, 198);
		 }
		 
		 if (huart == &huart1)
		 {
				Raspberry_Get();
  			HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Raspberry_Data, 198);
		 }
		 
	}

