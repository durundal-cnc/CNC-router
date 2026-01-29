#include "KMotionDef.h"
#include "init_definitions.h"
//Set up to watch for toolsetter knife bit
//When knife seen, set tool Z position = height of toolsetter (nominally 



//This program always gets started by software button in KmotionCNC
void ServiceToolsetter() 
{
//measures 79.0mm when 1037 goes from high to low (using crummy calipers)
	char text[1000];
	int offer_reset = 0;

	if (ReadBit(TOOLSETTER_ACTIVE)) { // don't trigger if not in toolsetting mode

		//toolsetter bits active low
		if(ReadBit(TOOLSET_OVERTRAVEL) && ReadBit(TOOLSET_KNIFE) && !ReadBit(FEEDHOLDBIT)){
			//wait
			if(ReadBit(GREEN_SW_BY_CABINET) || !ReadBit(GREEN_SW_FAR_SIDE)){ //far side reversed function
				SetBit(1298);
				if(!ReadBit(1299)){
					Jog(ZAXIS,-500); // move down same speed as homing
					SetBit(1299);
				}
			} else {
				if(ReadBit(1298)){
					ClearBit(1298);
					ClearBit(1299);
					Jog(ZAXIS,0.0); //StopMotion
				}
			}
			
		} else {
			ClearBit(1299);
			Jog(ZAXIS,0.0); // StopMotion
			printf("exiting toolsetter\n");

			if(!ReadBit(TOOLSET_OVERTRAVEL)) {
				SetBit(FEEDHOLDBIT); //also set in read buttons
				printf("Toolsetter overtravel, feedhold set\n");
				sprintf(text, "Toolsetter overtravel, feedhold set. Position %f\n", chan[ZAXIS].Position);
				DROLabel(MESSAGEMEM,MESSAGEVAR,text); //KFLOP memory location, KMotionCNC variable (?), value
				offer_reset = 0;
				
			} else if(ReadBit(FEEDHOLDBIT)){
				printf("Aborting toolsetter sequence due to external feedhold\n");
				sprintf(text, "Aborting toolsetter sequence due to external feedhold. Position %f\n", chan[ZAXIS].Position);
				DROLabel(MESSAGEMEM,MESSAGEVAR,text); //KFLOP memory location, KMotionCNC variable (?), value
				offer_reset = 0;
				
			} else if(!ReadBit(TOOLSET_KNIFE)) {
				sprintf(text, "Toolsetting Complete. Position %f\nSet to 4.000 inches on DRO.\n", chan[ZAXIS].Position);
				// 20807.68 counts/inches (79mm)
				// trigger is 3.1102" above table 0" above table
//				MsgBoxNoWait(text,MB_OK);
				MoveRel(ZAXIS,18514.67366);  // move up from knife to 4.0 inches off table (underside of toolsetter)
				//79mm = 3.110236"
				//4" - 3.110236" = 0.889763"
				//0.889763"*20807.68counts/in = 18513.93
//				MsgBoxNoWait("Move to 4.000 complete\n",MB_OK);

				//ToolTableSet_am(); // adjust tool table
				printf("Toolsetting Complete\nAdjust Z to 4.000 in");
				DROLabel(MESSAGEMEM,MESSAGEVAR,"Toolsetting Complete\n"); //KFLOP memory location, KMotionCNC variable (?), value
				offer_reset = 1;
			}

			if (offer_reset == 0){
				MsgBoxNoWait(text,MB_OK);
			}
			if (offer_reset == 1){
				Answer = MsgBox("Set Z to 4.000in?\n",MB_YESNO|MB_ICONEXCLAMATION);
				
				if (Answer == IDYES) {
					printf("Answer is Yes, set Z to 4.000in\n");
					DROLabel(MESSAGEMEM,MESSAGEVAR,"Answer is Yes, set Z to 4.000in\n"); //KFLOP memory location, KMotionCNC variable (?), value
					DoPCFloat(PC_COMM_SET_Z, 4.0);
				} else {
					printf("Answer is No, do not reset\n");
					DROLabel(MESSAGEMEM,MESSAGEVAR,"Answer is No, do not reset\n"); //KFLOP memory location, KMotionCNC variable (?), value
				}
			}
			ClearBit(TOOLSETTER_ACTIVE);
		}
	}
}
