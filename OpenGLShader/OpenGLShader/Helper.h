/*-----------------------------------------------------------------------------

	���ߣ����� 2015/06/04
	��ע��
	��ˣ�

-----------------------------------------------------------------------------*/
#if !defined(AFX_HELPER_H__E53B4CF7_B861_4FA0_87E6_7C94D2BB03DB__INCLUDED_)
#define AFX_HELPER_H__E53B4CF7_B861_4FA0_87E6_7C94D2BB03DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*===========================================================================

	����:     2015/05/18

===========================================================================*/
class Helper
{
public:
	static String GetShader(In String& A_FileName);
	static String GetImage(In String& A_FileName);
	static bool LoadTGATexture(ConstCharPtr szFileName, 
		GLenum minFilter, GLenum magFilter, GLenum wrapMode);

};

#endif // !defined(AFX_HELPER_H__E53B4CF7_B861_4FA0_87E6_7C94D2BB03DB__INCLUDED_)