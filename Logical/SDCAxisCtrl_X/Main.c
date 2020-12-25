
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void increase_counters(void);

void _INIT ProgramInit(void)
{
	//Устанваливаем типы SDC модулей
	AxisX_HW.EncIf1_Typ = ncSDC_ENC16;
	AxisX_HW.DiDoIf_Typ = ncSDC_DIDO;
	AxisX_HW.DrvIf_Typ = ncSDC_DRVSERVO16;
	
	//Устанавливаем имена переменных
	strcpy(AxisX_HW.EncIf1_Name, "AxisX_EncIf");
	strcpy(AxisX_HW.DrvIf_Name, "AxisX_DrvIf");
	strcpy(AxisX_HW.DiDoIf_Name, "AxisX_DiDoIf");
		
	//Устанавливаем входы готовности и нормальной работы
	AxisX_EncIf.iEncOK = 1;
	AxisX_DrvIf.iDrvOK = 1;
	AxisX_DrvIf.iStatusEnable = 1;
	AxisX_DiDoIf.iDriveReady = 1;

	fb_controller.dt = 0.002;
	fb_controller.k_i = 0.16;
	fb_controller.k_p = 0.0064;
	fb_controller.max_abs_value = 24.0;
	
	pwm_period = 200;
}

void _CYCLIC ProgramCyclic(void)
{
	if (coil_powered)
	{
		FB_Axis(&axis_X);
		coil_pwm_value = 32767;
	
		increase_counters();
	
		if (boost)
		{
			desired_speed = 96;
		}
		else
		{ 
			desired_speed = 60;
		}
		if (reference)
		{
			if (axis_X.endswitch_a_reached == 1 || axis_X.endswitch_b_reached == 1)
			{
				axis_X.pwm_value = 0;
				coil_pwm_value = 0;
				desired_speed = 0;
				axis_X.reset_counter = 1;
			}
			else
			{
				//axis_X.reset_counter = 0;
				//Формирование показаний датчика
				AxisX_EncIf.iActTime = (INT)	AsIOTimeCyclicStart();
				AxisX_EncIf.iActPos = axis_X.counter;
				AxisX_DiDoIf.iPosHwEnd = axis_X.endswitch_b_reached;
				AxisX_DiDoIf.iNegHwEnd = axis_X.endswitch_a_reached;
					
				FB_Controller(&fb_controller);
				
				fb_controller.in = desired_speed - axis_X.speed;	//AxisX_DrvIf.oSetPos;
				fb_controller.in = fb_controller.in / 120 * 32767 /* transfer in units/s */;
				if((fb_controller.in<min_task)&&(fb_controller.in>(-min_task))) fb_controller.in = 0; 
				axis_X.in = fb_controller.out;
				//FB_Axis(&Axis_X);
			}
		}
		else
		{
			if (!direction)
			{
				if (axis_X.endswitch_b_reached == 1)
				{
					axis_X.pwm_value = 0;
					coil_pwm_value = 0;
					desired_speed = 0;
				}
				else
				{
					axis_X.reset_counter = 0;
					//Формирование показаний датчика
					AxisX_EncIf.iActTime = (INT)	AsIOTimeCyclicStart();
					AxisX_EncIf.iActPos = axis_X.counter;
					AxisX_DiDoIf.iPosHwEnd = axis_X.endswitch_b_reached;
					AxisX_DiDoIf.iNegHwEnd = axis_X.endswitch_a_reached;
				
					FB_Controller(&fb_controller);
				
					fb_controller.in = (REAL)desired_speed - axis_X.speed;	//AxisX_DrvIf.oSetPos;
					
					fb_controller.in = fb_controller.in / 120 * 32767 /* transfer in units/s */;
					if((fb_controller.in<min_task)&&(fb_controller.in>(-min_task))) fb_controller.in = 0; 
					axis_X.in = fb_controller.out;
					//FB_Axis(&Axis_X);
				}
			}
			else
			{
				if (axis_X.endswitch_a_reached == 1)
				{
					axis_X.pwm_value = 0;
					coil_pwm_value = 0;
					desired_speed = 0;
				}
				else
				{
					axis_X.reset_counter = 0;
					//Формирование показаний датчика
					AxisX_EncIf.iActTime = (INT)	AsIOTimeCyclicStart();
					AxisX_EncIf.iActPos = axis_X.counter;
					AxisX_DiDoIf.iPosHwEnd = axis_X.endswitch_b_reached;
					AxisX_DiDoIf.iNegHwEnd = axis_X.endswitch_a_reached;
				
					FB_Controller(&fb_controller);
				
					fb_controller.in = (REAL)-desired_speed - axis_X.speed;	//AxisX_DrvIf.oSetPos;
					 
					fb_controller.in = fb_controller.in / 120 * 32767/* transfer in units/s */;
					if((fb_controller.in<min_task)&&(fb_controller.in>(-min_task))) fb_controller.in = 0;
					axis_X.in = fb_controller.out;
					//FB_Axis(&Axis_X);
				}
			}
		}
	}
	else
	{
		axis_X.pwm_value = 0;
		coil_pwm_value = 0;
		desired_speed = 0;
	}
}

void increase_counters(void)
{
	AxisX_EncIf.iLifeCnt++;
	AxisX_DiDoIf.iLifeCntDriveEnable++; 
	AxisX_DiDoIf.iLifeCntDriveReady++; 
	AxisX_DiDoIf.iLifeCntNegHwEnd++; 
	AxisX_DiDoIf.iLifeCntPosHwEnd++; 
	AxisX_DiDoIf.iLifeCntReference++;
	AxisX_DrvIf.iLifeCnt++;
}

void _EXIT ProgramExit(void)
{
	// Insert code here 

}

