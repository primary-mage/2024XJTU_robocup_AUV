#include "control.h"

//角度环PID 
PID_TYPE PID_ROL_Angle;
PID_TYPE PID_PIT_Angle;
PID_TYPE PID_YAW_Angle;
PID_TYPE PID_YAW_View;
//角速度环PID 
PID_TYPE PID_ROL_Rate;
PID_TYPE PID_PIT_Rate;
PID_TYPE PID_YAW_Rate;
//深度环PID
PID_TYPE PID_Depth;

float Moto_PWM_1=0.0f,Moto_PWM_2=0.0f,Moto_PWM_3=0.0f,Moto_PWM_4=0.0f,Moto_PWM_5=0.0f,Moto_PWM_6=0.0f;


void Control(CMD_TYPE *CMD)
{
	PID_Postion_Cal(&PID_ROL_Angle,0,Angle_Measure.rol);//ROLL角度环 
	PID_Postion_Cal(&PID_Depth,CMD->Depth_Exp,Depth_Measure);//深度环
//	PID_Postion_Cal(&PID_YAW_View,0,X_Measure);//视觉环
//	RC_mode.Yaw_Exp = RC_mode.Yaw_Exp+PID_YAW_View.OutPut;
	PID_Postion_Cal(&PID_YAW_Angle,CMD->Yaw_Exp,Angle_Measure.yaw);//YAW角度环
	
	Moto_PWM_1=MINTHROTTLE - MOVE_FORWARD_STEP * CMD->Forward + MOVE_SIDE_STEP * CMD->Side + MOVE_SWERVE_STEP * CMD->Swerve + PID_YAW_Angle.OutPut * CMD->PID_YAW_flag + PID_YAW_View.OutPut * RC_mode.Version_Flag;
	Moto_PWM_2=MINTHROTTLE - MOVE_FORWARD_STEP * CMD->Forward - MOVE_SIDE_STEP * CMD->Side - MOVE_SWERVE_STEP * CMD->Swerve - PID_YAW_Angle.OutPut * CMD->PID_YAW_flag - PID_YAW_View.OutPut * RC_mode.Version_Flag;
	Moto_PWM_3=MINTHROTTLE + MOVE_FORWARD_STEP * CMD->Forward - MOVE_SIDE_STEP * CMD->Side + MOVE_SWERVE_STEP * CMD->Swerve + PID_YAW_Angle.OutPut * CMD->PID_YAW_flag + PID_YAW_View.OutPut * RC_mode.Version_Flag;
	Moto_PWM_4=MINTHROTTLE + MOVE_FORWARD_STEP * CMD->Forward + MOVE_SIDE_STEP * CMD->Side - MOVE_SWERVE_STEP * CMD->Swerve - PID_YAW_Angle.OutPut * CMD->PID_YAW_flag - PID_YAW_View.OutPut * RC_mode.Version_Flag;
	Moto_PWM_5=MINTHROTTLE - PID_Depth.OutPut * CMD->PID_Depth_flag + PID_ROL_Angle.OutPut - PID_Depth.OutPut * CMD->PID_Depth_flag;                                                                  
	Moto_PWM_6=MINTHROTTLE - PID_Depth.OutPut * CMD->PID_Depth_flag - PID_ROL_Angle.OutPut - PID_Depth.OutPut * CMD->PID_Depth_flag;

	Moto_PWM_1=Moto_Constrain(Moto_PWM_1 );
	Moto_PWM_2=Moto_Constrain(Moto_PWM_2 );
	Moto_PWM_3=Moto_Constrain(Moto_PWM_3 );
	Moto_PWM_4=Moto_Constrain(Moto_PWM_4 );
	Moto_PWM_5=Moto_Constrain(Moto_PWM_5 );
	Moto_PWM_6=Moto_Constrain(Moto_PWM_6 );
	
	Moto_assign(CMD->Enable_flag);
}

void Moto_assign(uint8_t flag)
{
	if(flag && !CCR_mode.Set_flag)
	{
		MOTO1_CCR = Moto_PWM_1;
    MOTO2_CCR = Moto_PWM_2;
    MOTO3_CCR = Moto_PWM_3;
    MOTO4_CCR = Moto_PWM_4;
    MOTO5_CCR = Moto_PWM_5;
    MOTO6_CCR = Moto_PWM_6;

	}
	if(CCR_mode.Set_flag)
	{
		MOTO1_CCR = CCR_mode.CCR1;
		MOTO2_CCR = CCR_mode.CCR2;
		MOTO3_CCR = CCR_mode.CCR3;
		MOTO4_CCR = CCR_mode.CCR4;
		MOTO5_CCR = CCR_mode.CCR5;
		MOTO6_CCR = CCR_mode.CCR6;
	}

}

void Moto_Init(void)
{
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim12,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	
	MOTO1_CCR = MINTHROTTLE;
	MOTO2_CCR = MINTHROTTLE;
	MOTO3_CCR = MINTHROTTLE;
	MOTO4_CCR = MINTHROTTLE;
	MOTO5_CCR = MINTHROTTLE;
	MOTO6_CCR = MINTHROTTLE;
	
}

