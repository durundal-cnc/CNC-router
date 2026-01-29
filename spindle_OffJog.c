#include "KMotionDef.h"
#include "init_definitions.h"
#include "..\KflopToKMotionCNCFunctions.c"
#include "function_definitions.h" // load up all the definitions
#include "spindle_get_speed_from_adc.c" // turns spindle ADC counts to RPM

// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

double start_time;
double current_time;
float spindle_speed_reported;
char spindleRPM[80];

main()
{
	// spin down
	SetBit(SPINDLE_STARTING); //disable the speed match checks (will have commanded = 0 and actual = 14,000 and falling or whatever)
	ClearBit(SPINDLECW_BIT);
	ClearBit(SPINDLECCW_BIT);
	Jog(SPINDLEAXIS,0);
	printf("Jogging Spindle Stop\n");
	persist.UserData[STATEVAR] = 0;  // remember we are Off
	while (!CheckDone(SPINDLEAXIS)) {
		spindle_speed_reported = get_spindle_rpm_from_adc();		
		sprintf(spindleRPM, "%.0f", spindle_speed_reported);
		DROLabel(SPINDLESPEEDMEM,SPINDLESPEEDVAR,spindleRPM); //KFLOP memory location, KMotionCNC variable (?), value
		WaitNextTimeSlice();

	}
	start_time = Time_sec();
	current_time = Time_sec();
	DROLabel(SMALLMSGMEM,SMALLMSGVAR,"Spindle off\n"); //KFLOP memory location, KMotionCNC variable (?), value
	ClearBit(SPINDLE_STARTING);

	while(current_time < start_time ){ //+ 10)  { //leave chiller on for 10 seconds
		current_time = Time_sec();
		spindle_speed_reported = get_spindle_rpm_from_adc();		
		sprintf(spindleRPM, "%.0f", spindle_speed_reported);
		DROLabel(SPINDLESPEEDMEM,SPINDLESPEEDVAR,spindleRPM); //KFLOP memory location, KMotionCNC variable (?), value
		WaitNextTimeSlice();
	}
	ClearBit(AUXPOWER_OUT); // turn off aux plug (includes chiller)
}

