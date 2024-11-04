#include "system.h"

uint8_t Water_Flag=0;
void Systerm_Init()
{
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim12);
	HAL_TIM_Base_Start_IT(&htim14);
	PidParameter_init();
	Mode_Init();
	CCR_ModeInit();
	Moto_Init();
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart6, imu229_Data , 198);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart2, MS5837_Data , 198);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Raspberry_Data , 198);
	HAL_Delay(5000);
}
void Task_Schedule(void)
{
	if(PID_Scan) //500Hz
			{
				PID_Scan = 0;
				Mode_Run();
			}
		if(MS5837_Scan) //0.5Hz
		{
	
			MS5837_Scan  = 0;
			MS5837_Getdata();
			if(abs(Depth - Depth_Measure)<100) Depth_Measure = Depth ;
			
				if(Depth_Measure >= DEPTH_MINCHECK) Water_Flag = 1;
				else Water_Flag = 0;
			
			MS5837_Scan = 0;
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);    //常用在LED上
		}
			
}


