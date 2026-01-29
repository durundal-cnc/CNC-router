#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// check for limit switches being triggered

void enable_aux(void) {
	SetBit(AUXPOWER_OUT);
		
	return 0;
}