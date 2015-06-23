// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
	m_indicator = _T("");
	//}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Text(pDX, IDC_INDICATOR, m_indicator);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

void CDlgProgress::SetPosition(int pos)
{
	//m_indicator.SetWin=
	//((CStatic*)(g_pad->GetDlgItem(IDC_STATIC_ON)))->SetWindowText("½ûÓÃ");
	CString str;
	str.Format("%d £¥",pos);
	GetDlgItem(IDC_INDICATOR)->SetWindowText(str);
	m_progress.SetPos(pos);
}
