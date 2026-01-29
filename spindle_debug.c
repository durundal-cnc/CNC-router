#include "KMotionDef.h"
#include "init_definitions.h"
//#include "read_flow_meter.c"
	//calibrations from no-load measurements
	#define ADC8000 = 746;
	#define SLOPE8000 = 10.72386059;
	#define ADC12000 = 1109;
	#define SLOPE12000 = 10.82055906;
	#define ADC18000 = 1654;
	#define SLOPE18000 = 10.88270859;
	#define ADC24000 = 2047;
	#define SLOPE24000 = 11.72447484;

int spindle_speed_reported;
short int spindle_speed; //float as defined in axis struct kmotiondef.h
int i = 0;

float spindle_rpm = 0;// from main

// desired speed is passed from KMotionCNC in variable KMVAR
// save in user variable STATEVAR whether it was off, CW, or CCW (0,1,-1)
// save in user variable SPEEDVAR the last desired speed

//This checks that the spindle is not slowing down due to loading. If it is, a feedhold is called.

//A fancier version could throttle down the feed rate until the speeds match, then increase it again

main()
{
	float spindle_commanded_speed = *(float *)&persist.UserData[SPEEDVAR];  // value stored is actually a float (commanded speed)
	float spindle_speed_reported = ADC(SPINDLE_SPEED_AI);//ADC speed
	float spindle_speed = DAC_Buffer[SPINDLE_CONTROL_AO];//DAC speed (	extern short int DAC_Buffer[N_DACS]; // format 12 bits data)

	float LastState = persist.UserData[STATEVAR];  // get last state 
	
	static char spindlemsgtext[80];



	int trigger = 0;
	static float time_trigger = 0;
	float time_trigger_next = 0;
	float time_elapsed = 0;
	static float oldtime = 0;
	if (Time_sec() > time_trigger){ // if current time is past the time to trigger
		trigger = 1;
		time_trigger = time_trigger + DELTATIME; //reset the trigger
	}
	
	float spindle_counts = ADC(SPINDLE_SPEED_AI);
	float spindle_speed_interpolated = 0;
	if (trigger == 1) {
		trigger = 0;
		
		//make variable slope range based on ADC counts
		if (spindle_speed_reported< ADC8000){
			spindle_speed_interpolated = spindle_counts * SLOPE8000;
		if (spindle_speed_reported >= ADC8000  &&  spindle_speed_reported < ADC12000){
			spindle_speed_interpolated = spindle_counts * (SLOPE8000 + ((spindle_counts - 8000) * (ADC12000-ADC8000))   );
		} else if (spindle_speed_reported >= ADC12000  &&  spindle_speed_reported < ADC18000){
			spindle_speed_interpolated = spindle_counts * (SLOPE12000 + ((spindle_counts - 12000) * (ADC18000-ADC12000))   );
		} else if (spindle_speed_reported >= ADC18000  &&  spindle_speed_reported < ADC24000){
			spindle_speed_interpolated = spindle_counts * (SLOPE18000 + ((spindle_counts - 18000) * (ADC24000-ADC18000))   );
		}
		
		persist.UserData[SPEEDVAR] = spindle_speed_interpolated;
		
		printf("spindle_rpm %f commanded %f adc %d\n", spindle_speed_interpolated, spindle_commanded_speed, spindle_speed_reported)
	}
}
	