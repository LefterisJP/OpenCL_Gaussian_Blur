/**
** @date 21/08/2010
** @author Lefteris
**
** This file is a part of the assignment for the
** Parallel Numerical Assignment course in the University
** of Tokyo.
**
** Contains code for Gaussian blurring or .bmp images
**/

#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include "bitmap.h"
#include <rf_setup.h>


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
