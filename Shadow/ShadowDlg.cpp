// ShadowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Shadow.h"
#include "ShadowDlg.h"
#include <MATH.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog

extern GLfloat lightPos[];

CShadowDlg::CShadowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShadowDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CShadowDlg)
	m_ShowMode = 0;
	m_LockLight = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
/////////////////////////////////////////////////////////////////////////////
// 构造一个新的COpenGL对象
	m_pDisplay = new COpenGL;
/////////////////////////////////////////////////////////////////////////////

}

void CShadowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CShadowDlg)
	DDX_Control(pDX, IDC_SLIDER_LIGHT_HEIGHT, m_LightHeight);
	DDX_Radio(pDX, IDC_RADIO_SHOWMODE, m_ShowMode);
	DDX_Check(pDX, IDC_CHECK_LOCK_LIGHT, m_LockLight);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CShadowDlg, CDialog)
	//{{AFX_MSG_MAP(CShadowDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_RADIO_SHOWMODE, OnRadioShowmode)
	ON_BN_CLICKED(IDC_RADIO_SHOWMODE2, OnRadioShowmode2)
	ON_BN_CLICKED(IDC_RADIO_SHOWMODE3, OnRadioShowmode3)
	ON_BN_CLICKED(IDC_CHECK_LOCK_LIGHT, OnCheckLockLight)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg message handlers

BOOL CShadowDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	// 定义OpenGL绘图窗口的矩形大小
	CRect rect(0, 0, 800, 600);
	// 创建COpenGL类对象
	m_pDisplay->Create( NULL,   //缺省的窗口
						NULL,   //无窗口名称
						WS_CHILD|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_VISIBLE,
						        // 定义窗口风格
						rect,   // 窗口的大小
						this,   // 指定当前对话框为其父窗口指针
						0);     

	SetTimer(0,10,NULL);
	m_pDisplay->Init();
	m_LightHeight.SetRange(0, 100);
	m_LightHeight.SetPos(50);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShadowDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShadowDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CShadowDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
// 添加CMyDlgOpenGlg类的虚构函数
CShadowDlg::~CShadowDlg()
{
	if(m_pDisplay)
	{
		delete m_pDisplay;// 释放m_pDisplay指针
	}

}
/////////////////////////////////////////////////////////////////////////////

void CShadowDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	m_pDisplay->Rend();
	CDialog::OnTimer(nIDEvent);
}

BOOL CShadowDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_ShowMode != 2)
	{
		if(zDelta > 0)
			m_pDisplay->viewDis *= 1.05;
		else
			m_pDisplay->viewDis /= 1.05;
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CShadowDlg::OnRadioShowmode() 
{
	m_pDisplay->m_ShowMode = 0;
}

void CShadowDlg::OnRadioShowmode2() 
{
	m_pDisplay->m_ShowMode = 1;
}

void CShadowDlg::OnRadioShowmode3() 
{
	m_pDisplay->SetupRC();
	m_pDisplay->m_ShowMode = 2;
}

void CShadowDlg::OnCheckLockLight() 
{
	m_pDisplay->m_LockLight = !m_pDisplay->m_LockLight;
}

void CShadowDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	lightPos[1] = tan(m_LightHeight.GetPos()/200.0f*3.14f)*200;
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
