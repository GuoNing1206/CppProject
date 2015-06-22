// ShadowDlg.h : header file
//

#if !defined(AFX_MYDLGOPENGLDLG_H__C88233C1_49C0_40BC_82C3_CA672C7FD64D__INCLUDED_)
#define AFX_MYDLGOPENGLDLG_H__C88233C1_49C0_40BC_82C3_CA672C7FD64D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// 添加COpenGL类的说明文件
#include "opengl.h"
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog

class CShadowDlg : public CDialog
{
// Construction
public:
	CShadowDlg(CWnd* pParent = NULL);	// standard constructor
/////////////////////////////////////////////////////////////////////////////
// 添加类的虚构函数和成员变量
	~CShadowDlg(void);
	COpenGL *m_pDisplay;
/////////////////////////////////////////////////////////////////////////////


// Dialog Data
	//{{AFX_DATA(CShadowDlg)
	enum { IDD = IDD_SHADOW_DIALOG };
	CSliderCtrl	m_LightHeight;
	int		m_ShowMode;
	BOOL	m_LockLight;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CShadowDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRadioShowmode();
	afx_msg void OnRadioShowmode2();
	afx_msg void OnRadioShowmode3();
	afx_msg void OnCheckLockLight();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDLGOPENGLDLG_H__C88233C1_49C0_40BC_82C3_CA672C7FD64D__INCLUDED_)
