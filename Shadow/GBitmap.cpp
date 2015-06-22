// GBitmap.cpp: implementation of the GBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Shadow.h"
#include "GBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
GBitmap *GBitmap::pFirst=NULL;
int GBitmap::num_bitmap=0;
GBitmap::GBitmap()
{
	bitmapImage=NULL;
	map_width=0;map_height=0;
	with_alpha=false;
	num_bitmap++;
	loaded=false;
}

GBitmap::~GBitmap()
{
	if(bitmapImage!=NULL)
		free(bitmapImage);
}

bool GBitmap::LoadBitmapFile(char *filename)
{
	FILE *filePtr;								// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	BITMAPINFOHEADER	bitmapInfoHeader;

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return false;
	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != 0x4D42)//BITMAP_ID
	{
		fclose(filePtr);
		return false;
	}
	// read the bitmap information header
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	
	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	map_height=bitmapInfoHeader.biHeight;
	map_width=bitmapInfoHeader.biWidth;
	int offset=4-map_width*3%4;
	if(offset==4)
		offset=0;
	file_size=map_height*map_width*3+map_height*offset;

	// allocate enough memory for the bitmap image data
	unsigned char *oribitmapImage = new unsigned char[file_size];

	// verify memory allocation
	if (!oribitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return false;
	}
	// read in the bitmap image data
	fread(oribitmapImage, 1, file_size, filePtr);
	
	map_height=map_height/8*8;
	map_width=map_width/8*8;
	file_size=map_height*map_width*3;
	bitmapImage = new unsigned char[file_size];

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return false;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (int i=0;i<map_height;i++)
	{
		for(int j=0;j<map_width;j++)
		{
			bitmapImage[(i*map_width+j)*3+2] = oribitmapImage[(i*bitmapInfoHeader.biWidth+j)*3+2		+i*offset];//
			bitmapImage[(i*map_width+j)*3+1] = oribitmapImage[(i*bitmapInfoHeader.biWidth+j)*3+1	+i*offset];//
			bitmapImage[(i*map_width+j)*3] = oribitmapImage[(i*bitmapInfoHeader.biWidth+j)*3		+i*offset];//
		}
	}
	delete[]oribitmapImage;
	// close the file and return the bitmap image data
	fclose(filePtr);
	with_alpha=false;
	loaded=true;
	return true;
}

bool GBitmap::LoadBitmapFile_Alpha(char *filename,char *filename_A)
{
	/*FILE *filePtr;								// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	BITMAPINFOHEADER	bitmapInfoHeader;
	unsigned char		*bitmapImage;			// bitmap image data
	int					imageIdx = 0;			// image index counter
	unsigned char		tempRGB;				// swap variable*/
	LoadBitmapFile(filename);

	int map_height_old=map_height;
	int map_width_old=map_width;
	unsigned char *bitmapImage_old=bitmapImage;

	LoadBitmapFile(filename_A);//此时bitmapImage存储alpha通道的值

	if(map_width!=map_width_old || map_height!=map_height_old)
	{/*MessageBox("the size of two map do not match!");*/ free(bitmapImage_old);return false;}
	//str.Format("width: %d height: %d",map_width,map_height);MessageBox(str);

	unsigned char *bitmapWithAlpha =(unsigned char *)malloc(3*map_height*map_width*4/3*sizeof(unsigned char));
	if (bitmapImage == NULL || bitmapWithAlpha == NULL)//如果位图加载失败
	{/*MessageBox("failed to load alpha map");*/free(bitmapWithAlpha);free(bitmapImage_old);return false;}

	for (unsigned int src = 0, dst = 0; src <3*map_height*map_width*sizeof(unsigned char); src +=3, dst +=4)//
	{
		bitmapWithAlpha[dst+3] = (bitmapImage[src]+bitmapImage[src+1]+bitmapImage[src+2])/3;
		// copy pixel data over
		bitmapWithAlpha[dst] = bitmapImage_old[src];
		bitmapWithAlpha[dst+1] = bitmapImage_old[src+1];
		bitmapWithAlpha[dst+2] = bitmapImage_old[src+2];
	}
	free(bitmapImage_old);
	free(bitmapImage);
	bitmapImage=bitmapWithAlpha;
	with_alpha=true;
	return true;
}
bool GBitmap::SetData(unsigned char *data, int width, int height)
{
	//if(loaded)
	//	delete[]bitmapImage;  the buffer should be delete outside!
	bitmapImage=data;
	map_width=width;
	map_height=height;
	loaded=true;
	return true;
}

bool GBitmap::WriteBitmapFile(char *filename)
{
	FILE *filePtr;								// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	BITMAPINFOHEADER	bitmapInfoHeader;
	int					imageIdx;			// image index counter
	unsigned char		tempRGB;				// swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "wb");
	if (filePtr == NULL)
		return false;

	file_size=map_width*map_height*3;
	//define  bitmapFileHeader
	bitmapFileHeader.bfSize=sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType=0x4D42;
	bitmapFileHeader.bfReserved1=0;
	bitmapFileHeader.bfReserved2=0;
	bitmapFileHeader.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

	//define bitmapInfoHeader
	bitmapInfoHeader.biSize=sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biPlanes=1;
	bitmapInfoHeader.biBitCount=24;
	bitmapInfoHeader.biCompression=BI_RGB;
	bitmapInfoHeader.biSizeImage=map_width*abs(map_height)*3;
	bitmapInfoHeader.biXPelsPerMeter=0;
	bitmapInfoHeader.biYPelsPerMeter=0;
	bitmapInfoHeader.biClrUsed=0;
	bitmapInfoHeader.biClrImportant=0;
	bitmapInfoHeader.biWidth=map_width;
	bitmapInfoHeader.biHeight=map_height;

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < file_size; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}
	// write the bitmap file header
	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	// write the bitmap information header
	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	fwrite(bitmapImage, 1, file_size, filePtr);
	// close the file and return the bitmap image data
	fclose(filePtr);
	return true;
}
