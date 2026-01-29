#include "KMotionDef.h"
#include "init_definitions.h"
#include "function_definitions.h"
//#include "..\KflopToKMotionCNCFunctions.c"
#define TMP 10 // which spare persist to use to transfer data

// check for physical buttons being pressed and sets their bits
// Processing occurs elsewhere, this just reads their status 
// and hides the messiness of the debounce code


int Debounce(int n, int *cnt, int *last, int *lastsolid, int dbtime);
int result;

// state variables for switch debouncing

int alast=0,alastsolid=-1,acount=0;
int blast=0,blastsolid=-1,bcount=0;
int clast=0,clastsolid=-1,ccount=0;
int dlast=0,dlastsolid=-1,dcount=0;
int elast=0,elastsolid=-1,ecount=0;
int flast=0,flastsolid=-1,fcount=0;

int hlast=0,hlastsolid=-1,hcount=0;

int ilast=0,ilastsolid=-1,icount=0;
int jlast=0,jlastsolid=-1,jcount=0;
int klast=0,klastsolid=-1,kcount=0;
int llast=0,llastsolid=-1,lcount=0;
int mlast=0,mlastsolid=-1,mcount=0;
int nlast=0,nlastsolid=-1,ncount=0;
int olast=0,olastsolid=-1,ocount=0;



int rlast=0,rlastsolid=-1,rcount=0;
int zlast=0,zlastsolid=-1,zcount=0;



void read_external_buttons(void)
{
	result = Debounce(ReadBit(MAINPOWER_OPTO),&fcount,&flast,&flastsolid, 50);
	if (result == 1) { // not technically a switch but goes here for now
		// this reads the main power relay status as being on or off through a 24V line passed through one of the mechanical contacts
		SetBit(MAINPOWERSTATUSBIT);
		printf("Main power relay on\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Main power relay on\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	if (result == 0) {
		ClearBit(MAINPOWERSTATUSBIT);
		printf("Main power relay off\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Main power relay off\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(SERVO_OPTO),&ccount,&clast,&clastsolid, 50);
	if (result == 1) { // Read the amber feedhold button on the cabinet (old: read the feedhold buttons (dedicated amber one as well as red stop button))
		if(ReadBit(AXES_ENABLED)) {
			enable_servos(0, 1); //disable servos
			ClearBit(AXES_ENABLED);

			printf("external cabinet button and disable servos!\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Press from external cabinet button to disable servos!\n"); //KFLOP memory location, KMotionCNC variable (?), value
		} else {
			ClearStopImmediately(); // this will allow motion but not re-engage last command. Otherwie would restart motion when servos turn back on.

			enable_servos(1, 1); //Re-enable servos
			SetBit(AXES_ENABLED);
			
			printf("external cabinet button and re-enable servos\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Press from external cabinet button to re-enable servos\n"); //KFLOP memory location, KMotionCNC variable (?), value


		}
		//end toggle
	}	
/* 	old code
if (result == 1) { // Read the amber feedhold button on the cabinet (old: read the feedhold buttons (dedicated amber one as well as red stop button))
		//toggle FEEDHOLDBIT
		if(ReadBit(FEEDHOLDBIT)) {
			ClearBit(FEEDHOLDBIT);
			ClearStopImmediately(); // this will allow motion but not re-engage last command
			enable_servos(1, 1); //Re-enable servos
			// new addition
			enable axes
			
			//end new
			//ResumeCoordinatedMotion(); // this will allow motion and immediately resume last motion (!)
			printf("Release feedhold from external cabinet button and re-enable servos\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Release feedhold from external cabinet button and re-enable servos\n"); //KFLOP memory location, KMotionCNC variable (?), value

		} else {
			SetBit(FEEDHOLDBIT);
			StopCoordinatedMotion(); //redundant with call in cnc_router_main_supervisor?
			
			//new addition
			disable axes
			// end new
			enable_servos(0, 1); //disable servos
			printf("Set feedhold from external cabinet button and disable servos!\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Set feedhold from external cabinet button and disable servos!\n"); //KFLOP memory location, KMotionCNC variable (?), value

		}
		//end toggle
	}	
	 */
	result = Debounce(ReadBit(LIMITBYPASS_OPTO),&ecount,&elast,&elastsolid,50);
	if (result == 1) {
		SetBit(LIMITBYPASSBIT);
		printf("*****************************************************\n");
		printf("*** Use extreme caution! Limit switches bypassed!****\n");
		printf("***                                              ****\n");
		printf("*** Use extreme caution! Limit switches bypassed!****\n");
		printf("***                                              ****\n");
		printf("*** Use extreme caution! Limit switches bypassed!****\n");
		printf("***                                              ****\n");
		printf("*** Use extreme caution! Limit switches bypassed!****\n");
		printf("*****************************************************\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"*** Use extreme caution! Limit switches bypassed!****\n"); //KFLOP memory location, KMotionCNC variable (?), value


	}
	if (result == 0) {
		ClearBit(LIMITBYPASSBIT);
		printf("Mechanical limit switches enabled\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Mechanical limit switches enabled\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(AUXPOWER_OPTO),&hcount,&hlast,&hlastsolid,50);
	if (result == 1) {
		SetBit(AUXPOWERBIT);
		printf("Auxiliary 120V power on\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Auxiliary 120V power on\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	if (result == 0) {
		ClearBit(AUXPOWERBIT);
		printf("Auxiliary 120V power off\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Auxiliary 120V power off\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}		
	
	// treat the alarm bits on servos as same (since they seem to dither with motion but LED doesn't illuminate)
	result = Debounce(ReadBit(XAXIS_ALARM),&acount,&alast,&alastsolid,50);
	if (result == 1) {
		SetBit(FEEDHOLDBIT);
		printf("XAXIS servo alarm\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"XAXIS servo alarm\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(XAXISSLAVE_ALARM),&bcount,&blast,&blastsolid,50);
	if (result == 1) {
		SetBit(FEEDHOLDBIT);
		printf("XAXIS slave servo alarm\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"XAXIS slave servo alarm\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(ZAXIS_ALARM),&dcount,&dlast,&dlastsolid,50);
	if (result == 1) {
		SetBit(FEEDHOLDBIT);
		printf("ZAXIS servo alarm\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"ZAXIS servo alarm\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(RED_SW_BY_CABINET),&icount,&ilast,&ilastsolid,50);
	if (result == 1) {
		SetBit(FEEDHOLDBIT);
		printf("Manual feedhold (near side)\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Manual feedhold (near side)\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(GREEN_SW_BY_CABINET),&jcount,&jlast,&jlastsolid,50);
	if (result == 1) { // button pressed
		if(ReadBit(TOOLSETTER_ACTIVE)){ // alternate purpose in toolsetter (hold to advance)
			SetBit(TOOLSETTER_ADVANCE1);
			printf("advancing\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"advancing\n"); //KFLOP memory location, KMotionCNC variable (?), value

		} else {
			ClearBit(FEEDHOLDBIT);
			ResumeCoordinatedMotion();
			printf("Manual cycle start (near side)\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Manual cycle start (near side)\n"); //KFLOP memory location, KMotionCNC variable (?), value

		}
	} else { //button not pressed
		if(ReadBit(TOOLSETTER_ACTIVE)) {
			ClearBit(TOOLSETTER_ADVANCE1);
		}
	}
	
	result = Debounce(!ReadBit(GREEN_SW_FAR_SIDE),&kcount,&klast,&klastsolid,50);
	if (result == 0) { // button pressed (reversed logic)
		if(ReadBit(TOOLSETTER_ACTIVE)){ 
			SetBit(TOOLSETTER_ADVANCE2); // alternate purpose in toolsetter (hold to advance)
			printf("advancing\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"advancing\n"); //KFLOP memory location, KMotionCNC variable (?), value

		} else {
			ClearBit(FEEDHOLDBIT); // normal mode
			ResumeCoordinatedMotion(); //this will allow motion and immediately resume last motion (!)

			printf("Manual cycle start (far side)\n");
			DROLabel(MESSAGEMEM,MESSAGEVAR,"Manual cycle start (far side)\n"); //KFLOP memory location, KMotionCNC variable (?), value

		}
	} else { //button not pressed
		if(ReadBit(TOOLSETTER_ACTIVE)) {
			ClearBit(TOOLSETTER_ADVANCE2);
		}
	}
	
	result = Debounce(!ReadBit(RED_SW_FAR_SIDE),&lcount,&llast,&llastsolid,50);
	if (result == 1) {
		SetBit(FEEDHOLDBIT);
		printf("Manual feedhold (far side)\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"Manual feedhold (far side)\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(CW_SW_FAR_SIDE),&mcount,&mlast,&mlastsolid,50);
	if (result == 1) {
		printf("CW switch\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"CW switch\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	result = Debounce(ReadBit(CCW_SW_FAR_SIDE),&ncount,&nlast,&nlastsolid,50);
	if (result == 1) {
		printf("CCW switch\n");
		DROLabel(MESSAGEMEM,MESSAGEVAR,"CCW switch\n"); //KFLOP memory location, KMotionCNC variable (?), value

	}
	
	
// Only active when toolsetter active (in ServiceToolsetter)
//	result = Debounce(!ReadBit(TOOLSET_OVERTRAVEL),&ocount,&olast,&olastsolid,50);
//	if (result == 1) {
//		if(TOOLSETTER_ACTIVE){ //active low
//			SetBit(FEEDHOLDBIT);
//			printf("Toolsetter overtravel feedhold\n");
//		} else {
//			printf("Toolsetter overtravel active but not checked\n");
//		}
//	}

	return 0;
}



// Debounce a bit
//
// return 1 one time when first debounced high 
// return 0 one time when first debounced low 
// return -1 otherwise 
#define DBTIME 50 // default was 300

int Debounce(int n, int *cnt, int *last, int *lastsolid, int dbtime)
{

	int v = -1;
	
	if (n == *last)  // same as last time?
	{
		if (*cnt == dbtime-1)
		{
			if (n != *lastsolid)
			{
				v = *lastsolid = n;  // return debounced value
			}
		}
		if (*cnt < dbtime)	(*cnt)++;
	}
	else
	{
		*cnt = 0;  // reset count
	}
	*last = n;
	return v;
}
