#include "KMotionDef.h"
#include "init_definitions.h"
//#include "read_flow_meter.c"


//float spindle_rpm from main

// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

//This checks that the spindle is not slowing down due to loading. If it is, a feedhold is called.

//A fancier version could throttle down the feed rate until the speeds match, then increase it again

void service_spindle(void)
{
	float spindle_speed = *(float *)&persist.UserData[SPEEDVAR];  // value stored is actually a float (commanded speed in RPM)
	//float spindle_speed = DAC_Buffer[SPINDLE_CONTROL_AO];//DAC speed (	extern short int DAC_Buffer[N_DACS]; // format 12 bits data)

	float LastState = persist.UserData[STATEVAR];  // get last state 
	
	static char spindlemsgtext[80];
	static int trigger = 0; //only to restrict printing for debugging
	static float time_trigger = 0;

	
	float spindle_speed_reported = get_spindle_rpm_from_adc(); //convert the ADC counts to RPM
	char spindleRPM[80];
	sprintf(spindleRPM, "%.0f", spindle_speed_reported);
	DROLabel(SPINDLESPEEDMEM,SPINDLESPEEDVAR,spindleRPM); //KFLOP memory location, KMotionCNC variable (?), value

	
	if (LastState!=0)  		// Only check if the spindle is moving
	{
			
/*	Just for debugging
		if (Time_sec() > time_trigger){ // if current time is past the time to trigger
			trigger = 1;
			time_trigger = Time_sec() + 1; //reset the trigger
		}

		if (trigger == 1) {
			trigger = 0;
			//printf("reported %f commanded %f\n", spindle_speed_reported, spindle_speed);
		}
*/	
		// Check that the spindle isn't bogging down in the work
		// needs to know if the spindle is just starting or not (otherwise has mismatch on startup), so check SPINDLE_STARTING
		if (abs(spindle_speed) - abs(spindle_speed_reported) > 1000 && !ReadBit(SPINDLE_STARTING)) { //XJ200 only reports positive spindle speed regardless of direction
			printf("Spindle bogging down, reported %f commanded %f\n", spindle_speed_reported, spindle_speed);
			sprintf(spindleRPM, "SM %.0f\n", spindle_speed_reported);
			DROLabel(SPINDLESPEEDMEM,SPINDLESPEEDVAR,spindleRPM); //KFLOP memory location, KMotionCNC variable (?), value
			//bug: changing speed after spindle is started makes this trigger (like on @ 8000RPM, error when command to 24000 from 8000)

		//this whole section is buggy
		//add the velocity checks
		//also add velocity display to cnc screens
		
			float t_start = Time_sec();
			SetBit(FEEDHOLDBIT);//set feedhold bit
			StopCoordinatedMotion(); //stop motion
			
				//extern int CS0_StoppingState; 			
				// emergency stop in progress, 
				//0 = not stopping, 
				//1=stopping coord motion, 
				//2=stopping indep (rapids)
				//3=fully stopped
				//4=ind stopped
			char message[160];
			sprintf(message,"Feedhold due to spindle speed mismatch. Is %f, commanded %f\n", spindle_speed_reported, spindle_speed);
			DROLabel(MESSAGEMEM,MESSAGEVAR,message); //KFLOP memory location, KMotionCNC variable (?), value

		}

	}
	
}

