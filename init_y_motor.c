//Sets up the Y axis motor parameters (ch1)
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


void init_y_motor(void){

	ch1->InputMode=ENCODER_MODE;
	ch1->OutputMode=DAC_SERVO_MODE;
	ch1->Vel=35000;			//prior to tuning values: 40000; This is for rapids, feeds set in Kmotion
	ch1->Accel=75000;		//400000;
	ch1->Jerk=4e+06;		//4e+06;
	ch1->P=0.3;				//0.11;
	ch1->I=0.001;			//0.0001;
	ch1->D=32;				//8;
	ch1->FFAccel=0.00005;		//0;
	ch1->FFVel=0.008;		//0;
	ch1->MaxI=200;			//15;
	ch1->MaxErr=200;		//1e+06; //this is the maximum error sent to the PID servo control (in position units)
	ch1->MaxOutput=2047;	//2047;
	ch1->DeadBandGain=1;
	ch1->DeadBandRange=0;
	
	
	ch1->InputChan0=0;
	ch1->InputChan1=0;
	ch1->OutputChan0=1;
	ch1->OutputChan1=0;
	ch1->MasterAxis=-1;
	ch1->LimitSwitchOptions=0x113;
	ch1->LimitSwitchNegBit=1035;
	ch1->LimitSwitchPosBit=1034;
	ch1->SoftLimitPos=1e+09;
	ch1->SoftLimitNeg=-1e+09;
	ch1->InputGain0=-1; // changed to -1 to reverse direction of Y axis
	ch1->InputGain1=1;
	ch1->InputOffset0=0;
	ch1->InputOffset1=0;
	ch1->OutputGain=-1; // changed to -1 to reverse direction of Y axis
	ch1->OutputOffset=0;
	ch1->SlaveGain=1;
	ch1->BacklashMode=BACKLASH_OFF;
	ch1->BacklashAmount=0;
	ch1->BacklashRate=0;
	ch1->invDistPerCycle=1;
	ch1->Lead=0;
	ch1->MaxFollowingError=200; //this is the max error allowable in following a G code path
	ch1->StepperAmplitude=20;

	ch1->iir[0].B0=1;
	ch1->iir[0].B1=0;
	ch1->iir[0].B2=0;
	ch1->iir[0].A1=0;
	ch1->iir[0].A2=0;

	ch1->iir[1].B0=1;
	ch1->iir[1].B1=0;
	ch1->iir[1].B2=0;
	ch1->iir[1].A1=0;
	ch1->iir[1].A2=0;

	ch1->iir[2].B0=0.0166094; //2nd order low pass 500Hz Q1.4
	ch1->iir[2].B1=0.0332189;
	ch1->iir[2].B2=0.0166094;
	ch1->iir[2].A1=1.60679;
	ch1->iir[2].A2=-0.673229;
	
	
	return 0;
}
