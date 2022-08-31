#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include "cimage.h"
#include "segm.h"


using namespace std;

const int SegmenterMS::N = 30;
const unsigned int SegmenterMS::ITER_NUM = 30;
const float SegmenterMS::distR0 = 0.0001;
const double  SegmenterMS::H = 0.499999f;

/**************************************

Implementation according to Comaniciu, D., etc paper:

 "Mean Shift: A Robust Approach Toward Feature Space Analysis".
 in IEEE Transactions on Pattern Analysis and Machine Intelligence (IEEE) 24 (5): 603–619

 This is part of my work during my graduate study in Yunnan University, China from 2003 to 2004.

 @author Wan-Lei Zhao
 @date Nov/2004

**************************************/

inline int my_round(const double in_x)
{
	if (in_x < 0)
		return (int)(in_x - 0.5);
	else
		return (int)(in_x + 0.5);
}

SegmenterMS::SegmenterMS()
{
	SegmenterMS::radiusD2 = 7;
	SegmenterMS::radiusR2 = 6.5;
}

void SegmenterMS::RGBTOLUV(byte *rgbVal, float *luvVal)
{
	double	x, y, z, L0, u_prime, v_prime, constant;

	/**convert RGB to XYZ...**/
	x		= XYZ[0][0]*rgbVal[0] + XYZ[0][1]*rgbVal[1] + XYZ[0][2]*rgbVal[2];
	y		= XYZ[1][0]*rgbVal[0] + XYZ[1][1]*rgbVal[1] + XYZ[1][2]*rgbVal[2];
	z		= XYZ[2][0]*rgbVal[0] + XYZ[2][1]*rgbVal[1] + XYZ[2][2]*rgbVal[2];

	/**compute L* **/
	L0		= y / (255.0 * Yn);
	if(L0 > Lt)
		luvVal[0]	= (float)(116.0 * (pow(L0, 1.0/3.0)) - 16.0);
	else
		luvVal[0]	= (float)(903.3 * L0);

	//compute u_prime and v_prime
	constant	= x + 15 * y + 3 * z;
	if(constant != 0)
	{
		u_prime	= (4 * x) / constant;
		v_prime = (9 * y) / constant;
	}
	else
	{
		u_prime	= 4.0;
		v_prime	= 9.0/15.0;
	}

	/**compute u* and v* */

    luvVal[1] = (float) (13 * luvVal[0] * (u_prime - Un_prime));
    luvVal[2] = (float) (13 * luvVal[0] * (v_prime - Vn_prime));

	return;
}

void SegmenterMS::LUVTORGB(const float *luvVal, byte *rgbVal)
{
	int		r, g, b;
	double	x, y, z, u_prime, v_prime;

	//perform conversion
	if(luvVal[0] < 0.1)
		r = g = b = 0;
	else
	{
		//convert luv to xyz...
		if(luvVal[0] < 8.0)
			y	= Yn * luvVal[0] / 903.3;
		else
		{
			y	= (luvVal[0] + 16.0) / 116.0;
			y  *= Yn * y * y;
		}

		u_prime	= luvVal[1] / (13 * luvVal[0]) + Un_prime;
		v_prime	= luvVal[2] / (13 * luvVal[0]) + Vn_prime;

		x		= 9 * u_prime * y / (4 * v_prime);
		z		= (12 - 3 * u_prime - 20 * v_prime) * y / (4 * v_prime);

		//convert xyz to rgb...
		//[r, g, b] = RGB*[x, y, z]*255.0
		r		= my_round((RGB[0][0]*x + RGB[0][1]*y + RGB[0][2]*z)*255.0);
		g		= my_round((RGB[1][0]*x + RGB[1][1]*y + RGB[1][2]*z)*255.0);
		b		= my_round((RGB[2][0]*x + RGB[2][1]*y + RGB[2][2]*z)*255.0);

		//check bounds...
		if(r < 0)	r = 0; if(r > 255)	r = 255;
		if(g < 0)	g = 0; if(g > 255)	g = 255;
		if(b < 0)	b = 0; if(b > 255)	b = 255;

	}

	//assign rgb values to rgb vector rgbVal
	rgbVal[0]	= r;
	rgbVal[1]	= g;
	rgbVal[2]	= b;

	//done.
	return;
}

void SegmenterMS::RGBTOLUV(const float *rgbVal, float *luvVal, const unsigned int _width, const unsigned int _height)
{
	double	x, y, z, L0, u_prime, v_prime, constant;
	unsigned int i, j, rloc, loc;
	assert(rgbVal);	assert(luvVal);

   for(j = 0; j < _height; j++)
   {
       rloc = j*_width;
       for(i = 0; i < _width; i++)
       {
            loc = 3*(rloc + i);
            x		= XYZ[0][0]*rgbVal[loc] + XYZ[0][1]*rgbVal[loc+1] + XYZ[0][2]*rgbVal[loc+2];
            y		= XYZ[1][0]*rgbVal[loc] + XYZ[1][1]*rgbVal[loc+1] + XYZ[1][2]*rgbVal[loc+2];
            z		= XYZ[2][0]*rgbVal[loc] + XYZ[2][1]*rgbVal[loc+1] + XYZ[2][2]*rgbVal[loc+2];

            L0		= y / (255.0 * Yn);

            if(L0 > Lt)
            luvVal[loc]	= (float)(116.0 * (pow(L0, 1.0/3.0)) - 16.0);
            else
            luvVal[loc]	= (float)(903.3 * L0);

            constant	= x + 15 * y + 3 * z;
            if(constant != 0)
            {
                u_prime	= (4 * x) / constant;       v_prime = (9 * y) / constant;
            }else
            {
                u_prime	= 4.0;		v_prime	= 9.0/15.0;
            }
            /**compute u* and v* */
            luvVal[loc+1] = (float) (13 * luvVal[loc] * (u_prime - Un_prime));
            luvVal[loc+2] = (float) (13 * luvVal[loc] * (v_prime - Vn_prime));
        }
   }

	return;
}

void SegmenterMS::LUVTORGB(const float *luvVal, float *rgbVal, const unsigned int _width, const unsigned int _height)
{
	int		r, g, b;
	double	x, y, z, u_prime, v_prime;
    unsigned int i, j, rloc, loc;

    for(j = 0; j < _height; j++)
   {
       rloc = j*_width;
       for(i = 0; i < _width; i++)
       {
            loc = 3*(rloc + i);

            if(luvVal[loc] < 0.1)
            {
                r = g = b = 0;
            }else {
                //convert luv to xyz...
                if(luvVal[loc] < 8.0)
                    y	= Yn * luvVal[0] / 903.3;
                else{
                    y	= (luvVal[loc] + 16.0) / 116.0;
                    y  *= Yn * y * y;
                }

                u_prime	= luvVal[loc+1] / (13 * luvVal[loc]) + Un_prime;
                v_prime	= luvVal[loc+2] / (13 * luvVal[loc]) + Vn_prime;
                x		= 9 * u_prime * y / (4 * v_prime);
                z		= (12 - 3 * u_prime - 20 * v_prime) * y / (4 * v_prime);

                //convert xyz to rgb...
                r		= my_round((RGB[0][0]*x + RGB[0][1]*y + RGB[0][2]*z)*255.0);
                g		= my_round((RGB[1][0]*x + RGB[1][1]*y + RGB[1][2]*z)*255.0);
                b		= my_round((RGB[2][0]*x + RGB[2][1]*y + RGB[2][2]*z)*255.0);

                //check bounds...
                if(r < 0)	r = 0; if(r > 255)	r = 255;
                if(g < 0)	g = 0; if(g > 255)	g = 255;
                if(b < 0)	b = 0; if(b > 255)	b = 255;
            }
            //assign rgb values to rgb vector rgbVal
            rgbVal[loc]	= r;	rgbVal[loc+1]	= g;	rgbVal[loc+2]	= b;
       }
   }
	return;
}


/**********************************
Mean shift with RBF Kernel
**********************************/

void SegmenterMS::Meanshift(const float *luvVal, float *out_luvVal, int width_, int height_)
{
	  float newL = 0.0f ,  newU = 0.0f , newV = 0.0f;
      float mL   = 0.0f ,  mU   = 0.0f , mV   = 0.0f;

	  float RadioR=0.0f, distR2 = 0.0f;
      int   wSum   = 0    , xSum   = 0   , ySum = 0 ;
	  int   i      = 0    , j      = 0   , k    = 0 , depl = 0;
      int   newX   = 0 ,  winX,   newY   = 0, winY;
	  unsigned int ITER;
	  int center;
	  float dif0, dif1, dif2, dif3, dif4, difS;
	  float avgL, avgU, avgV;

	  for(winY = 0; winY < height_; winY++)
	  {
	      for(winX = 0; winX < width_; winX++)
		  {
			 center = 3*(winY * width_ + winX);

			 mL = luvVal[center]; mU = luvVal[center+1]; mV = luvVal[center+2];

             newX = winX;       newY = winY;
			 ITER = 0;			 distR2 = 10;
			 while(distR2 > SegmenterMS::distR0 && ITER < SegmenterMS::ITER_NUM)
	         {
			    wSum = 0; newL = 0.0f; newU = 0.0f; newV = 0.0f;
	            xSum = 0; ySum = 0;
				for(i = -N; i < N; i++)
				 {
					 k = newY + i;
		             if(k<0 || k>= height_) continue;
		              depl = k*width_;
					  for(j = -N; j < N; j++)
					  {
		                   k = newX + j;
		                   if(k<0 || k>= width_) continue;
		                   k += depl;

		                   difS=(i*i + j*j)/(radiusD2*radiusD2);
						   if(difS < 1.0f)
							{
							    dif0 = luvVal[3*k] - mL;
							    if(dif0 < 0.0f) dif0 = -1*dif0;

							    dif1 = luvVal[3*k+1] - mU;                dif2 = luvVal[3*k+2] - mV;
							    RadioR = (dif0*dif0 + dif1*dif1 + dif2*dif2)/(radiusR2*radiusR2);

							    if(RadioR < 1.0f)
							    {
							        wSum++; xSum+=j; ySum+=i;
							        newL+=luvVal[3*k]; newU+=luvVal[3*k+1]; newV+=luvVal[3*k+2];
                                }
						}//if-else
					} //for j --loop
				} // for i --loop

				 avgL = (float)newL/wSum;         avgU = (float)newU/wSum;		 avgV = (float)newV/wSum;

				 if(xSum>=0) xSum = (int)((float)xSum/wSum + H);
		         else xSum = (int)((float)xSum/wSum - H);
		         if(ySum>=0) ySum = (int)((float)ySum/wSum + H);
		         else ySum = (int)((float)ySum/wSum - H);

				dif0 = avgL - mL;              dif1 = avgU - mU;
				dif2 = avgV - mV;			  dif3 = xSum - newX;
				dif4 = ySum - newY;

		        distR2 = (dif0*dif0 + dif1*dif1 + dif2*dif2)+ (dif3*dif3 + dif4*dif4);
				distR2=sqrt(distR2);

		        mL=avgL; mU= avgU; mV = avgV;
		        newX+= xSum; newY += ySum;
				ITER++;
            }// end while
               out_luvVal[center]   = mL;           out_luvVal[center+1] = mU;	   out_luvVal[center+2] = mV;
		  }//for--loop
	  }//for--loop
}


void SegmenterMS::fast_Meanshift(const float *luvVal, float *out_luvVal, int width_, int height_)
{
    //setup ann search

}

int SegmenterMS::perform_MeanShift(const char *src_fn, const char *dst_fn, const unsigned int s, const unsigned int c)
{
	unsigned int _width, _height;

	cout<<"Loading image ... ";

	CImage *src_img = new CImage(src_fn);
	if(src_img == NULL)
	return 1;

	_width = src_img->width;        _height =  src_img->height;

	SegmenterMS *ms = new SegmenterMS();

	float *luv = new float[_width * _height * 3];
    float *out_luv = new float[_width * _height * 3];

    SegmenterMS::RGBTOLUV(src_img->pix, luv, _width, _height);

    delete src_img;
    cout<<_width<<"x"<<_height<<endl;
    cout<<"Meanshift filtering ... ";
    ms->Setparameters(s, c);
	ms->Meanshift(luv, out_luv, _width, _height);
	cout<<"done.\n";

	float *out_data   = new float[_width * _height * 3];

	SegmenterMS::LUVTORGB(out_luv, out_data, _width, _height);

	CImage *dst_img = new CImage(_width, _height, out_data);

	cout<<"Saving filtering results ...";
	dst_img->save(dst_fn);
	cout<<"done.\n";

	delete [] out_data;
	delete [] luv;
	delete [] out_luv;
	delete dst_img;
    return 0;
}

void SegmenterMS::Setparameters(const float Spatial, const float Color)
{
	radiusD2=Spatial;
	radiusR2=Color;
}
