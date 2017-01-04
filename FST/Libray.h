#pragma once
#include "afxwin.h"


// CLibray 对话框

class CLibray : public CDialog
{
	DECLARE_DYNAMIC(CLibray)

public:
	CLibray(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLibray();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBRAY_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_distance;
	CListBox m_lib_list;
	int m_no;
	int m_number;
	CString m_station;
	int m_type;

public:
	int			selNum;
	CString		filename;
	int			stationCount;
	char		stationName[512][12];
	double		stationDis[512];
	int			stationNum[512];
	int			type[512];

private:

	int			modified;
	int			stationCount1;
	char		stationName1[512][12];
	double		stationDis1[512];
	int			stationNum1[512];
	int			type1[512];
private:
	int SaveContents(CString fname);
	int ReadContents(CString fname);
	void BackupContents();
	void List2Vars();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUndoButton();
	afx_msg void OnBnClickedModifyButton();
	afx_msg void OnBnClickedInsertButton();
//	void OnSelchangeLibList();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedAppendButton();
	afx_msg void OnBnClickedSavediskButton();
	afx_msg void OnLbnSelchangeLibList();

	afx_msg void OnBnClickedSaveasButton();
//	afx_msg void OnEnChangeDistanceEdit();
	afx_msg void OnEnChangeDistanceEdit();
	BOOL OnInitDialog();
};
