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
		k = A1*pow(yb,3)+A2*pow(yb,2)+A3*yb+A4;
      	
		F_PROFILE(f,thread,position)=pow(CMU,0.75)*pow(k,1.5)/LREF;
    }
  	end_f_loop(f,thread)
}



/* reference velocity profile*/
DEFINE_PROFILE(velocity_profile_20_20_10, thread, position)
{
	float x[ND_ND];
  	face_t f;

	//Quartic fitted data see data_fitting/fitting.py
	float A1 = -1.88978572e+06;
	float A2 = 7.78781546e+04;
	float A3 = -1.10368954e+03;
	float A4 = 5.88531699e+00;
	float A5 = 8.17468387e-03;

	float v, yb;
  	begin_f_loop(f, thread)
    {
      	F_CENTROID(x,f,thread);
		yb=x[1];

		v = A1*pow(yb,4) + A2*pow(yb,3) + A3*pow(yb,2) + A4*yb + A5;
      	
		F_PROFILE(f,thread,position)=v;
    }
  	end_f_loop(f,thread)
}

DEFINE_PROFILE(velocity_profile_20_30_10, thread, position)
{
	float x[ND_ND];
  	face_t f;

	//Quartic fitted data see data_fitting/fitting.py
	float A1 = -2.27543821e+06;
	float A2 = 9.37959016e+04;
	float A3 = -1.32852717e+03;
	float A4 = 7.11614515e+00;
	float A5 = 7.94125808e-03;

	float v, yb;
  	begin_f_loop(f, thread)
    {
      	F_CENTROID(x,f,thread);
		yb=x[1];

		v = A1*pow(yb,4) + A2*pow(yb,3) + A3*pow(yb,2) + A4*yb + A5;
      	
		F_PROFILE(f,thread,position)=v;
    }
  	end_f_loop(f,thread)
}

DEFINE_PROFILE(velocity_profile_20_20_5, thread, position)
{
	float x[ND_ND];
  	face_t f;

	//Quartic fitted data see data_fitting/fitting.py
	float A1 = -1.29185557e+06;
	float A2 =  5.27723273e+04;
	float A3 = -7.41635489e+02;
	float A4 = 3.94656863e+00;
	float A5 = 4.08825029e-03;

	float v, yb;
  	begin_f_loop(f, thread)
    {
      	F_CENTROID(x,f,thread);
		yb=x[1];

		v = A1*pow(yb,4) + A2*pow(yb,3) + A3*pow(yb,2) + A4*yb + A5;
      	
		F_PROFILE(f,thread,position)=v;
    }
  	end_f_loop(f,thread)
}


DEFINE_PROFILE(velocity_profile_10_20_5, thread, position)
{
	float x[ND_ND];
  	face_t f;

	//Quartic fitted data see data_fitting/fitting.py
	float A1 = -4.03739162e+07;
	float A2 =  8.13973048e+05;
	float A3 = -5.62668521e+03;
	float A4 = 1.48226397e+01;
	float A5 = 5.87035882e-03;

	float v, yb;
  	begin_f_loop(f, thread)
    {
      	F_CENTROID(x,f,thread);
		yb=x[1];

		v = A1*pow(yb,4) + A2*pow(yb,3) + A3*pow(yb,2) + A4*yb + A5;
      	
		F_PROFILE(f,thread,position)=v;
    }
  	end_f_loop(f,thread)
}

DEFINE_PROFILE(velocity_profile_10_20_10, thread, position)
{
	float x[ND_ND];
  	face_t f;

	//Quartic fitted data see data_fitting/fitting.py
	float A1 = -6.32304507e+07;
	float A2 =  1.30489771e+06;
	float A3 = -9.40240278e+03;
	float A4 = 2.66092275e+01;
	float A5 = 1.60603934e-03;

	float v, yb;
  	begin_f_loop(f, thread)
    {
      	F_CENTROID(x,f,thread);
		yb=x[1];

		v = A1*pow(yb,4) + A2*pow(yb,3) + A3*pow(yb,2) + A4*yb + A5;
      	
		F_PROFILE(f,thread,position)=v;
    }
  	end_f_loop(f,thread)
}