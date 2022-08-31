#include "cimage.h"
#include <math.h>
#include <cstring>
#include "ioimage.h"
#include "tool.h"
#include "vmath.h"

/************************************
**
*Represent colorful image with RGB space
*@author Wanlei Zhao
*@date 27. Oct. 2008
************************************/

CImage::CImage(int width, int height)
 {
	assert(width > 0 && height > 0);
	channel  = 3;

	this->width = width;
	this->height = height;

      pix = new float[width*height*3];
      memset(pix,0,sizeof(float)*width*height*3);
}

CImage::CImage(int width, int height,const unsigned int InitVal)
 {
	assert(width > 0 && height > 0);
	channel  = 3;

	this->width = width;
	this->height = height;

    pix = new float[3*width*height];
    memset(pix, InitVal, sizeof(float)*width*height*3);

}

CImage::CImage(int width, int height, const float * new_data)
 {
	assert(width > 0 && height > 0);
	channel  = 3;

	this->width = width;
	this->height = height;

     pix = new float[3*height*width];

	memcpy(pix, new_data, 3*width * height * sizeof(float));
}

CImage::CImage(const char * filename)
{
	assert(filename);
	int maxval;
	unsigned char *data = NULL;
	int loc, loc1, color;
	unsigned int x, y;
	channel = 3;

	if(Tool::endWith(filename,"pgm")||Tool::endWith(filename,"PGM"))
	{
	    data = (unsigned char*)IOImage::pgm_readpgm(filename, this->width, this->height, &maxval);
	    color = 1;
	}else if(Tool::endWith(filename,"bmp")||Tool::endWith(filename,"BMP"))
	{
	    data = (unsigned char*)IOImage::read_bmp(filename, this->width, this->height, &maxval, channel);
	    color = channel;
	}else if(Tool::endWith(filename,"jpg")||Tool::endWith(filename,"JPG")||Tool::endWith(filename,"JPEG")|Tool::endWith(filename,"jpeg"))
	{
	    data = (unsigned char*)IOImage::read_jpg(filename, this->width, this->height,&maxval, channel);
	    color = channel;
	}else {
	    color = 0;
	    cout<<"CImage type is unrecorganizable or suffixed with wrong type!\n";
	}
	assert(this->width>0);
	assert(this->height>0);

	pix = new float[3*this->width*this->height];
	if(color == 3)
	{
	    for (y = 0; y < this->height; y++)
	    {
	        for (x = 0; x < this->width; x++)
			{
			    loc = color*(y*this->width+x);
		        pix[loc] = data[loc];
		        pix[loc + 1] = data[loc + 1];
		        pix[loc + 2] = data[loc + 2];
			}
        }
	}else{
           for (y = 0; y < this->height; y++)
           {
                for (x = 0; x < this->width; x++)
                {
                    loc = color*(y*this->width+x);
                    loc1 = channel*(y*this->width+x);
                    pix[loc1] = data[loc];
			        pix[loc1 + 1] = data[loc];
			        pix[loc1 + 2] = data[loc];
			    }
			}
	}
	delete [] data;
}

Image *CImage::getGrayImage()
{
    Image *grayImg = new Image(this->width,this->height);
    unsigned int i, j, loc;
    float val;

    for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
		        loc = 3*(j*width+i);
		        val = (pix[loc] + pix[loc+1]+pix[loc+2])/3.0;
		        grayImg->setPixel(i,j,val);
		   }
	}

    return grayImg;
}


void CImage::setPixel(const int x,const int y,const float*color)
{
    assert(color);
    if(y>=this->height||y<0)
    return ;

    if(x<0 || x >= this->width)
    return ;
    pix[3*(y*width+x)] = color[0];
    pix[3*(y*width+x)+1] = color[1];
    pix[3*(y*width+x)+2] = color[2];
}

void CImage::setPixelR(const int x,const int y, const float val)
{
    if(y>=this->height||y<0)
    return ;

    if(x<0||x>=this->width)
    return ;
    pix[3*(y*width+x)] = val;
}

void CImage::setPixelG(const int x,const int y, const float val)
{
    if(y>=this->height||y<0)
    return ;

    if(x<0||x>=this->width)
    return ;
    pix[3*(y*width+x)+1] = val;
}

void CImage::setPixelB(const int x,const int y, const float val)
{
    if(y>=this->height||y<0)
    return ;

    if(x<0||x>=this->width)
    return ;
    pix[3*(y*width+x)+2] = val;
}

void CImage::getPixel(const int x, const int y,float* RGB)
{
    assert(RGB);

    if(y>=this->height||y<0)
    return ;

    if(x<0||x>=this->width)
    return ;

    int loc = 3*(y*width+x);

    RGB[0] = pix[loc];
    RGB[1] = pix[loc+1];
    RGB[2] = pix[loc+2];

    return ;
}

void CImage::getPixelLUV(const int x,const int y, float *LUV)
{
    assert(LUV);

    if(y>=this->height||y<0)
    return ;

    if(x<0||x>=this->width)
    return ;

    int loc = y*width+x;

    if(luv[0] == NULL)
    {
        LUV[0] = 0;
        LUV[1] = 0;
        LUV[2] = 0;
    }else {
        LUV[0] = luv[0][loc];
        LUV[1] = luv[1][loc];
        LUV[2] = luv[2][loc];
    }
    return ;
}


CImage * CImage::halfSizeImage(CImage *im)
{
	int w = im->width/2;
	int h = im->height/2;

	CImage * nim = new CImage(w, h);
	float RGB[3];

	for (int j = 0; j < h; j++)
	{
		for (int i = 0; i < w; i++)
		 {
            im->getPixelBI(i*2, j*2,RGB);
			nim->setPixelR(i, j, RGB[0]);
			nim->setPixelG(i, j, RGB[1]);
			nim->setPixelB(i, j, RGB[2]);
		}
	}

	return nim;
}

CImage * CImage::doubleSizeImage(CImage *srcImg)
{
	int w = srcImg->width*2;
	int h = srcImg->height*2;
	int i,j;
	float RGB[3];
	float RGB1[3];

	CImage * dstImg = new CImage(w, h);

	for (j = 0; j < h; j++)
	{
		for (i = 0; i < w; i++)
		{
		    srcImg->getPixel(i/2, j/2,RGB);
			dstImg->setPixelR(i, j, RGB[0]);
			dstImg->setPixelG(i, j, RGB[1]);
			dstImg->setPixelB(i, j, RGB[2]);
		}
	}

	/*******
	  A B C
	  E F G
	  H I J

	  pixels A C H J are pixels from original image
	  pixels B E G I F are interpolated pixels
	/******/

	// interpolate pixels B and I
	for (j = 0; j < h; j += 2)
		for (i = 1; i < w - 1; i += 2)
			{
			    srcImg->getPixel(i/2, j/2,RGB);
			    srcImg->getPixel(i/2 + 1, j/2,RGB1);
			    dstImg->setPixelR(i, j,(RGB[0]+RGB1[0])/ 2.0);
			    dstImg->setPixelG(i, j,(RGB[1]+RGB1[1])/ 2.0);
			    dstImg->setPixelB(i, j,(RGB[2]+RGB1[2])/ 2.0);
			}

	// interpolate pixels E and G
	for (j = 1; j < h - 1; j += 2)
		for (i = 0; i < w; i += 2)
			{
			    srcImg->getPixel(i/2, j/2,RGB);
			    srcImg->getPixel(i/2, j/2+1,RGB1);
			    dstImg->setPixelR(i, j,(RGB[0]+RGB1[0])/ 2.0);
			    dstImg->setPixelG(i, j,(RGB[1]+RGB1[1])/ 2.0);
			    dstImg->setPixelB(i, j,(RGB[2]+RGB1[2])/ 2.0);
			}

	//interpolate pixel F
	//interpolate pixels E and G
	float RGB2[3];
	float RGB3[3];

	for (j = 1; j < h - 1; j += 2)
		for (i = 1; i < w - 1; i += 2)
			{
			    srcImg->getPixel(i/2, j/2,RGB);
			    srcImg->getPixel(i/2 + 1, j/2,RGB1);
			    srcImg->getPixel(i/2, j/2+1,RGB2);
			    srcImg->getPixel(i/2 + 1, j/2+1,RGB3);

			    dstImg->setPixelR(i, j,(RGB[0]+RGB1[0]+RGB2[0]+RGB3[0])/ 4.0);
			    dstImg->setPixelG(i, j,(RGB[1]+RGB1[1]+RGB2[1]+RGB3[1])/ 4.0);
			    dstImg->setPixelB(i, j,(RGB[2]+RGB1[2]+RGB2[2]+RGB3[2])/ 4.0);
			}
	return dstImg;
}

CImage * CImage::clone()
{
	CImage * im = new CImage(width, height, pix);
	return im;
}

void CImage::getPixelBI(float col, float row,float *RGB)
{
	int irow, icol;
	float rfrac, cfrac;
	float row1 = 0, row2 = 0;

	irow = (int) row;
	icol = (int) col;

	if (irow < 0 || irow >= height
	    || icol < 0 || icol >= width)
		return ;

	if (row > height - 1)
		row = height - 1;

	if (col > width - 1)
		col = width - 1;

	rfrac = 1.0 - (row - (float) irow);
	cfrac = 1.0 - (col - (float) icol);

      int row_loc =  irow*width;

	if (cfrac < 1)
	{
		row1 = cfrac * pix[row_loc+icol] + (1.0 - cfrac) * pix[row_loc+icol + 1];
	} else {
		row1 = pix[row_loc+icol];
	}

      row_loc = (irow+1)*width;
	if (rfrac < 1)
	{
		if (cfrac < 1)
		{
			row2 = cfrac * pix[row_loc+icol] + (1.0 - cfrac) * pix[row_loc+icol + 1];
		} else
		{
			row2 = pix[row_loc+icol];
		}
	}

	//return rfrac * row1 + (1.0 - rfrac) * row2;
	return ;
}

bool CImage::RGBtoLUV()
{
    int i,j,loc;
    for(i = 0; i < channel; i++)
    {
            if(luv[i] == NULL)
            {
                luv[i] =  new float[this->width*this->height];
            }
    }

    float L,M,S,l,m,s;
    float r,g,b;
    float RGB[3];

    for(i = 0; i < this->height;i++)
    {
        for(j = 0; j < this->width; j++)
        {
            this->getPixel(j,i,RGB);
            r=RGB[0];
            g=RGB[1];
            b=RGB[2];

            loc = width*i+j;

            l = 0.3811f*r + 0.5783f*g + 0.0402f*b;
            m = 0.1967f*r + 0.7244f*g + 0.0782f*b;
            s = 0.0241f*r + 0.1288f*g + 0.8444f*b;

            if(l <= 0)
            {
                l = 2.0f;
            }

            if(m <= 0)
            {
                m = 2.0f;
            }

            if(s <= 0)
            {
                s = 2.0f;
            }

            L = VMath::lgx(l,10);
            M = VMath::lgx(m,10);
            S = VMath::lgx(s,10);

			luv[0][loc]    = 0.57735f*L  + 0.57735f*M   + 0.57735f*S;
			luv[1][loc] = 0.408248f*L + 0.408248f*M   + -0.816496f*S;
			luv[2][loc] = 0.707107f*L + -0.707107f*M + 0.0f*S;
        }
    }
    return true;
}

CImage::~CImage()
{
}

void CImage::test()
{
     const char fname[]="e:/cideprj/stich/images/1.bmp";
     const char *dst = "e:/cideprj/stich/images/1_1.pgm";

     CImage *myimg = new CImage(fname);
     Image *gray = myimg->getGrayImage();
     //gray->save(dst);
     myimg->save(dst);
     //AbstractImage::save(dst,gray->width,gray->height,;
     //pgm::pgm_readpgm(fn,width,height,&maxVal);

     //cout<<myimg->width<<"\t"<<myimg->height<<endl;
     //myimg->write(dst);
     gray->save(dst);
     delete myimg;
     delete gray;
}
