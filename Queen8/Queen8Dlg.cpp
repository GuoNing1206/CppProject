// Queen8Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Queen8.h"
#include "Queen8Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CQueen8Dlg *pDlg = NULL;

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
// CQueen8Dlg dialog

extern CDlgProgress *g_pProgress;
extern bool stop;
extern int x[];
extern int index;
extern int result[];

CQueen8Dlg::CQueen8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQueen8Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQueen8Dlg)
	m_show = _T("");
	m_show2 = _T("");
	m_check_time = TRUE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	for(int i=0;i<64;i++)
		chess[i]=0;	//无棋子 也无禁用
	order[0]=0;

	pProgress=new CDlgProgress;
	pProgress->Create(IDD_DLG_PROGRESS,this);
	g_pProgress=pProgress;
	m=0;
	pDlg = this;
}

void CQueen8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQueen8Dlg)
	DDX_Text(pDX, IDC_SHOW, m_show);
	DDX_Text(pDX, IDC_SHOW2, m_show2);
	DDX_Check(pDX, IDC_CHECK_TIME, m_check_time);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQueen8Dlg, CDialog)
	//{{AFX_MSG_MAP(CQueen8Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
	ON_BN_CLICKED(IDC_END_THREAD, OnEndThread)
	ON_BN_CLICKED(IDC_RESULT, OnResult)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQueen8Dlg message handlers

BOOL CQueen8Dlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQueen8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQueen8Dlg::OnPaint() 
{
	CPaintDC dc(this);
	if (IsIconic())
	{
		 // device context for painting

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
	Rend(&dc);
	m_show.Format("当前有 %d 个皇后",order[0]);
	UpdateData(false);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQueen8Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQueen8Dlg::Rend(CPaintDC *pDC)
{
		//CPaintDC dc(this);
	CDC dcMemory;
	CBitmap bit,bitmap_2;
	bit.LoadBitmap(IDB_BITMAP2);
	CDC mapDC;
	pDC->Rectangle(9,9,411,411);
	if(dcMemory.CreateCompatibleDC(pDC))
	{
		if(bitmap_2.CreateCompatibleBitmap(pDC,400,400))
		{
			dcMemory.SelectObject(&bitmap_2);
			
			mapDC.CreateCompatibleDC(&dcMemory);
			mapDC.SelectObject(bit);
			for(int i=0;i<8;i++)
			{
				for(int j=0;j<8;j++)
				{
					if(chess[i*8+j]==1)
						dcMemory.StretchBlt(50*i,50*j,50,50,&mapDC,0,0,50,50,SRCCOPY);
					else
						dcMemory.StretchBlt(50*i,50*j,50,50,&mapDC,50,0,50,50,SRCCOPY);
				}
			}

			pDC->BitBlt(10,10,410,410,&dcMemory,0,0,SRCCOPY);//
		}
	}
/***************************************************************************/
	CBrush brush(RGB(200,200,200));
	int i,a,b;
	
	for(i=0;i<64;i++)
	{
		a=(int)(i/8.0);
		b=i%8;
		if(chess[i]==1)
			;//pDC->Ellipse(a*50+20,b*50+20,a*50+50,b*50+50);
		else if(chess[i]==2)
		{
			pDC->FillRect(CRect(a*50+10,b*50+10,a*50+60,b*50+60),&brush);
		}
	}
	for(i=1;i<8;i++)
	{
		pDC->MoveTo(10,i*50+10);
		pDC->LineTo(410,i*50+10);

		pDC->MoveTo(i*50+10,10);
		pDC->LineTo(i*50+10,410);
	}
}

void CQueen8Dlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(point.x<10 || point.x>410 || point.y<10 || point.y>410)
		return;
	int x,y;
	x=(int)((point.x-10)/50.0);
	y=(int)((point.y-10)/50.0);
	
	if(chess[x*8+y]==0)
	{
		chess[x*8+y]=1;	//落子状态
		order[order[0]+1]=x*8+y;
		order[0]++;
	}
	else
		MessageBox("此处不允许落子!");

	CalForbid();
	Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}
void  CQueen8Dlg::CalForbid()
{
	int i,j,p,temp;
	for(i=0;i<64;i++)
		if(chess[i]!=1)
			chess[i]=0;	//无禁用
	for(i=0;i<8;i++)
	{		
		for(j=0;j<8;j++)
		{	
			if(chess[i*8+j]==1)
			{
				if(i>j)
					temp=j;
				else
					temp=i;	
				for(p=0;p<16;p++)
				{
					if(p<8)
					{
						if(chess[i*8+p]!=1)
							chess[i*8+p]=2;
						if(chess[p*8+j]!=1)
							chess[p*8+j]=2;
					}
					if((i+8-p)*8+(j+8-p)<64 && (i+8-p)*8+(j+8-p)>=0 && chess[(i+8-p)*8+(j+8-p)]!=1
						&& (i+8-p)>=0 && (j+8-p)>=0 && (i+8-p)<8 && (j+8-p)<8)
						chess[(i+8-p)*8+(j+8-p)]=2;
					if((i+8-p)*8+(j-8+p)<64 && (i+8-p)*8+(j-8+p)>=0 && chess[(i+8-p)*8+(j-8+p)]!=1
						&& (i+8-p)>=0 && (j-8+p)>=0 && (i+8-p)<8 && (j-8+p)<8)
						chess[(i+8-p)*8+(j-8+p)]=2;
				}
			}
		}
	}
}

void CQueen8Dlg::OnClear()
{
	// TODO: Add your control notification handler code here
	for(int i=0;i<64;i++)
		chess[i]=0;	//无棋子 也无禁用
	order[0]=0;
	CalForbid();
	Invalidate();
}
int n;
void CQueen8Dlg::OnBack()
{
	// TODO: Add your control notification handler code here
	chess[order[order[0]]]=0;

	if(order[0]>0)
		order[0]--;
	CalForbid();
	Invalidate();
}

UINT CalculateQueen(LPVOID pParam)
{
	index=0;
		int a1,a2,a3,a4,a5,a6,a7,a8,   i;
		a1=0;
		//============================
layer1:	a1++;
		g_pProgress->SetPosition((int)((a1-1)*100.0/8.0));
		if(pDlg->m_check_time == TRUE)
			Sleep(100);
		if(a1<=8)
			x[0]=a1;
		else
			goto end;
		a2=0;
		//============================
layer2:	a2++;
		if(a2<=8)
		{
			if(a2!=a1 && a2!=a1+1 && a2!=a1-1)
				x[1]=a2;
			else
				goto layer2;
		}
		else
			goto layer1;
		a3=0;
		//============================
layer3:	a3++;
		if(a3<=8)
		{
			if(	a3!=a1 && a3!=a1+2 && a3!=a1-2 &&
				a3!=a2 && a3!=a2+1 && a3!=a2-1)
				x[2]=a3;
			else
				goto layer3;
		}
		else
			goto layer2;
		a4=0;
		//============================
layer4:	a4++;
		if(a4<=8)
		{
			if(	a4!=a1 && a4!=a1+3 && a4!=a1-3 &&
				a4!=a2 && a4!=a2+2 && a4!=a2-2 &&
				a4!=a3 && a4!=a3+1 && a4!=a3-1)
				x[3]=a4;
			else
				goto layer4;
		}
		else
			goto layer3;
		a5=0;
		//============================
layer5:	a5++;
		if(a5<=8)
		{
			if(	a5!=a1 && a5!=a1+4 && a5!=a1-4 &&
				a5!=a2 && a5!=a2+3 && a5!=a2-3 &&
				a5!=a3 && a5!=a3+2 && a5!=a3-2 &&
				a5!=a4 && a5!=a4+1 && a5!=a4-1)
				x[4]=a5;
			else
				goto layer5;
		}
		else
			goto layer4;
		a6=0;
		//============================
layer6:	a6++;
		if(a6<=8)
		{
			if(	a6!=a1 && a6!=a1+5 && a6!=a1-5 &&
				a6!=a2 && a6!=a2+4 && a6!=a2-4 &&
				a6!=a3 && a6!=a3+3 && a6!=a3-3 &&
				a6!=a4 && a6!=a4+2 && a6!=a4-2 &&
				a6!=a5 && a6!=a5+1 && a6!=a5-1)
				x[5]=a6;
			else
				goto layer6;
		}
		else
			goto layer5;
		a7=0;
		//============================
layer7:	a7++;
		if(a7<=8)
		{
			if(	a7!=a1 && a7!=a1+6 && a7!=a1-6 &&
				a7!=a2 && a7!=a2+5 && a7!=a2-5 &&
				a7!=a3 && a7!=a3+4 && a7!=a3-4 &&
				a7!=a4 && a7!=a4+3 && a7!=a4-3 &&
				a7!=a5 && a7!=a5+2 && a7!=a5-2 &&
				a7!=a6 && a7!=a6+1 && a7!=a6-1)
				x[6]=a7;
			else
				goto layer7;
		}
		else
			goto layer6;
		a8=0;
		//============================
layer8:	a8++;
		if(stop)
			goto end;
		if(a8<=8)
		{
			if(	a8!=a1 && a8!=a1+7 && a8!=a1-7 && 
				a8!=a2 && a8!=a2+6 && a8!=a2-6 && 
				a8!=a3 && a8!=a3+5 && a8!=a3-5 && 
				a8!=a4 && a8!=a4+4 && a8!=a4-4 && 
				a8!=a5 && a8!=a5+3 && a8!=a5-3 && 
				a8!=a6 && a8!=a6+2 && a8!=a6-2 && 
				a8!=a7 && a8!=a7+1 && a8!=a7-1)
			{
				x[7]=a8;
				for(i=0;i<8;i++)
					result[index*8+i]=x[i]-1;

				if(index<99)
					index++;
				else
					goto end;
			}
			goto layer8;
		}
		else
			goto layer7;/**/
end:	
	g_pProgress->ShowWindow(SW_HIDE);
	return 0;
}

void CQueen8Dlg::OnCalculate()
{
	// TODO: Add your control notification handler code here
	stop=false;
	UpdateData();
	CWinThread* myThread=AfxBeginThread(CalculateQueen,(LPVOID)n);
	pProgress->ShowWindow(SW_SHOW);
}


void CQueen8Dlg::OnEndThread() 
{
	// TODO: Add your control notification handler code here
	/*stop=true;
	pProgress->ShowWindow(SW_HIDE);*/
	if(index==0)
		return;
	m--;
	if(m<0)
		m+=index;
	OnClear();
	// TODO: Add your control notification handler code here
	for(int i=0;i<8;i++)
	{
		if(chess[i*8+result[m*8+i]]==0)
		{
			chess[i*8+result[m*8+i]]=1;	//落子状态
			order[order[0]+1]=i*8+result[m*8+i];
			order[0]++;
		}
	}
	m_show2.Format("共找到%d种可能,当前为 %d ",index,m+1);
	CalForbid();
	Invalidate();
	UpdateData(false);
}

void CQueen8Dlg::OnResult() 
{
	if(index==0)
		return;
	m++;
	m=m%index;
	OnClear();
	// TODO: Add your control notification handler code here
	for(int i=0;i<8;i++)
	{
		if(chess[i*8+result[m*8+i]]==0)
		{
			chess[i*8+result[m*8+i]]=1;	//落子状态
			order[order[0]+1]=i*8+result[m*8+i];
			order[0]++;
		}
	}
	m_show2.Format("共找到%d种可能,当前为 %d ",index,m+1);
	CalForbid();
	Invalidate();
	UpdateData(false);
	
}

void CQueen8Dlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();
}

void CQueen8Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CAboutDlg dlg;
	dlg.DoModal();
	//CDialog::OnClose();
}

void CQueen8Dlg::OnDestroy() 
{
	//CDialog::OnDestroy();
	CAboutDlg dlg;
	//dlg.DoModal();
	// TODO: Add your message handler code here
	
}

void CQueen8Dlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CAboutDlg dlg;
	dlg.DoModal();
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CQueen8Dlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	 if (pMsg->message == WM_KEYDOWN)
    { 
      if(pMsg->wParam==VK_ESCAPE)
	  {
		  MessageBox("Escape?");
          return true; 
	  }
      else if(pMsg->wParam==VK_RETURN) 
          return true; 
	  else if(pMsg->wParam==VK_LEFT)
			OnEndThread();
	  else if(pMsg->wParam==VK_RIGHT)
		  OnResult();
    } 
	return CDialog::PreTranslateMessage(pMsg);
}

void CQueen8Dlg::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType) 
{
	// TODO: Add your specialized code here and/or call the base class
	


	CDialog::CalcWindowRect(lpClientRect, nAdjustType);
}
