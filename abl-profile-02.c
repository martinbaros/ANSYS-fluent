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

/*  profile for kinetic energy, k  */


DEFINE_PROFILE(k_profile, thread, position)
{
	float x[ND_ND];
  	face_t f;

  	float A1 = 0.0042 ;
	float A2 = -0.0719 ;
	float A3 = 0.2692 ; 
	float A4 = 0.3671 ;
	float k, y, yb;
  	begin_f_loop(f, thread)
    	{
      		F_CENTROID(x,f,thread);
		y = x[1];
      		yb = y/LREF;
		k = A1*pow(yb,3)+A2*pow(yb,2)+A3*yb+A4 ;
      		F_PROFILE(f,thread,position)=k;
      		
    	}
  	end_f_loop(f, thread)
}

/* profile for dissipation rate, epsilon */


DEFINE_PROFILE(dissip_profile, thread, position)
{
  	float x[ND_ND];
  	face_t f;
	float A1 = 0.0042 ;
	float A2 = -0.0719 ;
	float A3 = 0.2692 ; 
	float A4 = 0.3671 ;
	float k, y, yb;
  	begin_f_loop(f, thread)
    	{
      		F_CENTROID(x,f,thread);
		y=x[1];
		yb = y/LREF;
		k = A1*pow(yb,3)+A2*pow(yb,2)+A3*yb+A4 ;
      		F_PROFILE(f,thread,position)=pow(CMU,0.75)*pow(k,1.5)/LREF;
    	}
  	end_f_loop(f,thread)
}

/* 
References:
[1] Yan MENG and Kazuki HIBI. Turbulent measurements of the flow field around a high-rise building. Journal of wind engineering. No 76, July 1998.
*/