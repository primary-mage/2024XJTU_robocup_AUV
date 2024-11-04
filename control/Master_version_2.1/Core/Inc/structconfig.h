#ifndef _STRUCTCONFIG_H
#define _STRUCTCONFIG_H
#include "stm32f4xx_hal.h"
#include "stdio.h"

typedef struct
{
	float Yaw_Exp;		//预期角度
	float Depth_Exp;	//预期深度
	
	int8_t Forward;		//前进速度，向前为正
	int8_t Side;			//平移速度，向左为正
	int8_t Swerve;		//转向速度，左转为正
	
	uint8_t PID_YAW_flag;		//控制yaw环开闭
	uint8_t PID_ROL_flag;		//控制rol环开闭
	uint8_t PID_Depth_flag; //控制深度环开闭
	uint8_t Version_Flag;		//视觉环
	uint8_t Enable_flag;		//电机使能标志位
}CMD_TYPE;

typedef struct
{
	uint8_t Set_flag;
	uint16_t CCR1;
	uint16_t CCR2;
	uint16_t CCR3;
	uint16_t CCR4;
	uint16_t CCR5;
	uint16_t CCR6;
}CCR_TYPE;


typedef struct
{
	float P;
	float I;
	float D;
	float Error;
	float Integral;
	float Differ;
	float PreError;
	float PrePreError;
	float Ilimit; 		//积分阈值（积分分离）
  float Irang;			//积分限幅
  float Pout;
  float Iout;
  float Dout;
  float OutPut;   
  uint8_t Ilimit_flag;    //积分分离	
}PID_TYPE;  

typedef struct
{ 
	float rol;
	float pit;
	float yaw;
} FLOAT_Angle;

typedef struct
{
	float x;
	float y;
	float z;
}FLOAT_xyz;

typedef struct
{ 
	float rol;
	float pit;
	float yaw;
} MS5837;


//控制模式
extern CMD_TYPE RC_mode;
extern CMD_TYPE TEST_mode;
extern CMD_TYPE MODE;
extern CCR_TYPE CCR_mode;

//视觉坐标
extern uint16_t Coordinate_X;
extern uint16_t Coordinate_Y;
//视觉环PID
extern PID_TYPE PID_YAW_View;
//角度环PID
extern PID_TYPE PID_ROL_Angle;
extern PID_TYPE PID_PIT_Angle;
extern PID_TYPE PID_YAW_Angle;
//角速度环PID
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
//深度环
extern PID_TYPE PID_Depth;

//陀螺仪原始数据数组
extern uint8_t imu229_Data[200];
//树莓派命令原始数组
extern uint8_t Raspberry_Data[200];
//树莓派CMD位
extern uint8_t bits[8];  
//深度计数据
extern uint8_t MS5837_Data[200];

//陀螺仪角度测量值
extern FLOAT_Angle Angle_Measure;
//陀螺仪角速度测量值
extern FLOAT_Angle AngleRate_Measure;
//陀螺仪重力加速度测量值
extern FLOAT_xyz   Gry_Measure;
//深度测量值（cm）
extern uint16_t Depth_Measure;
//压强测量值
extern uint16_t Pressure_Measure;


//角度控制值
extern FLOAT_Angle Angle_Expected;

//水深保护标志位
extern uint8_t Water_Flag;

//定时触发标志位
extern uint8_t PID_Scan;
extern uint8_t MS5837_Scan;
extern uint8_t RC_Scan;

#endif
