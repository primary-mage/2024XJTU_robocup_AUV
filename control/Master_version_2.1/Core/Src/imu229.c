#include "imu229.h"            
     


//临时借用

FLOAT_Angle Angle_Measure;
FLOAT_Angle AngleRate_Measure;
FLOAT_xyz   Gry_Measure;
uint8_t imu229_Data[200];//原始数据读取



//数据解析函数，一次33个字节数据，包含三个数据包

 void imu229_Translate(uint8_t data[],FLOAT_Angle *Angle,FLOAT_Angle *AngleRate,FLOAT_xyz *Gry)
 {
		
			
		
			Gry->x = ((int16_t )((int16_t)data[10]<<8|data[9])) ;
			Gry->y = ((int16_t )((int16_t)data[12]<<8|data[11]));
			Gry->z = ((int16_t )((int16_t)data[14]<<8|data[13]));
	
			AngleRate ->rol = ((int16_t )((int16_t)data[17]<<8|data[16]));
			AngleRate ->pit = ((int16_t )((int16_t)data[19]<<8|data[18]));
			AngleRate ->yaw = ((int16_t )((int16_t)data[21]<<8|data[20]));
		
			Angle ->pit = (float) ((int16_t )((int16_t)data[31]<<8)+data[30])/100;
			Angle ->rol = (float) ((int16_t )((int16_t)data[33]<<8)+data[32])/100;
			Angle ->yaw = (float) ((int16_t )((int16_t)data[35]<<8)+data[34])/10;
										
 
 }
 
 //数据读取函数

 void imu229_Get(void)
 {
		if(imu229_Data[0]==0x5A && imu229_Data[1] == 0xA5)
	 {
			imu229_Translate(imu229_Data,&Angle_Measure,&AngleRate_Measure,&Gry_Measure);
	 }
 }
