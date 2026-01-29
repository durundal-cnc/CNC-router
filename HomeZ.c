	
#include "KMotionDef.h"
#include "init_definitions.h"
#include "init_z_motor.c" // performs initial motor configuration
// Home 
//

#define AXIS 		ZAXIS
#define HOME_BIT 	ZPROXPOS
#define INDEX_BIT 	ZAXISINDEX_Q

#define HOME_SPEED 5000
#define INDEX_SPEED -500

#define BACK_DIST -7500  // amount to move inside the limits


void main() 
{
	init_z_motor();
	printf("*** Start Z axis home ***\n");
	int SaveLimit;
	float deltas;

	SaveLimit = chan[AXIS].LimitSwitchOptions;  // save and disable limits
	chan[AXIS].LimitSwitchOptions = 0;

	// check if we already are in the square/limits
	// if so, move away from the switch and re-square
	
	
	printf("Switch Status HOME_BIT=%d\n",ReadBit(HOME_BIT));

	if (ReadBit(HOME_BIT))
	{
		printf("started in home sensor\n");
		MoveRel(AXIS,BACK_DIST);  // move outside the sensor a small amount
		while (!CheckDone(AXIS))  ;  // wait till done
		printf("moved out of home sensor\n");
	}

	// drive up into proximity sensor
	printf("Move into proximity sensor\n");
	Jog(AXIS,HOME_SPEED); // move up towards sensor
	while (!ReadBit(HOME_BIT))
	{
		WaitNextTimeSlice();
	}
	Jog(AXIS,0.0); // StopMotion
	while (!CheckDone(AXIS))  ;  // wait till done
	
	//drive away from prox sensor to index
	printf("before move to index: %f\n",chan[AXIS].Position);
	Jog(AXIS,INDEX_SPEED); // move down towards index
	while (!ReadBit(INDEX_BIT))
	{
		WaitNextTimeSlice();
	}
	Jog(AXIS,0.0); // StopMotion
	while (!CheckDone(AXIS))  ;  // wait till done

	printf("after move to index: %f\n",chan[AXIS].Position);
	
	printf("Move away from prox sensor by %d\n",BACK_DIST);
	MoveRel(AXIS,BACK_DIST);  // move away from index (make sure don't run into prox sensor when running G53 Z0)
	while (!CheckDone(AXIS))  ;  // wait till done
	
	// we are now all square
	// zero Z axis
	Zero(AXIS);

	chan[AXIS].LimitSwitchOptions = SaveLimit;  // restore limits
   
   
   
	printf("Home Z Complete\n");
}
