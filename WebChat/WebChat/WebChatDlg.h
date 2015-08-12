/*-----------------------------------------------------------------------------

	作者：郭宁 2015/07/21
	备注：
	审核：

-----------------------------------------------------------------------------*/
#pragma once

/*===========================================================================

	功能:     2015/07/21

===========================================================================*/
class CWebChatDlg : public CDialogEx
{
public:
	CWebChatDlg(CWnd* pParent = NULL);	// 标准构造函数

	enum { IDD = IDD_WEBCHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	HICON m_hIcon;
	GIN::GNetClient m_NetClient;

	void ReceiveData();

	void AddMessage(In String& A_Msg);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
	DWORD m_IP;
	UINT m_Port;
	afx_msg void OnBnClickedBtnSend();
	void OnTimer(UINT_PTR nIDEvent);

};
