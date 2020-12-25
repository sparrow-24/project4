
#include <bur/plctypes.h>
#ifdef __cplusplus
	extern "C"
	{
#endif
	#include "Library.h"
#ifdef __cplusplus
	};
#endif

void FB_Axis(struct FB_Axis* inst)
{
	if (inst->i % 1001 == 0)
	{
		inst->speed = (REAL) (inst->counter - inst->last_counter);
		//inst->speed = inst->speed;
		inst->last_counter = inst->counter;
		inst->i = 1;
	}
	//inst->speed = (REAL) (inst->counter - inst->last_counter);
	//inst->speed = inst->speed * 62500; // * 3000 / 24.0) / 0.002
	//inst->last_counter = inst->counter;
	inst->pwm_value = (INT)(inst->in * 1365.3);//  / 24.0 * 32767
	inst->pwm_percentage = (SINT)(inst->in * 4.2);// / 24 * 100 
	inst->i++;
}
