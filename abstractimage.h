#ifndef ABSTRACTIMAGE_H
#define ABSTRACTIMAGE_H
#include <iostream>
#include <vector>
#include <assert.h>

using namespace std;

class AbstractImage {

    public:
    int channel;
    int width;
    int height;
    float *pix;

    public:
    virtual void getPixel(const int x,const int y,float *RGB) = 0;
    virtual void setPixel(const int x,const int y, const float *color) = 0;

    void get2DConVal(const int x,const int y,float *pix_val, vector<float> &kern);

    void save(const char *filename);
    ~AbstractImage()
    {
          delete [] pix;
          pix = NULL;
    }

};
#endif
