/*-----------------------------------------------------------------------------

	���ߣ����� 2015/08/09
	��ע��
	��ˣ�

-----------------------------------------------------------------------------*/
#include "StdAfx.h"
#include "GInternetConnect.h"

using namespace GIN;

/*===========================================================================

	����:     2015/08/10

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

	����:     2015/08/10

===========================================================================*/
/*---------------------------------------------------------------------------
	2015/08/11 ����: 
---------------------------------------------------------------------------*/
void GDataQueue::PushData(SYS::DataBufferPtr A_DataBuffer)
{
	m_Mutex.Lock();
	m_DataBuffArr.Add(A_DataBuffer);
	m_Mutex.Unlock();
}

/*---------------------------------------------------------------------------
	2015/08/11 ����: 
---------------------------------------------------------------------------*/
SYS::DataBufferPtr GDataQueue::PopData()
{
	m_Mutex.Lock();
	SYS::DataBufferPtr oDataBuff = m_DataBuffArr.ReleasePossession(0);
	m_Mutex.Unlock();
	return oDataBuff;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: 
---------------------------------------------------------------------------*/
GNetServer::GNetServer() : m_ServerSocket(0), m_ClientSocket(0), m_Port(1986)
{}

/*---------------------------------------------------------------------------
	2015/08/09 ����: 
---------------------------------------------------------------------------*/
GNetServer::~GNetServer()
{}

void GNetServer::SetPort(In short A_Port)
{
	m_Port = A_Port;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: 
---------------------------------------------------------------------------*/
bool GNetServer::Initialize()
{
	WSADATA wsa;
	//��ʼ���׽���DLL
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("�׽��ֳ�ʼ��ʧ��!");
		return false;
	}
	if(!CreateSocket())
		return false;

	m_ServerSocket = StartServer();	// ����������
	if(m_ServerSocket == 0)
		return false;

	_beginthread(ReceiveConnectThread, 0, this);

	return true;
}

/*---------------------------------------------------------------------------
	2015/08/11 ����: 
---------------------------------------------------------------------------*/
void GNetServer::ReceiveConnectThread(PVOID param)
{
	GNetServer* pServer = static_cast<GNetServer*>(param);

	pServer->m_ClientSocket = pServer->ReceiveConnect(pServer->m_ServerSocket);	// ���տͻ�������

	_beginthread(ReceiveThread, 0, pServer);//����һ���������ݵ��߳�
}

/*---------------------------------------------------------------------------
	2015/08/11 ����: 
---------------------------------------------------------------------------*/
void GNetServer::Finalize()
{
	if(m_ClientSocket)
	{
		closesocket(m_ServerSocket);//�رտͻ����׽��֣����Ϸ���FIN�źţ�����û�н��յ����Ƿ�����ɵ����ݶ��ᶪʧ��
		closesocket(m_ClientSocket);//�رշ������׽���
		WSACleanup();
	}
}

bool GNetServer::IsConnectSuccess() const
{
	return m_ClientSocket != 0;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: 
---------------------------------------------------------------------------*/
bool GNetServer::CreateSocket()
{
	//�����׽���
	SOCKET oSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(oSocket == INVALID_SOCKET)
	{
		printf("�����׽���ʧ�ܣ�");
		return false;
	}
	return true;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: ����������
---------------------------------------------------------------------------*/
SOCKET GNetServer::StartServer()
{
	//�����׽���
	SOCKET serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(serverSocket == INVALID_SOCKET)
	{
		printf("�����׽���ʧ�ܣ�");
		return 0;
	}

	// ��ʼ��ָ�����ڴ�����
	sockaddr_in oServerAddress;
	memset(&oServerAddress, 0, sizeof(sockaddr_in));
	oServerAddress.sin_family = AF_INET;
	oServerAddress.sin_addr.S_un.S_addr = htonl(INADDR_ANY);// 
	oServerAddress.sin_port = htons(m_Port);

	// ��
	int nBindResult = bind(serverSocket, (sockaddr*)&oServerAddress, sizeof(oServerAddress));
	if(nBindResult == SOCKET_ERROR)
	{
		printf("�׽��ְ󶨵��˿�ʧ�ܣ��˿�: %d", (int)m_Port);
		return 0;
	}

	// ��������״̬
	if(listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("����ʧ�ܣ�\n");
		return 0;
	}

	// ��ȡ������IP
	struct in_addr addr = GetHostName(); 
	printf("Server %s : %d is listening ...\n", inet_ntoa(addr), (int)m_Port);
	return serverSocket;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: ��ȡ����IP
---------------------------------------------------------------------------*/
in_addr GNetServer::GetHostName()
{
	in_addr nAddress;
	memset(&nAddress, 0, sizeof(in_addr));
	char host_name[255];

	// ��ȡ������������
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) 
	{
		printf("Error %d when getting local host name. %d\n", WSAGetLastError());
		return nAddress;
	}

	// �����������ݿ��еõ���Ӧ�ġ�IP�� 
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
	2015/08/09 ����: ���տͻ�������
---------------------------------------------------------------------------*/
SOCKET GNetServer::ReceiveConnect(SOCKET &serverSocket)
{
	SOCKET clientSocket;//�����Ϳͻ���ͨ�ŵ��׽���
	struct sockaddr_in clientAddress;//�����Ϳͻ���ͨ�ŵ��׽��ֵ�ַ
	memset(&clientAddress, 0, sizeof(clientAddress));//��ʼ����ſͻ�����Ϣ���ڴ�
	int addrlen = sizeof(clientAddress);

	//��������
	if((clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &addrlen)) == INVALID_SOCKET)
	{
		printf("���ܿͻ�������ʧ�ܣ�");
		return 0;
	}
	printf("Accept connection from %s\n", inet_ntoa(clientAddress.sin_addr));
	return clientSocket;
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: ���������߳�
---------------------------------------------------------------------------*/
void GNetServer::ReceiveThread(PVOID param)
{
	GNetServer* pServer = static_cast<GNetServer*>(param);

	char aBuffer[2048];
	int nDataSize;
	while(true)
	{
		//��������
		nDataSize = recv(pServer->m_ClientSocket, aBuffer, sizeof(aBuffer), 0);
		if(nDataSize == SOCKET_ERROR)
		{
			printf("��������ʧ��!\n");
			pServer->m_ClientSocket = 0;
			_endthread();//��ֹ��ǰ�߳�
		}

		aBuffer[nDataSize]='\0';
		printf("�ͻ���˵��%s\n", aBuffer);

		SYS::DataBufferPtr oDataBuffer = new SYS::DataBuffer(nDataSize + 1);
		memcpy(oDataBuffer->GetBuffer(), aBuffer, nDataSize + 1);
		pServer->PushData(oDataBuffer);

		if(pServer->m_ClientSocket == 0)
			break;
	}
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: 
---------------------------------------------------------------------------*/
void GNetServer::SendData(In SYS::DataBufferPtr A_DataBuffer)
{
	if(m_ClientSocket != 0)
		_beginthread(SendMsgThread, 0, new SendDataStruct(&m_ClientSocket, A_DataBuffer));
}

/*---------------------------------------------------------------------------
	2015/08/09 ����: ��������
---------------------------------------------------------------------------*/
void GNetServer::SendMsgThread(PVOID param)
{
	SendDataStruct* pSendData = static_cast<SendDataStruct*>(param);
	SYS::DataBufferPtr oDataBuff = pSendData->m_DataBuffer;
	if(send(*pSendData->m_ClientSocket, (char*)oDataBuff->GetBuffer(), oDataBuff->GetSize(), 0) == SOCKET_ERROR)
	{
		printf("��������ʧ�ܣ�\n");
	}

	delete pSendData;
}

/*===========================================================================

	����:     2015/08/10

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
	2015/08/10 ����: 
---------------------------------------------------------------------------*/
bool GNetClient::Initialize()
{
	WSADATA wsa;
	//��ʼ���׽���DLL
	if(WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("�׽��ֳ�ʼ��ʧ��!\n");
		return false;
	}

	//�����׽���
	m_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_ClientSocket == INVALID_SOCKET)
	{
		printf("�����׽���ʧ�ܣ�\n");
		return false;
	}

	if(!Connect())
		return false;

	_beginthread(ReceiveThread, 0, this);//�������������߳�

	return true;
}

void GNetClient::Finalize()
{
	closesocket(m_ClientSocket);//�رշ������׽���
	WSACleanup();
}

bool GNetClient::IsConnectSuccess() const
{
	return m_ClientSocket != 0;
}

/*---------------------------------------------------------------------------
	2015/08/10 ����: ��ȡ������IP
---------------------------------------------------------------------------*/
unsigned long GNetClient::GetServerIP()
{
	//���ַ�����IP��ַת��Ϊu_long
	char ipStr[20];
	//�õڶ�����������һ��������ָ���ڴ棬���ĳ���Ϊ�����������Ĵ�С
	memset(ipStr,0,sizeof(ipStr));
	unsigned long ip;
	if((ip=inet_addr("169.254.48.153"))==INADDR_NONE)
	{
		printf("���Ϸ���IP��ַ��\n");
		return 0;
	}
	return ip;
}

/*---------------------------------------------------------------------------
	2015/08/10 ����: 
---------------------------------------------------------------------------*/
bool GNetClient::Connect()
{
	unsigned long ip = GetServerIP();

	// �Ѷ˿ں�ת��������
	printf("Connecting to %s : %d\n", inet_ntoa(*(in_addr*)&ip), m_Port);
	struct sockaddr_in serverAddress;
	memset(&serverAddress,0,sizeof(sockaddr_in));
	serverAddress.sin_family=AF_INET;
	serverAddress.sin_addr.S_un.S_addr= ip;
	serverAddress.sin_port = htons(m_Port);

	// �����ͷ�����������
	if(connect(m_ClientSocket,(sockaddr*)&serverAddress,sizeof(serverAddress))==SOCKET_ERROR)
	{
		printf("��������ʧ��: %d\n", WSAGetLastError());
		return false;
	}
	return true;
}

/*---------------------------------------------------------------------------
	2015/08/10 ����: ��������
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
			printf("��������ʧ��!\n");
			break;
		}
		aBuffer[nDataSize]='\0';

		SYS::DataBufferPtr oDataBuffer = new SYS::DataBuffer(nDataSize + 1);
		memcpy(oDataBuffer->GetBuffer(), aBuffer, nDataSize + 1);
		pClient->PushData(oDataBuffer);

		printf("������˵�� %s\n", aBuffer);
	}
}

void GNetClient::SendData(In SYS::DataBufferPtr A_DataBuffer)
{
	_beginthread(SendMsgThread, 0, new SendDataStruct(&m_ClientSocket, A_DataBuffer));
}

/*---------------------------------------------------------------------------
	2015/08/10 ����: 
---------------------------------------------------------------------------*/
void GNetClient::SendMsgThread(PVOID param)
{
	SendDataStruct* pSendData = static_cast<SendDataStruct*>(param);
	SYS::DataBufferPtr oDataBuff = pSendData->m_DataBuffer;
	int nSendResult = ::send(*pSendData->m_ClientSocket, 
		(char*)oDataBuff->GetBuffer(), oDataBuff->GetSize(), 0);
	if(nSendResult == SOCKET_ERROR)
	{
		printf("��������ʧ�ܣ�\n");
	}

	delete pSendData;
}

/*===========================================================================

	����:     2015/08/10

===========================================================================*/
String InternetTool::GetLocalIP()
{
	char local[255] = {0};  
	gethostname(local, sizeof(local));  
	hostent* ph = gethostbyname(local);
	if (ph == NULL)  
		return String();  

	in_addr addr;
	memcpy(&addr, ph->h_addr_list[0], sizeof(in_addr)); // �������ȡ��һ��ip  

	String localIP;  
	localIP = inet_ntoa(addr);
	return localIP;
}