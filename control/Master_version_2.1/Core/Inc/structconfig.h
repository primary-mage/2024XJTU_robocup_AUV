#ifndef _STRUCTCONFIG_H
#define _STRUCTCONFIG_H
#include "stm32f4xx_hal.h"
#include "stdio.h"

typedef struct
{
	float Yaw_Exp;		//Ԥ�ڽǶ�
	float Depth_Exp;	//Ԥ�����
	
	int8_t Forward;		//ǰ���ٶȣ���ǰΪ��
	int8_t Side;			//ƽ���ٶȣ�����Ϊ��
	int8_t Swerve;		//ת���ٶȣ���תΪ��
	
	uint8_t PID_YAW_flag;		//����yaw������
	uint8_t PID_ROL_flag;		//����rol������
	uint8_t PID_Depth_flag; //������Ȼ�����
	uint8_t Version_Flag;		//�Ӿ���
	uint8_t Enable_flag;		//���ʹ�ܱ�־λ
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
	float Ilimit; 		//������ֵ�����ַ��룩
  float Irang;			//�����޷�
  float Pout;
  float Iout;
  float Dout;
  float OutPut;   
  uint8_t Ilimit_flag;    //���ַ���	
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


//����ģʽ
extern CMD_TYPE RC_mode;
extern CMD_TYPE TEST_mode;
extern CMD_TYPE MODE;
extern CCR_TYPE CCR_mode;

//�Ӿ�����
extern uint16_t Coordinate_X;
extern uint16_t Coordinate_Y;
//�Ӿ���PID
extern PID_TYPE PID_YAW_View;
//�ǶȻ�PID
extern PID_TYPE PID_ROL_Angle;
extern PID_TYPE PID_PIT_Angle;
extern PID_TYPE PID_YAW_Angle;
//���ٶȻ�PID
extern PID_TYPE PID_ROL_Rate;
extern PID_TYPE PID_PIT_Rate;
extern PID_TYPE PID_YAW_Rate;
//��Ȼ�
extern PID_TYPE PID_Depth;

//������ԭʼ��������
extern uint8_t imu229_Data[200];
//��ݮ������ԭʼ����
extern uint8_t Raspberry_Data[200];
//��ݮ��CMDλ
extern uint8_t bits[8];  
//��ȼ�����
extern uint8_t MS5837_Data[200];

//�����ǽǶȲ���ֵ
extern FLOAT_Angle Angle_Measure;
//�����ǽ��ٶȲ���ֵ
extern FLOAT_Angle AngleRate_Measure;
//�������������ٶȲ���ֵ
extern FLOAT_xyz   Gry_Measure;
//��Ȳ���ֵ��cm��
extern uint16_t Depth_Measure;
//ѹǿ����ֵ
extern uint16_t Pressure_Measure;


//�Ƕȿ���ֵ
extern FLOAT_Angle Angle_Expected;

//ˮ�����־λ
extern uint8_t Water_Flag;

//��ʱ������־λ
extern uint8_t PID_Scan;
extern uint8_t MS5837_Scan;
extern uint8_t RC_Scan;

#endif
