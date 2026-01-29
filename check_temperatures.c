#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"
#include "analog_voltage_to_temperature.c"
//#include "..\KflopToKMotionCNCFunctions.c"


// check for alarm conditions

float spindletemperature;
float xaxis1temperature;
float xaxis2temperature;
float yaxistemperature;
float zaxistemperature;
float cabinettemperature;
float coolanttemperature;
int Answer;
char text[1000];



void check_temperatures(int returnTemps) {

// note: these need to be adjusted for location and caution/warning values

/*
	spindletemperature = analog_voltage_to_temperature(SPINDLETEMP_AI);
	xaxis1temperature = analog_voltage_to_temperature(XAXIS1TEMP_AI);
	xaxis2temperature = analog_voltage_to_temperature(XAXIS2TEMP_AI);
	yaxistemperature = analog_voltage_to_temperature(YAXISTEMP_AI);
	zaxistemperature = analog_voltage_to_temperature(ZAXISTEMP_AI);
	coolanttemperature = analog_voltage_to_temperature(COOLANTTEMP_AI);

	cabinettemperature = analog_voltage_to_temperature(CABINETTEMP_AI);
#define SPINDLE_SPEED_AI		0
#define SPINDLETEMP_AI			1
#define XAXIS1TEMP_AI			2
#define XAXIS2TEMP_AI			3
#define YAXISTEMP_AI			4
#define ZAXISTEMP_AI			5
#define COOLANTTEMP_AI			6
#define CABINETTEMP_AI			7
*/

	spindletemperature = analog_voltage_to_temperature(SPINDLETEMP_AI);

	if (spindletemperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("Spindle temperature too high caution: %f\n", spindletemperature);
	} else if (spindletemperature > WARNING_TEMP) {
		printf("Spindle temperature too high warning: %f\n", spindletemperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	xaxis1temperature = analog_voltage_to_temperature(XAXIS1TEMP_AI);
	if (xaxis1temperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("X master motor temperature too high caution: %f\n", xaxis1temperature);
	} else if (xaxis1temperature > WARNING_TEMP) {
		printf("X master motor temperature too high warning: %f\n", xaxis1temperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	xaxis2temperature = analog_voltage_to_temperature(XAXIS2TEMP_AI);
	if (xaxis2temperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("X slave motor temperature too high caution: %f\n", xaxis2temperature);
	} else if (xaxis2temperature > WARNING_TEMP) {
		printf("X slave motor temperature too high warning: %f\n", xaxis2temperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	yaxistemperature = analog_voltage_to_temperature(YAXISTEMP_AI);
	if (yaxistemperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("Y motor temperature too high caution: %f\n", yaxistemperature);
	} else if (yaxistemperature > WARNING_TEMP) {
		printf("Y motor temperature too high warning: %f\n", yaxistemperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	zaxistemperature = analog_voltage_to_temperature(ZAXISTEMP_AI);
	if (zaxistemperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("Z motor temperature too high caution: %f\n", zaxistemperature);
	} else if (zaxistemperature > WARNING_TEMP) {
		printf("Z motor temperature too high warning: %f\n", zaxistemperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	
	coolanttemperature = analog_voltage_to_temperature(COOLANTTEMP_AI);
	if (coolanttemperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("Coolant temperature too high caution: %f\n", coolanttemperature);
	} else if (coolanttemperature > WARNING_TEMP) {
		printf("Coolant temperature too high warning: %f\n", coolanttemperature);
		SetBit(TEMPERATUREALARMBIT);
	}

	cabinettemperature = analog_voltage_to_temperature(CABINETTEMP_AI);
	if (cabinettemperature > CAUTION_TEMP) {
		returnTemps = 1;
		printf("cabinet air temperature too high caution: %f\n", cabinettemperature);
	} else if (cabinettemperature > WARNING_TEMP) {
		printf("cabinet air temperature too high warning: %f\n", cabinettemperature);
		SetBit(TEMPERATUREALARMBIT);
	}
	
	sprintf(text,"Spindle   %.1f",spindletemperature);
	DROLabel(SPINDLETEMPMEM,SPINDLETEMPVAR,text);
	sprintf(text,"X Axis    %.1f",xaxis1temperature);
	DROLabel(XAXISTEMPMEM,XAXISTEMPVAR, text);
	sprintf(text,"X slave   %.1f",xaxis2temperature);
	DROLabel(XAXISSLAVETEMPMEM,XAXISSLAVETEMPVAR, text);
	sprintf(text,"Y Axis    %.1f",yaxistemperature);
	DROLabel(YAXISTEMPMEM,YAXISTEMPVAR, text);
	sprintf(text,"Z Axis    %.1f",zaxistemperature);
	DROLabel(ZAXISTEMPMEM,ZAXISTEMPVAR, text);
	sprintf(text,"Coolant   %.1f",coolanttemperature);
	DROLabel(COOLANTTEMPMEM,COOLANTTEMPVAR, text);
	sprintf(text,"Cabinet   %.1f",cabinettemperature);
	DROLabel(CABINETTEMPMEM,CABINETTEMPVAR, text);



	if (returnTemps == 1) { // print out the current temperatures for reference
		sprintf(text,"Spindle %f\n Xaxis1 %f\n Xaxisslave %f\n Yaxis %f\n Zaxis %f\n Cabinet %f\n Coolant %f\n\n", spindletemperature, xaxis1temperature, xaxis2temperature, yaxistemperature, zaxistemperature, cabinettemperature, coolanttemperature);
		//Answer = MsgBox(text,MB_OK|MB_ICONEXCLAMATION);
		MsgBoxNoWait(text,MB_OK);
		//if (Answer == IDYES)
		//	printf("Answer is Yes\n");
		//else
		//	printf("Answer is No\n");
		//}
	}
	
	return 0;
}
