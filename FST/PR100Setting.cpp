// PR100Setting.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "PR100Setting.h"
#include "afxdialogex.h"


// PR100Setting 对话框

IMPLEMENT_DYNAMIC(PR100Setting, CDialogEx)

PR100Setting::PR100Setting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PR100_DIALOG, pParent)
	, m_AT(_T(" 0 dB"))
	, m_strFrequency(_T(" 15 kHz"))
	, m_FR(_T(" 467.925"))
	, m_LD(_T(" 1 ms"))
	, m_LU(_T(" dBuV"))
	, m_AD(_T(" FM"))
{

	curAT = 0;
}

PR100Setting::~PR100Setting()
{
}

void PR100Setting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AD_EDIT, m_AD);
	DDX_Control(pDX, IDC_AT_COMBO, m_ctrlAT);
	DDX_CBString(pDX, IDC_AT_COMBO, m_AT);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_cbFrequency);
	DDX_CBString(pDX, IDC_COMBO_Frequency, m_strFrequency);
	DDX_CBString(pDX, IDC_FR_COMBO, m_FR);
	DDX_Text(pDX, IDC_LD_EDIT, m_LD);
	DDX_Text(pDX, IDC_LU_EDIT, m_LU);
}


BEGIN_MESSAGE_MAP(PR100Setting, CDialogEx)
	ON_EN_CHANGE(IDC_LU_EDIT, &PR100Setting::OnEnChangeLuEdit)
	ON_CBN_SELCHANGE(IDC_FR_COMBO, &PR100Setting::OnCbnSelchangeFrCombo)
	ON_CBN_SELCHANGE(IDC_AT_COMBO, &PR100Setting::OnCbnSelchangeAtCombo)
	ON_EN_CHANGE(IDC_AD_EDIT, &PR100Setting::OnEnChangeAdEdit)
	ON_EN_CHANGE(IDC_LD_EDIT, &PR100Setting::OnEnChangeLdEdit)
	ON_BN_CLICKED(IDOK, &PR100Setting::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &PR100Setting::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_Frequency, &PR100Setting::OnCbnSelchangeAtCombo)
END_MESSAGE_MAP()


// PR100Setting 消息处理程序




void PR100Setting::OnEnChangeLuEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void PR100Setting::OnCbnSelchangeFrCombo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void PR100Setting::OnCbnSelchangeAtCombo()
{
	// TODO: 在此添加控件通知处理程序代码
	curAT = m_ctrlAT.GetCurSel() + 1;

}


void PR100Setting::OnEnChangeAdEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void PR100Setting::OnEnChangeLdEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void PR100Setting::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void PR100Setting::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}




BOOL PR100Setting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString dirName;

	dirName = _T("C:\\场强测试\\");

	CString strTemp, strTempA, strDisp;
	//	TCHAR szPath[MAX_PATH] = {"c:\\场强测试\\*.TXT"} ; 

	::GetPrivateProfileString(_T("Receiver_PARAMETERS"), _T("Frequency"), m_FR,
		strTemp.GetBuffer(20), 20, dirName + "fsmconfig.ini");
	m_FR = strTemp;
	strDisp.Format(_T("%s"), strTemp);
	//	MessageBox(strDisp);

	m_cbFrequency.InsertString(-1, _T("  9 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 15 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 30 kHz"));
	m_cbFrequency.InsertString(-1, _T(" 150 kHz"));
	m_cbFrequency.SetCurSel(3);

	UpdateData(FALSE);

	curAT = m_ctrlAT.GetCurSel() + 1;

	m_ctrlAT.SetCurSel(0);

	return TRUE;
}



