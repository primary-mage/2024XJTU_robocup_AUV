#include "stable.h"

//�ǶȻ�PID 
PID_TYPE PID_ROL_Angle;
PID_TYPE PID_PIT_Angle;
PID_TYPE PID_YAW_Angle;
//���ٶȻ�PID 
PID_TYPE PID_ROL_Rate;
PID_TYPE PID_PIT_Rate;
PID_TYPE PID_YAW_Rate;
//��Ȼ�PID
PID_TYPE PID_Depth;



void Stable(void)
{
	
	//�ǶȻ�
	PID_Postion_Cal(&PID_ROL_Angle,Angle_Expected.rol,Angle_Measure.rol);//ROLL�ǶȻ�PID ������Ƕ� ������ٶȣ�
	//PID_Postion_Cal(&PID_PIT_Angle,Angle_Expected.pit,Angle_Measure.pit);//PITH�ǶȻ�PID ������Ƕ� ������ٶȣ�

	//���ٶȻ�
/*PID_Postion_Cal(&PID_ROL_Rate,PID_ROL_Angle.OutPut,AngleRate_Measure.rol); //ROLL���ٶȻ�PID ������ǶȻ����������������������
	PID_Postion_Cal(&PID_PIT_Rate,PID_PIT_Angle.OutPut,AngleRate_Measure.pit); //PITH���ٶȻ�PID ������ǶȻ����������������������
*/
	//��Ȼ�
	PID_Postion_Cal(&PID_Depth,Depth_Expected,Depth_Measure);
}

