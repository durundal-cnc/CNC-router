#include "KMotionDef.h"
#include "init_definitions.h"
#include "read_flow_meter.c"
#include "abs.c"
#include "..\KflopToKMotionCNCFunctions.c"
#include "spindle_get_speed_from_adc.c" // turns spindle ADC counts to RPM

int   *css_mode = &persist.UserData[PC_COMM_CSS_MODE];			// Mode 1=Normal RPM mode. 2=CSS
double start_time;
double current_time;
float flowrate;
float spindle_speed_reported;
float spindle_speed; //float as defined in axis struct kmotiondef.h
int i = 0;
// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

main()
{
	float speed = *(float *)&persist.UserData[SPEEDVAR];  // value stored is actually a float 
	float LastState = persist.UserData[STATEVAR];  // get last state 
	char msgtext[160];
	

	if (LastState==-1)  
	{
		// if spindle was CCW now we want CW 
		// spin down
		
		ClearBit(SPINDLECW_BIT);
		ClearBit(SPINDLECCW_BIT);
		Jog(SPINDLEAXIS,0);
		while (!CheckDone(SPINDLEAXIS)) ;
	}

//Disabled for now (chiller not on that circuit)	
/* 	SetBit(AUXPOWER_OUT); // turn on aux plug (includes chiller)
	start_time = Time_sec();
	current_time = Time_sec();
	while(current_time < start_time + 2)  { // give chiller two seconds to start up
		current_time = Time_sec();
		WaitNextTimeSlice();
	} */
	
	flowrate = read_flow_meter();
	if (flowrate > FLOWRATE_MIN) { //if flowing, good to start spindle
	
		// turn spindle on CW and ramp to new speed
		SetBit(SPINDLECW_BIT);
		
		LastState = 1;          														 //  Add string
		
		
		if (*css_mode != 2) //css is constant surface speed, for lathes
		{
			// spindle is already on, so ramp to new speed
			if (USE_POS_NEG_VOLTAGE)
				Jog(SPINDLEAXIS,speed * FACTOR * LastState);
			else
				Jog(SPINDLEAXIS,speed * FACTOR);
			printf("Jogging Spindle %f counts/sec\n",speed * FACTOR);
		}
		persist.UserData[STATEVAR] = 1;  // remember we are CW
		
		
		// Wait for spindle to spin up before proceeding with cut
		SetBit(SPINDLE_STARTING);
		do {
			//spindle_speed = &chan[0].Output;//
			//spindle_speed = -1*(DAC_Buffer[SPINDLE_CONTROL_AO]-2048);
			spindle_speed = *(float *)&persist.UserData[SPEEDVAR];  // value stored is actually a float (commanded speed in RPM)
			spindle_speed_reported = get_spindle_rpm_from_adc();
			
			sprintf(msgtext,"Spindle %0.1f actual %0.1f\n",spindle_speed, spindle_speed_reported);
			DROLabel(SMALLMSGMEM,SMALLMSGVAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
			i++;
			if (i == 500) {
				printf("speed out %f speed reported %f\n",spindle_speed, spindle_speed_reported);
				i = 0;
			}
			WaitNextTimeSlice();

		} while ( abs(spindle_speed_reported - spindle_speed) > 500 );	//While the difference between commanded and reported is > 500rpm

		// Wait for two seconds after coming up to reported speed (might omit this if the RPM reporting works now)
		start_time = Time_sec();
		current_time = Time_sec();
		while(current_time < start_time + 2) {
			current_time = Time_sec();
			WaitNextTimeSlice();
		}
		
		ClearBit(SPINDLE_STARTING);

		printf("final speed commanded %f speed reported %f\n",spindle_speed, spindle_speed_reported);
		return 0;
	} else {
		SetBit(FEEDHOLDBIT); // prevent it from continuing
		printf("Spindle cooling flow not sufficient, not started\n");
		DROLabel(SMALLMSGMEM,SMALLMSGVAR,"Spindle cooling flow not sufficient, not started\n"); //KFLOP memory location, KMotionCNC variable (?), value
		return 1;
	}
	

}

