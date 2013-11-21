#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string>

void saveBmp(const char* cpFileName, const void* pBuffer, const int nBufferSize, bool isDepth)
{
	FILE *f;
	int w=320;
	int h=240;

	unsigned char* img = (unsigned char *)malloc(3*w*h);
	memset(img,0,sizeof(img));

	if (isDepth)
	{
		const short* p = (const short*)pBuffer;
		for(int i=0; i<w; i++)
		{
			for(int j=0; j<h; j++)
			{
				int idx = (i+j*w)*3;
				//bgr2rgb
				short v = p[i+j*w];
				img[idx+2] = (char) ((v/16) > 255 ? 255 : v/16);
				img[idx+1] = (char) v % 16;
				img[idx+0] = 0;
			}
		}
	}

	else //RGB
	{
		const char* p = (const char*)pBuffer;
		for(int i=0; i<w; i++)
		{
			for(int j=0; j<h; j++)
			{
				int idx = (i+j*w)*3;
				//bgr2rgb
				img[idx+2] = p[idx];
				img[idx+1] = p[idx+1];
				img[idx+0] = p[idx+2];
			}
		}
	}


	int filesize = 54 + nBufferSize;  //w is your image width, h is image height, both int

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[ 2] = (unsigned char)(filesize    );
	bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(       w    );
	bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
	bmpinfoheader[ 6] = (unsigned char)(       w>>16);
	bmpinfoheader[ 7] = (unsigned char)(       w>>24);
	bmpinfoheader[ 8] = (unsigned char)(       h    );
	bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
	bmpinfoheader[10] = (unsigned char)(       h>>16);
	bmpinfoheader[11] = (unsigned char)(       h>>24);

char str[80];
strcpy (str, cpFileName);
strcat (str,".bmp");

f = fopen(str,"wb");
fwrite(bmpfileheader,1,14,f);
fwrite(bmpinfoheader,1,40,f);
for(int i=0; i<h; i++)
{
    fwrite(img+(w*(h-i-1)*3),3,w,f);
    fwrite(bmppad,1,(4-(w*3)%4)%4,f);
}
fclose(f);
}