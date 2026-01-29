#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"
#include "analog_voltage_to_temperature.c"
#include "KflopToKMotionCNCFunctions.c"


// check for alarm conditions

int x_master_duty;
int x_slave_duty;
int y_duty;
int z_duty
int Answer;
char text[1000];

void check_servo_duty_cycle(void) {

	x_master_duty = ;
	x_slave_duty = ;
	y_duty = ;
	z_duty = ;
	spindletemperature = analog_voltage_to_temperature(SPINDLETEMP_AI);

	if (abs(x_master_duty) >= 1535) { // 2048counts*.75 since 750W motors on 1kW driver
		if(abs(x_slave_duty) >= 1535) {
			// this is ok because both are being torqued
		}
	
	
	
	
		returnTemps = 1;
		printf("Spindle temperature too high caution: %f\n", spindletemperature);
	} else if (spindletemperature > 75) {
		printf("Spindle temperature too high warning & feedhold: %f\n", spindletemperature);
		SetBit(FEEDHOLDBIT);
	}
	
	xaxis1temperature = analog_voltage_to_temperature(XAXIS1TEMP_AI);
	if (xaxis1temperature > 50) {
		returnTemps = 1;
		printf("X master motor temperature too high caution: %f\n", xaxis1temperature);
	} else if (xaxis1temperature > 75) {
		printf("X master motor temperature too high warning & feedhold: %f\n", xaxis1temperature);
		SetBit(FEEDHOLDBIT);
	}
	
	xaxis2temperature = analog_voltage_to_temperature(XAXIS2TEMP_AI);
	if (xaxis2temperature > 50) {
		returnTemps = 1;
		printf("X slave motor temperature too high caution: %f\n", xaxis2temperature);
	} else if (xaxis2temperature > 75) {
		printf("X slave motor temperature too high warning & feedhold: %f\n", xaxis2temperature);
		SetBit(FEEDHOLDBIT);
	}
	
	yaxistemperature = analog_voltage_to_temperature(YAXISTEMP_AI);
	if (yaxistemperature > 50) {
		returnTemps = 1;
		printf("Y motor temperature too high caution: %f\n", yaxistemperature);
	} else if (yaxistemperature > 75) {
		printf("Y motor temperature too high warning & feedhold: %f\n", yaxistemperature);
		SetBit(FEEDHOLDBIT);
	}
	
	zaxistemperature = analog_voltage_to_temperature(ZAXISTEMP_AI);
	if (zaxistemperature > 50) {
		returnTemps = 1;
		printf("Z motor temperature too high caution: %f\n", zaxistemperature);
	} else if (zaxistemperature > 75) {
		printf("Z motor temperature too high warning & feedhold: %f\n", zaxistemperature);
		SetBit(FEEDHOLDBIT);
	}
	
	
	coolanttemperature = analog_voltage_to_temperature(COOLANTTEMP_AI);
	if (coolanttemperature > 50) {
		returnTemps = 1;
		printf("Coolant temperature too high caution: %f\n", coolanttemperature);
	} else if (coolanttemperature > 75) {
		printf("Coolant temperature too high warning & feedhold: %f\n", coolanttemperature);
		SetBit(FEEDHOLDBIT);
	}

	cabinettemperature = analog_voltage_to_temperature(CABINETTEMP_AI);
	if (cabinettemperature > 28) {
		returnTemps = 1;
		printf("cabinet air temperature too high caution: %f\n", cabinettemperature);
	} else if (cabinettemperature > 75) {
		printf("cabinet air temperature too high warning & feedhold: %f\n", cabinettemperature);
		SetBit(FEEDHOLDBIT);
	}
	
	
	if (returnTemps == 1) { // print out the current temperatures for reference
		sprintf(text,"Spindle %f\n Xaxis1 %f\n Xaxisslave %f\n Yaxis %f\n Zaxis %f\n Cabinet %f\n Coolant %f\n\n", spindletemperature, xaxis1temperature, xaxis2temperature, yaxistemperature, zaxistemperature, cabinettemperature, coolanttemperature);
		//Answer = MsgBox(text,MB_OK|MB_ICONEXCLAMATION);
		MsgBoxNoWait(text,MB_OK);
		Delay_sec(10); // wait 10 seconds before popping up again
		//if (Answer == IDYES)
		//	printf("Answer is Yes\n");
		//else
		//	printf("Answer is No\n");
		//}
	}
	
	return 0;
}
