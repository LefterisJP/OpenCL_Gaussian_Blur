#ifndef READ_ARGS_H
#define READ_ARGS_H

#include <stdint.h>

extern char helpStr[];

//default argument values
#define DEFAULT_IMG_NAME        "image3.BMP"
#define DEFAULT_GAUSSIAN_SIZE   3
#define DEFAULT_GAUSSIAN_SIGMA  0.8

//! @brief Reads in the program's arguments
//!
//! @param argc The number of arguments
//! @param argV The arguments buffer
//! @param imgName The name of the image file argument
//! @param gaussianSize The size of the gaussian kernel argument
//! @param gaussianSigma The gaussian sigma parameter argument
//! @return Returns true for succesfull argument parsing and false if there was a failure
char readArguments(int argc, char *argv[],
                   char** imgName, uint32_t* gaussianSize, 
                   float* gaussianSigma);

#endif
