#include "KMotionDef.h"
#include "init_definitions.h"
//#include "function_definitions.h"
//#include "abs.c" //absolute value

//Get the spindle RPM from the WJ200 reported 10V analog output
//calibrations from no-load measurements
#define ADC0 		42
#define SLOPE0 		0 //need low speed measurement for accuracy but since don't use lower than 8000 anyway it's ok
#define ADC8000 	673
#define SLOPE8000 	11.88707280832095
#define ADC12000 	999
#define SLOPE12000 	12.01201201201201
#define ADC18000 	1491
#define SLOPE18000 	12.07243460764588
#define ADC20000 	1650
#define SLOPE20000 	12.12121212121212
#define ADC22000 	1815
#define SLOPE22000 	12.12121212121212
#define ADC24000 	1954
#define SLOPE24000 	12.28249744114637

/*
After adjusting WJ200 gain to 90 so doesn't offscale
24000 = 1954 = 12.28249744114637
22000 = 1815 = 12.12121212121212
20000 = 1650 = 12.12121212121212
18000 = 1491 = 12.07243460764588
16000 = 1325 = 12.07547169811321
14000 = 1163 = 12.0378331900258
12000 = 999 = 12.01201201201201
10000 = 834 = 11.99040767386091
8000 = 673 = 11.88707280832095
6000 = 506 = 11.85770750988142
off = 42


*/

float get_spindle_rpm_from_adc(void) {
	
	float spindle_RPM; //the computed RPM
	float adc_counts = ADC(SPINDLE_SPEED_AI);//ADC input in counts

	
	//make variable slope range based on ADC counts
	if (adc_counts < ADC0) {
		spindle_RPM = 0;
	} else if (adc_counts >= ADC0  &&  adc_counts < ADC8000) {
		spindle_RPM = adc_counts * (SLOPE0 + (SLOPE8000-SLOPE0) * ((adc_counts-ADC0)/(ADC8000-ADC0)));
	} else if (adc_counts >= ADC8000  &&  adc_counts < ADC12000) {
		spindle_RPM = adc_counts * (SLOPE8000 + (SLOPE12000-SLOPE8000) * ((adc_counts-ADC8000)/(ADC12000-ADC8000)));
	} else if (adc_counts >= ADC12000  &&  adc_counts < ADC18000){
		spindle_RPM = adc_counts * (SLOPE12000 + (SLOPE18000-SLOPE12000) * ((adc_counts-ADC12000)/(ADC18000-ADC12000)));
	} else if (adc_counts >= ADC18000  &&  adc_counts <= ADC20000){
		spindle_RPM = adc_counts * (SLOPE18000 + (SLOPE20000-SLOPE18000) * ((adc_counts-ADC18000)/(ADC20000-ADC18000)));
	} else if (adc_counts >= ADC20000  &&  adc_counts <= ADC22000){
		spindle_RPM = adc_counts * (SLOPE20000 + (SLOPE22000-SLOPE20000) * ((adc_counts-ADC20000)/(ADC22000-ADC20000)));
	} else if (adc_counts >= ADC22000  &&  adc_counts <= ADC24000){
		spindle_RPM = adc_counts * (SLOPE22000 + (SLOPE24000-SLOPE22000) * ((adc_counts-ADC22000)/(ADC24000-ADC22000)));
	} else if (adc_counts >= ADC24000){
		spindle_RPM = adc_counts * SLOPE24000;
	} else {
		spindle_RPM = -1;
	}
	//printf("Spindle RPM %f\n", spindle_RPM);
	return spindle_RPM;
}