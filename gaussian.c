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


#include <bitmap.h>
#include <gaussian.h>
#include <Time/rfc_timer.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#include <math.h>

#define PI_ 3.14159265359f

#define MAX_SOURCE_SIZE (1048576) //1 MB
#define MAX_LOG_SIZE    (1048576) //1 MB


//creates a gaussian kernel
float* createGaussianKernel(uint32_t size,float sigma)
{
    float* ret;
    uint32_t x,y;
    double center = size/2;
    float sum = 0;
    //allocate and create the gaussian kernel
    RF_MALLOC(ret,sizeof(float)*size*size);
    for(x = 0; x < size; x++)
    {
        for(y=0; y < size; y++)
        {
            ret[ y*size+x] = exp( (((x-center)*(x-center)+(y-center)*(y-center))/(2.0f*sigma*sigma))*-1.0f ) / (2.0f*PI_*sigma*sigma);
            sum+=ret[ y*size+x];
        }
    }
    //normalize
    for(x = 0; x < size*size;x++)
    {
        ret[x] = ret[x]/sum;
    }
    //print the kernel so the user can see it
    printf("The generated Gaussian Kernel is:\n");
    for(x = 0; x < size; x++)
    {
        for(y=0; y < size; y++)
        {
            printf("%f ",ret[ y*size+x]);
        }
        printf("\n");
    }
    printf("\n\n");
    return ret;
}

//Blurs the given image using the CPU algorithm
char pna_blur_cpu(char* imgname,uint32_t size,float sigma)
{
    uint32_t i,x,y,imgLineSize;
    int32_t center,yOff,xOff;
    float* matrix,value;
    matrix = createGaussianKernel(size,sigma);
    //read the bitmap
    ME_ImageBMP bmp;
    if(meImageBMP_Init(&bmp,imgname)==false)
    {
        printf("Image \"%s\" could not be read as a .BMP file\n",imgname);
        return false;
    }
    //find the size of one line of the image in bytes and the center of the gaussian kernel
    imgLineSize = bmp.imgWidth*3;
    center = size/2;
    //convolve all valid pixels with the gaussian kernel
    for(i = imgLineSize*(size-center)+center*3; i < bmp.imgHeight*bmp.imgWidth*3-imgLineSize*(size-center)-center*3;i++)
    {
        value = 0;
        for(y=0;y<size;y++)
        {
            if(y < center)
                yOff = -imgLineSize*(size-center);
            else if( y > center)
                yOff = imgLineSize*(size-center);
            else
                yOff = 0;
            for(x=0;x<size;x++)
            {
                if(x < center)
                    xOff = -3*(center-x);
                else if( x > center)
                    xOff = 3*(x-center);
                else
                    xOff = 0;
                value += matrix[y*size+x]*bmp.imgData[i+xOff+yOff];
            }
        }
        bmp.imgData[i] = value;
    }
    //free memory and save the image
    free(matrix);
    meImageBMP_Save(&bmp,"cpu_blur.bmp");
    return true;
}

//Blurs the given image using the GPU
char pna_blur_gpu(char* imgname,uint32_t size,float sigma)
{
    uint32_t imgSize;
    float* matrix;
    cl_int ret;//the openCL error code/s
    //get the image
    ME_ImageBMP bmp;
    meImageBMP_Init(&bmp,imgname);
    imgSize = bmp.imgWidth*bmp.imgHeight*3;
    //create the gaussian kernel
    matrix = createGaussianKernel(size,sigma);
    //create the pointer that will hold the new (blurred) image data
    unsigned char* newData;
    RF_MALLOC(newData,imgSize);
    // Read in the kernel code into a string array
    FILE* f;
    char* kernelSource;
    size_t kernelSrcSize;
    if( (f = fopen("kernel.cl", "r")) == NULL)
    {
        fprintf(stderr, "Failed to load OpenCL kernel code.\n");
        return false;
    }
    RF_MALLOC(kernelSource,MAX_SOURCE_SIZE)
    kernelSrcSize = fread( kernelSource, 1, MAX_SOURCE_SIZE, f);
    fclose(f);

    //Get platform and device information
    cl_platform_id platformID;//will hold the ID of the openCL available platform
    cl_uint platformsN;//will hold the number of openCL available platforms on the machine
    cl_device_id deviceID;//will hold the ID of the openCL device
    cl_uint devicesN; //will hold the number of OpenCL devices in the system
    if(clGetPlatformIDs(1, &platformID, &platformsN) != CL_SUCCESS)
    {
        printf("Could not get the OpenCL Platform IDs\n");
        return false;
    }
    if(clGetDeviceIDs(platformID, CL_DEVICE_TYPE_DEFAULT, 1,&deviceID, &devicesN) != CL_SUCCESS)
    {
        printf("Could not get the system's OpenCL device\n");
        return false;
    }
    // Create an OpenCL context
    cl_context context = clCreateContext( NULL, 1, &deviceID, NULL, NULL, &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Could not create a valid OpenCL context\n");
        return false;
    }
    // Create a command queue
    cl_command_queue cmdQueue = clCreateCommandQueue(context, deviceID, 0, &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Could not create an OpenCL Command Queue\n");
        return false;
    }
#ifdef MEASURE_COMM_OVERHEAD
    RF_Timer timer;
    rfTimer_Init(&timer,RF_TIMER_MICROSECONDS);
#endif
    /// Create memory buffers on the device for the two images
    cl_mem gpuImg = clCreateBuffer(context,CL_MEM_READ_ONLY,imgSize,NULL,&ret);
    if(ret != CL_SUCCESS)
    {
        printf("Unable to create the GPU image buffer object\n");
        return false;
    }
    cl_mem gpuGaussian = clCreateBuffer(context,CL_MEM_READ_ONLY,size*size*sizeof(float),NULL,&ret);
    if(ret != CL_SUCCESS)
    {
        printf("Unable to create the GPU image buffer object\n");
        return false;
    }
    cl_mem gpuNewImg = clCreateBuffer(context,CL_MEM_WRITE_ONLY,imgSize,NULL,&ret);
    if(ret != CL_SUCCESS)
    {
        printf("Unable to create the GPU image buffer object\n");
        return false;
    }
    //Copy the image data and the gaussian kernel to the memory buffer
    if(clEnqueueWriteBuffer(cmdQueue, gpuImg, CL_TRUE, 0,imgSize,bmp.imgData, 0, NULL, NULL) != CL_SUCCESS)
    {
        printf("Error during sending the image data to the OpenCL buffer\n");
        return false;
    }
    if(clEnqueueWriteBuffer(cmdQueue, gpuGaussian, CL_TRUE, 0,size*size*sizeof(float),matrix, 0, NULL, NULL) != CL_SUCCESS)
    {
        printf("Error during sending the gaussian kernel to the OpenCL buffer\n");
        return false;
    }
    //Create a program object and associate it with the kernel's source code.
    cl_program program = clCreateProgramWithSource(context, 1,(const char **)&kernelSource, (const size_t *)&kernelSrcSize, &ret);
    free(kernelSource);
    if(ret != CL_SUCCESS)
    {
        printf("Error in creating an OpenCL program object\n");
        return false;
    }
    //Build the created OpenCL program
    if((ret = clBuildProgram(program, 1, &deviceID, NULL, NULL, NULL))!= CL_SUCCESS)
    {
        printf("Failed to build the OpenCL program\n");
        //create the log string and show it to the user. Then quit
        char* buildLog;
        RF_MALLOC(buildLog,MAX_LOG_SIZE);
        if(clGetProgramBuildInfo(program,deviceID,CL_PROGRAM_BUILD_LOG,MAX_LOG_SIZE,buildLog,NULL) != CL_SUCCESS)
        {
            printf("Could not get any Build info from OpenCL\n");
            free(buildLog);
            return false;
        }
        printf("**BUILD LOG**\n%s",buildLog);
        free(buildLog);
        return false;
    }
    // Create the OpenCL kernel. This is basically one function of the program declared with the __kernel qualifier
    cl_kernel kernel = clCreateKernel(program, "gaussian_blur", &ret);
    if(ret != CL_SUCCESS)
    {
        printf("Failed to create the OpenCL Kernel from the built program\n");
        return false;
    }
    ///Set the arguments of the kernel
    if(clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&gpuImg) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"gpuImg\" argument\n");
        return false;
    }
    if(clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&gpuGaussian) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"gpuGaussian\" argument\n");
        return false;
    }
    if(clSetKernelArg(kernel, 2, sizeof(int), (void *)&bmp.imgWidth) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"imageWidth\" argument\n");
        return false;
    }
    if(clSetKernelArg(kernel, 3, sizeof(int), (void *)&bmp.imgHeight) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"imgHeight\" argument\n");
        return false;
    }
    if(clSetKernelArg(kernel,4,sizeof(int),(void*)&size) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"gaussian size\" argument\n");
        return false;
    }
    if(clSetKernelArg(kernel, 5, sizeof(cl_mem), (void *)&gpuNewImg) != CL_SUCCESS)
    {
        printf("Could not set the kernel's \"gpuNewImg\" argument\n");
        return false;
    }
#ifdef MEASURE_COMM_OVERHEAD
    double units = rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
#endif
    ///enqueue the kernel into the OpenCL device for execution
    size_t globalWorkItemSize = imgSize;//the total size of 1 dimension of the work items. Basically the whole image buffer size
    size_t workGroupSize = 64; //The size of one work group
    ret = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, &globalWorkItemSize, &workGroupSize,0, NULL, NULL);

#ifdef MEASURE_COMM_OVERHEAD
    rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
#endif
    ///Read the memory buffer of the new image on the device to the new Data local variable
    ret = clEnqueueReadBuffer(cmdQueue, gpuNewImg, CL_TRUE, 0,imgSize, newData, 0, NULL, NULL);
#ifdef MEASURE_COMM_OVERHEAD
    units += rfTimer_Query(&timer,RF_TIMER_MICROSECONDS);
    printf("GPU communication overhead is %f microseconds\n",units);
#endif
    ///Clean up everything
    free(matrix);
    clFlush(cmdQueue);
    clFinish(cmdQueue);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseMemObject(gpuImg);
    clReleaseMemObject(gpuGaussian);
    clReleaseMemObject(gpuNewImg);
    clReleaseCommandQueue(cmdQueue);
    clReleaseContext(context);
    ///save the new image and return success
    bmp.imgData = newData;
    meImageBMP_Save(&bmp,"gpu_blur.bmp");
    return true;
}
