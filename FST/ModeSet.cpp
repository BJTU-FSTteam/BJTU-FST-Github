// ModeSet.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "ModeSet.h"
#include "afxdialogex.h"


// CModeSet 对话框

IMPLEMENT_DYNAMIC(CModeSet, CDialog)

CModeSet::CModeSet(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MODESET, pParent)
	, m_nMode(_T(""))
{

}

CModeSet::~CModeSet()
{
}

void CModeSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_MODESET, m_nMode);
}


BEGIN_MESSAGE_MAP(CModeSet, CDialog)
	ON_BN_CLICKED(IDOK, &CModeSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CModeSet 消息处理程序


void CModeSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
