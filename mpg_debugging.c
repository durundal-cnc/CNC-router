#include "KMotionDef.h"
#include "init_definitions.h" // this loads up the mapping of input/output channels and bits to names
#include "function_definitions.h" // load up all the definitions

int NewPos;
int MPGpos;
int status;
int j;
float P, I, D;
float output;


main()
{
	
			MPGpos = chan[7].Position;
			printf("\nMPG position %d\n", MPGpos);
			printf("Axis start %d\n", NewPos);
	
			ch0->Enable=TRUE;
			ch0->Position=0;  // set current position to Zero


/*
			P = chan[0].P;
			I = chan[0].I;
			D = chan[0].D;
			output = chan[0].Output;
			printf("P %f I %f D %f\n", P, I, D);
			printf("output %f\n", output);
			
			
			status = CheckDone(0);
			printf("Axis status %d\n", status);			
			*/
			
			MoveRel(0,2500);
			while (!CheckDone(0)) {
				//printf("Destination: %d\n", chan[0].Dest);
			}

			NewPos = chan[0].Position;
			printf("Axis moved to %d\n", NewPos);
			return 0;
}

/*
void Move(int ch, double x);   // move using absolute coordinates
void MoveAtVel(int chno, double x, float MaxVel);        // move using absolute coordinates and specify the velocity
void MoveRel(int ch, double dx);    // move relative to current destination
void MoveRelAtVel(int chno, double x, float MaxVel);        //  move relative to current destinatio and specify the velocity
void Jog(int ch, double vel);       // move continiously at specified velocity
void MoveExp(int chno, double x, double Tau);  // exponentially approach a target at time constant Tau

int CheckDone(int ch);  // returns 1 if axis is Done, 0 if not, -1 if axis is disabled

void Zero(int ch);  // Zero the Encoder Position and Current Commanded Position
void StartMove(int ch);
*/