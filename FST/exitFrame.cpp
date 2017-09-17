// exitFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "exitFrame.h"
#include "afxdialogex.h"


// exitFrame 对话框

IMPLEMENT_DYNAMIC(exitFrame, CDialog)

exitFrame::exitFrame(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG_EXIT, pParent)
{

}

exitFrame::~exitFrame()
{
}

void exitFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(exitFrame, CDialog)
	ON_BN_CLICKED(IDOK, &exitFrame::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &exitFrame::OnBnClickedCancel)
END_MESSAGE_MAP()


// exitFrame 消息处理程序


void exitFrame::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}


void exitFrame::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}
