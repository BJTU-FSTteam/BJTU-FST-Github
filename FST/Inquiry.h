#pragma once
#include "afxwin.h"


// Inquiry �Ի���

class Inquiry : public CDialogEx
{
	DECLARE_DYNAMIC(Inquiry)

public:
	Inquiry(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Inquiry();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INQUIRY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
