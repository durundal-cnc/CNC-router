// Example Function as "smooth" MPG motion example
// which makes use of the exponential motion command.
// Additionally double Filtered to be very smoot with limited Jerk
#include "init_definitions.h"

#define TAU  0.01			// smoothness factor (Low Pass Time constant seconds for MoveExp)
#define TAU1 0.015			// smoothness factor (Low Pass Time constant seconds for pre filter 1)
#define TAU2 0.015			// smoothness factor (Low Pass Time constant seconds for pre filter 2)
#define FINAL_TIME 1.0		// Set final dest after this amount of time with no change



void ServiceMPG(void)
{
	static int Pos, FirstTime = TRUE;
	static int InMotion = FALSE, Axis, LastAxis = -1;
	static double LastChangeTime = 0, Target, Factor = 0;
	static double Target1, Target2, K1, K2, K1M, K2M;

	int Change1, NewPos;

	if (FirstTime)
	{
		Pos = chan[MPG_INPUT_AXIS].Position;
		K1 = exp(-2 * TIMEBASE / TAU1);	// filter coefficients
		K2 = exp(-2 * TIMEBASE / TAU2);
		K1M = 1.0 - K1;
		K2M = 1.0 - K2;
		FirstTime = FALSE;
	}

	NewPos = chan[MPG_INPUT_AXIS].Position;
	Change1 = NewPos - Pos;
	Pos = NewPos;

	if ((!ReadBit(MPG_XAXIS) && !ReadBit(MPG_YAXIS) && !ReadBit(MPG_ZAXIS)) || JOB_ACTIVE)	// if axis not selected or Job Active ignore the encoder.
		Change1 = 0;
	
	if (ReadBit(MPG_X1))		// is X1 selected?
		Factor = .001;
	else if (ReadBit(MPG_X10)){	// is X10 selected?
		Factor = .010;
	}
	else if (ReadBit(MPG_X100))	// is X100 selected?
		Factor = .100;
	else
		Factor = 0.0;

	
	Factor = Factor/4; //because 4 pulses/mechanical tick in MPG
	
	if (ReadBit(MPG_XAXIS)){	// is x selected?
		Axis = XAXIS;
		Factor = Factor*20807.68; // counts/rev
	}
	else if (ReadBit(MPG_YAXIS)){	// is y selected?
		Axis = YAXIS;
		Factor = Factor*25400; // counts/rev
	}
	else if (ReadBit(MPG_ZAXIS)){	// is z selected?
		Axis = ZAXIS;
		Factor = Factor*20807.68; // counts/rev
	}
	else {	// is off or 4
		Axis = -1;
		Factor = 0;
	}


	// Feedhold fully stopped ??
	if (CS0_StoppingState == 4 && InMotion)
	{
		Change1 = 0;			// ignore any MPG change
		Jog(LastAxis, 0);
		InMotion = FALSE;
	}

	// check if the Axis just changed or we have been 
	// converging to the target for a long time
	if(Axis != LastAxis && Axis != -1){
		printf("%d Axis selected\n", Axis);
	}
	
	
	if (Axis != LastAxis || (InMotion && Time_sec() > LastChangeTime + FINAL_TIME))
	{
		if (InMotion)
			Move(LastAxis, Target1);	//finalize any motion

		LastAxis = Axis;
		InMotion = FALSE;
		
	}

	
	if (Change1)				// did we move?
	{
		if (!InMotion)
			Target = Target1 = Target2 = chan[Axis].Dest;
		Target1 += Change1 * Factor;
		LastChangeTime = Time_sec();
		InMotion = TRUE;
	}

	if (InMotion)				// If moving
	{
		if (Target1 > chan[LastAxis].SoftLimitPos)
			Target1 = chan[LastAxis].SoftLimitPos;
		if (Target1 < chan[LastAxis].SoftLimitNeg)
			Target1 = chan[LastAxis].SoftLimitNeg;
		Target2 = Target2 * K1 + Target1 * K1M;
		Target = Target * K2 + Target2 * K2M;
		MoveExp(Axis, Target, TAU);
	}
}
