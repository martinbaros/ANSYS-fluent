#include "udf.h"
#define UREF 2.5// reference wind speed at ZREF
#define ZREF 18.7 // height of the building, 2b
#define LREF 12.8 // characteristic length, b
#define CMU 0.09 // 

/* reference velocity profile*/
DEFINE_PROFILE(reference_velocity_profile, thread, position)
{
	float x[ND_ND];
	float y;
	float u;
	face_t f;

	begin_f_loop(f, thread)
	{
		F_CENTROID(x,f,thread);
		y=x[1];

		u = UREF*pow(y/ZREF,0.27);

		F_PROFILE(f,thread,position) = u;
	}
	end_f_loop(f, thread)
}

/* velocity profile from 0 */
DEFINE_PROFILE(velocity_profile_starting0m, thread, position)
{
	float x[ND_ND];
	float y;
	float u;
	face_t f;
	float k_r, c_r, z;


	begin_f_loop(f, thread)
	{
		F_CENTROID(x,f,thread);
		y=x[1];

		z = y;
		k_r = 0.19*pow((1/0.05), 0.07);
		c_r = k_r * log(z);
		u = c_r * 1 * UREF;


		F_PROFILE(f,thread,position) = u;
	}
	end_f_loop(f, thread)
}

/* velocity profile from 10, else basic 2.5 */
DEFINE_PROFILE(velocity_profile_starting10m, thread, position)
{
	float x[ND_ND];
	float y;
	float u;
	face_t f;
	float k_r, c_r, z;

	begin_f_loop(f, thread)
	{
		F_CENTROID(x,f,thread);
		y=x[1];

		if (y < 10)  z = 10;
		else z = y;

		k_r = 0.19*pow((1/0.05), 0.07);
		c_r = k_r * log(z);
		u = c_r * 1 * UREF;

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
