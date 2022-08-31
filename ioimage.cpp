#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>

#ifndef __APPLE__
#include <malloc.h>
#endif

#include <math.h>
#include <setjmp.h>

#include "ioimage.h"
#include "cimage.h"
#include "bmp.h"
#include "vmath.h"

using namespace std;

struct my_error_mgr
{
  struct jpeg_error_mgr pub;
  /** "public" fields */
  jmp_buf setjmp_buffer;
  /** for return to caller */
};

typedef struct my_error_mgr * my_error_ptr;

METHODDEF(void)my_error_exit (j_common_ptr cinfo)
{
  my_error_ptr myerr = (my_error_ptr) cinfo->err;
  (*cinfo->err->output_message) (cinfo);
  longjmp(myerr->setjmp_buffer, 1);
}

char  *IOImage::pgm_readpgm(const char *fn ,int &width, int &height,int *MaxVal)
{
    char line[512];
    char ch;
    int binary;
    int nread;
    int i, j,int_tmp;
    char *data;

    width = height = 0;

    ifstream is;
    is.open(fn,fstream::in|ifstream::binary);        // open file
    if(!is.is_open())
    {
         printf("ERROR: file %s open failed\n",fn);
         return NULL;
    }


    is.get(line[0]);
    is.get(line[1]);
    //is.get(line[2]);
    line[2] = '\0';
    //cout<<line<<endl;

    if(strcmp(line,"P5"))
    {
    	if(strcmp(line,"P2"))
    	{
    			printf("pgm read: not a pgm file\n");
    			height = width = 0;
    			return NULL;
    	}else
    		binary = 0;
    }
    else
    {
        binary = 1;
     }

     int jump = 0;

     if(binary)
     {
         i = 0;
         is.get(ch);
         while(ch==0x0A)
         is.get(ch);

         while(ch == '#')
         {
             i = 0;
             do
             {
                    is.get(ch);
                    i++;
             }while(ch != '\n' && ch != '\r');

              is.get(ch);
              while(ch == '\n' || ch == '\r')
              is.get(ch);
         }

         i = 0;
         jump = 0;

         do
         {
             //cout<<ch;
             if(ch == ' ')
             {
                 if(i != 0)
                 {
                     line[i] = ch;
                     jump++;
                     i++;
                     line[i] = '\0';
                 }
             }else if(ch == '\r')
             {
             }else  if(ch == '\n')
             {
                 if(i != 0)
                 {
                     line[i] = ' ';
                     jump++;
                     i++;
                     line[i] = '\0';
                 }
             }else  if(ch >= '0' && ch <= '9')
             {
                  line[i] = ch;
                  i++;
                  line[i] = '\0';
             }else{
                 jump++;
             }
             if(jump < 3)
             is.get(ch);
         }while(jump < 3);
        //cout<<line<<endl;
        sscanf(line," %d %d %d", &width, &height,MaxVal);

        data = new char[width*height];
         is.read(data, width*height*sizeof(char));

         is.close();
	}
	else
	{
	    is.close();
	    is.open (fn,ifstream::in);        // open file
	    is.getline(line,256);

         is.getline(line, 256);
         while (line[0] == '#')
         {
             is.getline(line, 256);
        }

        sscanf(line,"%d %d", &width, &height);
        is.getline(line,256);
        sscanf(line,"%d",MaxVal);
        data = new char[width*height];

        for (i = 0; i < (height)*(width); i++)
	    {
	    	is>>int_tmp;
	    	data[i] = int_tmp;
	    }
	    is.close();
   }
   //cout<<"done.\n";
   return data;
}

char *IOImage::read_bmp(const char *fn,int &width,int &height,int *MaxVal, const int channel)
{
    ifstream file;

	file.open(fn, std::ios::in | std::ios::binary );

	if(file.fail())
	{
		printf("File %s can not open",fn);
		return NULL;
	}

	file.seekg(0, std::ios::end);
	long flen = file.tellg();
	file.seekg(0, std::ios::beg);
	width = height = 0;

	char* buffer = new char[flen + 1];
	file.read(buffer, flen);
        char *pBuff = buffer;

	file.close();

	BITMAPFILEHEADER* bmpheader = new BITMAPFILEHEADER;
	memcpy(bmpheader,pBuff,sizeof(BITMAPFILEHEADER));
	BITMAPFILEHEADER* bmfh = bmpheader;
	pBuff += sizeof(BITMAPFILEHEADER);

	// verify that it's a BITMAP file
	if(bmfh->bfType != BITMAP_ID)
	{
		delete [] buffer;
		buffer = NULL;
        cout<<hex<<bmfh->bfType<<endl;
		cout<<"File is not standard BMP type or the program is unable to recorgnize it!\n";
		return NULL;
	}

	BITMAPINFOHEADER*  bmih = (BITMAPINFOHEADER *)pBuff;

	pBuff += sizeof(BITMAPINFOHEADER);

	int bitCount	= bmih->biBitCount;
	int colors = bitCount/8;
	int compression	= bmih->biCompression;

	width  = bmih->biWidth;
	height = bmih->biHeight;

	int jump;
	if(bmih->biSizeImage>(colors*width*height))
	{
	    jump = bmih->biSizeImage-(colors*width*height);
	    jump = jump/height;
	}else{
	    jump = 0;
	}

	// move the pixel data Pointer to the begening of bitmap data
	pBuff = buffer + (bmfh->bfOffBits * sizeof(char));
	char *data = NULL;
	if(channel == 1 || channel == 3)
	{
	    data = new char[channel*width*height*sizeof(char)];
	}else{
	    cout<<"channel should be eigther 1 or 3\n";
	    exit(1);
	}
	unsigned char r,g,b;
	unsigned int pt;
	*MaxVal = 0;
	int i,j,cusor = 0;
	for(j = height ; j > 0  ; j--)
	{
	    pt = width*(j-1);
		for(i = 0 ; i < width ; i++)
		{
		    if(channel == 3 && colors == 3)
		    {
		        data[3 * (pt+ i)] = *(pBuff++);
		        data[3 * (pt + i)+1] = *(pBuff++);
		        data[3 * (pt + i)+2]   = *(pBuff++);
		    }else if(channel == 1&& colors == 3)
		    {
		        r = *(pBuff++);
		        g = *(pBuff++);
		        b = *(pBuff++);
		        data[pt] = (r+g+b)/3;
		        *MaxVal = (*MaxVal)>data[pt]?(*MaxVal):data[pt];
		        pt++;
		    }else if(channel == 3 && colors == 1)
		    {
		        data[3 * (pt+ i)] = *(pBuff);
		        data[3 * (pt + i)+1] = *(pBuff);
		        data[3 * (pt + i)+2]   = *(pBuff++);
		    }else
		    {
		        data[pt+ i] = *(pBuff++);
		    }
		}

		pBuff = pBuff+jump;
	}
	delete bmpheader;
	delete [] buffer;
	return data;
}

char *IOImage::read_jpg(const char *srcfn,int &w,int &h, int *MaxVal,const int channel)
{
    struct jpeg_decompress_struct cinfo;
    struct my_error_mgr jerr;

    FILE * infile;
    JSAMPARRAY buffer;
    int row_stride;
    /** physical row width in output buffer **/

    if ((infile = fopen(srcfn, "rb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", srcfn);
        return 0;
    }

    /** Step 1: allocate and initialize JPEG decompression object **/
    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = my_error_exit;
    if (setjmp(jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
        return 0;
    }

    jpeg_create_decompress(&cinfo);
    /** Step 2: specify data source (eg, a file) **/
    jpeg_stdio_src(&cinfo, infile);
    /** Step 3: read file parameters with jpeg_read_header() **/
    jpeg_read_header(&cinfo, TRUE);

    /** Step 4: set parameters for decompression **/

    /** Step 5: Start decompressor **/
    (void) jpeg_start_decompress(&cinfo);
    /* JSAMPLEs per row in output buffer */
    row_stride = cinfo.output_width * cinfo.output_components;
    w = cinfo.output_width;     h = cinfo.output_height;

    int ch = cinfo.output_components;
    unsigned char *body = new unsigned char[ w*h*ch];
    unsigned char *data = new unsigned char[ w*h*channel];

    /** Make a one-row-high sample array that will go away when done with image **/
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    /** Step 6: while (scan lines remain to be read) **/
    int k, row_number;
    *MaxVal = 0;
    float pix_val;
    int ich,    _ich;
    unsigned char *row;

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        row_number = cinfo.output_scanline-1;
        row = body+row_number*row_stride;
        for(k = 0; k < row_stride; k+=ch )
        {
           for(ich = 0; ich < ch; ich++)
           {
               _ich = ch - ich - 1;
               row[k+ich] = buffer[0][k+_ich];
               if(row[k+ich] > *MaxVal)
               {
                   *MaxVal = row[k+ich];
               }
           }
       }
    }
    int rowloc, rowloc_dt;

    if(ch == channel)
    {
        memcpy(data,body,sizeof(unsigned char)*w*h*ch);
    }else if (ch ==1 && channel ==3)
    {
        //cout<<"in"<<"\t"<<ch<<"\t"<<channel<<"\t"<<row_stride<<endl;
        for(int i = 0; i < h; i++)
        {
            for(int j = 0; j < w; j+=ch)
            {
                rowloc_dt = (i*w+j)*channel;
                rowloc = i*w + j;
               data[rowloc_dt] = body[rowloc];
               data[rowloc_dt+1] = body[rowloc];
               data[rowloc_dt+2] = body[rowloc];
           }
       }
    }else if(ch == 3 && channel == 1){
       unsigned char tmpval = 0;
       //cout<<"in"<<"\t"<<ch<<"\t"<<channel<<"\t"<<row_stride<<endl;
       for(int i = 0; i < h; i++)
       {
           for(int j = 0; j < w; j++)
           {
               rowloc = (i*w+j)*ch;     rowloc_dt = i*w+j;
               pix_val = (body[rowloc]+body[rowloc+1]+body[rowloc+2])/3.0f;
               tmpval = (unsigned char)round(pix_val);
               if(tmpval > *MaxVal)
               {
                   *MaxVal = tmpval;
               }
               data[rowloc_dt] = tmpval;
           }
       }
   }

  /** Step 7: Finish decompression **/

  (void) jpeg_finish_decompress(&cinfo);

  /** Step 8: Release JPEG decompression object **/
  jpeg_destroy_decompress(&cinfo);
  fclose(infile);
  delete [] body;
  return (char*)data;
}

int IOImage::pgm_pgmwrite(const char *fn, int w, int h, const unsigned char*data, unsigned char maxval,
			const char* comment_string, const int channel)
{
    char line[256];
    int binary;
    int nread;
    FILE *file= fopen(fn,"wb");
    unsigned char *tmpdata = new unsigned char[w*h];
    int pt = 0;
    float val;
    unsigned int uval;
    //unsigned char *local_data = (unsigned char*)data;

    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            if(channel == 1)
            {
                //cout<<pt<<endl;
                tmpdata[pt] = (unsigned char)round(data[pt]);
            }else
            {
                //cout<<"hi\n";
                val = data[3*(i*w+j)];
                val = data[3*(i*w+j)+1] + val;
                val = data[3*(i*w+j)+2] + val;
                uval = (unsigned int)floor(val/3.0);
                maxval = uval > maxval?uval:maxval;
                tmpdata[pt] = (unsigned char)val;
            }
            pt++;
        }
    }

    if (file== NULL)
	{
	    printf("File %s open failed in write pgm\n",fn);
	    return(-1);
	}
    fprintf(file,"P5\n");

    if (comment_string != NULL)
	fprintf(file,"# %s\n", comment_string);
	fprintf(file,"%d %d\n", w, h);

	/* compute the maximum value */
	//cout<<(int)maxval<<endl;
	fprintf(file, "%d\n", 255);
	nread = fwrite((void*)tmpdata, sizeof(unsigned char), (w)*(h), file);
	fclose(file);
	delete [] tmpdata;
	return 0;
}

int IOImage::pgm_pgmwrite(const char *fn, int w, int h, const float*data, unsigned char maxval,
			const char* comment_string, const int channel)
{
    char line[256];
    int binary;
    int nread;
    FILE *file= fopen(fn,"wb");
    unsigned char *tmpdata = new unsigned char[w*h];
    int pt = 0;
    float val;
    unsigned int uval;

    for(int i = 0; i < h; i++)
    {
        for(int j = 0; j < w; j++)
        {
            if(channel == 1)
            {
                tmpdata[pt] = (unsigned char)round(data[pt]);
            }else
            {
                //cout<<"hi\n";
                val = data[3*(i*w+j)];
                val = data[3*(i*w+j)+1] + val;
                val = data[3*(i*w+j)+2] + val;
                uval = (unsigned int)floor(val/3.0);
                maxval = uval > maxval?uval:maxval;
                tmpdata[pt] = (unsigned char)val;
            }
            pt++;
        }
    }

    //cout<<"bug 1.1\n";

    if (file== NULL)
	{
	    printf("File %s open failed in write pgm\n",fn);
	    return(-1);
	}
    fprintf(file,"P5\n");

    if (comment_string != NULL)
	fprintf(file,"# %s\n", comment_string);
	fprintf(file,"%d %d\n", w, h);

	/* compute the maximum value */
	//cout<<(int)maxval<<endl;
	fprintf(file, "%d\n", 255);
	nread = fwrite((void*)tmpdata, sizeof(unsigned char), (w)*(h), file);
	fclose(file);
	//cout<<"bug 1.2\n";

	delete [] tmpdata;
	return 0;
}

void IOImage::write_bmp(const char *dstfn,const int w,const int h, const unsigned char *body,const int channel)
{
      int image_data_size = w * h;

       //if( channel == 3 )
      image_data_size *= 4;

      int hexWidth[4];
      int hexHeight[4];
      int hexFileSize[4];
      int hexIdent[4];

      VMath::convert_hex(w, hexWidth);
      VMath::convert_hex(h, hexHeight);
      VMath::convert_hex(image_data_size+54,hexFileSize);
      VMath::convert_hex(image_data_size,hexIdent);

      //unsigned char* local_body = (unsigned char*)body;

      FILE * maskFile  = fopen(dstfn , "w+b");

      char headerArray[54];
      headerArray[0] =(char)0x42 ;
      headerArray[1] =(char)0x4D ;
      headerArray[2] =(char)hexFileSize[0] ;
      headerArray[3] =(char)hexFileSize[1] ;
      headerArray[4] =(char)hexFileSize[2] ;
      headerArray[5] =(char)hexFileSize[3] ;
      headerArray[6] = (char)0x0;
      headerArray[7] = (char)0x0;
      headerArray[8] = (char)0x0;
      headerArray[9] = (char)0x0;
      headerArray[10] = (char)0x36;
      headerArray[11] = (char)0x0;
      headerArray[12] = (char)0x0;
      headerArray[13] = (char)0x0;
      headerArray[14] = (char)0x28;
      headerArray[15] = (char)0x0;
      headerArray[16] = (char)0x0;
      headerArray[17] = (char)0x0;
      headerArray[18] = (char)hexWidth[0];
      headerArray[19] = (char)hexWidth[1];
      headerArray[20] = (char)hexWidth[2];
      headerArray[21] = (char)hexWidth[3];
      headerArray[22] = (char)hexHeight[0];
      headerArray[23] = (char)hexHeight[1];
      headerArray[24] = (char)hexHeight[2];
      headerArray[25] = (char)hexHeight[3];
      headerArray[26] = (char)0x01;
      headerArray[27] = (char)0x0;
      headerArray[28] = (char)0x20;
      headerArray[29] = (char)0x0;
      headerArray[30] = (char)0x0;
      headerArray[31] = (char)0x0;
      headerArray[32] = (char)0x0;
      headerArray[33] = (char)0x0;
      headerArray[34] = (char)hexIdent[0];
      headerArray[35] = (char)hexIdent[1];
      headerArray[36] = (char)hexIdent[2];
      headerArray[37] = (char)hexIdent[3];
      headerArray[38] = (char)0xC4;
      headerArray[39] = (char)0x0E;
      headerArray[40] = (char)0x0;
      headerArray[41] = (char)0x0;
      headerArray[42] = (char)0xC4;
      headerArray[43] = (char)0x0E;
      headerArray[44] = (char)0x0;
      headerArray[45] = (char)0x0;
      headerArray[46] = (char)0x0;
      headerArray[47] = (char)0x0;
      headerArray[48] = (char)0x0;
      headerArray[49] = (char)0x0;
      headerArray[50] = (char)0x0;
      headerArray[51] = (char)0x0;
      headerArray[52] = (char)0x0;
      headerArray[53] = (char)0x0;

      fwrite(headerArray, sizeof(char), 54, maskFile);
      fclose(maskFile);
      maskFile  = fopen( dstfn, "a+b");

      unsigned char* data = new unsigned char[image_data_size];

      int index=0;
      int indexM;

      for(int m=0; m<h; m++)
      {
         for(int n=0; n<w; n++)
         {
            index   = channel*(m*w+n);
            indexM  = 4*((h-m-1)*w+n);

            if( channel == 3 )
            {
                 data[indexM  ] = (int)round(body[index]);
                 data[indexM+1] = (int)round(body[index+1]); //(int)round(body[index+1]);
                 data[indexM+2] = (int)round(body[index+2]); //(int)round(body[index+2]);
                 data[indexM+3] = 0;
            }
            else if( channel == 1 )
            {
               data[indexM  ] = (int)round(body[index]);
               data[indexM+1] = (int)round(body[index]);
               data[indexM+2] = (int)round(body[index]);
               data[indexM+3] = 0;
            }
         }
      }
      fwrite(data, sizeof(char), image_data_size, maskFile);
      fclose(maskFile);
      delete []data;
      data = NULL;
}

void IOImage::write_bmp(const char *dstfn,const int w,const int h,const float *body,const int channel)
{
      int image_data_size = w * h;

      image_data_size *= 4;

      int hexWidth[4];
      int hexHeight[4];
      int hexFileSize[4];
      int hexIdent[4];

      VMath::convert_hex(w, hexWidth);
      VMath::convert_hex(h, hexHeight);
      VMath::convert_hex(image_data_size+54,hexFileSize);
      VMath::convert_hex(image_data_size,hexIdent);

      FILE * maskFile  = fopen(dstfn , "w+b");

      char headerArray[54];
      headerArray[0] =(char)0x42 ;
      headerArray[1] =(char)0x4D ;
      headerArray[2] =(char)hexFileSize[0] ;
      headerArray[3] =(char)hexFileSize[1] ;
      headerArray[4] =(char)hexFileSize[2] ;
      headerArray[5] =(char)hexFileSize[3] ;
      headerArray[6] = (char)0x0;
      headerArray[7] = (char)0x0;
      headerArray[8] = (char)0x0;
      headerArray[9] = (char)0x0;
      headerArray[10] = (char)0x36;
      headerArray[11] = (char)0x0;
      headerArray[12] = (char)0x0;
      headerArray[13] = (char)0x0;
      headerArray[14] = (char)0x28;
      headerArray[15] = (char)0x0;
      headerArray[16] = (char)0x0;
      headerArray[17] = (char)0x0;
      headerArray[18] = (char)hexWidth[0];
      headerArray[19] = (char)hexWidth[1];
      headerArray[20] = (char)hexWidth[2];
      headerArray[21] = (char)hexWidth[3];
      headerArray[22] = (char)hexHeight[0];
      headerArray[23] = (char)hexHeight[1];
      headerArray[24] = (char)hexHeight[2];
      headerArray[25] = (char)hexHeight[3];
      headerArray[26] = (char)0x01;
      headerArray[27] = (char)0x0;
      headerArray[28] = (char)0x20;
      headerArray[29] = (char)0x0;
      headerArray[30] = (char)0x0;
      headerArray[31] = (char)0x0;
      headerArray[32] = (char)0x0;
      headerArray[33] = (char)0x0;
      headerArray[34] = (char)hexIdent[0];
      headerArray[35] = (char)hexIdent[1];
      headerArray[36] = (char)hexIdent[2];
      headerArray[37] = (char)hexIdent[3];
      headerArray[38] = (char)0xC4;
      headerArray[39] = (char)0x0E;
      headerArray[40] = (char)0x0;
      headerArray[41] = (char)0x0;
      headerArray[42] = (char)0xC4;
      headerArray[43] = (char)0x0E;
      headerArray[44] = (char)0x0;
      headerArray[45] = (char)0x0;
      headerArray[46] = (char)0x0;
      headerArray[47] = (char)0x0;
      headerArray[48] = (char)0x0;
      headerArray[49] = (char)0x0;
      headerArray[50] = (char)0x0;
      headerArray[51] = (char)0x0;
      headerArray[52] = (char)0x0;
      headerArray[53] = (char)0x0;

      fwrite(headerArray, sizeof(char), 54, maskFile);
      fclose(maskFile);
      maskFile  = fopen( dstfn, "a+b");

      unsigned char* data = new unsigned char[image_data_size];

      int index=0;
      int indexM;
      //create bitmap data//
      //cout<<"before assigning \n";
      for(int m=0; m<h; m++)
      {
         for(int n=0; n<w; n++)
         {
            index   = channel*(m*w+n);
            indexM  = 4*((h-m-1)*w+n);

            if( channel == 3 )
            {
                //cout<<"hello\n";
                 data[indexM  ] = (int)round(body[index]);
                 data[indexM+1] = (int)round(body[index+1]); //(int)round(body[index+1]);
                 data[indexM+2] = (int)round(body[index+2]); //(int)round(body[index+2]);
                 data[indexM+3] = 0;
                 //cout<<m<<"\t"<<n<<endl;
            }
            else if( channel == 1 )
            {
                //cout<<"hello\n";
               data[indexM  ] = (int)round(body[index]);
               data[indexM+1] = (int)round(body[index]);
               data[indexM+2] = (int)round(body[index]);
               data[indexM+3] = 0;
            }
         }
         //cout<<m<<endl;
      }
      fwrite(data, sizeof(char), image_data_size, maskFile);
      fclose(maskFile);
      //cout<<"end of assigning \n";
      delete []data;
      data = NULL;
}

void IOImage::write_jpg(const char *srcfn, const unsigned char *data,const int w, const int h, const int ch, const int quality)
{
    //cout<<"save image\n";
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    assert(data);

    FILE * outfile;
    JSAMPROW row_pointer[1];
    /** pointer to JSAMPLE row[s] **/

    int row_stride;
    /** physical row width in image buffer **/
    /** Step 1: allocate and initialize JPEG compression object **/
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    //cout<<"bug 1.0\n";

    unsigned char* body = new unsigned char[h*w*ch];
    int i,  j,  k,  rowloc, tmpval;
    row_stride = w*ch;
    if(ch == 3)
    {
        for(i = 0; i < h; i++)
        {
            rowloc = i*w*ch;
            for(j = 0; j < row_stride; j+=ch)
            {
                body[rowloc+j] = (unsigned char) data[rowloc+j+2];
                body[rowloc+j+1] = (unsigned char) data[rowloc+j+1];
                body[rowloc+j+2] = (unsigned char) data[rowloc+j];
            }
        }
    }else{
        for(i = 0; i < h; i++)
        {
            rowloc = i*w*ch;
            for(j = 0; j < row_stride; j+=ch)
            {
                  body[rowloc+j] = data[rowloc+j];
            }
        }
    }

    /** Step 2: specify data destination (eg, a file) **/
    /** Note: steps 2 and 3 can be done in either order. **/

    if ((outfile = fopen(srcfn, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", srcfn);
        exit(1);
    }

    jpeg_stdio_dest(&cinfo, outfile);
    /** Step 3: set parameters for compression **/

    cinfo.image_width = w;
    cinfo.image_height = h;
    cinfo.input_components = ch;

    if( ch == 3 )
    cinfo.in_color_space = JCS_RGB;
    else
    cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);

    //cout<<"bug 1.1\n";

    /** Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling:
    **/

    /** limit to baseline-JPEG values **/
    jpeg_set_quality(&cinfo, quality, TRUE);

    /** Step 4: Start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    /** Step 5: while (scan lines remain to be written) */
    /**           jpeg_write_scanlines(...); */

    /** Here we use the library's state variable cinfo.next_scanline as the
    * loop counter, so that we don't have to keep track ourselves.
    * To keep things simple, we pass one scanline per call; you can pass
    * more if you wish, though.*/

    row_stride = w * ch;
    /** JSAMPLEs per row in image_buffer */
    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = & body[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /** Step 6: Finish compression **/
    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    /** Step 7: release JPEG compression object **/
    jpeg_destroy_compress(&cinfo);
    delete [] body;
}

void IOImage::write_jpg(const char *srcfn, const float *data,const int w, const int h, const int ch, const int quality)
{
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    assert(data);

    FILE * outfile;
    JSAMPROW row_pointer[1];
    /** pointer to JSAMPLE row[s] **/
    int row_stride;
    /** physical row width in image buffer **/
    /** Step 1: allocate and initialize JPEG compression object **/
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    unsigned char* body = new unsigned char[h*w*ch];

    int i, j, k, rowloc, tmpval;
    row_stride = w*ch;
    if(ch == 3)
    {
        for(i = 0; i < h; i++)
        {
            rowloc = i*w*ch;
            for(j = 0; j < row_stride; j+=ch)
            {
                body[rowloc+j] = (unsigned char) round(data[rowloc+j+2]);
                body[rowloc+j+1] = (unsigned char) round(data[rowloc+j+1]);
                body[rowloc+j+2] = (unsigned char)round( data[rowloc+j]);
            }
        }
    }else{
        for(i = 0; i < h; i++)
        {
            rowloc = i*w*ch;
            for(j = 0; j < row_stride; j+=ch)
            {
                  body[rowloc+j] = data[rowloc+j];
            }
        }
    }

    /** Step 2: specify data destination (eg, a file) **/
    /** Note: steps 2 and 3 can be done in either order. **/

    if ((outfile = fopen(srcfn, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", srcfn);
        exit(1);
    }

    jpeg_stdio_dest(&cinfo, outfile);
    /** Step 3: set parameters for compression **/

    cinfo.image_width = w;
    cinfo.image_height = h;
    cinfo.input_components = ch;

    if( ch == 3)
    cinfo.in_color_space = JCS_RGB;
    else
    cinfo.in_color_space = JCS_GRAYSCALE;

    jpeg_set_defaults(&cinfo);
    /** Now you can set any non-default parameters you wish to.
    * Here we just illustrate the use of quality (quantization table) scaling: **/

    /** limit to baseline-JPEG values **/
    jpeg_set_quality(&cinfo, quality, TRUE);

    /** Step 4: Start compressor */
    jpeg_start_compress(&cinfo, TRUE);

    /** Step 5: while (scan lines remain to be written) */
    /**           jpeg_write_scanlines(...); */

    row_stride = w * ch;
    /** JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = & body[cinfo.next_scanline * row_stride];
        (void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    /** Step 6: Finish compression **/
    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    /** Step 7: release JPEG compression object **/
    jpeg_destroy_compress(&cinfo);

    delete [] body;
}


void IOImage::testbmp()
{
    char fn[]="e:/siftlab/src/graf_img2.jpg";
      const char *fnw = "e:/siftlab/test7.jpg";
      int height,width;
      int maxg,ch = 1;
      unsigned char *pixels;
      /**/
      //pixels = (unsigned char*)IOImage::read_bmp(fn,width,height,&maxg,3);
      pixels = (unsigned char*)IOImage::read_jpg(fn,width,height,&maxg,ch);
      //IOImage::write_bmp(fnw,width,height,pixels,3);
      cout<<width<<"\t"<<height<<endl;
      IOImage::write_jpg(fnw,pixels,width,height,1,60);
      //IOImage::pgm_pgmwrite(fnw,width,height,pixels,maxg,"",3);

}

void IOImage::test()
{
      char fn[]="e:/siftlab/src/matlab_1.pgm";
      const char *fnw = "e:/siftlab/test5.pgm";
      int height,width;
      int maxg;
      unsigned char *pixels;
      /**/
      //pixels = (float*)IOImage::read_bmp(fn,width,height,&maxg,3);
      //pixels = (unsigned char*)IOImage::read_bmp(fn,width,height);
      //pixels = (float*)pgm_readpgm(fn,width,height,&maxg);
      //IOImage::write_bmp(fnw,width,height,pixels,3);
      //cout<<sizeof(float)<<endl;
      //cout<<sizeof(unsigned char)<<endl;
      //
      //CImage *img = new CImage(fn);
      //img->save(fnw);
      pixels = (unsigned char*)IOImage::pgm_readpgm(fn,width,height,&maxg);
      IOImage::pgm_pgmwrite(fnw,width,height,pixels,255,NULL,1);
      /**/

}

