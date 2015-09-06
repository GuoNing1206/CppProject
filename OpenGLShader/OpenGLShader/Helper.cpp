#include "stdafx.h"
#include "Helper.h"

#ifdef _DEBUG
#undef THIS_FILE
		static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*===========================================================================

	功能:     2015/06/04

===========================================================================*/
/*---------------------------------------------------------------------------
	2015/05/18 功能: 
---------------------------------------------------------------------------*/
String Helper::GetShader(In String& A_FileName)
{
	return SYS::FileTool::GetModuleFilePath() + "Shaders\\" + A_FileName;
}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 
---------------------------------------------------------------------------*/
String Helper::GetImage(In String& A_FileName)
{
	return SYS::FileTool::GetModuleFilePath() + "Images\\" + A_FileName;
}

/*---------------------------------------------------------------------------
	2015/05/18 功能: 
---------------------------------------------------------------------------*/
bool Helper::LoadTGATexture(ConstCharPtr szFileName, GLenum minFilter, GLenum magFilter, GLenum wrapMode)
{
	GLbyte* pBits;
	int nWidth, nHeight, nConponents;
	GLenum eFormat;
	pBits = gltReadTGABits(szFileName, &nWidth, &nHeight, &nConponents, &eFormat);
	if(pBits == NULL)
		return false;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, nConponents, nWidth, nHeight, 0, eFormat, GL_UNSIGNED_BYTE, pBits);
	free(pBits);
	if(minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	return true;
}