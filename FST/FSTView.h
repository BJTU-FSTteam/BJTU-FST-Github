
// FSTView.h : CFSTView 类的接口
//

#pragma once

#include "PR100Setting.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <string>
#include <fstream>
using namespace std;
#include <afxtempl.h>
#include "afxwin.h"

/*********************此处定义常量并添加注释********************/   //edit by zwbai 161221

typedef CList<CRect, CRect> CListRect;
const unsigned int nBUFSIZE = 128;     //64组数据，每组占两字节
#define tagQueryEspi 5       //查询ESPI场强值的定时器ID
#define tagATCtimeCntDn 6    //控发时间倒计时
#define tagAutoSave 7        //场强文件自动存盘定时器
#define MSG_COMDATARECV WM_USER+26
#define ShareBufferSize 10000000		//场强接收缓存区size
									   /*********************以上定义常变量并添加注释********************/


class CFSTView : public CFormView
{
protected: // 仅从序列化创建
	CFSTView();
	DECLARE_DYNCREATE(CFSTView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FST_FORM };
#endif

// 特性
public:
	CFSTDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CFSTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStartButton();
	afx_msg void OnStopButton();
	afx_msg void OnUpdateButton();
	afx_msg void OnSavedataButton();
	afx_msg void OnPr100Setting();
	afx_msg void OnModeButton();
	BOOL InitPr100();
public:
	/******************主要用于场强接收线程以及与主线程传递数据使用***********************///add by bzw 161220 start
	struct RECVPARAM //用于向线程传递参数
	{
		SOCKET sock;
		HWND hwnd;
		CFSTView* m_pView;
	};
	BOOL InitPR100flag; // FALSE:没有初始化场强仪  TRUE: 场强仪完成初始化
	bool stopPR100;   //true停止 false不停止 
	CString Pr100freq;
	unsigned char nLevel[10000];
	int nCountLevel;
	SOCKET  nSocketUdp;
	SOCKET  nSocketTcp;
	bool WorkAreaFlag;   //FALSE 可读,TRUE 可写
	bool SampleAreaFlag;  //FALSE 可读,TRUE 可写
	int pWorkArea;
	int pSampleArea;
	unsigned char WorkArea[ShareBufferSize];
	unsigned char SampleArea[ShareBufferSize];
	UINT Antenna;
	static DWORD WINAPI RecvProc(LPVOID lpParameter);
	void ReadData(unsigned char inData[], unsigned char sharebuffer[], int sizeofBuf);
	BOOL starRecordLevel;
	unsigned char TaxData[6];
	/*****************************************///add by bzw 161220 end
private:
	int firstTime;
	bool bSecondTime;
	double PI;
	enum
	{
		CtrlShow = 0,
		CtrlHide,
		CtrlMove
	};
private:
	CWinThread*	myThread;
	int caiji_status;
public:
	bool StartTestFlag;
	int selftestStatus;
private:
	int currentSel;
	int firstMapped;
	int currentSavePos;
public:
	CString m_controlstatus;
	float m_dbbcvalue;
	CString m_diameter;
	CString m_distance;
	CString m_filename;
	int m_filenum;
	CString m_line;
	int m_lossdb;
	CString m_maintance;
	int m_modify;
	CString m_rate;
	int m_speed;
	CString m_updown;
	CListBox m_inquiry_list;
};


UINT	MyThreadProc(LPVOID pParam);
float	AD2dB(int AD_val);
float	AD2dB5(int AD_val);
void    DisplayOthers(CDC*, CString, CString);
void	sendCode(int addCode, int tmCode);
void	prepareCode(int comingStation, int speed0);


#ifndef _DEBUG  // FSTView.cpp 中的调试版本
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

