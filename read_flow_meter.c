#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"

// This reads the pulses coming off of a hall sensor in a flowmeter to monitor the
// flow rate of the spindle cooling pump.

// possible bug: introduce problems with spanning time slices? Slow rotation
// of spinner might mean not enough bit changes in the time before the thread 
// gets interrupted?

// Needs real value for pump flowing through all the hose, then 10% lower

// From spec sheet, 1L water = 463 pulses +/- 10%

float read_flow_meter(void)
{
								
	int flowBit;		// the value of the bit being read 
	int oldBit = 0;		// the old value to look for a change
	float startTime;		// the starting time of the measurement
	float secBetweenPulses = 1;	// how long to average for
	int bitsCounted = 0;	// the number of bits counted in each period
	int allowableRate = 10;
	float flowrate;
	float endTime;
	float litersPerMin;

	startTime = Time_sec();

	//printf("start time %f\n", startTime);
	
	while (Time_sec() < startTime + secBetweenPulses){
		endTime = Time_sec() - startTime;
		flowBit = ReadBit(FLOWSENSOR);
		//printf("%i", flowBit);

		if (flowBit == oldBit) {
			//printf("%i", flowBit);
			bitsCounted++;
			oldBit = !flowBit;
		}
		
	}
	
	flowrate = bitsCounted/endTime;
	litersPerMin = (flowrate/463)*60;
	

	//printf("alarm: %i\n", ReadBit(FLOWRATEALARMBIT));
	bitsCounted = 0; // reset for next reading

	return litersPerMin;

}
