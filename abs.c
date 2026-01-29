#include "KMotionDef.h"

int abs(int v) //Because abs() not included in kflop for some reason
{
  return v * ((v>0) - (v<0));
}
