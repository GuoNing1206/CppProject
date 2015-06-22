#if !defined(AFX_OPENGL_H__99E34DDC_206E_4E76_99C9_95271B2A801C__INCLUDED_)
#define AFX_OPENGL_H__99E34DDC_206E_4E76_99C9_95271B2A801C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGL.h : header file
//
#include "GBitmap.h"
/////////////////////////////////////////////////////////////////////////////
// COpenGL window

class COpenGL : public CWnd
{
// Construction
public:
	COpenGL();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGL)
	//}}AFX_VIRTUAL

// Implementation
public:
	GLint maxTexSize;                       // maximum allowed size for 1D/2D texture
	GLfloat factor;                  // for polygon offset
	GLuint shadowTextureID;
	GLboolean ambientShadowAvailable;
	float pos;

	void Init();
	void Rend();
	virtual ~COpenGL();
////////////////////////////////////////////////////////////////
//添加的成员函数与成员变量
	int MySetPixelFormat(HDC hdc);
	HDC hdc ;
	HGLRC hglrc;
////////////////////////////////////////////////////////////////

	void SetupRC();
	float longitude,latitude,viewDis;
	int m_ShowMode;
	bool m_LockLight;
private:
	HCURSOR m_hCursor;	//存储光标的句柄
	bool m_LBDown;
	CPoint m_OriginPoi;	//存储鼠标位置,旋转视图用
	
	float trans_x,trans_y,trans_z;
	// Generated message map functions
	void DrawModels(GLboolean drawBasePlane, bool WithObjTexture);

	void RegenerateShadowMap();
	bool LoadTexture(int x, char *filename);

	GBitmap m_Bitmap;
	unsigned int p_textures[10];
	unsigned char *data_map;
protected:
	//{{AFX_MSG(COpenGL)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGL_H__99E34DDC_206E_4E76_99C9_95271B2A801C__INCLUDED_)
