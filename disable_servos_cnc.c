#include "KMotionDef.h"
#include "enable_servos.c"
#include "..\KflopToKMotionCNCFunctions.c"

main()
{
	enable_servos(0,1); // Enable servo bits and axes
	// MessageBox("test"); // can send message to kmotioncnc but not sure how to do yet
}
