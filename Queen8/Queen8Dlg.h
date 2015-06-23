// Queen8Dlg.h : header file
//

#if !defined(AFX_QUEEN8DLG_H__FC243443_01E4_47A1_ACFA_D030E975E8D9__INCLUDED_)
#define AFX_QUEEN8DLG_H__FC243443_01E4_47A1_ACFA_D030E975E8D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DlgProgress.h"
/////////////////////////////////////////////////////////////////////////////
// CQueen8Dlg dialog

class CQueen8Dlg : public CDialog
{
// Construction
public:
	CDlgProgress *pProgress;
	void Rend(CPaintDC * pDC);
	CQueen8Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQueen8Dlg)
	enum { IDD = IDD_QUEEN8_DIALOG };
	CString	m_show;
	CString	m_show2;
	BOOL	m_check_time;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQueen8Dlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	int m;
	// Generated message map functions
	//{{AFX_MSG(CQueen8Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClear();
	afx_msg void OnBack();
	afx_msg void OnCalculate();
	afx_msg void OnEndThread();
	afx_msg void OnResult();
	afx_msg void OnAbout();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void CalForbid();
	int chess[64];
	int order[9];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUEEN8DLG_H__FC243443_01E4_47A1_ACFA_D030E975E8D9__INCLUDED_)
