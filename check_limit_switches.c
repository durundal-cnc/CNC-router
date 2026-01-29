#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// check for limit switches being triggered

void check_limit_switches(void) {
	if(!PROXSENSORBYPASS) {
		// X motors
		if (ReadBit(XPROXPOS)) {
			if(ReadBit(X1MASTER_ENABLE)) {
				printf("feedhold from X positive proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from X positive proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}
			SetBit(X1MASTER_ENABLE); // tells servo to stop (grounding line == stop for DMMs)
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}
		
		if (ReadBit(XPROXNEG)) {
			if(ReadBit(X1MASTER_ENABLE)) {
				printf("feedhold from X negative proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from X negative proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}	
			SetBit(X1MASTER_ENABLE); // tells servo to stop
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}		
		
		// Y motor (FET grounding behavior for Leadshine is different than DMM motors)
		if (ReadBit(YPROXPOS)) {
			if(ReadBit(LEADSHINE_ENABLE)) {
				printf("feedhold from Y positive proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from Y positive proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}
			ClearBit(LEADSHINE_ENABLE); // tells servo to stop
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}
		
		if (ReadBit(YPROXNEG)) {
			if(ReadBit(LEADSHINE_ENABLE)) {
				printf("feedhold from Y negative proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from Y negative proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}	
			ClearBit(LEADSHINE_ENABLE); // tells servo to stop
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}	
		
		// Z motor
		if (ReadBit(ZPROXPOS)) {
			if(ReadBit(Z_ENABLE)) {
				printf("feedhold from Z positive proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from Z positive proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}
			SetBit(Z_ENABLE); // tells servo to stop
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}
		
		if (ReadBit(ZPROXNEG)) {
			if(ReadBit(Z_ENABLE)) {
				printf("feedhold from Z negative proximity sensor\n"); // only print if servos on (ie, not if loading stock nearby it)
				DROLabel(MESSAGEMEM,MESSAGEVAR,"feedhold from Z negative proximity sensor\n"); //KFLOP memory location, KMotionCNC variable (?), value

			}	
			SetBit(Z_ENABLE); // tells servo to stop
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		}		
		
		
		
		
		//repeat for other proximity sensors when installed
	
	}
	return 0;
}
