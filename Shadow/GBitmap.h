// GBitmap.h: interface for the GBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GBITMAP_H__3F45852E_C798_4814_8F62_60EF9C04D13B__INCLUDED_)
#define AFX_GBITMAP_H__3F45852E_C798_4814_8F62_60EF9C04D13B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class GBitmap
{
public:
	bool WriteBitmapFile(char* filename);
	bool SetData(unsigned char*data,int width,int  height);
	GBitmap();
	virtual ~GBitmap();
	static GBitmap* pFirst;
	static int num_bitmap;
	unsigned char *bitmapImage;
	int map_width,map_height;
	long file_size;
	bool with_alpha;
	bool LoadBitmapFile(char *filename);
	bool LoadBitmapFile_Alpha(char *filename,char *filename_A);

	bool loaded;
};

#endif // !defined(AFX_GBITMAP_H__3F45852E_C798_4814_8F62_60EF9C04D13B__INCLUDED_)
