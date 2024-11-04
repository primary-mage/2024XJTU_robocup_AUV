#include"ms5837.h"

#define MS5837 0xEC//宏定义设备地址
#define IIC_MS5837 hi2c1

uint8_t MS_RESET = 0x1E;//复位命令

uint8_t ADC_READ = 0x00;//ADC读取开始命令


/*
C1 压力灵敏度 SENS|T1
C2  压力补偿  OFF|T1
C3	温度压力灵敏度系数 TCS
C4	温度系数的压力补偿 TCO
C5	参考温度 T|REF
C6 	温度系数的温度 TEMPSENS
*/

uint32_t  Cal_C[7];	        //用于存放PROM中的6组数据1-6

double OFF_;
float Aux;
/*
dT 实际和参考温度之间的差异
TEMP 实际温度	
*/
uint64_t dT,TEMP;
/*
OFF 实际温度补偿
SENS 实际温度灵敏度
*/
uint64_t SENS;
uint32_t D1_Pres,D2_Temp=0;	// 数字压力值,数字温度值
uint32_t TEMP2,T2,OFF2,SENS2;	//温度校验值

uint32_t Pressure;				//气压
uint32_t Atmdsphere_Pressure;//大气压
uint32_t Depth;

/*******************************************************************************
  * @函数名称	MS583730BA_RESET
  * @函数说明   复位MS5611
  * @输入参数   无
  * @输出参数   无
  * @返回参数   无
*******************************************************************************/
void MS583703BA_RESET(void)
{
		HAL_I2C_Master_Transmit (&IIC_MS5837,MS5837,&MS_RESET,1, HAL_MAX_DELAY);
}

/**************************实现函数********************************************
*函数原型:unsigned long MS561101BA_getConversion(void)
*功　　能:    读取 MS5837 的转换结果 
*******************************************************************************/
unsigned long MS583703BA_getConversion(uint8_t command)
{
	unsigned long conversion = 0;
	uint8_t temp[3];
	HAL_I2C_Master_Transmit (&IIC_MS5837,MS5837,&command,1,HAL_MAX_DELAY);
	HAL_Delay(10);
	
	HAL_I2C_Master_Transmit(&IIC_MS5837, MS5837, &ADC_READ, 1, HAL_MAX_DELAY);
	
	HAL_I2C_Master_Receive(&IIC_MS5837,MS5837,temp,3,HAL_MAX_DELAY);
	
  conversion = (unsigned long)temp[0] * 65536 + (unsigned long)temp[1] * 256 + (unsigned long)temp[2];
	
	return conversion;
}

void MS5837_init(void)
{	 
  uint8_t num[2];
  uint8_t i; 
	 
	MS583703BA_RESET();	 // Reset Device  复位MS5837
	HAL_Delay(20);       //复位后延时（注意这个延时是一定必要的，可以缩短但似乎不能少于20ms）
	 
  for (i=1;i<=6;i++) 
	{
		HAL_I2C_Mem_Read(&IIC_MS5837,MS5837,0xA0 + (i*2),1,num,2,HAL_MAX_DELAY);
		
		Cal_C[i] = (((uint16_t)num[0] << 8) | num[1]);//高位先行
		
	}
	
	for(i=0;i<5;i++)
	{
		HAL_Delay(1);
		MS5837_Getdata();   //获取大气压
    Atmdsphere_Pressure+=Pressure;	
	}
	Atmdsphere_Pressure=Atmdsphere_Pressure/5;
}

///***********************************************
//  * @brief  读取气压
//  * @param  None
//  * @retval None
//************************************************/
void MS5837_Getdata(void)
{
	D1_Pres= MS583703BA_getConversion(0x48);
	D2_Temp = MS583703BA_getConversion(0x58);
	
	if(D2_Temp > (((uint32_t)Cal_C[5])*256))
	{		
		dT=D2_Temp - (((uint32_t)Cal_C[5])*256);
		TEMP=2000+dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536+((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768+((uint32_t)Cal_C[3]*dT)/256;		
	}
	else 
	{	
		dT=(((uint32_t)Cal_C[5])*256) - D2_Temp;			
		TEMP=2000-dT*((uint32_t)Cal_C[6])/8388608;	
		OFF_=(uint32_t)Cal_C[2]*65536-((uint32_t)Cal_C[4]*dT)/128;
		SENS=(uint32_t)Cal_C[1]*32768-((uint32_t)Cal_C[3]*dT)/256;		
	}
//	printf("1:%d\t%d\t%llu\t%llu\t%f\t%llu\r\n",D1_Pres,D2_Temp,dT,TEMP,OFF_,SENS);
//printf("%d\t",Cal_C[5]);
	if(TEMP<2000)  // low temp
	{
	  Aux = (2000-TEMP)*(2000-TEMP);	
		T2 = 3*(dT*dT)/8589934592; 
		OFF2 = 3*Aux/2;
		SENS2 = 5*Aux/8;	
//		printf("2:%f\t%d\t%d\t%d\r\n",Aux,T2,OFF2,SENS2);		
	}
	else
	{
//		printf("!!");
		
	  Aux = (TEMP-2000)*(TEMP-2000);		
	  T2 = 2*(dT*dT)/137438953472;
		OFF2 = 1*Aux/16;
		SENS2 = 0;	
	}
	OFF_ = OFF_ - OFF2;
	SENS = SENS - SENS2;	
	TEMP=(TEMP-T2)/100;	
  Pressure= ((D1_Pres*SENS/2097152-OFF_)/8192)/10;
	Depth=0.983615*(Pressure-Atmdsphere_Pressure);	 
}
