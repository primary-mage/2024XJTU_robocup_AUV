#ifndef   _CONTROL_H
#define   _CONTROL_H

#include "stm32f4xx_hal.h"
#include "structconfig.h"
#include "main.h"
#include "pid.h"
#include "tim.h"
#define MINTHROTTLE						1500 //���ͣתֵ
#define MAXTHROTTLE_LIMIT				1500+500*0.7 //����70%�����������ֵ
#define MAXTHROTTLE_LIMIT_INV 			1500-500*0.7 //����70%�����������ֵ
#define COMMANDER_MINCHECK 				1550 //������ֵ
#define COMMANDER_MINCHECK_INV 			1450 //������ֵ
#define COMMANDER_MINCHECK_RELATIVE 	25   //�������ŵ���ֵ���ֵ
#define COMMANDER_MINCHECK_RELATIVE_INV -25  //�������ŵ������ֵ

#define X_Max 320
#define Y_Max 240
#define X_Measure Coordinate_X-(X_Max/2)
#define Y_Measure Y_Max/2-Coordinate_Y
#define MS5837_BEGIN 0xAA		//��ȴ��������ݰ�ͷ

#define MOVE_FORWARD_STEP 40		//ǰ������
#define MOVE_SIDE_STEP		60		//��ƽ�Ʋ���
#define MOVE_SWERVE_STEP  50		//��ת����

#define DEPTH_MINCHECK   3  //��ˮ����

#define MOTO3_CCR htim12.Instance->CCR2
#define MOTO2_CCR htim12.Instance->CCR1
#define MOTO4_CCR htim2.Instance->CCR4
#define MOTO1_CCR htim2.Instance->CCR3
#define MOTO6_CCR htim3.Instance->CCR4
#define MOTO7_CCR htim3.Instance->CCR3
#define MOTO5_CCR htim3.Instance->CCR2
#define MOTO8_CCR htim3.Instance->CCR1

void Control(CMD_TYPE *CMD);
void Moto_Init(void);
void Moto_assign(uint8_t flag);
#endif

