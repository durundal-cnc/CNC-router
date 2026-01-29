#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// check for limit switches being triggered

void startup_configuration(void)
{
	
// Configure KFLOP to service Konnect 32 Input 16 output IO board
// Board address is 0, 
// 16 Outputs are mapped to Virtual IO 48-63 (VirtualBits)
// 32 Inputs are mapped to Virtual IO 1024-1055 (VirtualBits[0])
	InitAux();
	AddKonnect(0,&VirtualBits,VirtualBitsEx);
	FPGAW(ENC_NOISE_FILTER_ADD)=7; // this is defaulted to 7, and that will cause lost encoder pulses as of 6-16-18. Fixed issue with grounds later so leave at default.
// Fixed by rewiring - now using default as of 6-25-18


// define axis chan numbers to use as x,y,z,a (set -1 to disable)
	DefineCoordSystem(XAXIS, YAXIS, ZAXIS, -1);

	
	SetBit(FEEDHOLDBIT); // startup in a feedhold
	ClearBit(PROXSENSORBYPASS); // startup with proximty sensor checks enabled
	SetBit(MAINPOWER_ENA); // allows start button e-stop chain to complete. Turns on main contactor SSR.
	
	//Set unused axes to be explicitly disabled
	ch5->InputMode=NO_INPUT_MODE;
	ch5->OutputMode=NO_OUTPUT_MODE;
	ch6->InputMode=NO_INPUT_MODE;
	ch6->OutputMode=NO_OUTPUT_MODE;
	ch7->InputMode=ENCODER_MODE;  // for MPG
	ch7->OutputMode=NO_OUTPUT_MODE;
	ch7->InputChan0=MPG_INPUT_AXIS; 
	ch7->InputChan1=0;
	
	DAC(SPINDLE_CONTROL_AO,0);
	DAC(XAXIS1_CONTROL_AO,0);
	DAC(XAXIS2_CONTROL_AO,0);
	DAC(YAXIS_CONTROL_AO,0);
	DAC(ZAXIS_CONTROL_AO,0);

	//Configure FET switches on Kanalog, starting bit 152 ending bit 159
	ClearBit(LEADSHINE_ENABLE); // bit 152
	SetBit(X1MASTER_ENABLE); // motor is disabled when FET connects to ground
	SetBit(X2SLAVE_ENABLE);	 // motor is disabled when FET connects to ground
	SetBit(Z_ENABLE);		// motor is disabled when FET connects to ground
	ClearBit(SERVO_LAMP_SW);
	ClearBit(FEEDHOLD_LAMP_SW);
	ClearBit(SPINDLECCW_BIT);
	ClearBit(SPINDLECW_BIT); 
	ClearBit(AUXPOWER_OUT);
	ClearBit(TOOLSETTER_ACTIVE);
	
	return 0;
}
