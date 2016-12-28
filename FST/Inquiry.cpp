// Inquiry.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "Inquiry.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
// Inquiry 对话框

IMPLEMENT_DYNAMIC(Inquiry, CDialogEx)

Inquiry::Inquiry(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INQUIRY_DIALOG, pParent)
	, m_fname(_T(""))
{
	itemNum = 0;
	for (int i = 0; i < 300; i++)
		strcpy(item[i], "");
	itemNum = 0;
}

Inquiry::~Inquiry()
{
}

void Inquiry::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FNAME_EDIT, m_fname);
	DDV_MaxChars(pDX, m_fname, 20);
	DDX_Control(pDX, IDC_STATUS_LIST, m_status_list);
}


BEGIN_MESSAGE_MAP(Inquiry, CDialogEx)
	ON_BN_CLICKED(IDOK, &Inquiry::OnBnClickedOk)
END_MESSAGE_MAP()


// Inquiry 消息处理程序


void Inquiry::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


//void Inquiry::OnEnChangeFnameEdit()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


BOOL Inquiry::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_status_list.SetTabStops(40);

	m_status_list.ResetContent();

	for (int i = 0; i < itemNum; i++)
	{
		m_status_list.AddString(item[i]);
	}

	if (itemNum > 0)
	{
		CString curFilename = CString(item[itemNum - 1]);
		CString  tt1 = _T("");
		CString  tt2 = _T("");
		curFilename.MakeUpper();
		int Pos = curFilename.Find(".DAT");
		curFilename.MakeLower();
		int fNum = atoi(curFilename.Mid(Pos - 3, Pos));
		tt1.Format("%03d.dat", fNum + 1);
		tt2 = curFilename.Left(Pos - 3);
		m_fname.Format("%s%s", tt2, tt1);
	}


	UpdateData(FALSE);


	return TRUE;
}
