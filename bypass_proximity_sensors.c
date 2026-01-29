#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"
#include "..\KflopToKMotionCNCFunctions.c"

// check for limit switches being triggered

void bypass_proximity_sensors(int enable_disable) {

	if (enable_disable == 0) { // don't bypass

		ClearBit(PROXSENSORBYPASS);
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Inductive proximity sensors active.\n"); //KFLOP memory location, KMotionCNC variable (?), value

	//	if (disp_message) {
			printf("Inductive proximity sensors active.\n");
	//	}
	}
	
	
	if (enable_disable == 1){ // bypass
		// first check that main power is already applied - if not breaker will trip on startup if servos enabled

		SetBit(PROXSENSORBYPASS);
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Inductive proximity sensors bypassed.\n"); //KFLOP memory location, KMotionCNC variable (?), value

	//	if (disp_message) {
			printf("Inductive proximity sensors bypassed.\n");
	//	}
	} 

		
	return 0;
}