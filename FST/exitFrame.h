#pragma once


// exitFrame �Ի���

class exitFrame : public CDialog
{
	DECLARE_DYNAMIC(exitFrame)

public:
	exitFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~exitFrame();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
