
// ConnectMySQL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CConnectMySQLApp:
// �йش����ʵ�֣������ ConnectMySQL.cpp
//

class CConnectMySQLApp : public CWinApp
{
public:
	CConnectMySQLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CConnectMySQLApp theApp;