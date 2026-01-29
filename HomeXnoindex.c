	
#include "KMotionDef.h"
#include "init_definitions.h"
#include "init_x_motors.c" // performs initial motor configuration

// Home  (X0 and X1 are slaved; X0 is master)
//

//need to install second prox saensor within 1 index of the other axis

#define X0_AXIS 		XAXIS
#define X0_HOME_BIT 	XPROXPOS
#define X0_INDEX_BIT 	XAXIS2INDEX_Q

#define X1_AXIS 		XAXISSLAVE
#define X1_HOME_BIT		XSLAVEHOMEPROX
#define X1_INDEX_BIT 	XAXIS1INDEX_Q

#define SLOW_SPEED 500
#define FAST_SPEED 10000//2500
#define SUPER_SPEED 25000

#define BACK_DIST -20000  // amount to move to make sure not in the limits
#define MEAS_DIST -2000	  // amount to move back for measurements
#define MEASURED_DELTAS 0 // from manual squaring measurements (need to re-do with better precision or any time prox sensors moved)
//starting off with a big manual error and adjusting from there works well (halve the MEAS_DELTAS and compare against absolute value error on indicator)

#define PRECISION 5 // amount the average can be off by


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
	printf("Start X home, disable limit checks\n");

	chan[X0_AXIS].LimitSwitchOptions = 0; //disable limit switch checks
	chan[X1_AXIS].LimitSwitchOptions = 0;
	
	

	// check if we already are in the square/limits
	// if so, move away from the switch and re-square
	if (ReadBit(X0_HOME_BIT) || ReadBit(X1_HOME_BIT))
	{
		printf("Switch Status master=%d slave=%d\n",ReadBit(X0_HOME_BIT),ReadBit(X1_HOME_BIT));
		printf("Inside a sensor, moving out of it\n");
		MoveRelAtVel(X0_AXIS,BACK_DIST,FAST_SPEED);  // move outside the sensor a small amount
		while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	}
	
	move2home(FAST_SPEED); // go near home sensors
	
	printf("Unslave axes\n");
	
	chan[X0_AXIS].MasterAxis = -1;  //  De-Slave both
	chan[X1_AXIS].MasterAxis = -1;
	
	moveBoth2home(SLOW_SPEED,indices);
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	home0a = indices[0];
	home1a = indices[1];
	MoveRelAtVel(X0_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	MoveRelAtVel(X1_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done

	
	moveBoth2home(SLOW_SPEED,indices); //go back to try finding index again
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	home0b = indices[0];
	home1b = indices[1];
	MoveRelAtVel(X0_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	MoveRelAtVel(X1_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done

	moveBoth2home(SLOW_SPEED,indices); //go back to try finding index again
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	home0c = indices[0];
	home1c = indices[1];
	MoveRelAtVel(X0_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	MoveRelAtVel(X1_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done

	home0avg = (home0a + home0b + home0c)/3;
	home1avg = (home1a + home1b + home1c)/3;
	
	moveBoth2home(SLOW_SPEED,indices); //go back to try finding index again
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	home0c = indices[0];
	home1c = indices[1];
	MoveRelAtVel(X0_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	MoveRelAtVel(X1_AXIS,MEAS_DIST,FAST_SPEED);  // move outside the sensor a small amount
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done

	home0delta = indices[0] - home0avg;
	home1delta = indices[1] - home1avg;
	printf("home0 %f home1 %f home0avg %f home1avg %f\n",indices[0],indices[1],home0avg,home1avg);
	printf("home0 delta %f home1 delta %f\n",home0delta,home1delta);

	if(home0delta < PRECISION && home0delta > -PRECISION && home1delta < PRECISION && home1delta > -PRECISION){
		Zero(X0_AXIS);
		Zero(X1_AXIS);	
		printf("Found homes, now adjust slave axis\n");
	} else {
		printf("Home not repeatable, ending home and re-slaving\n");
		printf("Error between home0 %f home1 %f\n",home0delta, home1delta);
		init_x_motors(); //restore all settings
		return 1;
	}

//printf("**** bump to relative motion\n");
//while (!ReadBit(1032))  ; 

	MoveRelAtVel(X1_AXIS,MEASURED_DELTAS,SLOW_SPEED);//Adjust X1 to the measured distance to make the gantry square via manual measurements
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
	printf("Re-slaved\n    Find master home sensor and back off\n");
	move2home(FAST_SPEED);

	MoveAtVel(X0_AXIS,-12000,FAST_SPEED); // move well outside of the proximity sensors
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done

	// zero both x axis
	Zero(X0_AXIS);
	Zero(X1_AXIS);

	// add section to set soft limits here?
	
	init_x_motors(); //restore all settings
	printf("Axes slaved, limits re-enabled, X home Complete\n");
}


void moveBoth2home(long speed, float* indices)
{
	int found0,found1;
	float x0home, x1home;
	found0 = found1 = 0;

	printf("Start moving to home at %d cts/sec\n",speed);
	Jog(X0_AXIS,speed); // move towards sensor
	Jog(X1_AXIS,speed); // move towards sensor

	while (!found0 || !found1)
	{
		if (!found0 && ReadBit(X0_HOME_BIT))
		{
			indices[0]=chan[X0_AXIS].Position;

			MoveAtVel(X0_AXIS,indices[0],speed);
			found0=1;
		}
		if (!found1 && ReadBit(X1_HOME_BIT))
		{
			indices[1]=chan[X1_AXIS].Position;
			MoveAtVel(X1_AXIS,indices[1],speed);
			found1=1;
		}
	}

	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	//while ((chan[X0_AXIS].Position != x0home) || (chan[X1_AXIS].Position != x1home))	;
	printf("X0 home position %f\n",indices[0]);
	printf("X1 home position %f\n",indices[1]);

	return 0;
}

float move2home(long speed)
{
	printf("Moving towards home sensors\n");
	Jog(X0_AXIS,speed); // move towards sensor
	while (!ReadBit(X0_HOME_BIT) || !ReadBit(X1_HOME_BIT)) ;

	MoveRelAtVel(X0_AXIS,MEAS_DIST,speed);
	while (!CheckDone(X0_AXIS) || !CheckDone(X1_AXIS))  ;  // wait till done
	printf("Found home sensors\n");

	return 0;
}
