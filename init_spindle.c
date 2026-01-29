//Sets up the spindle parameters (ch0)
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
//FET driver #6 controls spindle forward (motion in direction of arrow)
//FET driver #7 controls spindle reverse (motion against direction of arrow)
//If FET#6 and #7 both on, spindle stops
//Set to 100Hz at ~-490 counts/2.393V
//Set to 400Hz at full scale

void init_spindle(void){
	

	
	ch0->InputMode=ADC_MODE;
	ch0->OutputMode=DAC_SERVO_MODE;
	ch0->Vel=0.5;
	ch0->Accel=1;
	ch0->Jerk=5;
	ch0->P=0;
	ch0->I=0;
	ch0->D=0;
	ch0->FFAccel=0;
	ch0->FFVel=2047;
	ch0->MaxI=2047;
	ch0->MaxErr=1e10;
	ch0->MaxOutput=2047;
	ch0->DeadBandGain=1;
	ch0->DeadBandRange=0;
	ch0->InputChan0=0;
	ch0->InputChan1=0;
	ch0->OutputChan0=0;
	ch0->OutputChan1=0;
	ch0->MasterAxis=-1;
	ch0->LimitSwitchOptions=0x100;
	ch0->LimitSwitchNegBit=0;
	ch0->LimitSwitchPosBit=0;
	ch0->SoftLimitPos=1e+09;
	ch0->SoftLimitNeg=-1e+09;
	ch0->InputGain0=11.72;
	ch0->InputGain1=1;
	ch0->InputOffset0=0;
	ch0->InputOffset1=0;
	ch0->OutputGain=-1;
	ch0->OutputOffset=0;
	ch0->SlaveGain=1;
	ch0->BacklashMode=BACKLASH_OFF;
	ch0->BacklashAmount=0;
	ch0->BacklashRate=0;
	ch0->invDistPerCycle=1;
	ch0->Lead=0;
	ch0->MaxFollowingError=1000000000;
	ch0->StepperAmplitude=20;

	ch0->iir[0].B0=1;
	ch0->iir[0].B1=0;
	ch0->iir[0].B2=0;
	ch0->iir[0].A1=0;
	ch0->iir[0].A2=0;

	ch0->iir[1].B0=1;
	ch0->iir[1].B1=0;
	ch0->iir[1].B2=0;
	ch0->iir[1].A1=0;
	ch0->iir[1].A2=0;

	ch0->iir[2].B0=1;
	ch0->iir[2].B1=0;
	ch0->iir[2].B2=0;
	ch0->iir[2].A1=0;
	ch0->iir[2].A2=0;

	
	return 0;
}
