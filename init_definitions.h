//// Axis channel (ch0..7) definitions (these are in order with the JP1 differential inputs for clarity but don't need to be)
#define SPINDLEAXIS	0 // this is the spindle (just an AO channel)
#define YAXIS		1 // this is the Leadshine drive
// Quad A/B Channel 1 used for Leadshine and XAXIS index
#define XAXISSLAVE	2 // this is the two 750W servos
#define XAXIS	    3 // This is the 2nd motor, slaved to the first
// QuadA/B Channel  4 used for XAXISSLAVE and ZAXIS index
#define ZAXIS		4 // this is the 1000W DMM servo	
// Quad A/B Channel 6 unfilled 
#define MPG_INPUT_AXIS 7 // This is the pulse output of the MPG pendant

/*Axis mode "input" set to encoder
InputChan0..7 means encoder channel
Axis mode "output" set to DAC servo
(if set to other options can conflict with Kanalog/Konnect communications bus)
OutputChan0..7 means analog out channel

Axis 0 = ch0 = spindle = OutputChan0
Axis 1 = ch1 = Y axis = 			InputChan0 = OutputChan1
Axis 2 = ch2 = X axis master = 	Inputchan2 = OutputChan2
Axis 3 = ch3 = X axis slave = 	InputChan3 = OutputChan3
Axis 4 = ch4 = Z axis =			InputChan5 = OutputChan4
Axis 5 = unused
Axis 6 = unused
"Axis" 7 = pendant = 			InputChan7
*/


/* Thread allocations
1 - run programs from user buttons in tool setup screen (home and spindle controls)
2 - run other programs from main CNC screen
3 - available
4 - ShowInstantVelocityDROBar.c
5 - MPG
6 - check_time_insensitive_thread
7 - main programs
*/




//// Aux0 port
// unused



#define XAXIS_ALARM			1045 // pin 21
#define XAXISSLAVE_ALARM	1046 // pin 22
#define YAXIS_ALARM			1048 // not yet added (will be when Leadshine replaced by DMM)
#define ZAXIS_ALARM			1047 // pin 23

//// JP1 Differential inputs
#define YAXISMOTOR_QA		0 // y axis motor A
#define YAXISMOTOR_QB		1 // y axis motor B							InputChan0

#define YAXISINDEX_Q		2 // y index
#define XAXIS1INDEX_Q		3 // x index slaved							InputChan1

#define XAXIS1MOTOR_QA		4 // x axis motor slaved A
#define XAXIS1MOTOR_QB		5 // x axis motor slaved B					InputChan2

#define XAXIS2MOTOR_QA		6 // x axis motor A
#define XAXIS2MOTOR_QB		7 // x axis motor B							InputChan3

//// JP2 differential inputs
#define XAXIS2INDEX_Q		36 // x index 
#define ZAXISINDEX_Q		37 // z index								InputChan4

#define ZAXISMOTOR_QA		38 // z axis motor A
#define ZAXISMOTOR_QB		39 // z axis motor B						InputChan5

//	#define unused				40 //replaced chip 8-6-20 
//	#define unused				41										InputChan6

#define MPG_QA 				42	// mpg pendant A quadrature bit 
#define MPG_QB 				43	// mpg pendant B quadrature bit			InputChan7


//// JP6 analog outputs
// values are channel, called by DAC(ch, value);
#define SPINDLE_CONTROL_AO		0
#define YAXIS_CONTROL_AO		1
#define XAXIS1_CONTROL_AO		2
#define XAXIS2_CONTROL_AO		3
#define ZAXIS_CONTROL_AO		4
//	#define unused				5
//	#define unused				6
//	#define unused				7


//after rewire
#define AUXPOWER_OUT 			152
//#define 		153 // 
//#define 		154 // 
//#define 		155 // 
//#define		156 //
//#define 		157 //
//#define		158 //
//#define 		159 //

//// JP11 analog inputs
// values are channel, called by ADC(ch);
#define SPINDLE_SPEED_AI		0
#define SPINDLETEMP_AI			1
#define XAXIS1TEMP_AI			2
#define XAXIS2TEMP_AI			3
#define YAXISTEMP_AI			4
#define ZAXISTEMP_AI			5
#define COOLANTTEMP_AI			6
#define CABINETTEMP_AI			7


//// JP13 opto outputs
#define MAINPOWER_ENA			144	//main power relay (chains to SWE)
#define SPINDLECCW_BIT			145	// was SPINDLE_REV_SW
#define SPINDLECW_BIT			146	// was SPINDLE_FWD_SW
//	#define 			147	
#define X1MASTER_ENABLE			148	
#define X2SLAVE_ENABLE			149	
#define Z_ENABLE				150	
#define LEADSHINE_ENABLE		151	


//// JP15 opto inputs
//Reserve these for primary power and control inputs, use Konnect for external inputs
#define VFDPIN_11_OTPO		136	
#define VFDPIN_12_OPTO		137	
#define ZPROXNEG			138 // other prox sensors on Konnect
#define ZPROXPOS			139 // other prox sensors on Konnect
#define MAINPOWER_OPTO		140	
#define AUXPOWER_OPTO		141	
#define SERVO_OPTO			142	
#define LIMITBYPASS_OPTO	143	

// Konnect outputs
#define SERVO_LAMP_SW		48	
#define FEEDHOLD_LAMP_SW	49	
//#define 					50	
//#define 					51	
//#define 					52	
//#define 					53
#define VIRTUALBIT54		54	
#define VIRTUALBIT55		55	
#define VIRTUALBIT56		56	
#define VIRTUALBIT57		57	
#define VIRTUALBIT58		58	
#define VIRTUALBIT59		59	
#define VIRTUALBIT60		60	
#define VIRTUALBIT61		61
#define VIRTUALBIT62		62	
#define VIRTUALBIT63		63	
//also extended bits 1056 through 2047


//Konnect inputs
//#define 					1024
//#define 					1025
//#define 					1026
//#define 					1027
//#define 					1028
//#define 					1029
//#define 					1030
#define FLOWSENSOR			1031
//#define 					1032
#define XPROXPOS			1033 // check
#define XPROXNEG			1032 // check
#define YPROXPOS			1034
#define YPROXNEG			1035 
#define XSLAVEHOMEPROX		1036	
#define TOOLSET_KNIFE		1037
#define RED_SW_BY_CABINET	1038
#define GREEN_SW_BY_CABINET	1039
#define GREEN_SW_FAR_SIDE	1040
#define RED_SW_FAR_SIDE		1041
#define CW_SW_FAR_SIDE		1042
#define CCW_SW_FAR_SIDE		1043
#define TOOLSET_OVERTRAVEL	1044
//#define Servo alarm 1		1045
//#define Servo alarm 2		1046
//#define Servo alarm 3		1047

// Aux1 to Konnect (if need PWM, move over to Aux0 with some additional complexity in initialization
// "off" is not wired (all axes bits = 0)
#define MPG_XAXIS      		1050	//pins 24-31
#define MPG_YAXIS      		1055
#define MPG_ZAXIS      		1054
#define MPG_4AXIS      		1049
#define MPG_X1         		1051
#define MPG_X10        		1053
#define MPG_X100       		1052
#define MPG_ESTOP	  		1048 //circular switch on MPG, not part of e-stop chain. Used to enable MPG or not

// universal bits (using as though global variables inside KFLOP)
#define FEEDHOLDBIT 1248
#define MAINPOWERSTATUSBIT 1249
#define LIMITBYPASSBIT 1250
#define LIMITZPLUS 1251
#define LIMITZMINUS 1252
#define LIMITYPLUS 1253
#define LIMITYMINUS 1254
#define LIMITXPLUS 1255
#define LIMITXMINUS 1256
#define AUXPOWERBIT 1257
#define FLOWRATEALARMBIT 1258
#define PROXSENSORBYPASS 1259
#define TOOLSETTER_ACTIVE 1260
#define TOOLSETTER_ADVANCE1 1261 // used for jogging down into toolsetter
#define TOOLSETTER_ADVANCE2 1262 // used for jogging down into toolsetter
#define SPINDLE_STARTING 1263 // is the spindle starting up?
#define AXES_ENABLED 1264 //are axes enabled or disabled

#define TEMPERATUREALARMBIT 1300

// persistant user variables & memory
// Kflop contains a global array of 200 32-bit integer values that are zeroed on power up unless flash user data is done
//These are what get sent from the KFLOP to KMotionCNC as a way to send data (included in Bulk Status record)
#define TMP 10 // which persist variable functions in KflopToKMotionCNCFunctions.c will use. Unclear how this works but Tom Kerekes seems to indicate ten persistants are used after TMP? (so TMP = 10 means 10,11,..,20 are used)

/* ____VAR    global persistant variable to store flowmeter rate, also associated with a control on KmotionCNC
____MEM    used to allocate memory in gather buffer to be passed to DROLabels (this number is the address in 32 bit space). Size is 1000000 8 byte doubles

Once KmotionCNC reads the value in the persist buffer it will clear it. But a thread reading it needs to waitfornexttimeslice() to get the updated, cleared value.
If you wait for the value to clear the same memory space in the gather buffer can be used for all ____VARs without overwritting if lengths are wrong.
From KmotionDef.h:
#define MAX_GATHER_DATA 1000000 // Size of gather buffer (number of doubles, 8 bytes each).                   
extern double *gather_buffer;   // Large buffer for data gathering, Bode plots, or User use

From KflopToKMotionCNCFunctions.c:
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
} */

/* 
#define FLOWVAR 		90	
#define FLOWMEM 		1000
#define SPINDLETEMPVAR 	91
#define SPINDLETEMPMEM 	1100
#define XAXISTEMPVAR 	92
#define XAXISTEMPMEM 	1200
#define XAXISSLAVETEMPVAR 93
#define XAXISSLAVETEMPMEM 1300
#define YAXISTEMPVAR 	94
#define YAXISTEMPMEM 	1400
#define ZAXISTEMPVAR 	95
#define ZAXISTEMPMEM 	1500
#define COOLANTTEMPVAR 	96
#define COOLANTTEMPMEM 	1600
#define CABINETTEMPVAR 	97
#define CABINETTEMPMEM 	1700
#define INSTANTFEEDVELVAR 162
#define INSTANTFEEDVELMEM 1800
#define INSTANTFEEDVELBAR 163 */
//need reference list of persist variables from kflop.h files so don't duplicate accidentally

#define FLOWVAR 		90				// global persistant variable to store flowmeter rate
#define FLOWMEM 		1000			//used to allocate memory to be passed to DROLabels (this number is the address in 32 bit space). Size is 1000000 8 byte doubles
//#define MAX_GATHER_DATA 1000000 // Size of gather buffer (number of doubles, 8 bytes each).                   
//extern double *gather_buffer;   // Large buffer for data gathering, Bode plots, or User use
#define SPINDLETEMPVAR 	91
#define SPINDLETEMPMEM 	2000 //count of how many 32 bit words
#define XAXISTEMPVAR 	92
#define XAXISTEMPMEM 	3000
#define XAXISSLAVETEMPVAR 93
#define XAXISSLAVETEMPMEM 4000
#define YAXISTEMPVAR 	94
#define YAXISTEMPMEM 	5000
#define ZAXISTEMPVAR 	95
#define ZAXISTEMPMEM 	6000
#define COOLANTTEMPVAR 	96
#define COOLANTTEMPMEM 	7000
#define CABINETTEMPVAR 	97
#define CABINETTEMPMEM 	8000
#define INSTANTFEEDVELVAR 162
#define INSTANTFEEDVELMEM 9000
#define INSTANTFEEDVELBAR 163
#define SPINDLESPEEDVAR 164
#define SPINDLESPEEDMEM 10000

#define STATEVAR 		98				// global persistant variable to store latest state (-1=CCW,0=off,1=CW)
#define SPEEDVAR 		99				// global persistant variable to store latest speed
//variables from 100 to 107 are special (see https://dynomotion.com/Help/KMotionCNC/KMotionCNCCmdsFrKFLOP.htm )
#define TIMELEFTVAR		87
#define TIMELEFTMEM		11000
#define SMALLMSGVAR		88
#define SMALLMSGMEM		12000
#define MESSAGEVAR		89
#define MESSAGEMEM		13000

#define DRO_X1VEL_VAR	86
#define DRO_X1VEL_MEM	14000
#define DRO_X2VEL_VAR	85
#define DRO_X2VEL_MEM	15000
#define DRO_YVEL_VAR	84
#define DRO_YVEL_MEM	16000
#define DRO_ZVEL_VAR	83
#define DRO_ZVEL_MEM	17000

// define constants of interest
#define CAUTION_TEMP 35.0 // caution temperature limit
#define WARNING_TEMP 50.0 // warning temperature limit
#define FLOWRATE_MIN 1.0 // the minimum flow rate to the spindle in L/min
#define FACTOR (1.0/24000.0) // to convert RPM to counts/sec for the spindle
#define KMVAR PC_COMM_CSS_S 	// variable KMotionCNC will pass speed parameter (113)
#define USE_POS_NEG_VOLTAGE 0 	// 0 = output Magnitude, 1 = output positive and negative speed 
#define DELTATIME 1.0 // Seconds to wait for time insensitive thread
