
// ChatServerDlg.h : 头文件
//

#pragma once


// CChatServerDlg 对话框
class CChatServerDlg : public CDialog
{
// 构造
public:
	CChatServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHATSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;
	GIN::GNetServer m_NetServer;

	void HandleData();
	void AddMessage(In String& A_Msg);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnListen();
	UINT m_Port;
	afx_msg void OnBnClickedBtnSendMsg();
//	virtual void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
