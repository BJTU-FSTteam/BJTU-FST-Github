#pragma once


// CModeSet �Ի���

class CModeSet : public CDialog
{
	DECLARE_DYNAMIC(CModeSet)

public:
	CModeSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModeSet();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODESET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_nMode;
};
