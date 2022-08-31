/**
 Mode:C++; c-basic-offset:8; tab-width:8; indent-tabs-mode:t

Author: Yan Ke
Dec 2003

Wanlei Zhao Collect this class from Yan Ke
and made several modifications
**/
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

#ifndef IMAGE_H
#define IMAGE_H

#include "abstractimage.h"
#include <vector>

using namespace std;

/**
 * Image class
 */
class Image:public AbstractImage {
//private:
	/**
	 * Allocates memory for new image
	 *
	 * @param width width in pixels of new image
	 * @param height height in pixels of new image
	 */

	/**
	 * Constructs new black image.
	 *
	 * @param width width in pixels of new image
	 * @param height height in pixels of new image
	 */
	 public:
	Image(int width, int height);

	/**
	 * Constructs new image with existing data.
	 * Copies new_data into own data structure.
	 *
	 * @param width width in pixels of new image
	 * @param height height in pixels of new image
	 * @param new_data image data to copy.
	 */
	Image(int width, int height, const float * new_data);

	/**
	 * Constructs new black image from file.
	 *Normalize image with maxval, by default, it
	 * is 256
	 *
	 * @param width width in pixels of new image
	 * @param height height in pixels of new image
	 */

	Image(const char * filename,const float norm);

	/**
	 * Constructs new image from file (PGM only).
	 */
	Image(const char * filename);
	~Image();

	/**
    *multiply each pixel with a certain value
    *
    *pix = pix * val;
    *  val multiply with each pixel in that image
    */

    void multiply(const float val);

    /**
    *
    *make each pixel exponentialized with a cerntain value
    *pix = pix^val
    */
    void exp(const int val);

	/**
	 * Sets pixel to a value.
	 * Does NOT check for bounds.
	 *
	 * @param x x pixel location
	 * @param y y pixel location
	 * @param val Value to set.
	 */
	 /**/
	inline void setPixel(const int x, const int y, const float val)
	{
	    if(y>=this->height||y<0)
	    return ;

	    if(x<0||x>=this->width)
	    return ;
	    pix[y*width+x] = val;
    }

	inline void setPixel(const int x,const int y,const float *gray)
	{
	        assert(gray);

	        if(y>=this->height||y<0)
	        return ;

	        if(x<0||x>=this->width)
	        return ;

	        pix[y*width+x] = *gray;
	}

	inline float getPixel(const int x,const int y) const
	{
	     if(y>=this->height||y<0)
	      {
	          return 0;
         }

         if(x<0||x>=this->width)
         {
             return 0;
         }
         return pix[y*width+x];
	}

	inline void  getPixel(const int x,const int y,float *val)
	{
	        if(y>=this->height||y<0)
	        {
	            *val = 0;
	            return ;
            }

            if(x<0||x>=this->width)
            {
                *val = 0;
                return ;
            }
            *val = pix[y*width+x];
    }
    /**/

	/**
	*get pixel value which has been bilinearly interpolated
	*/
	 float getPixelBI(float x, float y) const;


	 /**
	 *get halfsized image of the original image
	 */

	 static Image * halfSizeImage(Image *im);

     /**
     * get double sized image of the orignial image
     */

	 static Image *doubleSizeImage(Image *srcImg);
	 /**
	 * Clones a copy of the image.
	 * @return Cloned copy.
	 */
	 Image * clone();

	 /**
	 *get value convolved by a certain kernal, do local convolution
	 */

	 /**
	 *substract image with im2 and store it to 'dst' image
	 **/
	 void sub(Image * im2, Image * dst) ;

	 float getConValWidth(const int x,const int y,vector<float> &dkern,vector<float> &ikern);
	 float getConValHeight(const int x,const int y,vector<float> &dkern,vector<float> &ikern);

	 float getConValWidth(const int x,const int y,vector<float> &dkern,float *umatrix);
	 float getConValHeight(const int x,const int y,vector<float> &dkern,float *umatrix);

	 float getConValWidth(const int x,const int y,vector<float> &dkern);
	 float getConValHeight(const int x,const int y,vector<float> &dkern);

	 Image *getPatch(const int x,const int y,const int win,float *umatrix);

	static void test(const char *fn);
};


#endif
