#ifndef		_SEGM_H_
#define		_SEGM_H_

#include "bmp.h"

const double Xn			= 0.95050;
const double Yn			= 1.00000;
const double Zn			= 1.08870;
const double Un_prime	= 0.19784977571475;
const double Vn_prime	= 0.46834507665248;
const double Lt			= 0.008856;

//RGB to LUV conversion
const double XYZ[3][3] = {	{  0.4125,  0.3576,  0.1804 },
							{  0.2125,  0.7154,  0.0721 },
							{  0.0193,  0.1192,  0.9502 }	};

//LUV to RGB conversion
const double RGB[3][3] = {	{  3.2405, -1.5371, -0.4985 },
							{ -0.9693,  1.8760,  0.0416 },
							{  0.0556, -0.2040,  1.0573 }	};

const float radiusD = 50.0f;
const float radiusR = 50.0f;

typedef unsigned char byte;

class SegmenterMS {

 public:
	 float radiusR2, radiusD2;
	 static const int     N;
	 static const double  H;
	 static const unsigned int ITER_NUM;
	 static const float distR0;

	 void Setparameters(float Spatial,float Color);
	 void Meanshift(const float*, float*, int,int);
	 void fast_Meanshift(const float *luvVal, float *out_luvVal, int width_, int height_);
	 static int perform_MeanShift(const char *src_fn, const char *dst_fn, const unsigned int s, const unsigned int c);
    SegmenterMS();
    ~SegmenterMS(){}

  void RGBTOLUV(byte*, float*);
  void LUVTORGB(const float*, byte*);
  static void RGBTOLUV(const float *rgbVal, float *luvVal, const unsigned int _width, const unsigned int _height);
  static void LUVTORGB(const float *luvVal, float *rgbVal, const unsigned int _width, const unsigned int _height);
  void MS_Segment(const float*, float*, int, int);

};

#endif
