#include "KMotionDef.h"
#include "bypass_proximity_sensors.c"

main()
{
	bypass_proximity_sensors(1); // disable servo bits and axes with message
	// MessageBox("test"); // can send message to kmotioncnc but not sure how to do yet
}
