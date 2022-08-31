#include "abstractimage.h"
#include "tool.h"
#include "ioimage.h"
#include <math.h>
#include <iostream>

using namespace std;


void AbstractImage::save(const char *filename)
{
    assert(filename);
	unsigned char maxval = 0;

	if(Tool::endWith(filename,".pgm")||Tool::endWith(filename,".PGM"))
	{
	    IOImage::pgm_pgmwrite(filename, width, height, pix, maxval, NULL, channel);

	}else if(Tool::endWith(filename,".bmp")||Tool::endWith(filename,".BMP"))
	{
	    IOImage::write_bmp(filename, width, height, pix, channel);
	}else if(Tool::endWith(filename,".jpg")||Tool::endWith(filename,".JPG"))
	{
	    IOImage::write_jpg(filename, pix, width, height, channel, 100);
	}
}


void AbstractImage::get2DConVal(const int x,const int y,float *pix_val, vector<float> &kern)
{
    int ic = kern.size()/2;
    int i,j;
    int row,col;

    float pixel;
    float gray;

    assert(pix_val);

    float RGB[3];
    float abc[3];

    pix_val[0] = 0;
    pix_val[1] = 0;
    pix_val[2] = 0;

    if(this->channel == 3)
    {
        for(i = -ic; i <= ic; i++)
        {
            row = y + i;
            abc[0]  = 0.0;
            abc[1] = 0;
            abc[2] = 0;
            for (j = -ic; j <= ic; j++)
            {
                col = x + j;
                getPixel(col,row,RGB); //xp,yp
                abc[0] += kern[ic+j] * RGB[0];
                abc[1] += kern[ic+j] * RGB[1];
                abc[2] += kern[ic+j] * RGB[2];
            }
            pix_val[0] = pix_val[0] + abc[0]*kern[ic+i];
            pix_val[1] = pix_val[1] + abc[1]*kern[ic+i];
            pix_val[2] = pix_val[2] + abc[2]*kern[ic+i];
        }
    }else
    {
        *pix_val = 0;
        for(i = -ic; i <= ic; i++)
        {
            row = y + i;
            pixel = 0.0;
            for (j = -ic; j <= ic; j++)
            {
                col = x + j;
                this->getPixel(col,row,&gray); //xp,yp
                pixel += kern[ic+j] * gray;
            }
            *pix_val  = *pix_val  + pixel*kern[ic+i];
        }
    }
    return ;
}
