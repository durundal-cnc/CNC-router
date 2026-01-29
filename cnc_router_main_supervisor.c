#include "KMotionDef.h" // This is the c library that KFLOP understands
#include "init_definitions.h" // this loads up the mapping of input/output channels and bits to names
#include "function_definitions.h" // load up all the definitions

#include "..\KflopToKMotionCNCFunctions.c"


#include "check_limit_switches.c" // Performs the proximity sensor limit checking
#include "init_x_motors.c" // performs initial motor configuration
#include "init_y_motor.c" // performs initial motor configuration
#include "init_z_motor.c" // performs initial motor configuration
#include "init_spindle.c" // performs initial spindle configuration
#include "startup_configuration.c" // sets all bits to appropriate startup configuration
#include "check_temperatures.c"	// reads thermistors and acts on them
#include "read_external_buttons.c" // checks for any external buttons being pressed
#include "check_alarms.c" // checks for alarms
#include "safe_system.c" // performs actions to halt motion and ensure router is in a safe configuration
#include "enable_servos.c" // enables/disables servos halt bits and axes
#include "MPGServiceSmoothHardwareEncFilteredRev3_am.c" // MPG servicer
#include "ToolTableSet_am.c" //Tool setting after hitting toolsetter
#include "ServiceToolsetter.c"// Toolsetter servicer
#include "service_time_insensitive.c" // Flowmeter and other time-insensitive read tasks
#include "service_spindle.c" //Spindle speed and flow checks
#include "abs.c" //absolute value
#include "spindle_get_speed_from_adc.c" // turns spindle ADC counts to RPM
#include "update_velocity_DROs.c" //reads axis velocities and displays on screen
//#include "readout_status.c" // prints pertinant data to console and file on disk
//#include "pause_motion.c" // stops at a convenient point for routine tool changes, stock changes, etc, returns 1 when done
//#include "graceful_shutdown.c" // stops motion gracefully and readies system for power off, returns 1 when done
//#include "mpg_smoother_modified.c" // controls the MPG pendant

//uncomment when file exists enough to compile


// Load me into thread 7 so I can be large in kilobytes and keep running even if emergency stop called in SW
// This program runs to look for general machine state monitoring and can react to things like limit switches, external buttons, etc
// It should basically always be running to make sure it stops if a limit hits or such
// Special codes (eg, MPG control) run as separate programs called from either the CNC gui or from an external switch
// For the external switch case, it will kick off the program in another thread to run independently from this one

/* Dedicated M codes
M0 - Program Stop
M1 - Optional Program Stop
M2 - Program Stop, Rewind, Reset Interpreter Settings
M3 - Spindle On CW
M4 - Spindle On CCW
M5 - Spindle Off - Note Automatically called on Program Stop/Rewind
M6 - Tool Change (T variable is passed to the C program in the persist variable specified)
M7 - Mist On
M8 - Flood On
M9 - Mist and Flood Off
*/


/* basic structure of homing for reference:

		if (!ReadBit(8))  // check for index mark
		{
			ch0->Position=0;  // set current position to Zero
			
			break;
		}
		*/

/* To do

	Enable MPG controller to not move when job running (ie Kmotion CNC needs to set flag when jobs running)
	Build program to check for servo output saturation and feedhold if so (so if one of the X axes loses itself stop entirely, other axes nice to have?)
	Build program to home axes
	Make notifications for when running into proximity sensor limits
	Rename everything to test_control instead of TestControl
*/
float nexttime;

int servoAlarm = 0;
double T0 = 0; //used for the time-remaining computations

int stopping_state = -1;

main()
{
	printf("Starting initialization tasks...\n");
	nexttime = Time_sec()+DELTATIME; //for the time insensitive thread

// Configure all outputs appropriately
	startup_configuration();

// Load up motor configurations
	init_x_motors();
	init_y_motor();
	init_z_motor();
	init_spindle();

    enable_servos(0,1); // always disable servo inputs on startup
	printf("Completed initialization tasks!\n");
    DROLabel(MESSAGEMEM,MESSAGEVAR,"Completed initialization tasks!"); //KFLOP memory location, KMotionCNC variable (?), value

	
	for(;;)
	{
			 
// Collect inputs			
		check_limit_switches(); // polls all proximity sensors and sends feedhold if found
		read_external_buttons(); // looks for any external physical buttons being pressed
		//check_alarms(); //doesn't do anything right now but looks to see if the DMMs still blip their alarm bits as they did previously before fixing grounding and adding RC filters
		update_velocity_DROs();


//Service MPG
		//ServiceMPG();
		
// Service spindle (check that commanded speed = actual speed to stop if bogged down
		service_spindle();
		
// Check spindle flow rate and other time insensitive functions
		nexttime = service_time_insensitive(nexttime);
		
//Service toolsetter
		ServiceToolsetter();
			
// Perform actions
		if (!ReadBit(MAINPOWERSTATUSBIT)) { // if don't see main power relay ...
			if (!ReadBit(FEEDHOLDBIT)) {	// ... and if this happened when we weren't in an intentional feedhold condition...
				printf("Safe system from main power and feedhold!\n");
				safe_system();				// ... perform emergency safing!
			}
			SetBit(FEEDHOLDBIT);			// always feedhold
			enable_servos(0,0);				// ensure servos are disabled
			servoAlarm = 0;					// reset the servo alarm (servos power down with main power and reset)
		}
		
		// Start feedhold check
		if(ReadBit(FEEDHOLDBIT)) {			//if something set the feedhold bit
			//StopCoordinatedMotion();		//perform the feedhold
			
			//does hammering this every cycle do anything bad?
			
			//Tests:
			//stop coordinated motion G1 commands
			//stop G0 commands
			//stop long jog command
			
			
			
			UpdateStoppingState();		// Update Stopping Status (only required for indep stopping)
			if (stopping_state != CS0_StoppingState){
				printf("CS0_StoppingState = %d\n",CS0_StoppingState);
			}
			stopping_state = CS0_StoppingState;
			
			
//extern int CS0_StoppingState; 			// emergency stop in progress, 0 = not stopping, 1=stopping coord motion, 2=stopping indep, 3=fully stopped, 4=ind stopped
			if (CS0_StoppingState == 0) { //not stopping
				StopCoordinatedMotion();
			} else {
				// motion is not coordinated e.g. G0 command or jog
				//Jog(XAXIS,0);
				//Jog(XAXISSLAVE,0);
				//Jog(YAXIS,0);
				//Jog(ZAXIS,0);
			}
			SetBit(FEEDHOLD_LAMP_SW);		//turn on the feedhold lamp
		}
		if(!ReadBit(FEEDHOLDBIT)) {
			ClearBit(FEEDHOLD_LAMP_SW);		//turn off the feedhold lamp
		}		
		// End feedhold check
		
		
		// Look to see FETs are enabled and display "on" lamp if so
		if(!ReadBit(X1MASTER_ENABLE) || !ReadBit(X2SLAVE_ENABLE) || !ReadBit(Z_ENABLE) || ReadBit(LEADSHINE_ENABLE)){		//if servos enabled
			SetBit(SERVO_LAMP_SW);			//turn on the programmable lamp
		} else {
			ClearBit(SERVO_LAMP_SW);			//turn off the programmable lamp
		}
		// end servo enabled lamp

		//Display time remaining for G-code
		double TimeRemaining = CS0_TimeDownloaded - CS0_TimeExecuted - CS0_t;
		char timeremainingtext[1000];
		
		if (TimeRemaining > 0.0) {// Print only with time remaining
			sprintf(timeremainingtext,"%12.1f / %f",Time_sec() - T0, TimeRemaining);
			//sprintf(timeremainingtext,"%12.6f\n\nTime Remaining = %12.1f\n",Time_sec() - T0, TimeRemaining);

		} else {
			T0 = Time_sec(); // Time stamp last time we weren't executing motion
		}
		DROLabel(TIMELEFTMEM,TIMELEFTVAR,timeremainingtext); //KFLOP memory location, KMotionCNC variable (?), value

		
		
// Finish perform actions

		WaitNextTimeSlice();
	}
    return 0;
}

