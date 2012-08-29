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
**
** @author Lefteris
** @date 28/08/2012
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
