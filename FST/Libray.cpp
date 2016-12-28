// Libray.cpp : 实现文件
//

#include "stdafx.h"
#include "FST.h"
#include "Libray.h"
#include "afxdialogex.h"
#include <fstream>
using namespace std;
#include <string.h>
#include <stdio.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CLibray 对话框
extern CStatusBar  m_wndStatusBar;
IMPLEMENT_DYNAMIC(CLibray, CDialog)

CLibray::CLibray(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LIBRAY_DIALOG, pParent)
	, m_distance(0.0)
	, m_no(0)
	, m_number(0)
	, m_station(_T(""))
	, m_type(-1)
{
	for (int ii = 0; ii < 512; ii++)
	{
		strcpy(stationName[ii], "");
		stationDis[ii] = 0.0;
		stationNum[ii] = 0;
		type[ii] = 0;

		strcpy(stationName1[ii], "");
		stationDis1[ii] = 0.0;
		stationNum1[ii] = 0;
		type1[ii] = 0;
	}

	selNum = 0 - 1;
	filename = _T("");
}

CLibray::~CLibray()
{
}

void CLibray::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DISTANCE_EDIT, m_distance);
	DDX_Control(pDX, IDC_LIB_LIST, m_lib_list);
	DDX_Text(pDX, IDC_NO_EDIT, m_no);
	DDX_Text(pDX, IDC_NUMBER_EDIT, m_number);
	DDX_Text(pDX, IDC_STATION_EDIT, m_station);
	DDX_CBIndex(pDX, IDC_TYPE_COMBO, m_type);
}


BEGIN_MESSAGE_MAP(CLibray, CDialog)
	ON_BN_CLICKED(IDOK, &CLibray::OnBnClickedOk)
	ON_BN_CLICKED(IDC_UNDO_BUTTON, &CLibray::OnBnClickedUndoButton)
	ON_BN_CLICKED(IDC_MODIFY_BUTTON, &CLibray::OnBnClickedModifyButton)
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &CLibray::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CLibray::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_APPEND_BUTTON, &CLibray::OnBnClickedAppendButton)
	ON_BN_CLICKED(IDC_SAVEDISK_BUTTON, &CLibray::OnBnClickedSavediskButton)
	ON_LBN_SELCHANGE(IDC_LIB_LIST, &CLibray::OnLbnSelchangeLibList)
	ON_BN_CLICKED(IDC_SAVEAS_BUTTON, &CLibray::OnBnClickedSaveasButton)
	ON_EN_CHANGE(IDC_DISTANCE_EDIT, &CLibray::OnEnChangeDistanceEdit)
	ON_EN_CHANGE(IDC_DISTANCE_EDIT, &CLibray::OnEnChangeDistanceEdit)
END_MESSAGE_MAP()


// CLibray 消息处理程序


void CLibray::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (modified == 1)
	{
		if (MessageBox(_T("文件修改未存盘，退出吗？"), NULL, MB_ICONEXCLAMATION | MB_YESNO) == IDYES)
			CDialog::OnOK();
		else
			return;
	}
	CDialog::OnOK();
}


void CLibray::OnBnClickedUndoButton()
{
	// TODO: 在此添加控件通知处理程序代码
	int		i, selNo;
	char	pp[120];

	selNo = m_lib_list.GetCurSel();
	m_lib_list.ResetContent();
	sprintf(pp, " 序号\t车站\t里程标\t站编号\t类型");
	m_lib_list.AddString(pp);

	stationCount = stationCount1;
	for (i = 1; i < stationCount; i++)
	{
		strcpy(stationName[i], stationName1[i]);
		stationDis[i] = stationDis1[i];
		stationNum[i] = stationNum1[i];
		type[i] = type1[i];
		//		strcpy(Mode[i], Mode1[i]);
		//		Hei[i]=Hei1[i];
	}

	for (i = 1; i <= stationCount; i++)
	{
		sprintf(pp, " %03d\t%s\t%0.2f%\t%d\t%d",
			i, stationName[i], stationDis[i], stationNum[i], type[i]);


		m_lib_list.AddString(pp);
	}

	m_lib_list.SetCurSel(selNo);
}


void CLibray::OnBnClickedModifyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	char	tmp[120];
	int		num;

	if (UpdateData(TRUE) != 0)
	{
		num = m_lib_list.GetCurSel();
		if (num > 0)
		{
			modified = 1;
			BackupContents();
			sprintf(tmp, " %03d\t%s\t%0.2f%\t%d\t%d",
				m_no, m_station, m_distance, m_number, m_type);


			m_lib_list.DeleteString(m_no);
			m_lib_list.InsertString(m_no, (LPTSTR)tmp);
			m_lib_list.SetCurSel(m_no);
			List2Vars();

		}
	}
}


void CLibray::BackupContents()
{
	stationCount1 = stationCount;
	for (int i = 1; i < stationCount1; i++)
	{
		strcpy(stationName1[i], stationName[i]);
		stationDis1[i] = stationDis[i];
		stationNum1[i] = stationNum[i];
		type1[i] = type[i];
		//	strcpy(Mode1[i], Mode[i]);
		//	Hei1[i]=Hei[i];
	}
}


void CLibray::List2Vars()
{
	char	tmp[120], *token;
	int		item;

	stationCount = m_lib_list.GetCount() - 1;
	for (int i = 1; i <= stationCount; i++)
	{
		m_lib_list.GetText(i, (LPTSTR)tmp);

		item = 0;
		token = strtok(tmp, " \t");
		while (token != NULL)
		{
			switch (item)
			{
			case 1:
				strcpy(stationName[i], token);
				break;
			case 2:
				stationDis[i] = atof(token);
				break;
			case 3:
				stationNum[i] = atoi(token);
				break;
			case 4:
				type[i] = atoi(token);
				break;
				/*
				case 5:
				strcpy(Mode[i],token);
				break;
				case 6:
				Hei[i]=atof(token);
				break;
				*/
			}
			token = strtok(NULL, " \t");
			item++;
		}
	}
}


void CLibray::OnBnClickedInsertButton()
{
	// TODO: 在此添加控件通知处理程序代码
	char	tmp[120], substr[120], bak[120], *token;
	int		num, count;
	if (UpdateData(TRUE) != 0)
	{
		num = m_lib_list.GetCurSel();
		count = m_lib_list.GetCount();
		if (num > 0)
		{
			modified = 1;
			BackupContents();
			sprintf(tmp, " %03d\t%s\t%0.2f%\t%d\t%d",
				num, m_station, m_distance, m_number, m_type);

			m_lib_list.InsertString(num, tmp);
			m_lib_list.SetCurSel(num);
			OnLbnSelchangeLibList();

			stationCount++;
			count++;

			if (num != count)			//Adjust following No
				for (int i = num + 1; i < count; i++)
				{
					m_lib_list.GetText(i, (LPTSTR)tmp);

					strcpy(bak, tmp);
					token = strtok(tmp, " \t");
					token = strtok(NULL, " \t");
					strcpy(substr, strstr(bak, token));
					m_wndStatusBar.SetPaneText(0, substr, TRUE);

					sprintf(tmp, " %03d\t%s", i, substr);

					m_lib_list.DeleteString(i);
					m_lib_list.InsertString(i, (LPTSTR)tmp);
				}

			List2Vars();
		}
	}
}


//void CLibray::OnSelchangeLibList()
//{
//	char	tmp[120], *token;
//	int		num, item;
//
//	num = m_lib_list.GetCurSel();
//
//	if (num>0)
//	{
//		selNum = num;
//
//		m_lib_list.GetText(num, (LPTSTR)tmp);
//
//		item = 0;
//		token = strtok(tmp, " \t");
//		while (token != NULL)
//		{
//			switch (item)
//			{
//			case 0:
//				m_no = atoi(token);
//				break;
//			case 1:
//				m_station = CString(token);
//				break;
//			case 2:
//				m_distance = atof(token);
//				break;
//			case 3:
//				m_number = atoi(token);
//				break;
//			case 4:
//				m_type = atoi(token);
//				break;
//				/*
//				case 5:
//				m_mode=CString(token);
//				break;
//				case 6:
//				m_height=atof(token);
//				break;
//				*/
//			}
//			token = strtok(NULL, " \t");
//			item++;
//		}
//
//		UpdateData(FALSE);
//	}
//
//}


void CLibray::OnBnClickedDeleteButton()
{
	// TODO: 在此添加控件通知处理程序代码
	char	tmp[120], substr[120], bak[120], *token;
	int		num, count;

	num = m_lib_list.GetCurSel();
	count = m_lib_list.GetCount();
	if (num > 0)
	{
		modified = 1;
		BackupContents();

		m_lib_list.DeleteString(num);
		m_lib_list.SetCurSel(num - 1);
		OnLbnSelchangeLibList();

		stationCount--;
		count--;

		if (num != count)			//Adjust following No
			for (int i = num; i < count; i++)
			{
				m_lib_list.GetText(i, (LPTSTR)tmp);

				strcpy(bak, tmp);
				token = strtok(tmp, " \t");
				token = strtok(NULL, " \t");
				strcpy(substr, strstr(bak, token));
				m_wndStatusBar.SetPaneText(0, substr, TRUE);

				sprintf(tmp, " %03d\t%s", i, substr);

				m_lib_list.DeleteString(i);
				m_lib_list.InsertString(i, (LPTSTR)tmp);
			}
		List2Vars();
	}
}


void CLibray::OnBnClickedAppendButton()
{
	// TODO: 在此添加控件通知处理程序代码
	char	tmp[120];
	int	    count;
	if (UpdateData(TRUE) != 0)
	{

		count = m_lib_list.GetCount();
		if (count > 0)
		{
			modified = 1;
			BackupContents();

			sprintf(tmp, " %03d\t%s\t%0.2f%\t%d\t%d",
				count, m_station, m_distance, m_number, m_type);

			m_lib_list.InsertString(-1, tmp);
			m_lib_list.SetCurSel(count);

			stationCount++;
			OnLbnSelchangeLibList();
			List2Vars();
		}

	}
}


void CLibray::OnBnClickedSavediskButton()
{
	// TODO: 在此添加控件通知处理程序代码
	if (modified == 1)
		if (SaveContents(filename) > 0)
			modified = 0;

}


int CLibray::SaveContents(CString fname)
{
	int	kk = 1;
	FILE	*fp;


	fp = fopen(fname, "w+t");

	if (fp == NULL)
	{
		AfxMessageBox("Save File Failed");
		return 0;
	}

	fprintf(fp, "车站\t\t");
	fprintf(fp, "里程标\t\t");
	fprintf(fp, "站编号\t\t");
	fprintf(fp, "类型\n");
	for (kk = 1; kk <= stationCount; kk++)
	{
		fprintf(fp, "%s\t\t", stationName[kk]);
		fprintf(fp, "%6.3f\t\t", stationDis[kk]);
		fprintf(fp, "%d\t\t", stationNum[kk]);
		fprintf(fp, "%d\n", type[kk]);
		//		fprintf(fp, "%s\t", Mode[kk]);
		//		fprintf(fp, "%6.3f\t", Hei[kk]);
	}
	fclose(fp);

	return kk - 1;
}


int CLibray::ReadContents(CString fname)
{
	return 0-1;
}


void CLibray::OnLbnSelchangeLibList()
{
	// TODO: 在此添加控件通知处理程序代码
	char	tmp[120], *token;
	int		num, item;

	num = m_lib_list.GetCurSel();

	if (num>0)
	{
		selNum = num;

		m_lib_list.GetText(num, (LPTSTR)tmp);

		item = 0;
		token = strtok(tmp, " \t");
		while (token != NULL)
		{
			switch (item)
			{
			case 0:
				m_no = atoi(token);
				break;
			case 1:
				m_station = CString(token);
				break;
			case 2:
				m_distance = atof(token);
				break;
			case 3:
				m_number = atoi(token);
				break;
			case 4:
				m_type = atoi(token);
				break;
				/*
				case 5:
				m_mode=CString(token);
				break;
				case 6:
				m_height=atof(token);
				break;
				*/
			}
			token = strtok(NULL, " \t");
			item++;
		}

		UpdateData(FALSE);
	}
}


void CLibray::OnBnClickedSaveasButton()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlgFile(FALSE, "TXT", filename, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "(TXT) *.txt |*.txt||", this);
	if (dlgFile.DoModal() == IDOK)
	{
		CString	tmp_name = dlgFile.GetPathName();
		if (modified == 1)
			if (SaveContents(tmp_name) > 0)
			{
				modified = 0;
				filename = tmp_name;
			}
	}
}


//void CLibray::OnEnChangeDistanceEdit()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，它将不
//	// 发送此通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//}


void CLibray::OnEnChangeDistanceEdit()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


BOOL CLibray::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	int	i;
	char    	pp[120];

	m_lib_list.SetTabStops(40);

	m_lib_list.ResetContent();

	sprintf(pp, " 序号\t车站\t里程标\t站编号\t类型");
	m_lib_list.AddString(pp);


	modified = 0;

	if (stationCount > 0)
	{
		m_no = 1;
		m_station = stationName[1];
		m_distance = stationDis[1];
		m_number = stationNum[1];
		m_type = type[1];

		UpdateData(FALSE);

		//		strcat(stationName[1],"      ");

		for (i = 1; i <= stationCount; i++)
		{
			sprintf(pp, " %03d\t%s\t%0.2f%\t%d\t%d",
				i, stationName[i], stationDis[i], stationNum[i], type[i]);

			m_lib_list.AddString(pp);
		}

		m_lib_list.SetCurSel(1);
		selNum = 1;
	}
	else if (stationCount == 0)
	{
		m_no = 1;
		m_station = _T("");
		m_distance = 0.0;
		m_number = 1;
		m_type = 0;

		UpdateData(FALSE);
	}


	return TRUE;
}
