#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// check for alarm conditions

char flowtext[80];
char temperaturetext[80];
char motortext[80];

void check_alarms(void) {

		

		if (ReadBit(XAXIS_ALARM)) {
			printf("feedhold due to X motor driver alarm\n"); // only print if servos on (ie, not if loading stock nearby it)
			sprintf(motortext, "Alarm on X axis driver!\n");
//			MsgBoxNoWait(motortext,MB_OK);
//			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
//			Delay_sec(10); // wait 10 seconds before popping up again
		}

		if (ReadBit(XAXISSLAVE_ALARM)) {
			printf("feedhold due to X slave motor driver alarm\n"); // only print if servos on (ie, not if loading stock nearby it)
			sprintf(motortext, "Alarm on X SLAVE axis driver!\n");
	//		MsgBoxNoWait(motortext,MB_OK);
	//		SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
	//		Delay_sec(10); // wait 10 seconds before popping up again
		}	
		
		// check for maxing of servos inputs
		//DAC_Buffer[ch]=((v-2048)&0xfff)
//		if (DAC_Buffer[2] > 200) {
//			printf("X master hitting servo input max %d\n", DAC_Buffer[2]); // only print if servos on (ie, not if loading stock nearby it)
//		}
		
		
		
		if (ReadBit(ZAXIS_ALARM)) {
			printf("feedhold due to Z motor driver alarm\n"); // only print if servos on (ie, not if loading stock nearby it)
			sprintf(motortext, "Alarm on Z axis driver!\n");
		//	MsgBoxNoWait(motortext,MB_OK);
		//	SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		//	Delay_sec(10); // wait 10 seconds before popping up again
		}
		if (ReadBit(YAXIS_ALARM)) {
			printf("feedhold due to Y motor driver alarm\n"); // only print if servos on (ie, not if loading stock nearby it)
			sprintf(motortext, "Alarm on Y axis driver!\n");
		//	MsgBoxNoWait(motortext,MB_OK);
		//	SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
		//	Delay_sec(10); // wait 10 seconds before popping up again
		}

/*	Not installed due to EMI concerns
	if (ReadBit(TEMPERATUREALARMBIT)) {
			sprintf(temperaturetext, "Temperatures too high!\n");
			MsgBoxNoWait(temperaturetext,MB_OK);
			Delay_sec(10); // wait 10 seconds before popping up again
		} */
		
/* Now in dedicated spindle handler in service_time_insensitive
		if(ReadBit(FLOWRATEALARMBIT) && (ReadBit(SPINDLECCW_BIT) || ReadBit(SPINDLECW_BIT))) { // only report flow rate if pump is supposed to be on (spindle active)
			printf("Flow rate too low!");
			SetBit(FEEDHOLDBIT);	// sets the feedhold bit (collected in main loop)
			ClearBit(SPINDLECW_BIT); // tell spindle to stop
			ClearBit(SPINDLECCW_BIT);// tell spindle to stop
			Jog(SPINDLEAXIS,0);
			printf("Jogging Spindle Stop\n");
			persist.UserData[STATEVAR] = 0;  // remember we are Off
			sprintf(flowtext, "Flow rate too low!");
			MsgBoxNoWait(flowtext,MB_OK);
			Delay_sec(10); // wait 10 seconds before popping up again
		}
		//repeat for other alarms if wired (servo, Leadshine drive) when installed
*/
	return 0;
}
