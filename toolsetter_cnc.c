#include "KMotionDef.h"
#include "bypass_proximity_sensors.c"
#include "init_definitions.h" // this loads up the mapping of input/output channels and bits to names
//#include "..\KflopToKMotionCNCFunctions.c"

main()
{
	char text[1000];

	if (ReadBit(TOOLSETTER_ACTIVE)){
		sprintf(text,"Toolsetter canceled");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Toolsetter canceled"); //KFLOP memory location, KMotionCNC variable (?), value
		ClearBit(TOOLSETTER_ACTIVE);
	} else {
		sprintf(text,"Toolsetter active, move bit to toolsetter top");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Toolsetter active, move bit to toolsetter top"); //KFLOP memory location, KMotionCNC variable (?), value
		SetBit(TOOLSETTER_ACTIVE); // start servicing toolsetter
	}
	MsgBoxNoWait(text,MB_OK);
}
