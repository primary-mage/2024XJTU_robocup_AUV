#include "pid.h"


/*****************************************************************************
* 函  数：void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
* 功  能：位置式PID算法
* 参  数：PID: 算法P I D参数的结构体
*         target: 目标值
*         measure: 测量值 
* 返回值：无 
* 备  注: 角度环和角速度环共用此函数
*****************************************************************************/
void PID_Postion_Cal(PID_TYPE*PID,float target,float measure)
{
	
	PID->Error  = target - measure;              //误差
	PID->Differ = PID->Error - PID->PreError;    //微分量
	
	PID->Pout = PID->P * PID->Error;                        //比例控制
	PID->Iout = PID->Ilimit_flag * PID->I * PID->Integral;  //积分控制
	PID->Dout = PID->D * PID->Differ;                       //微分控制
	
	PID->OutPut =  PID->Pout + PID->Iout + PID->Dout;       //比例 + 积分 + 微分总控制
	
	
		if(absf(PID->Error)>PID->Ilimit)   //积分分离
		{PID->Ilimit_flag = 0;}
		else
		{
			PID->Ilimit_flag = 1;                               //加入积分(只有测量值在-PID->Ilimit~PID->Ilimit 范围内时才加入积分)
			PID->Integral += PID->Error;                        //对误差进行积分
			 containf(PID->Integral,PID->Irang);
			/*if(PID->Integral > PID->Irang)                      //积分限幅
				PID->Integral = PID->Irang;
			if(PID->Integral < -PID->Irang)                     //积分限幅
			    PID->Integral = -PID->Irang;   */                 
		}
			PID->PreError = PID->Error ;                            //前一个误差值
	}

/*****************************************************************************
* 函  数：void PidParameter_init(void)
* 功  能：初始化PID结构体里的一些成员值
* 参  数：无
* 返回值：无 
* 备  注: 由于PID参数我们都保存在FLASH中，所以此函数初始化时不用初始化这些参数，
*         但是Flash中的参数有可能因为误操作被擦除，如果Flash读取参数失败，则就
*         初始化为默认参数。
*****************************************************************************/
void PidParameter_init(void)
{
		
		PID_ROL_Angle.P =5;
    PID_ROL_Angle.I =0;
    PID_ROL_Angle.D =0.5;
		PID_ROL_Angle.Ilimit_flag = 0;//禁用积分分离
		PID_ROL_Angle.Irang = 120; //积分限幅
	
		PID_YAW_Angle.P =4;
    PID_YAW_Angle.I =0.01;
    PID_YAW_Angle.D =0.5;
		PID_YAW_Angle.Ilimit_flag = 0;//禁用积分分离
		PID_YAW_Angle.Irang = 40; //积分限幅

		PID_Depth.P =5;
    PID_Depth.I =0.00001;
    PID_Depth.D =0;
		PID_Depth.Ilimit_flag = 1;//启用积分分离
		PID_Depth.Ilimit = 10;
		PID_Depth.Irang = 80; //积分限幅
	
		PID_YAW_View.P =0.4;
    PID_YAW_View.I =0;
    PID_YAW_View.D =1;
		PID_YAW_View.Ilimit_flag = 0;//禁用积分分离
		PID_YAW_View.Irang = 40; //积分限幅
	
	
}
