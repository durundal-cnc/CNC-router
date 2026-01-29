#include "KMotionDef.h"
#include "bypass_proximity_sensors.c" //bypass_proximity_sensors.c has both enable and disable inputs to function. _cnc is for the button command in KmotionCNC.

main()
{
	bypass_proximity_sensors(0); // disable servo bits and axes with message
	// MessageBox("test"); // can send message to kmotioncnc but not sure how to do yet
}
