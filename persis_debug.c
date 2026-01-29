#define FLOWVAR 		90				// global persistant variable to store flowmeter rate
#define FLOWMEM 		1000			//used to allocate memory to be passed to DROLabels (this number is the address in 32 bit space). Size is 1000000 8 byte doubles
#define SPINDLETEMPVAR 	91
#define SPINDLETEMPMEM 	1010 //count of how many 32 bit words
#define XAXISTEMPVAR 	92
#define XAXISTEMPMEM 	1020
#define XAXISSLAVETEMPVAR 93
#define XAXISSLAVETEMPMEM 3000
// 1 = 4 chars

#define TMP 10

#include "KMotionDef.h"
#include "..\KflopToKMotionCNCFunctions.c"

/*
memory address is set. Things write to it. Then more writes to it. Then whole thing is addressed and sent.
block 0: 12345678
write to block 1: abcd
now memory is 1234abcd

actual thing is sending MEM value to location VAR 

Does actually reset the parameter after read by kmotioncnc but does not read that zeroing in the same thread? Need to let it WaitNextTimeSlice to get updates from other threads that clear the persist bit
*/


float flowrate = -1;
char text[80];
int i = 0;
int j = 0;
int k = 0;
int msg = -1;

main()
{
	
	
	
	
/* 	printf("a %d\n", CS0_StoppingState);
	StopCoordinatedMotion(); // bring any coordinated motion to a emergency stop ASAP
	printf("a %d\n", CS0_StoppingState);
	 */
	//CS0_StoppingState might be better
	//extern int CS0_StoppingState; 			
	// emergency stop in progress, 
	//0 = not stopping, 
	//1=stopping coord motion, 
	//2=stopping indep (rapids)
	//3=fully stopped
	//4=ind stopped

//				extern int CS0_StoppingState; 			// emergency stop in progress, 0 = not stopping, 1=stopping coord motion, 2=stopping indep, 3=fully stopped, 4=ind stopped

	//Wait to stop, or for 10 seconds as a failsafe
/* 	float t_start = Time_sec();
	printf("a %d\n", CS0_StoppingState);
	int done1, done2;

	while ( (CS0_StoppingState != 3) && (CS0_StoppingState != 4)) { // If we have come to a stop from coordinated or rapid motion
		done1 = CheckDoneXYZABC(); // Check if all CS axis have completed
		done2 = CheckDoneBuf(); // Checks that coordinated motion buffer is done
		printf("b %d %d %d\n", CS0_StoppingState, done1,done2 );
		if (Time_sec() > t_start + 0.01) {
			printf("t_start %f\n", Time_sec());
			break; // abort if taking too long
		}
		WaitNextTimeSlice();
	}
} */

/*
main()//abcddefghijklmnopqrstuvwxyz
{
	printf("moved out of home sensor\n");
	printf("Fixture1:%d\n", persist.UserData[90]);
	while(i<1000){

		DROLabel(FLOWMEM,FLOWVAR,"abcddefghijklmnopqrstuvwxyz"); //KFLOP memory location, KMotionCNC variable (?), value
		
		while(persist.UserData[FLOWVAR] == 1000) {
			WaitNextTimeSlice();
		}
	//	Delay_sec(0.02);
		printf("Fixture2:%d\n", persist.UserData[90]);
		printf("\n%d\n", i);
		DROLabel(FLOWMEM,SPINDLETEMPVAR,"zzzzzz12345678901234567890"); //KFLOP memory location, KMotionCNC variable (?), value
		while(persist.UserData[SPINDLETEMPVAR] == 1000) {
			WaitNextTimeSlice();
		}

		DROLabel(FLOWMEM,XAXISTEMPVAR,"shortsafd"); //KFLOP memory location, KMotionCNC variable (?), value
		while(persist.UserData[XAXISTEMPVAR] == 1000) {
			WaitNextTimeSlice();
		}
		i++;
	}
}
*/

/*
		if (j<10) {
			j++;
			for(k=0; k<1000000; k++){
				
			}
		}
	*/		
/*		while(persist.UserData[FLOWVAR] != 0){
		while(j<10){
			msg = persist.UserData[FLOWVAR]; // Mach3 notify Message 10000-10999

			//printf("userdata: %d\n", msg);
			j++;
			//WaitNextTimeSlice();
		}
		*/
		//DROLabel(SPINDLETEMPMEM,SPINDLETEMPVAR,"12345678901234567890"); //KFLOP memory location, KMotionCNC variable (?), value
		//DROLabel(XAXISTEMPMEM,XAXISTEMPVAR,"short"); //KFLOP memory location, KMotionCNC variable (?), value
/*		i++;
		WaitNextTimeSlice();

	}
}
*/


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

}*/

