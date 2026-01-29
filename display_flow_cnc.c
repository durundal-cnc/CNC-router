#include "KMotionDef.h"
#include "init_definitions.h"
#include "display_flow_cnc.c"

char text[80];

main()
{
	float spindle_flow_rate = persist.UserData[FLOWVAR];  // get last state 
	sprintf(text,"Spindle flow rate: %f liters/min\n", spindle_flow_rate);
	MsgBoxNoWait(text,MB_OK);
}
