#include "commander.h"

CMD_TYPE RC_mode;		//任务模式
CMD_TYPE TEST_mode;	//调试模式
CMD_TYPE MODE;
CCR_TYPE CCR_mode;

void Mode_Run(void)
{
	MODE = RC_mode;		//模式切换
	Control(&MODE);
}

void CCR_ModeInit()
{
	CCR_mode.CCR1 = 1500;
	CCR_mode.CCR2 = 1500;
	CCR_mode.CCR3 = 1500;
	CCR_mode.CCR4 = 1500;
	CCR_mode.CCR5 = 1500;
	CCR_mode.CCR6 = 1500;
}

void Mode_Init()
{
	CCR_mode.Set_flag 				= 1;
	TEST_mode.Enable_flag	    = 0;
	RC_mode.Enable_flag	    	= 0;
	
	TEST_mode.Yaw_Exp					= 0;
  TEST_mode.Depth_Exp       = 10;
  TEST_mode.Forward         = 0;
  TEST_mode.Side            = 0;
  TEST_mode.Swerve          = 0;
  TEST_mode.PID_YAW_flag	  = 1;
  TEST_mode.PID_ROL_flag	  = 1;
  TEST_mode.PID_Depth_flag  = 0;
  
	RC_mode.Yaw_Exp						= 0;
  RC_mode.Depth_Exp       	= 10;
  RC_mode.Forward         	= 0;
  RC_mode.Side            	= 0;
  RC_mode.Swerve          	= 0;
  RC_mode.PID_YAW_flag	  	= 0;
  RC_mode.PID_ROL_flag	  	= 1;
  RC_mode.PID_Depth_flag  	= 1;
	RC_mode.Version_Flag			= 1;
  
}                           
