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

#ifndef READ_ARGS_H
#define READ_ARGS_H

#include <rf_setup.h>

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
char readArguments(int argc, char *argv[],char** imgName,uint32_t* gaussianSize,float* gaussianSigma);

#endif
