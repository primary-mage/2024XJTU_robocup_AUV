#include "stable.h"

//角度环PID 
PID_TYPE PID_ROL_Angle;
PID_TYPE PID_PIT_Angle;
PID_TYPE PID_YAW_Angle;
//角速度环PID 
PID_TYPE PID_ROL_Rate;
PID_TYPE PID_PIT_Rate;
PID_TYPE PID_YAW_Rate;
//深度环PID
PID_TYPE PID_Depth;



void Stable(void)
{
	
	//角度环
	PID_Postion_Cal(&PID_ROL_Angle,Angle_Expected.rol,Angle_Measure.rol);//ROLL角度环PID （输入角度 输出角速度）
	//PID_Postion_Cal(&PID_PIT_Angle,Angle_Expected.pit,Angle_Measure.pit);//PITH角度环PID （输入角度 输出角速度）

	//角速度环
/*PID_Postion_Cal(&PID_ROL_Rate,PID_ROL_Angle.OutPut,AngleRate_Measure.rol); //ROLL角速度环PID （输入角度环的输出，输出电机控制量）
	PID_Postion_Cal(&PID_PIT_Rate,PID_PIT_Angle.OutPut,AngleRate_Measure.pit); //PITH角速度环PID （输入角度环的输出，输出电机控制量）
*/
	//深度环
	PID_Postion_Cal(&PID_Depth,Depth_Expected,Depth_Measure);
}

