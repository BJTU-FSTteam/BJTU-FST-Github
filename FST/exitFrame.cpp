// exitFrame.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FST.h"
#include "exitFrame.h"
#include "afxdialogex.h"


// exitFrame �Ի���

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


// exitFrame ��Ϣ�������


void exitFrame::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}


void exitFrame::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnCancel();
}
