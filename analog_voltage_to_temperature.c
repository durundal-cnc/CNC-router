#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"


// check for temperature conditions

float analog_voltage_to_temperature(int ADCchannel) {

	float temperature;
	float ADCinput = ADC(ADCchannel);

/* Measurements with resistors

10k = 1.826V = 374cts
30k = 0.044V = 10cts (think offscale)
200 = 4.199V = 860cts
1000 = 3.848V = 788cts
4.61k = 1.606V = 329cts

*/

	float R = (ADCinput * (-29.61)) + 21070;
	
	// for a 10k thermistor
	float A = 0.001125308852122;
	float B = 0.000234711863267;
	float C = 0.000000085663516;
	
	temperature = 1/(A + B*logf(R) + C * powf(logf(R),3)) - 273.15;
	
	return temperature;
}
