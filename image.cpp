/**

shared from Yan Ke, but several changes have been made by Wanlei Zhao

*/

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ioimage.h"
#include "image.h"
#include "tool.h"

Image::Image(int width, int height)
 {
	assert(width > 0 && height > 0);

	this->width = width;
	this->height = height;
	channel = 1;

	pix = new float[width*height];
        memset(pix, 0, sizeof(float)*width*height);
}

Image::Image(int width, int height, const float * new_data)
 {
	assert(width > 0 && height > 0);

	this->width = width;
	this->height = height;
	channel = 1;

     pix = new float[height*width];

	memcpy(pix, new_data, width * height * sizeof(float));
}

Image::Image(const char * filename,const float norm)
{
	assert(filename);
	int maxval;
	char line[256];
	int binary;
	int nread;
	unsigned char *data;
	int i, j,int_tmp;
	channel = 1;

	if(Tool::endWith(filename,"pgm")||Tool::endWith(filename,"PGM"))
	{
	    data = (unsigned char*)IOImage::pgm_readpgm(filename, this->width, this->height, &maxval);
	}else if(Tool::endWith(filename,"bmp")||Tool::endWith(filename,"BMP"))
	{
            
	    data = (unsigned char*)IOImage::read_bmp(filename, this->width, this->height, &maxval,channel);
            
	}else if(Tool::endWith(filename,"jpg")||Tool::endWith(filename,"JPG")||Tool::endWith(filename,"JPEG")|Tool::endWith(filename,"jpeg"))
	{
	    data = (unsigned char*)IOImage::read_jpg(filename, this->width, this->height,&maxval,channel);
	}else
	{
	    cout<<filename<<endl;
	    cout<<"Image type is unrecorganizable or suffixed with wrong type!\n";
	}
	assert(this->width>0);
	assert(this->height>0);
	
	pix = new float[this->width*this->height];
	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
		{
		    pix[y*this->width+x] = data[y*this->width+x]/norm;
		}
	}
	delete [] data;
}

Image::Image(const char * filename)
{
	assert(filename);
	int maxval;
	char line[256];
	int binary;
	int nread;
	unsigned char *data;
	int i, j,int_tmp;
	channel = 1;

	if(Tool::endWith(filename,"pgm")||Tool::endWith(filename,"PGM"))
	{
	    data = (unsigned char*)IOImage::pgm_readpgm(filename, this->width, this->height, &maxval);
	}else if(Tool::endWith(filename,"bmp")||Tool::endWith(filename,"BMP"))
	{
	    data = (unsigned char*)IOImage::read_bmp(filename, this->width, this->height, &maxval,channel);
	}else if(Tool::endWith(filename,"jpg")||Tool::endWith(filename,"JPG")||Tool::endWith(filename,"JPEG")|Tool::endWith(filename,"jpeg"))
	{
	    data = (unsigned char*)IOImage::read_jpg(filename, this->width, this->height,&maxval,channel);
	}else
	{
	    cout<<"Image type is unrecorganizable or suffixed with wrong type!\n";
	}
	assert(this->width>0);
	assert(this->height>0);
	pix = new float[this->width*this->height];
	for (int y = 0; y < this->height; y++)
	{
		for (int x = 0; x < this->width; x++)
			{
			    pix[y*this->width+x] = data[y*this->width+x];
			}
	}
	delete [] data;
}

Image::~Image()
{
}


void Image::multiply(const float val)
{
		//cout<<"in multiply1\n";
    int len = width*height;
    //cout<<"in multiply\n";
    for(unsigned int i = 0; i < len; i++)
    {
        this->pix[i] = this->pix[i]*val;
    }
    //cout<<"end of multiply";
}

void Image::exp(const int val)
{
    int len = this->width*this->height;
    for(unsigned int i = 0; i < len; i++)
    {
        this->pix[i] = this->pix[i]*this->pix[i];
    }
}


Image * Image::halfSizeImage(Image *im)
{
	int w = im->width/2;
	int h = im->height/2;
	//cout<<"hi"<<endl;

	Image * nim = new Image(w, h);

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		 {
			nim->setPixel(i, j, im->getPixelBI(i*2, j*2));
		}
	}

	return nim;
}

Image * Image::doubleSizeImage(Image *srcImg)
{
	int w = srcImg->width*2;
	int h = srcImg->height*2;
	int i,j;

	Image * dstImg = new Image(w, h);

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
			dstImg->setPixel(i, j, srcImg->getPixel(i/2, j/2));
		}
	}

	/**
	  A B C
	  E F G
	  H I J

	  pixels A C H J are pixels from original image
	  pixels B E G I F are interpolated pixels
	/**/

	// interpolate pixels B and I
	for (j = 0; j < h; j += 2)
		for (i = 1; i < w - 1; i += 2)
			{
			    dstImg->setPixel(i, j, (srcImg->getPixel(i/2, j/2) + srcImg->getPixel(i/2 + 1, j/2)) / 2.0);
			}


	// interpolate pixels E and G
	for (j = 1; j < h - 1; j += 2)
		for (i = 0; i < w; i += 2)
			{
			    dstImg->setPixel(i, j, (srcImg->getPixel(i/2, j/2) + srcImg->getPixel(i/2, j/2 + 1)) / 2.0);
			}

	// interpolate pixel F
	// interpolate pixels E and G
	for (j = 1; j < h - 1; j += 2)
		for (i = 1; i < w - 1; i += 2)
			{
			    dstImg->setPixel(i, j, (srcImg->getPixel(i/2, j/2) + srcImg->getPixel(i/2, j/2 + 1)
					    + srcImg->getPixel(i/2 + 1, j/2) + srcImg->getPixel(i/2 + 1, j/2 + 1))/4.0);
			}
	return dstImg;
}

void Image::sub(Image * im2, Image * dstImg)
{
	assert(im2);
	assert(dstImg);

	assert(width == im2->width && width == dstImg->width);
	assert(height == im2->height && height == dstImg->height);

	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			dstImg->setPixel(i, j, getPixel(i, j) - im2->getPixel(i, j));
		}
	}


	return ;
}

Image * Image::clone()
{
	Image * im = new Image(width, height, pix);
	return im;
}

Image *Image::getPatch(const int x,const int y,const int win,float *umatrix)
{
    int wc = win/2;
    Image *winImg = new Image(win,win);

    float dx,dy,xl,yl,xr,yr;
    int xp,yp,row,col,irow,icol;
    float tmp,pixel;
    //cout<<"step 0.0\n";
    //cout<<dkern.size()<<"\t"<<ikern.size()<<endl;
    for(row = -wc; row <= wc; row++)
    {
        for(col = -wc; col <= wc; col++)
        {
            xr = umatrix[0]*col + umatrix[1]*row;
            yr = umatrix[2]*col + umatrix[3]*row;
            xl = floor(xr);
            yl = floor(yr);
            dx = xr - xl;
            dy = yr - yl;
            xp = (int)round(xr);
            yp = (int)round(yr);
            xp = x + xp;
            yp = y + yp;
            //cout<<xp<<" "<<yp<<",";
            //tmp = this->getPixel(xp,yp);
            /**/
            tmp = this->getPixel(xp,yp)*(1-dx)*(1-dy); //xp,yp
            tmp += this->getPixel(xp+1,yp)*(1-dy)*dx; //xp+1,yp
            tmp += this->getPixel(xp,yp+1)*(1-dx)*dy; //xp,yp+1
            tmp += this->getPixel(xp+1,yp+1)*dx*dy;   //xp+1,yp+1
            /**/
            irow = row + wc;
            icol = col + wc;
            winImg->setPixel(icol,irow,tmp);
            //cout<<col+wc<<"\t"<<row+wc<<endl;
        }
        //cout<<endl;
    }

    return winImg;
}


float Image::getConValWidth(const int x,const int y,vector<float> &dkern,vector<float> &ikern)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int dscale = dkern.size();
    int dc = dscale/2;
    int ic = ikern.size()/2;
    float pixel,tmp,sumVal;
    int i,j;

    int row,col;
    float dx,dy,xl,yl,xr,yr;
    int xp,yp;
    sumVal = 0.0;
    for(i = -ic; i <= ic; i++)
    {
            row = y + i;
            pixel = 0.0;
            for (j = -dc; j <= dc; j++)
            {
                col = x + j;
                tmp = this->getPixel(col,row); //xp,yp
                pixel += dkern[dc+j] * tmp;
            }
            sumVal = sumVal + pixel*ikern[ic+i];
    }

    return sumVal;
}

float Image::getConValHeight(const int x,const int y,vector<float> &dkern,vector<float> &ikern)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int dscale = dkern.size();
    int dc = dscale/2;
    int ic = ikern.size()/2;
    float pixel,tmp,sumVal;
    int i,j;

    int row,col;
    float dx,dy,xl,yl,xr,yr;
    int xp,yp;
    sumVal = 0.0;
    for(i = -ic; i <= ic; i++)
    {
            row = y + i;
            pixel = 0.0;
            for (j = -dc; j <= dc; j++)
            {
                col = x + j;
                tmp = this->getPixel(col,row); //xp,yp
                pixel += dkern[dc+j] * tmp;
            }
            sumVal = sumVal + pixel*ikern[ic+i];
    }

    return sumVal;
}

float Image::getConValWidth(const int x,const int y,vector<float> &dkern)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int dscale = dkern.size();
    int dc = dscale/2;
    float pixel,tmp;
    int j;
    pixel = 0.0;
    for (j = -dc; j <= dc; j++)
     {
            tmp = this->getPixel(x+j,y);
            pixel += dkern[dc+j] * tmp;
    }
    return pixel;
}

float Image::getConValHeight(const int x,const int y,vector<float> &dkern)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int j,dscale = dkern.size();
    int dc = dscale/2;
    float pixel,tmp;
     pixel = 0.0;
     for (j = -dc; j <= dc; j++)
      {
          tmp = this->getPixel(x,y+j);
          pixel += dkern[dc+j] * tmp;
        }
    return pixel;
}


float Image::getConValWidth(const int x,const int y,vector<float> &dkern,float *umatrix)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int dscale = dkern.size();
    int dc = dscale/2;
    float pixel,tmp;
    int j;

    float dx,dy,xl,yl,xr,yr;
    int xp,yp;

    pixel = 0.0;
    for (j = -dc; j <= dc; j++)
     {
            xr = umatrix[0]*j;
            yr = umatrix[2]*j;
            xl = floor(xr);
            yl = floor(yr);
            dx = xr - xl;
            dy = yr - yl;
            xp = (int)round(xr);
            yp = (int)round(yr);
            xp = x + xp;
            yp = y + yp;
            //tmp = this->getPixel(x+j,y);
            //cout<<dx<<"\t"<<dy<<endl;
            /**/
            tmp = this->getPixel(xp,yp)*(1-dx)*(1-dy); //xp,yp
            tmp += this->getPixel(xp+1,yp)*(1-dy)*dx; //xp+1,yp
            tmp += this->getPixel(xp,yp+1)*(1-dx)*dy; //xp,yp+1
            tmp += this->getPixel(xp+1,yp+1)*dx*dy;   //xp+1,yp+1
            /**/
            pixel += dkern[dc+j] * tmp;
    }
    return pixel;
}

float Image::getConValHeight(const int x,const int y,vector<float> &dkern,float *umatrix)
{
    /**
    *blur in the x direction with dkern, and smooth in y direction with ikern
    *sharp denotes pixel location where the operation made on
    /**/

    int i,j,dscale = dkern.size();
    int dc = dscale/2;
    float pixel,tmp;

    float dx,dy,xl,yl,xr,yr;
    int xp,yp;
     pixel = 0.0;
     for (j = -dc; j <= dc; j++)
      {
                xr = umatrix[1]*j;
                yr = umatrix[3]*j;
                xl = floor(xr);
                yl = floor(yr);
                dx = xr - xl;
                dy = yr - yl;
                xp = (int)round(xr);
                yp = (int)round(yr);
                xp = x + xp;
                yp = y + yp;
                //tmp = this->getPixel(x,y+j);
                /**/
                tmp = this->getPixel(xp,yp)*(1-dx)*(1-dy); //xp,yp
                tmp += this->getPixel(xp+1,yp)*(1-dy)*dx; //xp+1,yp
                tmp += this->getPixel(xp,yp+1)*(1-dx)*dy; //xp,yp+1
                tmp += this->getPixel(xp+1,yp+1)*dx*dy;   //xp+1,yp+1
                /**/
                pixel += dkern[dc+j] * tmp;
        }
    return pixel;
}

float Image::getPixelBI(float col, float row) const
{
	int irow, icol;
	float dx, dy;
	float row1 = 0, row2 = 0;

	irow = (int)floor(row);
	icol = (int)floor(col);

    irow = irow < 0 ? 0 : irow;
    irow = irow >= height ? (height-1) : irow;
    icol  = icol < 0 ? 0 : icol;
    icol  = icol >= width ? (width - 1) : icol;

	dy = row -  irow;
	dx = col -  icol;
	dx = dx > 1?(dx-1):dx;
	dy = dy > 1?(dy-1):dy;

    int row_loc =  irow*this->width;
    row1 =  (1 - dy)*(1 - dx)*pix[row_loc + icol] + (1 - dy)* dx*pix[row_loc + icol + 1];

    if(row_loc < (width -1))
    {
        row_loc =row_loc + this->width;
        row2 =  (1 - dx)*dy*pix[row_loc + icol] + dx*dy*pix[row_loc + icol + 1];
    }

	return row1 + row2;
}


void Image::test(const char *fn)
{
     const char fname[]="e:/siftlab/src/graf_img1.bmp";
     const char *dst = "e:/siftlab/graf_img2.pgm";

     int width,height;
     char maxVal;
     Image *myimg = new Image(fname);
     //pgm::pgm_readpgm(fn,width,height,&maxVal);
     cout<<fn<<endl;
     cout<<myimg->width<<"\t"<<myimg->height<<endl;
     myimg->save(dst);
     delete myimg;
}

