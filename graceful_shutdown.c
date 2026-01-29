#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// performs graceful shutdown with checks to make sure not in the middle of anything
// this is what should be used for typical shutdowns

int graceful_shutdown(void)
{
	
	for (i = 0; i==7; i++;) { 		// check every axis to make sure it is either disabled for finished moving
		if(CheckDone(i) != 0) {		// returns 1 if axis is Done, 0 if not, -1 if axis is disabled
			printf("Found that axis channel %d wasn't finished moving\n", %i);
			return 0;
			end; // syntax issue
		}
	}
	
	StopCoordinatedMotion(); 	//feedhold all axes

	
	ClearBit(SPINDLE_REV_SW);	//disable the spindle motion in reverse
	ClearBit(SPINDLE_FWD_SW);	//disable the spindle motion in forward

	while (spindleanalogin > [some value]) {
		printf("spindle speed = %f", spindleanalaogin);
		//Wait for spindown
	}
	
	
	//might want to add some checking for temperature for spindle coolant?
	ClearBit(SERVOENABLE_SW);	//disable servo enable signal (servo controller brings to quick stop)

	while (time < 5 minutes) {
		printf("countdown of spindle coolant before turn off");
	}
	ClearBit(AUXPOWER_SW);		//disable auxiliary 120V outlets (unless bypass switch is on)

	
	// clear all the analog outs just in case
	DAC(SPINDLE_CONTROL_AO,0);
	DAC(XAXIS1_CONTROL_AO,0);
	DAC(XAXIS2_CONTROL_AO,0);
	DAC(YAXIS_CONTROL_AO,0);
	DAC(ZAXIS_CONTROL_AO,0);

	// set lamps as appropriate
	ClearBit(FEEDHOLD_LAMP_SW);
	ClearBit(PROGRAM_LAMP_SW);

	// set bits as appropriate
	SetBit(FEEDHOLDBIT);


	// print a message
	printf("* Graceful shutdown completed.\n");

	return 1;
}
