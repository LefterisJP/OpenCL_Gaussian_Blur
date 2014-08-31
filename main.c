#include <gaussian.h>
#include <args.h>
#include <stdbool.h>


int main ( int argc, char *argv[] )
{
    double units;
    uint32_t gaussianSize;
    float gaussianSigma;
    char* imgname;//the name of the image file, taken by the arguments

    //read in the program's arguments
    if(readArguments(argc,argv,&imgname,&gaussianSize,&gaussianSigma)==false)
        return -1;


    //perform CPU blurring
    if(pna_blur_cpu(imgname,gaussianSize,gaussianSigma)==false)//time it
        return -2;

    //perform GPU blurring and then read the timer
    if(pna_blur_gpu(imgname,gaussianSize,gaussianSigma)==false)
        return -3;

    return 0;
}
