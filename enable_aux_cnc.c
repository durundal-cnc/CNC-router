#include "KMotionDef.h"
#include "enable_aux.c"

main()
{
	enable_aux(); // disable servo bits and axes with message
	// MessageBox("test"); // can send message to kmotioncnc but not sure how to do yet
}
