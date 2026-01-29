#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

char msgtext[160];

void update_velocity_DROs(void) 
{

	float VelX1,VelX2,VelY,VelZ;
	VelX1=ch3->last_vel;
	VelX2=ch2->last_vel;
	VelY=ch1->last_vel;
	VelZ=ch4->last_vel;
		
	sprintf(msgtext,"%.1f\n",VelX1);
	DROLabel(DRO_X1VEL_MEM,DRO_X1VEL_VAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
	sprintf(msgtext,"%.1f\n",VelX2);
	DROLabel(DRO_X2VEL_MEM,DRO_X2VEL_VAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
	sprintf(msgtext,"%.1f\n",VelY);
	DROLabel(DRO_YVEL_MEM,DRO_YVEL_VAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
	sprintf(msgtext,"%.1f\n",VelZ);
	DROLabel(DRO_ZVEL_MEM,DRO_ZVEL_VAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value

}
