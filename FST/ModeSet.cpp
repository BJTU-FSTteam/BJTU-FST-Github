// ModeSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FST.h"
#include "ModeSet.h"
#include "afxdialogex.h"


// CModeSet �Ի���

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


// CModeSet ��Ϣ�������


void CModeSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialog::OnOK();
}
