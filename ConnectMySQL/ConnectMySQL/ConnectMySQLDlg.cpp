
// ConnectMySQLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ConnectMySQL.h"
#include "ConnectMySQLDlg.h"
#include "afxdialogex.h"
#include <afxsock.h>
#include <mysql.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CConnectMySQLDlg 对话框




CConnectMySQLDlg::CConnectMySQLDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectMySQLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CConnectMySQLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CConnectMySQLDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()

#include <Winsock2.h>

void TestSoket()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD( 1, 1 );

	err = WSAStartup( wVersionRequested, &wsaData );
	if(err != 0)
		return;

	if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE( wsaData.wVersion ) != 1) 
	{
		WSACleanup( );
		return;
	}
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("90.10.5.120");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(6000);
	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	send(sockClient,"hello",strlen("hello")+1,0);
	char recvBuf[50];
	memset(recvBuf, 0, 50);
	recv(sockClient,recvBuf,50,0);
	printf("%s\n",recvBuf);

	closesocket(sockClient);
	WSACleanup();
}

// CConnectMySQLDlg 消息处理程序

BOOL CConnectMySQLDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	m_ImageList.Create(16, 16, ILC_COLOR32, 4, 4);
	m_ImageList.Add(AfxGetApp()->LoadIcon(IDI_LIST));
	m_ListCtrl.SetImageList(&m_ImageList, LVSIL_SMALL);

	m_ListCtrl.SetExtendedStyle(
		m_ListCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ListCtrl.InsertColumn(0, "", 0, 50);
	m_ListCtrl.InsertColumn(1, "", 0, 300);
	m_ListCtrl.InsertColumn(2, "", 0, 300);

	// TODO: 在此添加额外的初始化代码
	TestConnectionMySQL();
//	TestSoket();
//	TestMySQL();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CConnectMySQLDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CConnectMySQLDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CConnectMySQLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CConnectMySQLDlg::TestConnectionMySQL()
{
	CString sOut;

	try
	{
		ConnectionMySQL mysqlConnection;
		mysqlConnection.Connect("localhost", "guodb", "root", "");

		LVCOLUMN oInfo;
		oInfo.mask = LVCF_TEXT;

		oInfo.pszText = "ID";
		m_ListCtrl.SetColumn(0, &oInfo);
		oInfo.pszText = "名字";
		m_ListCtrl.SetColumn(1, &oInfo);
		oInfo.pszText = "电话号码";
		m_ListCtrl.SetColumn(2, &oInfo);

		Result* queryResult = mysqlConnection.Query("SELECT * FROM addressbook ORDER BY id ASC;");
		int affectedRows = mysqlConnection.GetAffectedRows();

		while(queryResult->Next())
		{
			Row* row = queryResult->GetCurrentRow();
			sOut += SYS::StrTool::Format("id: %s\t", row->GetField(0));
			sOut += SYS::StrTool::Format("name: %s\t", row->GetField(1));
			sOut += SYS::StrTool::Format("cellphone: %s\r\n", row->GetField(2));

			m_ListCtrl.InsertItem(m_ListCtrl.GetItemCount(), "");
			m_ListCtrl.SetItemText(m_ListCtrl.GetItemCount() - 1, 0, row->GetField(0));
			m_ListCtrl.SetItemText(m_ListCtrl.GetItemCount() - 1, 1, row->GetField(1));
			m_ListCtrl.SetItemText(m_ListCtrl.GetItemCount() - 1, 2, row->GetField(2));

			delete row;
		}
		sOut += SYS::StrTool::Format("Total rows: %d row(s)\r\n",queryResult->GetRowCount());
		queryResult->Close();
		delete queryResult;

		queryResult=mysqlConnection.Query("SELECT HEX(id) FROM testbin;");
		while(queryResult->Next())
		{
			Row* row = queryResult->GetCurrentRow();
// 			BytePtr pBuff = (BytePtr)row->GetField(0);
// 			String sBuf;
// 			SYS::StrTool::DataBuffToHexStr(SYS::DataBuffer(pBuff, 16, true), sBuf);
			sOut += SYS::StrTool::Format("id: %s\t",row->GetField(0));
			delete row;
		}
		sOut += SYS::StrTool::Format("Total rows: %d row(s)\r\n",queryResult->GetRowCount());
		queryResult->Close();
		delete queryResult;
	}
	catch(const char* exceptionString)
	{
		sOut += SYS::StrTool::Format("%s\r\n",exceptionString);
	}
}

void CConnectMySQLDlg::TestMySQL()
{
	CString sOut;

	char* host="localhost";
	char* user="root";
	char* port ="3306";
	char* passwd="";
	char* dbname="guodb"; 
	char* charset = "GBK";//支持中文
	char* Msg = "";//消息变量
	//初始化
	VspdCToMySQL * vspdctomysql = new VspdCToMySQL;
	if(vspdctomysql->ConnMySQL(host,port,dbname,user,passwd,charset,Msg) == 0)
		sOut += ("连接成功\r\n");
	else
		sOut += (Msg);

	//查询
	char * SQL = "SELECT * FROM addressbook";
	CString str = vspdctomysql->SelectData(SQL, Msg);
	if( str.GetLength() > 0 )
	{
		sOut += ("查询成功\r\n");
		sOut += (str);
		sOut += ("\r\n");
	}
	else
	{
		sOut += (Msg);
	}

	//插入
// 	SQL = "insert into addressBook values('Lacie Heart', '789456132')";
// 	if(vspdctomysql->InsertData(SQL,Msg) == 0)
// 		sOut += ("插入成功/r/n");


// 	//更新
// 	SQL = "update vcaccesstest set username = '修改了',passwd='2345' where ids = 3 ";
// 	if(vspdctomysql->UpdateData(SQL,Msg) == 0)
// 		printf("更新成功/r/n");
// 	//删除
// 	SQL = "delete from vcaccesstest where ids = 3 ";
// 	if(vspdctomysql->DeleteData(SQL,Msg) == 0)
// 		printf("删除成功/r/n");

	vspdctomysql->CloseMySQLConn();
}