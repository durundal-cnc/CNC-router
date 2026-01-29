//Sets up the X axis motor parameters (ch2 and 3)
/*
Axis mode "input" set to encoder
InputChan0..7 means encoder channel
Axis mode "output" set to DAC servo
(if set to other options can conflict with Kanalog/Konnect communications bus)
OutputChan0..7 means analog out channel

Axis 0 = ch0 = spindle = OutputChan0
Axis 1 = ch1 = Y axis = 		InputChan0 = OutputChan1
Axis 2 = ch2 = X axis master = 	Inputchan2 = OutputChan2
Axis 3 = ch3 = X axis slave = 	InputChan3 = OutputChan3
Axis 4 = ch4 = Z axis =			InputChan5 = OutputChan4
Axis 5 = unused
Axis 6 = unused
"Axis" 7 = pendant = 			InputChan7
*/
#include "init_definitions.h"


void init_x_motors(void){

	ch2->InputMode=ENCODER_MODE;
	ch2->OutputMode=DAC_SERVO_MODE;
	ch2->Vel=104035; //was 70000 This is for rapids, feeds set in Kmotion
	ch2->Accel=150000; //was 150000
	ch2->Jerk=4e+06;
	ch2->P=7.67; // for bare motors
	ch2->I=0.05;
	ch2->D=550;
	ch2->FFAccel=0.0006;
	ch2->FFVel=0.05;
	ch2->MaxI=60;
	ch2->MaxErr=200; //this is the maximum error sent to the PID servo control (in position units)
	ch2->MaxOutput=1535;
	ch2->DeadBandGain=1;
	ch2->DeadBandRange=0;
	
	
	ch2->InputChan0=XAXISSLAVE;
	ch2->InputChan1=0;
	ch2->OutputChan0=XAXISSLAVE;
	ch2->OutputChan1=0;
	ch2->MasterAxis=XAXIS;
	ch2->LimitSwitchOptions=0x113;
	ch2->LimitSwitchNegBit=XPROXNEG;
	ch2->LimitSwitchPosBit=XPROXPOS;
	ch2->SoftLimitPos=1e+09;
	ch2->SoftLimitNeg=-1e+09;
	ch2->InputGain0=1;
	ch2->InputGain1=1;
	ch2->InputOffset0=0;
	ch2->InputOffset1=0;
	ch2->OutputGain=1;
	ch2->OutputOffset=0;
	ch2->SlaveGain=1;
	ch2->BacklashMode=BACKLASH_OFF;
	ch2->BacklashAmount=0;
	ch2->BacklashRate=0;
	ch2->invDistPerCycle=1;
	ch2->Lead=0;
	ch2->MaxFollowingError=200; //this is the max error allowable in following a G code path
	ch2->StepperAmplitude=20;

	ch2->iir[0].B0=1;
	ch2->iir[0].B1=0;
	ch2->iir[0].B2=0;
	ch2->iir[0].A1=0;
	ch2->iir[0].A2=0;

	ch2->iir[1].B0=1;
	ch2->iir[1].B1=0;
	ch2->iir[1].B2=0;
	ch2->iir[1].A1=0;
	ch2->iir[1].A2=0;

	ch2->iir[2].B0=0.0166094; //2nd order low pass 500Hz Q1.4
	ch2->iir[2].B1=0.0332189;
	ch2->iir[2].B2=0.0166094;
	ch2->iir[2].A1=1.60679;
	ch2->iir[2].A2=-0.673229;
	
	
	
	// slave motor
	
	ch3->InputMode=ENCODER_MODE;
	ch3->OutputMode=DAC_SERVO_MODE;
	ch3->Vel=104035; //was 70000 This is for rapids, feeds set in Kmotion
	ch3->Accel=150000;
	ch3->Jerk=4e+06;
	ch3->P=7.67; // for bare motors
	ch3->I=0.05;
	ch3->D=550;
	ch3->FFAccel=0.0006;
	ch3->FFVel=0.05;
	ch3->MaxI=60;
	ch3->MaxErr=200; //this is the maximum error sent to the PID servo control (in position units)
	ch3->MaxOutput=1535;
	ch3->DeadBandGain=1;
	ch3->DeadBandRange=0;
	
	ch3->InputChan0=XAXIS;
	ch3->InputChan1=0;
	ch3->OutputChan0=XAXIS;
	ch3->OutputChan1=0;
	ch3->MasterAxis=-1;
	ch3->LimitSwitchOptions=0x113;
	ch3->LimitSwitchNegBit=XPROXNEG;
	ch3->LimitSwitchPosBit=XPROXPOS;
	ch3->SoftLimitPos=1e+09;
	ch3->SoftLimitNeg=-1e+09;
	ch3->InputGain0=1;
	ch3->InputGain1=1;
	ch3->InputOffset0=0;
	ch3->InputOffset1=0;
	ch3->OutputGain=1;
	ch3->OutputOffset=0;
	ch3->SlaveGain=1;
	ch3->BacklashMode=BACKLASH_OFF;
	ch3->BacklashAmount=0;
	ch3->BacklashRate=0;
	ch3->invDistPerCycle=1;
	ch3->Lead=0;
	ch3->MaxFollowingError=200; //this is the max error allowable in following a G code path
	ch3->StepperAmplitude=20;

	ch3->iir[0].B0=1;
	ch3->iir[0].B1=0;
	ch3->iir[0].B2=0;
	ch3->iir[0].A1=0;
	ch3->iir[0].A2=0;

	ch3->iir[1].B0=1;
	ch3->iir[1].B1=0;
	ch3->iir[1].B2=0;
	ch3->iir[1].A1=0;
	ch3->iir[1].A2=0;

	ch3->iir[2].B0=0.0166094; //2nd order low pass 500Hz Q1.4
	ch3->iir[2].B1=0.0332189;
	ch3->iir[2].B2=0.0166094;
	ch3->iir[2].A1=1.60679;
	ch3->iir[2].A2=-0.673229;
	
	return 0;
}
