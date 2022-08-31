#ifndef CIMAGE_H
#define CIMAGE_H

#include "abstractimage.h"
#include "image.h"

class CImage :public AbstractImage {

	/**
	 * Constructs new black image.
	 *
	 * @param width width in pixels of new image
	 * @param height height in pixels of new image
	 */

	 private:
            float *luv[3];

	 public:
            CImage(const int width, const int height);
            CImage(const int width,const int height,const unsigned int InitVal);

            /**
            * Constructs new image with existing data.
            * Copies new_data into own data structure.
            *
            * @param width width in pixels of new image
            * @param height height in pixels of new image
            * @param new_data image data to copy.
            */
            CImage(const int width, const int height, const float * new_data);

            /**
            * Constructs new image from file (PGM only).
            */
            CImage(const char * filename);
            ~CImage();
            /**
            * @param x x pixel location
            * @param y y pixel location
            * @param val Value to set.
            */
            void setPixelR(const int x, const int y, const float val);
            void setPixelG(const int x, const int y, const float val);
            void setPixelB(const int x, const int y, const float val);
            void setPixel(const int x,const int y,const float*color);
            /**
            * get convolve value of a pixel according to a 2D Gaussian Blur
            */
            void getPixel(const int x,const int y,float* RGB);
            void getPixelBI(float col,float row,float *RGB);
            //void getPixelLUV(const int x,const int y, float *LUV);
            void get2DConVal(const int x,const int y,float pixel[],vector<float> &kern);
            Image *getGrayImage();
            /**
            * get double sized image of the orignial image
            */
            bool RGBtoLUV();
            void getPixelLUV(const int x,const int y, float *LUV);
            static CImage *doubleSizeImage(CImage *srcImg);
            static CImage *halfSizeImage(CImage *srcImg);
            /**
            * Clones a copy of the image.
            * @return Cloned copy.
            */
            CImage *clone();
            static void test();
};
#endif
