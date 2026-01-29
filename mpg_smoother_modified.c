#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// Hi guys, I was a little disappointed with the performance from the MPGsmooth.c program. It worked, but depending 
// on how i set the smoothing factor i would either have to live with fairly rough motion when on 0.010" increments, 
// or movement that dragged on forever until eventually the time limit finished the movement with a jolt at the end.

// I modified the program to start with a high smoothing factor (TAU) and then decrease the value based on the "no change" time,
// so that its very smooth at first and still finishes up motion in a reasonable amount of time. The starting value, minimum 
// value and rate of change are all easily adjustable in the program. Seems to be working very well for me so far. 
// I also took it a step further and set it up to modify axis acceleration during mpg movement and set it back to normal 
// when finished. These lines are ignored with comments in the file in case it might get confusing, but they are there 
// if you feel like setting up acceleration numbers too. A value of about half seemed to work well for me, if you go too 
// low it can do some odd motions when reversing.

// Anyway i thought I would share my program with you guys. Takes a little while to tune it, but it can provide 
// much better performance then all the mpg programs included with kmotioncnc. One last thing, im not a C programmer, 
// didnt know a thing a few months ago, so it may not be perfect, but it does work well. Hope it helps someone.

// Mark


//#alreadydefined MPG_QA		// which IO bits the AB signals are connected 
//#alreadydefined MPG_QB 		// which IO bits the AB signals are connected 
#define TAU 0.35 		// Max smoothing factor (not tuned by Andy)
#define MINTAU 0.12 	// min smoothing factor (not tuned by Andy)
#define RATE 1.1 		// Smoothing factor rate of change per second after no mpg movement(not tuned by Andy)
#define FINAL_TIME 1.0 	// Set final dest after this amount of time with no change (not tuned by Andy)

// MPG quadrature gives 400 counts per rotation
// Encoder on motor gives 10000 counts per rotation
// Adjust by 10000/400 to get 1/1 rev/rev

main()
{	
	int Change1=0, NewPos;
	chan[MPG_INPUT_AXIS].Position = 0; //reset the MPG axis every time it is activated
	int Pos = chan[MPG_INPUT_AXIS].Position;	

	
	int InMotion=FALSE,Axis,LastAxis=-1;
	double LastChangeTime=0,Target,Factor=0,testvar;
	float NEWTAU;


	printf("initial Pos %d\n", Pos);
	
	
	
	for(;;)
	{	
		if(!ReadBit(FEEDHOLDBIT)) { //only do things if feedhold not present (otherwise can build up MPG commands that execute as soon as feedhold released)
	/* This code is for using non-hardware-quadrature bits for the encoder.
	It is possible to spin the dial too fast for it so hardware quad reading is better.

		//int BitA,Change1=0,Change2=0, DiffX2;
		//int PosNoWrap, NewPos, Pos=0, wraps;

			// convert quadrature to 2 bit binary
			BitA = ReadBit(MPG_QA);   
			PosNoWrap = (ReadBit(MPG_QB) ^ BitA) | (BitA<<1);
			// Diff between expected position based on average of two prev deltas
			// and position with no wraps.  (Keep as X2 to avoid division by 2)		
			
			// DiffX2 = 2*(Pos-PosNoWrap) + (Change2+Change1); --------origonal line--------	
			DiffX2 = 2*(Pos-PosNoWrap); // ----------modified to prevent runaway--------------
			
			// Calc quadrature wraparounds to bring Diff nearest zero
			// offset by 128 wraps to avoid requiring floor()
			wraps = ((DiffX2+1028)>>3)-128;		
			// factor in the quadrature wraparounds
			NewPos = PosNoWrap + (wraps<<2);
			Change2 = Change1;
			Change1 = NewPos - Pos;
	*/
			
			NewPos = chan[MPG_INPUT_AXIS].Position;
			Change1 = NewPos - Pos;
			if(Pos != NewPos){
				//printf("Pos %d NewPos %d\n", Pos, NewPos);
				//printf("Target %d Change1 %d\n", Target, Change1);
			}
			Pos = NewPos;
			

	//		if (!ReadBit(1029) || JOB_ACTIVE) // if enable button not pressed or Job Active ignore the encoder.
	//			Change1 = 0;

			if (ReadBit(MPG_X1))  // is X1 selected?
				Factor = .001;
			else if (ReadBit(MPG_X10))  // is X10 selected?
				Factor = .010;
			else if (ReadBit(MPG_X100))  // is X100 selected?
				Factor = .100;
			else {					// ignore encoder, this will trigger anytime the MPG knob is turned
				//printf("Error: no reading for X1 X10 X100 scale factor input from MPG!\n");
				Change1 = 0;
			}
			
			Factor = Factor / 4; // because MPG is outputting 4 per tick of the wheel
				
			if (ReadBit(MPG_XAXIS))  // is x selected?
			{
				Axis=XAXIS;
				Factor = Factor*20807.68; // counts/rev
			}		
			else if (ReadBit(MPG_YAXIS))  // is y selected?
			{
				Axis=YAXIS;
				Factor = Factor*25400; // counts/rev
			}		
			else if (ReadBit(MPG_ZAXIS))  // is z selected?
			{
				Axis=ZAXIS;
				Factor = Factor*20807.68; // counts/rev
			} else {
				Axis = -1;
			}
			
			
			
			// check if the Axis just changed or we have been 
			// converging to the target for a long time
			if (Axis != LastAxis || 
				(InMotion && Time_sec() > LastChangeTime+FINAL_TIME))
			{
				if(Axis != -1) {
					printf("%d Axis selected\n", Axis);
				}

				if (InMotion)
					Move(LastAxis,Target);  //finalize any motion	
				LastAxis = Axis;
				InMotion = FALSE;
			}		

			
			
			if (Change1) // did we command a move from the MPG?
			{

	//			printf("Change1 modification, InMotion: %d axis: %d target: %d tau: %f time: %f\n", InMotion, Axis, Target, TAU, LastChangeTime);

				if (!InMotion) Target = chan[Axis].Dest; {
					Target = Target + (Change1 * Factor);
				}
				
				testvar = chan[Axis].Position;
				//printf("testvar: %d\n%f\n%f\n%f\n", Axis, Target, testvar, TAU);
				//Move(Axis, Target);
				MoveExp(Axis,Target,TAU);  // note: contains a WaitNextTimeSlice
				LastChangeTime = Time_sec();
				InMotion=TRUE;

			}
			
	// this will work buggy until all axes are present (an unplugged axis can never finish motion
			if (InMotion) 							// If still moving
			{		
				NEWTAU = TAU - (RATE * (Time_sec() - LastChangeTime));	 		// Adjust NEWTAU
				if (NEWTAU < MINTAU) NEWTAU = MINTAU; 				// Limit NEWTAU Mininum
				MoveExp(Axis,Target,NEWTAU);  					// Move with updated smoothing factor
				
			} else {
				WaitNextTimeSlice();
			}
			
			
		}  
	}
}