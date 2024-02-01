#include "udf.h"
#define UREF 2.5// reference wind speed at ZREF
#define ZREF 18.70999 // height of the building, 2b
#define LREF 12.8 // characteristic length, b
#define CMU 0.09 // 

DEFINE_PROFILE(velocity_profile, thread, position)
{
	float x[ND_ND];
	float y;
	float u;
	face_t f;

	begin_f_loop(f, thread)
	{
		F_CENTROID(x,f,thread);
		y=x[1];
		//u = UREF*pow(y/ZREF,0.27);
		if (y < 10)  u = UREF;
		else u = (0.19–pow(1/0.05, 0.07))*(log(y)*UREF);
		F_PROFILE(f,thread,position) = u;
	}
	end_f_loop(f, thread)
}
