#include"rc.h"

FLOAT_Angle Angle_Measure;
FLOAT_Angle AngleRate_Measure;
FLOAT_xyz   Gry_Measure;
uint16_t Depth_Measure = 0;

uint16_t Coordinate_X = 0;
uint16_t Coordinate_Y = 0;


uint8_t imu229_Data[200];//原始数据读取
uint8_t Raspberry_Data[200];
//Raspberry_Data[1];
uint8_t MS5837_Data[200];
uint8_t bits[8];  

void GetBits(uint8_t data)
{
	 for (int i = 7; i >= 0; i--) 
	{  
     bits[7-i] = (data >> i) & 1;  // 右移并与1做与运算  
  }  
}
/*****************************************************************************
* 函  数：void CMD_Get(void)
* 功  能：获取来自树莓派的控制命令
* 参  数：
* 返回值：
* 备  注：
*****************************************************************************/
void Raspberry_Get(void)
{
	Raspberry_Translate(Raspberry_Data,&RC_mode);
}

void Raspberry_Translate(uint8_t data[],CMD_TYPE *CMD)
{
		if(data[0] == 0xCE)
	{
		CMD->Forward=1;//1
		if(data[2]== 0xCE)
		{
			Coordinate_X = data[1];
			Coordinate_Y = data[3];
		}
		if(data[2]!=0xCE &&data[3] ==0xCE)
		{
			Coordinate_X = data[1]<<8 |data[2];
			Coordinate_Y = data[4];
		}
	}
	if(data[0] == 0xFE)
	{
		CMD->Forward=0;
		Coordinate_X = 160;
		Coordinate_Y = 120;
	}
		
	

}


 /*****************************************************************************
* 函  数：void imu229_Get(void)
* 功  能：读取陀螺仪角度，角速度
* 参  数：
* 返回值：
* 备  注：相关数据存储在全局变量结构体型数据Angle_Measure,AngleRate_Measure,Gry_Measure
*					陀螺仪数据解析函数，一次33个字节数据，包含三个数据包
*****************************************************************************/
 void imu229_Get(void)
 {
		if(imu229_Data[0]==0x5A && imu229_Data[1] == 0xA5)
	 {
			imu229_Translate(imu229_Data,&Angle_Measure,&AngleRate_Measure,&Gry_Measure);
	 }
 }
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
 
 
