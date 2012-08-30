//The Gaussian blur function that runs on the gpu
__kernel void gaussian_blur(__global const unsigned char *image, __global const float* G, const int W,const int H,const int size, __global unsigned char* newImg) 
{ 
	unsigned int x,y,imgLineSize,center;
	float value;
	int i,xOff,yOff;
    //Get the index of the current element being processed
    i = get_global_id(0);
	//Calculate some needed variables
	imgLineSize = W*3;
	center = size/2;
	//pass the pixel through the kernel if it can be centered inside it
	if(i >= imgLineSize*(size-center)+center*3 &&
	   i < W*H*3-imgLineSize*(size-center)-center*3)
	{
		value=0;
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
                value += G[y*size+x]*image[i+xOff+yOff];
            }
        }
        newImg[i] = value;
	}
	else//if it's in the edge keep the same value
	{
		newImg[i] = image[i];
	}
 }
