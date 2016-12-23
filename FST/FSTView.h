
// FSTView.h : CFSTView ��Ľӿ�
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

/*********************�˴����峣�������ע��********************/   //edit by zwbai 161221

typedef CList<CRect, CRect> CListRect;
const unsigned int nBUFSIZE = 128;     //64�����ݣ�ÿ��ռ���ֽ�
#define tagQueryEspi 5       //��ѯESPI��ǿֵ�Ķ�ʱ��ID
#define tagATCtimeCntDn 6    //�ط�ʱ�䵹��ʱ
#define tagAutoSave 7        //��ǿ�ļ��Զ����̶�ʱ��
#define MSG_COMDATARECV WM_USER+26
#define ShareBufferSize 10000000		//��ǿ���ջ�����size
									   /*********************���϶��峣���������ע��********************/


class CFSTView : public CFormView
{
protected: // �������л�����
	CFSTView();
	DECLARE_DYNCREATE(CFSTView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_FST_FORM };
#endif

// ����
public:
	CFSTDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CFSTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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
	/******************��Ҫ���ڳ�ǿ�����߳��Լ������̴߳�������ʹ��***********************///add by bzw 161220 start
	struct RECVPARAM //�������̴߳��ݲ���
	{
		SOCKET sock;
		HWND hwnd;
		CFSTView* m_pView;
	};
	BOOL InitPR100flag; // FALSE:û�г�ʼ����ǿ��  TRUE: ��ǿ����ɳ�ʼ��
	bool stopPR100;   //trueֹͣ false��ֹͣ 
	CString Pr100freq;
	unsigned char nLevel[10000];
	int nCountLevel;
	SOCKET  nSocketUdp;
	SOCKET  nSocketTcp;
	bool WorkAreaFlag;   //FALSE �ɶ�,TRUE ��д
	bool SampleAreaFlag;  //FALSE �ɶ�,TRUE ��д
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


#ifndef _DEBUG  // FSTView.cpp �еĵ��԰汾
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

