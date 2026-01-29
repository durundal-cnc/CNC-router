#include "KMotionDef.h"
#include "disable_aux.c"

main()
{
	disable_aux(); // disable servo bits and axes with message
	// MessageBox("test"); // can send message to kmotioncnc but not sure how to do yet
}
