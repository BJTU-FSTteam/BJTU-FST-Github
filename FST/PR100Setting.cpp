// PR100Setting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FST.h"
#include "PR100Setting.h"
#include "afxdialogex.h"


// PR100Setting �Ի���

IMPLEMENT_DYNAMIC(PR100Setting, CDialogEx)

PR100Setting::PR100Setting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PR100_DIALOG, pParent)
{

}

PR100Setting::~PR100Setting()
{
}

void PR100Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(PR100Setting, CDialogEx)
	ON_EN_CHANGE(IDC_LU_EDIT2, &PR100Setting::OnEnChangeLuEdit2)
END_MESSAGE_MAP()


// PR100Setting ��Ϣ�������


void PR100Setting::OnEnChangeLuEdit2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
