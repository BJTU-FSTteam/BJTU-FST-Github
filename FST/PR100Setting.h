#pragma once


// PR100Setting 对话框

class PR100Setting : public CDialogEx
{
	DECLARE_DYNAMIC(PR100Setting)

public:
	PR100Setting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PR100Setting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PR100_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeLuEdit2();
};
