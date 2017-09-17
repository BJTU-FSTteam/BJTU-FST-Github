#pragma once


// exitFrame 对话框

class exitFrame : public CDialog
{
	DECLARE_DYNAMIC(exitFrame)

public:
	exitFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~exitFrame();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
