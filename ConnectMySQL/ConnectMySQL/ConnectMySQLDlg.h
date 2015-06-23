
// ConnectMySQLDlg.h : ͷ�ļ�
//

#pragma once

#include "VspdCToMySQL.h"
#include "ConnectionMySQL.h"

// CConnectMySQLDlg �Ի���
class CConnectMySQLDlg : public CDialog
{
// ����
public:
	CConnectMySQLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CONNECTMYSQL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CImageList m_ImageList;

	void TestMySQL();
	void TestConnectionMySQL();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
};
