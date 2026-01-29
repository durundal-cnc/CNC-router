#include "KMotionDef.h"
#include "check_temperatures.c"
#include "read_flow_meter.c"
#include "init_definitions.h"
#include "..\KflopToKMotionCNCFunctions.c"


float flowrate;
char text[160];
main()
{
	flowrate = read_flow_meter();
	
	if(flowrate < FLOWRATE_MIN && ReadBit(AUXPOWER_OUT)) { // only read flow rate if pump is supposed to be on (aux port)
		printf("Flow rate too low!: %f liters/min\n", flowrate);
		sprintf(text, "Flow rate too low!: %f liters/min\n", flowrate);
		MsgBoxNoWait(text,MB_OK);
	} else {
		printf("Flow rate: %f liters/min\n", flowrate);
		sprintf(text, "Flow rate: %f liters/min\n", flowrate);
		MsgBoxNoWait(text,MB_OK);
	}
}
