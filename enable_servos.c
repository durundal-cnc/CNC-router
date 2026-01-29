#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"
//#include "..\KflopToKMotionCNCFunctions.c"

// check for limit switches being triggered

void enable_servos(int enable_disable, int disp_message) {

	if (enable_disable == 0) { // disable
		SetBit(X1MASTER_ENABLE);
		DisableAxis(XAXIS);
		
		SetBit(X2SLAVE_ENABLE);
		DisableAxis(XAXISSLAVE);
	
		ClearBit(LEADSHINE_ENABLE);
		DisableAxis(YAXIS);

		SetBit(Z_ENABLE);
		DisableAxis(ZAXIS);

		DisableAxis(SPINDLEAXIS);
		ClearBit(SPINDLECCW_BIT);
		ClearBit(SPINDLECW_BIT);
		
		if (disp_message) {
			printf("Servos off.\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Servos off.\n"); //KFLOP memory location, KMotionCNC variable (?), value

		}
	}
	
	
	if (enable_disable == 1 && ReadBit(MAINPOWERSTATUSBIT)){ // enable only if main power is on
		// first check that main power is already applied - if not breaker will trip on startup if servos enabled

		Delay_sec(0.150); // wait 150ms to ensure drives will have booted after main power applied (even if applying at same time)
		ClearBit(X1MASTER_ENABLE);
		EnableAxis(XAXIS);
		
		ClearBit(X2SLAVE_ENABLE);
		EnableAxis(XAXISSLAVE);
	
		SetBit(LEADSHINE_ENABLE);
		EnableAxis(YAXIS);

		ClearBit(Z_ENABLE);
		EnableAxis(ZAXIS);
		
		EnableAxis(SPINDLEAXIS); // still needs M3 or M4 commands/FET swiches to turn on spindle
		
		if (disp_message) {
			printf("Servos on.\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Servos on.\n"); //KFLOP memory location, KMotionCNC variable (?), value

		}
	} 
	
	if (enable_disable == 1 && !ReadBit(MAINPOWERSTATUSBIT) && disp_message){ // attempting to turn servos on with main power off
		printf("Main power not on, verify all servos off and turn main power on.\n");
		printf("If any servos enabled while cycling on main power can result in breaker trip.\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Main power not on, verify all servos off and turn main power on.\nIf any servos enabled while cycling on main power can result in breaker trip.\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
		
	return 0;
}