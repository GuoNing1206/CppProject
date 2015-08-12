/*-----------------------------------------------------------------------------

	���ߣ����� 2015/07/21
	��ע��
	��ˣ�

-----------------------------------------------------------------------------*/
#include "stdafx.h"
#include "WebChat.h"
#include "WebChatDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*===========================================================================

	����:  ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���   2015/07/21

===========================================================================*/
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

	����:     2015/07/21

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


// CWebChatDlg ��Ϣ�������

BOOL CWebChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

// 	m_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
// 
// 	char local[255] = {0};  
// 	gethostname(local, sizeof(local));  
// 	hostent* ph = gethostbyname(local);  
// 	if (ph == NULL)  
// 		return TRUE;  
// 
// 	in_addr addr;  
// 	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // �������ȡ��һ��ip  
// 
// 	CString localIP;  
// 	localIP = inet_ntoa(addr);  
	SetDlgItemText(IDC_IP_ADDRESS, "");
	SetDlgItemText(IDC_EDIT_PORT, "1986");

//	SYS::Extent::OpenCommandWnd();

	SetTimer(0, 100, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

/*---------------------------------------------------------------------------
	2015/07/21 ����: 
---------------------------------------------------------------------------*/
void CWebChatDlg::OnBnClickedBtnConnect()
{
	String sPort;
	GetDlgItemText(IDC_EDIT_PORT, sPort);
	m_NetClient.SetPort(SYS::StrTool::StrToInt(sPort));
	m_NetClient.Initialize();
	AddMessage("���ӷ�����...");
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
			AddMessage("���ӳɹ�");
	}
	else
	{
		if(Success)
			AddMessage("����ʧ��");
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWebChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CWebChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}