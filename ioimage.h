/**
*@author Wanlei Zhao
*@version 1.0
*All rights reserved by Wanlei Zhao
*
*Anyone receive this program should not redistribute it to other people
*without permission of the author
*
*This program should only be used for non-commercial purpose
**/

#ifndef IOIMAGE_H
#define IOIMAGE_H

#include <iostream>
#include <fstream>

extern "C"
{
    #include "./include/jpeglib.h"
}

using namespace std;

class IOImage
{
      public:
      IOImage();
     static char *pgm_readpgm(const char *fn, int &width, int &height,int *MaxVal);
     static int pgm_pgmwrite(const char *fn, int w, int h, const float* data,unsigned char maxval,const char* comment_string, const int channel);
     static int pgm_pgmwrite(const char *fn, int w, int h, const unsigned char*data, unsigned char maxval,
	const char* comment_string, const int channel);

     static char *read_bmp(const char *fn,int &width,int &height,int *MaxVal,const int channel);
     static void write_bmp(const char *fn,const int w,const int h, const float*data,const int channel);
     static void write_bmp(const char *dstfn,const int w,const int h,const unsigned char *body,const int channel);

     static char *read_jpg(const char *srcfn,int &w,int &h,int *MaxVal, const int channel);
     static void write_jpg(const char *srcfn, const unsigned char *data, const int w, const int h,const int ch, const int quality);
     static void write_jpg(const char *srcfn, const float *data,const int w, const int h, const int ch, const int quality);

     static void  test();
     static void testbmp();
      virtual ~IOImage();
};
#endif

