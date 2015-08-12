/*-----------------------------------------------------------------------------

	作者：郭宁 2015/08/09
	备注：
	审核：

-----------------------------------------------------------------------------*/
#if !defined(AFX_GINTERNETCONNECT_H__171D713D_6E02_4F65_BAFD_8BA280EDDA28__INCLUDED_)
#define AFX_GINTERNETCONNECT_H__171D713D_6E02_4F65_BAFD_8BA280EDDA28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <process.h>

#pragma comment(lib,"ws2_32.lib")

namespace GIN
{
	/*===========================================================================

		功能:     2015/08/10

	===========================================================================*/
	class IpAddress
	{
	private:
		unsigned long m_IP;

	public:
		IpAddress();

		String AsString();
		void BuildFromString(In String& A_IP);

	};//end IpAddress

	/*===========================================================================

		功能:     2015/08/10

	===========================================================================*/
	class GDataQueue
	{
	private:
		CMutex m_Mutex;
		GDS::ObjectArray<SYS::DataBuffer> m_DataBuffArr;

	public:
		void PushData(SYS::DataBufferPtr A_DataBuffer);
		SYS::DataBufferPtr PopData();

	};//end GDataQueue

	/*===========================================================================

		功能:     2015/08/09

	===========================================================================*/
	class GNetServer : public GDataQueue
	{
	private:
		SOCKET m_ServerSocket;
		SOCKET m_ClientSocket;
		short m_Port;	// 端口号

		bool CreateSocket();
		SOCKET StartServer();
		in_addr GetHostName();
		SOCKET ReceiveConnect(SOCKET &serverSocket);

		static void ReceiveThread(PVOID param);
		static void SendMsgThread(PVOID param);
		static void ReceiveConnectThread(PVOID param);

	public:
		GNetServer();
		~GNetServer();

		void SetPort(In short A_Port);

		bool Initialize();
		void Finalize();

		bool IsConnectSuccess() const;

		void SendData(In SYS::DataBufferPtr A_DataBuffer);

	};//end GNetServer

	/*===========================================================================

		功能:     2015/08/10

	===========================================================================*/
	class GNetClient : public GDataQueue
	{
	private:
		SOCKET m_ClientSocket;
		short m_Port;	// 端口号

		unsigned long GetServerIP();
		bool Connect();

		static void ReceiveThread(PVOID param);
		static void SendMsgThread(PVOID param);

	public:
		GNetClient();
		~GNetClient();

		void SetPort(In short A_Port);

		bool Initialize();
		void Finalize();

		bool IsConnectSuccess() const;

		void SendData(In SYS::DataBufferPtr A_DataBuffer);

	};//end GNetClient

	/*===========================================================================

		功能:     2015/08/10

	===========================================================================*/
	class InternetTool
	{
	public:
		static String GetLocalIP();

	};//end InternetTool

}//end GIN

#endif // !defined(AFX_GINTERNETCONNECT_H__171D713D_6E02_4F65_BAFD_8BA280EDDA28__INCLUDED_)



