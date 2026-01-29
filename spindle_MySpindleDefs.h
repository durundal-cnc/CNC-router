#include "init_definitions.h"
#define FACTOR (1.0/24000.0) // to convert RPM to counts/sec 
//	#define SPINDLE_REV_SW			145	
//	#define SPINDLE_FWD_SW			146	
#define SPINDLECW_BIT SPINDLE_FWD_SW   	// bit to activate to cause CW rotation
#define SPINDLECCW_BIT SPINDLE_REV_SW		// bit to activate to cause CCW rotation
#define SPEEDVAR 99				// global persistant variable to store latest speed
#define STATEVAR 98				// global persistant variable to store latest state (-1=CCW,0=off,1=CW)
#define KMVAR PC_COMM_CSS_S 	// variable KMotionCNC will pass speed parameter (113)
#define USE_POS_NEG_VOLTAGE 0 	// 0 = output Magnitude, 1 = output positive and negative speed 
