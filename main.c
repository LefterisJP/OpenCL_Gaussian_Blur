#include <Time/rfc_timer.h>

#include <gaussian.h>



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
        return 0;

    //start a timer to time the CPU performance
    RF_Timer timer;
    rfTimer_Init(&timer,RF_TIMER_MICROSECONDS);
    //perform CPU blurring
    if(pna_blur_cpu(imgname,gaussianSize,gaussianSigma)==false)//time it
        return -1;
    //query the timer for the CPU
    units  = rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
    printf("CPU Gaussian blurring took %f microseconds\n",units);
    //reset the timer for the GPU
    rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
    //perform GPU blurring and then read the timer
    if(pna_blur_gpu(imgname,gaussianSize,gaussianSigma)==false)
        return -1;
    units  = rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);//time it
    printf("GPU Gaussian blurring took %f microseconds\n",units);

    return 0;
}
