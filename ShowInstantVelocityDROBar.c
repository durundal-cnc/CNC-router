#include "KMotionDef.h"
#define TMP 11 // which spare persist to use to transfer data
#include "..\KflopToKMotionCNCFunctions.c"
#include "init_definitions.h"


#define CNTS_PER_INCH_X 20807.68
#define CNTS_PER_INCH_Y 25400.0
#define CNTS_PER_INCH_Z 20807.68

#define FULL_BAR_SCALE 200.0


main()
{
	double Rate,d,x0,dx,y0,dy,z0,dz,t0,t1;
	char s[80];
	
	for(;;)
	{
		t0=WaitNextTimeSlice();
		x0 = ch0->Dest / CNTS_PER_INCH_X;
		y0 = ch1->Dest / CNTS_PER_INCH_Y;
		z0 = ch2->Dest / CNTS_PER_INCH_Z;
		t1=WaitNextTimeSlice();
		dx = ch0->Dest / CNTS_PER_INCH_X - x0;
		dy = ch1->Dest / CNTS_PER_INCH_Y - y0;
		dz = ch2->Dest / CNTS_PER_INCH_Z - z0;
		
		d = sqrt(dx*dx + dy*dy + dz*dz);

		Rate = d/(t1-t0)*60.0;
		//sprintf(s,"Rate %8.2f ipm\n",Rate);
		sprintf(s,"%8.2f ipm\n",Rate);

		// Put it onto the Screen
		DROLabel(INSTANTFEEDVELMEM, INSTANTFEEDVELVAR, s);
		WriteVarFloat(INSTANTFEEDVELBAR, Rate/FULL_BAR_SCALE);
		Delay_sec(0.5);
	}
}

