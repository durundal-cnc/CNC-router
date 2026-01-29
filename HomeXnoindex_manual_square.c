	
#include "KMotionDef.h"
#include "init_definitions.h"
#include "init_x_motors.c" // performs initial motor configuration

// Home  (X0 and X1 are slaved; X0 is master)
//

//Used to adjust one axis independent of the other for squaring manually (just shifts, does not check against inductive or indexes)
#define X0_AXIS 		XAXIS
#define X0_HOME_BIT 	XPROXPOS
#define X0_INDEX_BIT 	XAXIS2INDEX_Q

#define X1_AXIS 		XAXISSLAVE
#define X1_HOME_BIT		XSLAVEHOMEPROX
#define X1_INDEX_BIT 	XAXIS1INDEX_Q

#define SLOW_SPEED 500

#define MANUAL_ADJUSTMENT -5 // from manual squaring measurements (need to re-do with better precision or any time prox sensors moved)
// negative manual adjustment is away from motor
// positive manual adjustment is towards motor



// 20807.68 counts per inch
//starting off with a big manual error and adjusting from there works well (halve the MEAS_DELTAS and compare against absolute value error on indicator)



/* Home steps

Move both to master and slave sensors at constant velocity
Move both to index for master and slave
Move slave to delta to make square
Re-slave and Zero
*/

void main() 
{
	float home0delta,home1delta,indices[1];
	float home0a, home0b, home0c, home1a, home1b, home1c, home0avg, home1avg;
	
	init_x_motors();
	printf("Start X manual adjustment\n");
	printf("Before slave adjustment\n    X index: %f Xslave index: %f\n",chan[X0_AXIS].Position,chan[X1_AXIS].Position);

	printf("Unslave axes\n");
	
	chan[X0_AXIS].MasterAxis = -1;  //  De-Slave both
	chan[X1_AXIS].MasterAxis = -1;

	MoveRelAtVel(X1_AXIS,MANUAL_ADJUSTMENT,SLOW_SPEED);//Adjust X1 to the measured distance to make the gantry square via manual measurements
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	printf("after slave adjustment\n    X index: %f Xslave index: %f\n",chan[X0_AXIS].Position,chan[X1_AXIS].Position);

/* Gantry squaring procedure
Use this to establish zeros
Move (slaved) to location of dial indicator on square
Zero axes
Move slave axis until square (solo)
Gang together to move into dial indicator range
When done the MEASURED_DELTAS is the slave - master
*/	
	// we are now all square
	chan[X1_AXIS].MasterAxis = X0_AXIS;  //  Slave X1 to X0
	chan[X1_AXIS].SlaveGain = 1;
	chan[X0_AXIS].MasterAxis = -1; // do not slave X0

	// start re-home after squaring
	printf("Re-slaved\n");
	
	init_x_motors(); //restore all settings
	printf("Axes slaved, limits re-enabled, X home Complete\n");
}

