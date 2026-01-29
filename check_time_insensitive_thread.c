#include "KMotionDef.h"
#include "init_definitions.h"
#include "read_flow_meter.c"
#include "check_temperatures.c"
#include "..\KflopToKMotionCNCFunctions.c"

// This program runs realtime-insensitive tasks
// like reading temperatures, reading flow rates
// It should run in its own thread on startup and send values to the KmotionCNC screen, and call feedholds as required by setting bits in the Kflop

float flowrate = -1;
char text[80];

main()
{
	for(;;) {
		check_temperatures(0); // check temperature limits with feedback (prints to console)

		flowrate = read_flow_meter(); // read the flow rate on the flowmeter
		sprintf(text,"%1.2f",flowrate);
		DROLabel(FLOWMEM,FLOWVAR,text); //KFLOP memory location, KMotionCNC variable (?), value
	
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
	
	
		if(flowrate < FLOWRATE_MIN) { // only read flow rate if pump is supposed to be on (aux port)
			SetBit(FLOWRATEALARMBIT);
		} else {
			ClearBit(FLOWRATEALARMBIT);
		}
	}
}
