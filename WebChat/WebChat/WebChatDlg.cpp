/*-----------------------------------------------------------------------------

	作者：郭宁 2015/07/21
	备注：
	审核：

-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "WebChat.h"
#include "WebChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*===========================================================================

	功能:  用于应用程序“关于”菜单项的 CAboutDlg 对话框   2015/07/21

===========================================================================*/
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/*===========================================================================

	功能:     2015/07/21

===========================================================================*/
CWebChatDlg::CWebChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWebChatDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_IP = 0;
	m_Port = 0;
}

void CWebChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IP_ADDRESS, m_IP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
}

BEGIN_MESSAGE_MAP(CWebChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CWebChatDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, &CWebChatDlg::OnBnClickedBtnSend)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWebChatDlg 消息处理程序

BOOL CWebChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

// 	m_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 
// 	char local[255] = {0};  
// 	gethostname(local, sizeof(local));  
// 	hostent* ph = gethostbyname(local);  
// 	if (ph == NULL)  
// 		return TRUE;  
// 
// 	in_addr addr;  
// 	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip  
// 
// 	CString localIP;  
// 	localIP = inet_ntoa(addr);  
	SetDlgItemText(IDC_IP_ADDRESS, "");
	SetDlgItemText(IDC_EDIT_PORT, "1986");

//	SYS::Extent::OpenCommandWnd();

	SetTimer(0, 100, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

/*---------------------------------------------------------------------------
	2015/07/21 功能: 
---------------------------------------------------------------------------*/
void CWebChatDlg::OnBnClickedBtnConnect()
{
	String sPort;
	GetDlgItemText(IDC_EDIT_PORT, sPort);
	m_NetClient.SetPort(SYS::StrTool::StrToInt(sPort));
	m_NetClient.Initialize();
	AddMessage("连接服务器...");
}

BOOL CWebChatDlg::PreTranslateMessage(MSG* pMsg)
{	
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnSend();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CWebChatDlg::OnBnClickedBtnSend()
{
	CString sMsg;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(sMsg);

	BytePtr pBuff = new Byte[sMsg.GetLength()];
	memcpy(pBuff, sMsg.GetBuffer(0), sMsg.GetLength());
	m_NetClient.SendData(new SYS::DataBuffer(pBuff, sMsg.GetLength()));

	AddMessage("Me: " + sMsg);

	GetDlgItem(IDC_EDIT_MSG)->SetWindowText("");
}

void CWebChatDlg::OnTimer(UINT_PTR nIDEvent)
{
	static bool Success = false;
	if(m_NetClient.IsConnectSuccess())
	{
		if(!Success)
			AddMessage("连接成功");
	}
	else
	{
		if(Success)
			AddMessage("连接失败");
	}
	Success = m_NetClient.IsConnectSuccess();

	GDS::SPtr<SYS::DataBuffer> oDataBuffer(m_NetClient.PopData());
	if(oDataBuffer.IsValid())
	{
		String sMsg = (char*)oDataBuffer->GetBuffer();
		AddMessage("He: " + sMsg);
	}
	CDialog::OnTimer(nIDEvent);
}

void CWebChatDlg::AddMessage(In String& A_Msg)
{
	String sOutput;
	GetDlgItemText(IDC_EDIT_OUT, sOutput);
	if(sOutput == "")
		sOutput += A_Msg;
	else
		sOutput += "\r\n" + A_Msg;
	SetDlgItemText(IDC_EDIT_OUT, sOutput);
}

void CWebChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWebChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWebChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}