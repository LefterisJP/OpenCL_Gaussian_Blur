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

#include <Time/rfc_timer.h>

#include <gaussian.h>
#include <args.h>



int main ( int argc, char *argv[] )
{
    double units;
    uint32_t gaussianSize;
    float gaussianSigma;
    char* imgname;//the name of the image file, taken by the arguments
    //init the Refu library
    rfInit("errorLog","infoLog");
    //read in the program's arguments
    if(readArguments(argc,argv,&imgname,&gaussianSize,&gaussianSigma)==false)
        return -1;
    //start a timer to time the CPU performance
    RF_Timer timer;
    rfTimer_Init(&timer,RF_TIMER_MICROSECONDS);
#ifndef MEASURE_COMM_OVERHEAD
    //perform CPU blurring
    if(pna_blur_cpu(imgname,gaussianSize,gaussianSigma)==false)//time it
        return -2;
    //query the timer for the CPU
    units  = rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
    printf("CPU Gaussian blurring took %f microseconds\n",units);
    //reset the timer for the GPU
    rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
#endif
    //perform GPU blurring and then read the timer
    if(pna_blur_gpu(imgname,gaussianSize,gaussianSigma)==false)
        return -3;
    units  = rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);//time it
    printf("GPU Gaussian blurring took %f microseconds\n",units);

    return 0;
}
