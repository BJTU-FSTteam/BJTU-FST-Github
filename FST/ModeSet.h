#pragma once


// CModeSet 对话框

class CModeSet : public CDialog
{
	DECLARE_DYNAMIC(CModeSet)

public:
	CModeSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModeSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODESET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_nMode;
};
