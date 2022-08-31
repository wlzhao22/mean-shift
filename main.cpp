#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#include "segm.h"

using namespace std;

/*************************************************************************************
Implementation according to Comaniciu, D., etc paper:

 "Mean Shift: A Robust Approach Toward Feature Space Analysis".
 in IEEE Transactions on Pattern Analysis and Machine Intelligence (IEEE) 24 (5): 603–619

 This is part of my work during my graduate study in Yunnan University as an MPhil, China from 2003 to 2004.

Copyright (C) 2004 Wan-Lei Zhao. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2 of the License, or (at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

@author Wan-Lei Zhao
@date Nov/2004
************************************************************************************/

void test()
{
    SegmenterMS::perform_MeanShift("./data/clown.jpg", "./data/clown_16x20.jpg", 16, 20);
    return ;
}

void help()
{
    cout<<" meanshift -i image.bmp -o target_image [-s spatial_thresh] [-c color thresh]\n";
    cout<<" -i\t Input image\n";
    cout<<" -o\t Output image\n";
    cout<<" -s\t Spatial threshold (12 pixels by default)\n";
    cout<<" -c\t Color threshold (8 levels of intensity difference)\n";
    cout<<" This program only accepts BMP and JPG images.\n";
    cout<<"\n Version 1.0\n";
    cout<<" Author: Wan-Lei Zhao, ";
    cout<<" email-to: stonescx@gmail.com\n";
    return ;
}

int main(const int argc, const char *argv[])
{
    /**
    test();
    return 0;
    /**/

    float s = 12.0f, c = 8.0f;
    int i;
    char src_fn[512], dst_fn[512];
    int args[2] ={0,0};

    if(argc < 3)
    {
        help();
        exit(0);
    }

    for(i = 1; i < argc; i+=2)
    {
        if(strcmp("-i", argv[i]))
        {
            strcpy(src_fn, argv[i+1]);
            args[0] = 1;
        }else if(strcmp("-o", argv[i]))
        {
            strcpy(src_fn, argv[i+1]);
            args[1] = 1;
        }else if(strcmp("-c", argv[i]))
        {
            c = atoi(argv[i+1]);
        }else if(strcmp("-s", argv[i]))
        {
            s = atoi(argv[i+1]);
        }
    }

    for(i = 0; i < 2; i++)
    {
        if(args[0] == 0)
        {
            cout<<"Input image '-i image' is missing!\n";
            exit(0);
        }

        if(args[1] == 0)
        {
            cout<<"Destine image '-o image' is missing!\n";
            exit(0);
        }
    }

    SegmenterMS::perform_MeanShift(src_fn, dst_fn, s, c);

}
