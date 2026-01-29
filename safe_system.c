#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// performs safing actions to de-energize system
// this is called for emergencies (eg, main relay contact opens)
// Use graceful shutdown procedures for everyday stops to let axes finish motion and spindle stop, etc.


void safe_system(void)
{
	StopCoordinatedMotion();
	//disable all axes (servo controller stops ASAP)
	DisableAxis(0);
	DisableAxis(1);
	DisableAxis(2);
	DisableAxis(3);
	DisableAxis(4);
	DisableAxis(5);
	DisableAxis(6);
	DisableAxis(7);	
	
	
	ClearBit(AUXPOWER_OUT);		//disable auxiliary 120V outlets (unless bypass switch is on)
	//might want to add some checking for temperature for spindle coolant?
	ClearBit(LEADSHINE_ENABLE);	//disable servo enable signal (servo controller brings to quick stop)
	SetBit(X1MASTER_ENABLE);	// motor is disabled when FET connects to ground
	SetBit(X2SLAVE_ENABLE);		// motor is disabled when FET connects to ground
	SetBit(Z_ENABLE);			// motor is disabled when FET connects to ground
	
	
	ClearBit(SPINDLECCW_BIT);	//disable the spindle motion in reverse
	ClearBit(SPINDLECW_BIT);	//disable the spindle motion in forward
	
	// clear all the analog outs just in case
	DAC(SPINDLE_CONTROL_AO,0);
	DAC(XAXIS1_CONTROL_AO,0);
	DAC(XAXIS2_CONTROL_AO,0);
	DAC(YAXIS_CONTROL_AO,0);
	DAC(ZAXIS_CONTROL_AO,0);

	// set bits as appropriate
	SetBit(FEEDHOLDBIT);


	// print a message
	printf("**********************************\n");
	printf("* Emergency stop! Safing system! *\n");
	printf("**********************************\n");

	return 0;
}
