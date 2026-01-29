//Sets up the Z axis motor parameters (ch4)
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


void init_z_motor(void){

	ch4->InputMode=ENCODER_MODE;
	ch4->OutputMode=DAC_SERVO_MODE;
	ch4->Vel=120000; //This is for rapids, feeds set in Kmotion
	ch4->Accel=150000; //used for rapids only
	ch4->Jerk=4e+06; //used for rapids only
	ch4->P=10; // As tuned 5/24/18 with basic Z (no structure on moving part)
	ch4->I=0.05;
	ch4->D=500;
	ch4->FFAccel=0.0005; //found by trial and error (no Bode) after FFVel = 0.048
	ch4->FFVel=0.05; //found by trial and error (no Bode) with FFaccel =0
	ch4->MaxI=100;
	ch4->MaxErr=200; //this is the maximum error sent to the PID servo control (in position units)
	ch4->MaxOutput=2047;
	ch4->DeadBandGain=1;
	ch4->DeadBandRange=1;
	
	
	ch4->InputChan0=5;
	ch4->InputChan1=0;
	ch4->OutputChan0=4;
	ch4->OutputChan1=0;
	ch4->MasterAxis=-1;
	ch4->LimitSwitchOptions=0x113;
	ch4->LimitSwitchNegBit=138;
	ch4->LimitSwitchPosBit=139;
	ch4->SoftLimitPos=1e+09;
	ch4->SoftLimitNeg=-1e+09;
	ch4->InputGain0=1;
	ch4->InputGain1=1;
	ch4->InputOffset0=0;
	ch4->InputOffset1=0;
	ch4->OutputGain=1;
	ch4->OutputOffset=0;
	ch4->SlaveGain=1;
	ch4->BacklashMode=BACKLASH_OFF;
	ch4->BacklashAmount=0;
	ch4->BacklashRate=0;
	ch4->invDistPerCycle=1;
	ch4->Lead=0;
	ch4->MaxFollowingError=200; //this is the max error allowable in following a G code path
	ch4->StepperAmplitude=20;

	ch4->iir[0].B0=1;
	ch4->iir[0].B1=0;
	ch4->iir[0].B2=0;
	ch4->iir[0].A1=0;
	ch4->iir[0].A2=0;

	ch4->iir[1].B0=1;
	ch4->iir[1].B1=0;
	ch4->iir[1].B2=0;
	ch4->iir[1].A1=0;
	ch4->iir[1].A2=0;

	ch4->iir[2].B0=0.0166094; //2nd order low pass 500Hz Q1.4
	ch4->iir[2].B1=0.0332189;
	ch4->iir[2].B2=0.0166094;
	ch4->iir[2].A1=1.60679;
	ch4->iir[2].A2=-0.673229;
	
	
	return 0;
}
