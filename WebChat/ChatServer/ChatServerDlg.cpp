
// ChatServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

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

// CChatServerDlg �Ի���

CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_Port);
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_LISTEN, &CChatServerDlg::OnBnClickedBtnListen)
	ON_BN_CLICKED(IDC_BTN_SEND_MSG, &CChatServerDlg::OnBnClickedBtnSendMsg)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChatServerDlg ��Ϣ�������

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	SetDlgItemText(IDC_IP_ADDRESS, GIN::InternetTool::GetLocalIP());
	SetDlgItemText(IDC_EDIT_PORT, "1986");

//	SYS::Extent::OpenCommandWnd();

	SetTimer(0, 100, NULL);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CChatServerDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnSendMsg();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChatServerDlg::OnTimer(UINT_PTR nIDEvent)
{
	static bool Success = false;
	if(m_NetServer.IsConnectSuccess())
	{
		if(!Success)
			AddMessage("���ӳɹ�");
	}
	else
	{
		if(Success)
			AddMessage("����ʧ��");
	}
	Success = m_NetServer.IsConnectSuccess();

	GDS::SPtr<SYS::DataBuffer> oDataBuffer(m_NetServer.PopData());
	if(oDataBuffer.IsValid())
	{
		String sMsg = (char*)oDataBuffer->GetBuffer();
		AddMessage("He: " + sMsg);
	}
	CDialog::OnTimer(nIDEvent);
}

void CChatServerDlg::OnBnClickedBtnListen()
{
	String sPort;
	GetDlgItemText(IDC_EDIT_PORT, sPort);
	m_NetServer.SetPort(SYS::StrTool::StrToInt(sPort));
	m_NetServer.Initialize();
	AddMessage("�����˿�...");
}

void CChatServerDlg::OnBnClickedBtnSendMsg()
{
	CString sMsg;
	GetDlgItem(IDC_EDIT_MSG)->GetWindowText(sMsg);

	BytePtr pBuff = new Byte[sMsg.GetLength()];
	memcpy(pBuff, sMsg.GetBuffer(0), sMsg.GetLength());
	m_NetServer.SendData(new SYS::DataBuffer(pBuff, sMsg.GetLength()));
	AddMessage("Me: " + sMsg);

	GetDlgItem(IDC_EDIT_MSG)->SetWindowText("");
}

void CChatServerDlg::AddMessage(In String& A_Msg)
{
	String sOutput;
	GetDlgItemText(IDC_EDIT_OUT, sOutput);
	if(sOutput == "")
		sOutput += A_Msg;
	else
		sOutput += "\r\n" + A_Msg;
	SetDlgItemText(IDC_EDIT_OUT, sOutput);
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChatServerDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
