#ifndef GAUSSIAN_H
#define GAUSSIAN_H

//uncomment if you want to measure the communication overhead instead of normally running the program
//#define MEASURE_COMM_OVERHEAD

#include "bitmap.h"
#include <stdint.h>


//! @brief Blurs the given image using the CPU algorithm
//!
//! @param imgname The string literal of the image
//! @param size The size of the gaussian kernel
//! @param sigma The sigma parameter for the gaussian kernel
//! @return Returns true for success or false for failure
char pna_blur_cpu(char* imgname,uint32_t size,float sigma);
//! @brief Blurs the given image using the GPU
//!
//! @param imgname The string literal of the image
//! @param size The size of the gaussian kernel
//! @param sigma The sigma parameter for the gaussian kernel
//! @return Returns true for success or false for failure
char pna_blur_gpu(char* imgname,uint32_t size,float sigma);

#endif
