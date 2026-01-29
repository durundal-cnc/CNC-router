#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

//#include "..\KflopToKMotionCNCFunctions.c" //uncommenting this one breaks it somehow?

// This program runs realtime-insensitive tasks
// like reading temperatures, reading flow rates
// The reads occur every cycle, but printouts are only done on the time basis



//main loop calls this every cycle, updating the time each call
//nexttime = Time_sec()+DELTATIME to initialize
//main{
//	nexttime = service_time_insensitive(nexttime);

float service_time_insensitive(float time_trigger) {
	 
	static char msgtext[160];
	char flowtext[160];
	float VelX1,VelX2,VelY,VelZ;

	int trigger = 0;
	float time_trigger_next = 0;
	float time_elapsed = 0;
	static float oldtime = 0;
	if (Time_sec() > time_trigger){ // if current time is past the time to trigger
		trigger = 1;
		time_trigger_next = time_trigger + DELTATIME; //reset the trigger
		time_elapsed = Time_sec()-oldtime; // compute how long it actually took
		oldtime = Time_sec();
	} else {
		time_trigger_next = time_trigger; // otherwise regurgitate the time to trigger again
	}
	
/* 	Timer Functions:
double Time_sec();
void WaitUntil(double time_sec);
void Delay_sec(double sec);
double WaitNextTimeSlice(void);> */
	
	
	// Count the flowmeter bit flips
	int flow_bit;		// the value of the bit being read 
	static int old_bit = 0;		// the old value to look for a change
	static int bits_counted = 0;	// the number of bits counted in each period
	
	flow_bit = ReadBit(FLOWSENSOR);
	if (flow_bit == old_bit) {
		//printf("%i", flowBit);
		bits_counted++;
		old_bit = !flow_bit;
	}
	

	if (trigger == 1) { //we only look for problems every second
		trigger = 0;
		
		//process flow rate

		float flowrate = bits_counted/time_elapsed;
		float litersPerMin = (flowrate/463)*60;
		bits_counted = 0;
		//printf("%f, %f\n", litersPerMin, FLOWRATE_MIN);
		if ((time_elapsed < 0.8) && (time_elapsed>1.2)) {
			//print any funky times for debugging
			printf("bits_counted %d, time elapsed %f\n", bits_counted, time_elapsed);
		}
		
		int spindle_speed = -1*(DAC_Buffer[SPINDLE_CONTROL_AO]-2048);
		int spindle_speed_reported = ADC(SPINDLE_SPEED_AI);
		//printf("Spindle %d, %d\n", spindle_speed, spindle_speed_reported);

		static int flow_trips = 0; //how many times a low flow has to hit (so occasional spurious readings don't set it off)
		if((litersPerMin < FLOWRATE_MIN) && (time_elapsed > 0.8) && (time_elapsed<1.2)) { // only read flow rate if pump is supposed to be on (aux port)
			//While other processes are waiting the flowrate doesn't get updated, so wait until the time gets back to a reasonable number
			flow_trips++;
			SetBit(FLOWRATEALARMBIT);

			if ((ReadBit(SPINDLECW_BIT) || ReadBit(SPINDLECCW_BIT)) && (flow_trips > 2) ) { //if flow rate low and the spindle is on
				flow_trips = 0;
				printf("Flow rate too low, is %f should be %f. Initiating feedhold and spindle shutdown.\n", litersPerMin, FLOWRATE_MIN);
				SetBit(FEEDHOLDBIT);//set feedhold bit
				StopCoordinatedMotion(); // bring any coordinated motion to a emergency stop ASAP
				sprintf(msgtext,"Velocities %f %f %f %f CS0_StoppingState %d \n", VelX1,VelX2,VelY,VelZ,CS0_StoppingState ); //display warning
				DROLabel(MESSAGEMEM,MESSAGEVAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
				//done1 = CheckDoneXYZABC(); // Check if all CS axis have completed
				//done2 = CheckDoneBuf(); // Checks that coordinated motion buffer is done
				//1 if all axes in the defined coordinate system have completed their motion
				//0 if any axis in the defined coordinate system has not completed its motion
				//-1 if any axis in the defined coordinate system is disabled
				
				//CS0_StoppingState might be better
				//extern int CS0_StoppingState; 			
				// emergency stop in progress, 
				//0 = not stopping, 
				//1=stopping coord motion, 
				//2=stopping indep (rapids)
				//3=fully stopped
				//4=ind stopped

				//Wait to stop, or for 10 seconds as a failsafe
				float t_start = Time_sec();
				//See if any axes are moving
				VelX1=ch3->last_vel;
				VelX2=ch2->last_vel;
				VelY=ch1->last_vel;
				VelZ=ch4->last_vel;
				printf("Before stop Velocities %f %f %f %f CS0_StoppingState %d \n", VelX1,VelX2,VelY,VelZ,CS0_StoppingState );
		
				//if not in motion, CS0 state is 0. Check that motion really has stopped by checking velocities.
				while ( (CS0_StoppingState == 1) || (CS0_StoppingState == 2) || (abs(VelX1)>0) || (abs(VelX2)>0) || (abs(VelY)>0) || (abs(VelZ)>0)   ) { // If we have come to a stop from coordinated or rapid motion
				// not sure when =4 will be true, only seems to return 3 for both G0 and G1 motion
				// =0 for both moving and not stopping, and not moving but not feedholding. So compare axis velocities directly.
				// if not moving, it defaults to zero
					//printf("CS0_StoppingState is %d\n", CS0_StoppingState);
					if (Time_sec() > t_start + 10) {
							printf("Timer abort after 10 seconds\n");
							break; // abort if taking too long to stop for some reason (if axes dithering so not hitting 0 velocity or something)
					}
					
					VelX1=ch3->last_vel;
					VelX2=ch2->last_vel;
					VelY=ch1->last_vel;
					VelZ=ch4->last_vel;
					//printf("Mid Velocities %f %f %f %f\n", VelX1,VelX2,VelY,VelZ);
					sprintf(msgtext,"Velocities %f %f %f %f CS0_StoppingState %d \n", VelX1,VelX2,VelY,VelZ,CS0_StoppingState ); //display warning
					DROLabel(MESSAGEMEM,MESSAGEVAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value
					WaitNextTimeSlice();
				}
				VelX1=ch3->last_vel;
				VelX2=ch2->last_vel;
				VelY=ch1->last_vel;
				VelZ=ch4->last_vel;
				printf("Bfter stop Velocities %f %f %f %f CS0_StoppingState %d \n", VelX1,VelX2,VelY,VelZ,CS0_StoppingState );
				
				//stop spindle now that we aren't moving (don't want to stop spindle while moving and crash)
				printf("CS0_StoppingState is %d\n", CS0_StoppingState);
				ClearBit(SPINDLECW_BIT);
				ClearBit(SPINDLECCW_BIT);
				Jog(SPINDLEAXIS,0);
				printf("Jogging Spindle Stop\n");
				persist.UserData[STATEVAR] = 0;  // remember we are Off
				
				//update status label
				sprintf(msgtext,"Warning: flow rate low: %1.2f! Commanding spindle off!",flowrate); //display warning
				DROLabel(MESSAGEMEM,MESSAGEVAR,msgtext); //KFLOP memory location, KMotionCNC variable (?), value

			}
		} else {
			ClearBit(FLOWRATEALARMBIT);
		}
		
		//update flow rate variable
		sprintf(flowtext,"%1.2f",litersPerMin);
				//do the KmotionCNC updates
		DROLabel(FLOWMEM,FLOWVAR,flowtext); //KFLOP memory location, KMotionCNC variable (?), value
		
	}
	
	check_temperatures(0); // check the other ADCs set up for thermistors (not hooked up due to worries about noise right now)


	
	
	return time_trigger_next; 
}
	
	

/* 	// Write a string to a DRO Label on the screen.
// Put the string into the gather buffer at the specified offset (in words)
// Then place the offset in the specified persist variable
// KMotionCNC will upload and display the message and then
// clear the persist variable.
//
// in order to avoid any possibility of an unterminated message
// write the message in reverse so the termination is added first

void DROLabel(int gather_offset, int persist_var, char *s)
{
	char *p=(char *)gather_buffer+gather_offset*sizeof(int);
	int i,n;
	
	// first find length of string
	for (n=0; n<256; n++) if (s[n]==0) break;

	// now copy string backwards
	for (i=n; i>=0; i--) p[i]=s[i]; 
	
	persist.UserData[persist_var] = gather_offset; // set gather offset
	return;
	
	From KMotionDef.h:
	#define MAX_GATHER_DATA 1000000 // Size of gather buffer (number of doubles, 8 bytes each).                   
extern double *gather_buffer;   // Large buffer for data gathering, Bode plots, or User use
} */
	