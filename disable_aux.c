#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// check for limit switches being triggered

void disable_aux(void) {
	ClearBit(AUXPOWER_OUT);
		
	return 0;
}