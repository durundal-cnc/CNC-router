#include "KMotionDef.h"

#include "spindle_MySpindleDefs.h"
#include "abs.c"
#include "..\KflopToKMotionCNCFunctions.c"
#include "spindle_get_speed_from_adc.c" // turns spindle ADC counts to RPM

int   *css_mode = &persist.UserData[PC_COMM_CSS_MODE];			// Mode 1=Normal RPM mode. 2=CSS
float spindle_speed_reported, spindle_speed;
char msgtext[160];
int i;

// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

main()
{
	float speed = *(float *)&persist.UserData[KMVAR];  // value stored is actually a float 
	float LastState = persist.UserData[STATEVAR];  // get last state 
	
	persist.UserData[SPEEDVAR] = persist.UserData[KMVAR];  // Always save the last desired speed 
	
	if (LastState==0 || *css_mode == 2)  
	{
		// if spindle is off (or CSS mode) and User Changes the speed 
		// just save the desired speed
		
		return 0;
	}
	
	// spindle is already on, so ramp to new speed
	if (USE_POS_NEG_VOLTAGE)
		Jog(SPINDLEAXIS,speed * FACTOR * LastState);
	else
		Jog(SPINDLEAXIS,speed * FACTOR);
		
		
	// Wait for spindle to spin up before proceeding with cut
	SetBit(SPINDLE_STARTING);
	do {
		spindle_speed = *(float *)&persist.UserData[SPEEDVAR];  // value stored is actually a float (commanded speed in RPM)
		spindle_speed_reported = get_spindle_rpm_from_adc();
		
		sprintf(msgtext,"Change RPM %0.1f actual %0.1f\n",spindle_speed, spindle_speed_reported);
		DROLabel(SMALLMSGMEM,SMALLMSGVAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
		i++;
		if (i == 500) {
			printf("speed out %f speed reported %f\n",spindle_speed, spindle_speed_reported);
			i = 0;
		}
		WaitNextTimeSlice();

	} while ( abs(spindle_speed_reported - spindle_speed) > 500 );	//While the difference between commanded and reported is > 500rpm

	
	printf("Jogging Spindle %f counts/sec\n",speed * FACTOR);
}
