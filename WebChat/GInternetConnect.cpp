/*-----------------------------------------------------------------------------

	作者：郭宁 2015/08/09
	备注：
	审核：

-----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "GInternetConnect.h"

using namespace GIN;

/*===========================================================================

	功能:     2015/08/10

===========================================================================*/
struct SendDataStruct
{
	SOCKET* m_ClientSocket;
	SYS::DataBufferPtr m_DataBuffer;

	SendDataStruct(SOCKET* A_Socket, In SYS::DataBufferPtr A_DataBuffer) : 
		m_ClientSocket(A_Socket),
		m_DataBuffer(A_DataBuffer)
	{}

	~SendDataStruct()
	{
		DeleteAndNULL(m_DataBuffer);
	}

};//end SendDataStruct

/*===========================================================================

	功能:     2015/08/10

===========================================================================*/
/*---------------------------------------------------------------------------
	2015/08/11 功能: 
---------------------------------------------------------------------------*/
void GDataQueue::PushData(SYS::DataBufferPtr A_DataBuffer)
{
	m_Mutex.Lock();
	m_DataBuffArr.Add(A_DataBuffer);
	m_Mutex.Unlock();
}

/*---------------------------------------------------------------------------
	2015/08/11 功能: 
---------------------------------------------------------------------------*/
SYS::DataBufferPtr GDataQueue::PopData()
{
	m_Mutex.Lock();
	SYS::DataBufferPtr oDataBuff = m_DataBuffArr.ReleasePossession(0);
	m_Mutex.Unlock();
	return oDataBuff;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 
---------------------------------------------------------------------------*/
GNetServer::GNetServer() : m_ServerSocket(0), m_ClientSocket(0), m_Port(1986)
{}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 
---------------------------------------------------------------------------*/
GNetServer::~GNetServer()
{}

void GNetServer::SetPort(In short A_Port)
{
	m_Port = A_Port;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 
---------------------------------------------------------------------------*/
bool GNetServer::Initialize()
{
	WSADATA wsa;
	//初始化套接字DLL
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("套接字初始化失败!");
		return false;
	}
	if(!CreateSocket())
		return false;

	m_ServerSocket = StartServer();	// 启动服务器
	if(m_ServerSocket == 0)
		return false;

	_beginthread(ReceiveConnectThread, 0, this);

	return true;
}

/*---------------------------------------------------------------------------
	2015/08/11 功能: 
---------------------------------------------------------------------------*/
void GNetServer::ReceiveConnectThread(PVOID param)
{
	GNetServer* pServer = static_cast<GNetServer*>(param);

	pServer->m_ClientSocket = pServer->ReceiveConnect(pServer->m_ServerSocket);	// 接收客户端连接

	_beginthread(ReceiveThread, 0, pServer);//启动一个接收数据的线程
}

/*---------------------------------------------------------------------------
	2015/08/11 功能: 
---------------------------------------------------------------------------*/
void GNetServer::Finalize()
{
	if(m_ClientSocket)
	{
		closesocket(m_ServerSocket);//关闭客户端套接字（马上发送FIN信号，所有没有接收到或是发送完成的数据都会丢失）
		closesocket(m_ClientSocket);//关闭服务器套接字
		WSACleanup();
	}
}

bool GNetServer::IsConnectSuccess() const
{
	return m_ClientSocket != 0;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 
---------------------------------------------------------------------------*/
bool GNetServer::CreateSocket()
{
	//创建套接字
	SOCKET oSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(oSocket == INVALID_SOCKET)
	{
		printf("创建套接字失败！");
		return false;
	}
	return true;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 启动服务器
---------------------------------------------------------------------------*/
SOCKET GNetServer::StartServer()
{
	//创建套接字
	SOCKET serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serverSocket == INVALID_SOCKET)
	{
		printf("创建套接字失败！");
		return 0;
	}

	// 初始化指定的内存区域
	sockaddr_in oServerAddress;
	memset(&oServerAddress, 0, sizeof(sockaddr_in));
	oServerAddress.sin_family = AF_INET;
	oServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);// 
	oServerAddress.sin_port = htons(m_Port);

	// 绑定
	int nBindResult = bind(serverSocket, (sockaddr*)&oServerAddress, sizeof(oServerAddress));
	if(nBindResult == SOCKET_ERROR)
	{
		printf("套接字绑定到端口失败！端口: %d", (int)m_Port);
		return 0;
	}

	// 进入侦听状态
	if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("侦听失败！\n");
		return 0;
	}

	// 获取服务器IP
	struct in_addr addr = GetHostName(); 
	printf("Server %s : %d is listening ...\n", inet_ntoa(addr), (int)m_Port);
	return serverSocket;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 获取本机IP
---------------------------------------------------------------------------*/
in_addr GNetServer::GetHostName()
{
	in_addr nAddress;
	memset(&nAddress, 0, sizeof(in_addr));
	char host_name[255];

	// 获取本地主机名称
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) 
	{
		printf("Error %d when getting local host name. %d\n", WSAGetLastError());
		return nAddress;
	}

	// 从主机名数据库中得到对应的“IP” 
	hostent* phe = gethostbyname(host_name);
	if(phe == 0) 
	{
		printf("Yow! Bad host lookup.");
		return nAddress;
	}

	memcpy(&nAddress, phe->h_addr_list[0], sizeof(struct in_addr));
	return nAddress;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 接收客户端连接
---------------------------------------------------------------------------*/
SOCKET GNetServer::ReceiveConnect(SOCKET &serverSocket)
{
	SOCKET clientSocket;//用来和客户端通信的套接字
	struct sockaddr_in clientAddress;//用来和客户端通信的套接字地址
	memset(&clientAddress, 0, sizeof(clientAddress));//初始化存放客户端信息的内存
	int addrlen = sizeof(clientAddress);

	//接受连接
	if((clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &addrlen)) == INVALID_SOCKET)
	{
		printf("接受客户端连接失败！");
		return 0;
	}
	printf("Accept connection from %s\n", inet_ntoa(clientAddress.sin_addr));
	return clientSocket;
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 接收数据线程
---------------------------------------------------------------------------*/
void GNetServer::ReceiveThread(PVOID param)
{
	GNetServer* pServer = static_cast<GNetServer*>(param);

	char aBuffer[2048];
	int nDataSize;
	while(true)
	{
		//接收数据
		nDataSize = recv(pServer->m_ClientSocket, aBuffer, sizeof(aBuffer), 0);
		if(nDataSize == SOCKET_ERROR)
		{
			printf("接收数据失败!\n");
			pServer->m_ClientSocket = 0;
			_endthread();//终止当前线程
		}

		aBuffer[nDataSize]='\0';
		printf("客户端说：%s\n", aBuffer);

		SYS::DataBufferPtr oDataBuffer = new SYS::DataBuffer(nDataSize + 1);
		memcpy(oDataBuffer->GetBuffer(), aBuffer, nDataSize + 1);
		pServer->PushData(oDataBuffer);

		if(pServer->m_ClientSocket == 0)
			break;
	}
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 
---------------------------------------------------------------------------*/
void GNetServer::SendData(In SYS::DataBufferPtr A_DataBuffer)
{
	if(m_ClientSocket != 0)
		_beginthread(SendMsgThread, 0, new SendDataStruct(&m_ClientSocket, A_DataBuffer));
}

/*---------------------------------------------------------------------------
	2015/08/09 功能: 发送数据
---------------------------------------------------------------------------*/
void GNetServer::SendMsgThread(PVOID param)
{
	SendDataStruct* pSendData = static_cast<SendDataStruct*>(param);
	SYS::DataBufferPtr oDataBuff = pSendData->m_DataBuffer;
	if(send(*pSendData->m_ClientSocket, (char*)oDataBuff->GetBuffer(), oDataBuff->GetSize(), 0) == SOCKET_ERROR)
	{
		printf("发送数据失败！\n");
	}

	delete pSendData;
}

/*===========================================================================

	功能:     2015/08/10

===========================================================================*/
GNetClient::GNetClient() : m_ClientSocket(0), m_Port(1986)
{}

GNetClient::~GNetClient()
{}

void GNetClient::SetPort(In short A_Port)
{
	m_Port = A_Port;
}

/*---------------------------------------------------------------------------
	2015/08/10 功能: 
---------------------------------------------------------------------------*/
bool GNetClient::Initialize()
{
	WSADATA wsa;
	//初始化套接字DLL
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("套接字初始化失败!\n");
		return false;
	}

	//创建套接字
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_ClientSocket == INVALID_SOCKET)
	{
		printf("创建套接字失败！\n");
		return false;
	}

	if(!Connect())
		return false;

	_beginthread(ReceiveThread, 0, this);//启动接收数据线程

	return true;
}

void GNetClient::Finalize()
{
	closesocket(m_ClientSocket);//关闭服务器套接字
	WSACleanup();
}

bool GNetClient::IsConnectSuccess() const
{
	return m_ClientSocket != 0;
}

/*---------------------------------------------------------------------------
	2015/08/10 功能: 获取服务器IP
---------------------------------------------------------------------------*/
unsigned long GNetClient::GetServerIP()
{
	//把字符串的IP地址转化为u_long
	char ipStr[20];
	//用第二个参数填充第一个参数所指的内存，填充的长度为第三个参数的大小
	memset(ipStr,0,sizeof(ipStr));
	unsigned long ip;
	if((ip=inet_addr("169.254.48.153"))==INADDR_NONE)
	{
		printf("不合法的IP地址：\n");
		return 0;
	}
	return ip;
}

/*---------------------------------------------------------------------------
	2015/08/10 功能: 
---------------------------------------------------------------------------*/
bool GNetClient::Connect()
{
	unsigned long ip = GetServerIP();

	// 把端口号转化成整数
	printf("Connecting to %s : %d\n", inet_ntoa(*(in_addr*)&ip), m_Port);
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0,sizeof(sockaddr_in));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.S_un.S_addr= ip;
	serverAddress.sin_port = htons(m_Port);

	// 建立和服务器的连接
	if(connect(m_ClientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR)
	{
		printf("建立连接失败: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

/*---------------------------------------------------------------------------
	2015/08/10 功能: 接收数据
---------------------------------------------------------------------------*/
void GNetClient::ReceiveThread(PVOID param)
{
	GNetClient* pClient = static_cast<GNetClient*>(param);
	SOCKET sock = pClient->m_ClientSocket;

	char aBuffer[2048];
	while(true)
	{
		int nDataSize = recv(sock, aBuffer, sizeof(aBuffer), 0);
		if(nDataSize == SOCKET_ERROR)
		{
			printf("接收数据失败!\n");
			break;
		}
		aBuffer[nDataSize]='\0';

		SYS::DataBufferPtr oDataBuffer = new SYS::DataBuffer(nDataSize + 1);
		memcpy(oDataBuffer->GetBuffer(), aBuffer, nDataSize + 1);
		pClient->PushData(oDataBuffer);

		printf("服务器说： %s\n", aBuffer);
	}
}

void GNetClient::SendData(In SYS::DataBufferPtr A_DataBuffer)
{
	_beginthread(SendMsgThread, 0, new SendDataStruct(&m_ClientSocket, A_DataBuffer));
}

/*---------------------------------------------------------------------------
	2015/08/10 功能: 
---------------------------------------------------------------------------*/
void GNetClient::SendMsgThread(PVOID param)
{
	SendDataStruct* pSendData = static_cast<SendDataStruct*>(param);
	SYS::DataBufferPtr oDataBuff = pSendData->m_DataBuffer;
	int nSendResult = ::send(*pSendData->m_ClientSocket, 
		(char*)oDataBuff->GetBuffer(), oDataBuff->GetSize(), 0);
	if(nSendResult == SOCKET_ERROR)
	{
		printf("发送数据失败！\n");
	}

	delete pSendData;
}

/*===========================================================================

	功能:     2015/08/10

===========================================================================*/
String InternetTool::GetLocalIP()
{
	char local[255] = {0};  
	gethostname(local, sizeof(local));  
	hostent* ph = gethostbyname(local);
	if (ph == NULL)  
		return String();  

	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // 这里仅获取第一个ip  

	String localIP;  
	localIP = inet_ntoa(addr);
	return localIP;
}