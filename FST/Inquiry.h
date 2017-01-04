#pragma once
#include "afxwin.h"


// Inquiry 对话框

class Inquiry : public CDialogEx
{
	DECLARE_DYNAMIC(Inquiry)

public:
	Inquiry(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Inquiry();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INQUIRY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_fname;
	CListBox m_status_list;
	int itemNum;
	char item[300][80];
//	afx_msg void OnEnChangeFnameEdit();
	virtual BOOL OnInitDialog();
};
