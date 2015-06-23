
// ConnectMySQLDlg.h : 头文件
//

#pragma once

#include "VspdCToMySQL.h"
#include "ConnectionMySQL.h"

// CConnectMySQLDlg 对话框
class CConnectMySQLDlg : public CDialog
{
// 构造
public:
	CConnectMySQLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CONNECTMYSQL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CImageList m_ImageList;

	void TestMySQL();
	void TestConnectionMySQL();

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ListCtrl;
};
