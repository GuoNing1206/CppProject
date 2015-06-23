#if !defined(AFX_DLGPROGRESS_H__1F8DC4C7_5045_4784_B898_A64021AA0C4F__INCLUDED_)
#define AFX_DLGPROGRESS_H__1F8DC4C7_5045_4784_B898_A64021AA0C4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProgress.h : header file
//
#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog

class CDlgProgress : public CDialog
{
// Construction
public:
	void SetPosition(int pos);
	CDlgProgress(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProgress)
	enum { IDD = IDD_DLG_PROGRESS };
	CProgressCtrl	m_progress;
	CString	m_indicator;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProgress)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProgress)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROGRESS_H__1F8DC4C7_5045_4784_B898_A64021AA0C4F__INCLUDED_)
