/**
** This sourcecode is part of the article on OpenCL and Gaussian blurring located here
** http://lefteris.realintelligence.net/?p=663
** For any feedback/comments/criticism please contact me: lefteris@realintelligence.net
** The code is licensed under the BSD3 license shown below. Basically you can do whatever you want with it
** as long as you give credit where it's due.
**
** -- License Begins --
** Copyright (c) 2011-2012, Karapetsas Eleftherios
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
**  1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
**  2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the distribution.
**  3. Neither the name of the Original Author of Refu nor the names of its contributors may be used to endorse or promote products derived from
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
**  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
**  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
**  SERVICES;LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
**  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
**  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
** -- License Ends --
**/

#include <args.h>
#include <string.h>
#include <stdlib.h>


char helpStr[] = {"Usage:\n\t-h\t Displays this help message\n\t\-i\tThe name of the input image for blurring\n\
\t-g\tThe size of the gaussian kernel. Default is 3\n\
\t-s\tThe sigma parameter of the gaussian. Default is 0.8\n\
"};

char readArguments(int argc, char *argv[],char** imgname,uint32_t* gaussianSize,float* gaussianSigma)
{
    uint32_t i;
    char gSizeYes,gSigmaYes;
    gSizeYes = gSigmaYes = false;
    *imgname=0;
    //read the arguments of the program
    for(i=1;i<argc;i++)
    {
        if(strcmp(argv[i],"-h")==0)
        {
            printf("\n%s\n",helpStr);
            return false;
        }
        //read the image name
        else if(strcmp(argv[i],"-i")==0)
        {
            *imgname = argv[i+1];
            i++;
        }
        //read the gaussian size
        else if(strcmp(argv[i],"-g")==0)
        {
            *gaussianSize = atoi(argv[i+1]);
            i++;
            //only accept odd sized gaussian
            if((*gaussianSize)%2 == 0 && (*gaussianSize) > 1)
            {
                printf("Argument Error: The size of the squared gaussian kernel has to be an odd number greater than one.\n");
                return false;
            }
            gSizeYes = true;
        }
        //read the gaussian sigma parameter
        else if(strcmp(argv[i],"-s")==0)
        {
            *gaussianSigma = atof(argv[i+1]);
            i++;
            gSigmaYes = true;
        }
        else
        {
            printf("Unrecognized program argument given. The correct program usage can be seen below:\n%s",helpStr);
            return false;
        }
    }
    //if arguments are not given take default value
    if(*imgname == 0)
        *imgname = DEFAULT_IMG_NAME;
    if(gSizeYes == false)
        *gaussianSize = DEFAULT_GAUSSIAN_SIZE;
    if(gSigmaYes == false)
        *gaussianSigma = DEFAULT_GAUSSIAN_SIGMA;
    //success
    return true;
}
