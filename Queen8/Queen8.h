// Queen8.h : main header file for the QUEEN8 application
//

#if !defined(AFX_QUEEN8_H__86468A18_3FE7_4A81_8AB0_601A459196C0__INCLUDED_)
#define AFX_QUEEN8_H__86468A18_3FE7_4A81_8AB0_601A459196C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQueen8App:
// See Queen8.cpp for the implementation of this class
//

class CQueen8App : public CWinApp
{
public:
	CQueen8App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueen8App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQueen8App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUEEN8_H__86468A18_3FE7_4A81_8AB0_601A459196C0__INCLUDED_)
