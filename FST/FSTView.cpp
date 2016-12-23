
// FSTView.cpp : CFSTView ���ʵ��
//
#pragma once
#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "FST.h"
#endif

#include "FSTDoc.h"
#include "FSTView.h"
#include "PR100Setting.h"
#include "MainFrm.h"
#include "math.h"
#include <iostream>

using namespace std;
extern CFSTApp theApp;
//extern CStatusBar  m_wndStatusBar;
//extern HICON  train;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CFSTView


// CFSTView.cpp
/*********************�˴�����ȫ�ֳ��������ע��********************/   //edit by zwbai 161221
#define nDrawRangeXMin 65
#define nDrawRangeXMax 965
#define nPix500M 30				//���������ÿ30�����ص��ʾ500��
#define nKMDisplayNum 29		//ÿ�λ�14km:((840Pix/60)*28)
const int nPix100m = 6;

/*********************���϶���ȫ�ֳ��������ע��********************/

/*********************�˴�����ȫ�ֱ��������ע��********************/   //edit by zwbai 161221
int		Pr100ProcFlag = 0;			//Pr100�����̹߳������Ʊ���  
float	db_value = 0.0;			//��ǿ����ֵ
double	unit;					//��̼�����ر���
int		pulse = 45;			//200/4=50	//360/4=90;	//80/2=40;          //pulses each circle
int     pulseDivision = 2;	//2;        //80 /2 =40
int		pulse100M = 100;
int		pulse95 = 5;
int		pulse90 = 10;
int		pulse50 = 50;
int		is4403 = 0;		//default: 0 = not using 4403
int     setStatus = 0;
int		sectionNum = 0;
int		AD_number = 0;
int		AD_num100 = 0;
int		currentStation;
int		nextStation;
int		delta;
float	AD_value1[30720];//InitScreen�а���
CString	currentName;
CString	nextName;
CString	currentCode;
CString	nextCode;
CString	startKM;
CString	nextKM;
int		offset;
int		readStatus = 0;
int		Pr100flag = 0;
double	startDis;
double	currentDis;


/*********************���϶���ȫ�ֱ��������ע��********************/



IMPLEMENT_DYNCREATE(CFSTView, CFormView)

BEGIN_MESSAGE_MAP(CFSTView, CFormView)
	ON_COMMAND(ID_START_BUTTON, &CFSTView::OnStartButton)
	ON_COMMAND(ID_STOP_BUTTON, &CFSTView::OnStopButton)
	ON_COMMAND(ID_UPDATE_BUTTON, &CFSTView::OnUpdateButton)
	ON_COMMAND(ID_SAVEDATA_BUTTON, &CFSTView::OnSavedataButton)
	ON_COMMAND(ID_PR100_SETTING, &CFSTView::OnPr100Setting)
	ON_COMMAND(ID_MODE_BUTTON, &CFSTView::OnModeButton)
END_MESSAGE_MAP()

// CFSTView ����/����

CFSTView::CFSTView()
	: CFormView(IDD_FST_FORM)
	, m_controlstatus(_T(""))
	, m_dbbcvalue(0)
	, m_diameter(_T(""))
	, m_distance(_T(""))
	, m_filename(_T(""))
	, m_filenum(0)
	, m_line(_T(""))
	, m_lossdb(0)
	, m_maintance(_T(""))
	, m_modify(0)
	, m_rate(_T(""))
	, m_speed(0)
	, m_updown(_T(""))
{
	// TODO: �ڴ˴���ӹ������

}

CFSTView::~CFSTView()
{
}

void CFSTView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CONTROLSTATUS, m_controlstatus);
	DDX_Text(pDX, IDC_EDIT_DBBC, m_dbbcvalue);
	DDX_Text(pDX, IDC_DIAMETER_EDIT, m_diameter);
	DDX_Text(pDX, IDC_DISTANCE_EDIT, m_distance);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_filename);
	DDX_Text(pDX, IDC_FILENUM_EDIT, m_filenum);
	DDX_Text(pDX, IDC_LINE_EDIT, m_line);
	DDX_Text(pDX, IDC_LOSSDB_EDIT, m_lossdb);
	DDX_CBString(pDX, IDC_MAINTANCE_COMBO, m_maintance);
	DDX_Text(pDX, IDC_MODIFY_EDIT, m_modify);
	DDX_CBString(pDX, IDC_RATE_COMBO, m_rate);
	DDX_Text(pDX, IDC_SPEED_EDIT, m_speed);
	DDX_CBString(pDX, IDC_UPDOWN_COMBO, m_updown);
	DDX_Control(pDX, IDC_INQUIRY_LIST, m_inquiry_list);
}

BOOL CFSTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CFSTView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CFSTView ���

#ifdef _DEBUG
void CFSTView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFSTView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFSTDoc* CFSTView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFSTDoc)));
	return (CFSTDoc*)m_pDocument;
}
#endif //_DEBUG


// CFSTView ��Ϣ�������


void CFSTView::OnStartButton()
{
}


void CFSTView::OnStopButton()
{
	// TODO: �ڴ���������������
}


void CFSTView::OnUpdateButton()
{
	// TODO: �ڴ���������������
}


void CFSTView::OnSavedataButton()
{
	// TODO: �ڴ���������������
}


void CFSTView::OnPr100Setting()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	PR100Setting m_PR100SetDlg;
	if (IDOK == m_PR100SetDlg.DoModal())
	{

		switch (m_PR100SetDlg.curAT)
		{
		case 0: Antenna = 0; break;
		case 1: Antenna = 3; break;
		case 2: Antenna = 10; break;
		case 3: Antenna = 20; break;
		case 4: Antenna = 30; break;
		case 5: Antenna = 40; break;
		case 6: Antenna = 50; break;
		default: break;

		}
		BeginWaitCursor();
		// 		m_wndStatusBar.SetPaneText(1, _T("���������豸���������Ե�...   "), TRUE);
		Pr100freq = m_PR100SetDlg.m_FR;
		if (TRUE == InitPr100())//PR100��ʼ��
		{
			InitPR100flag = TRUE;
			MessageBox(_T("     ���ó�ǿ�ǳɹ���"));
			wchar_t buffer[150];
			wprintf(buffer, "��ǿ�ǲ��� Band:15kHz LU:dBuV LD:5ms AD:FM AT:%ddB Frq:%sMHz", Antenna, Pr100freq);
			//��������״̬�����
			// 			CClientDC dc(this);
			// 			CSize sz = dc.GetTextExtent(buffer);
			// 			m_wndStatusBar.SetPaneInfo(1, 0, 0, sz.cx);
			// 			m_wndStatusBar.SetPaneText(1, buffer, TRUE);
		}
		EndWaitCursor();
	}
}


void CFSTView::OnModeButton()
{
	// TODO: �ڴ���������������
}


BOOL CFSTView::InitPr100()
{
	if (TRUE == InitPR100flag)
	{
		closesocket(nSocketUdp);
	}
	struct sockaddr_in addr;
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	int maxV = 2;
	int minV = 0;

	wVersionRequested = MAKEWORD(maxV, minV);
	//WSASYSNOTREADY ָ������ͨ��������������ϵͳ��û��׼����.
	//WSAVERNOTSUPPORTED �����Windows Sockets API�İ汾δ���ض���Windows Socketsʵ���ṩ.
	//WSAEINVAL Ӧ�ó���ָ����Windows Sockets�汾������DLL֧��
	err = WSAStartup(wVersionRequested, &wsaData);//Windows�׽����첽��������
	if (err != 0)
	{
		return FALSE;
	}
	//16��������ͣ����ұߣ��ֽڵ����� || 16��������ߣ�����ߣ��ֽڵ�����
	if (LOBYTE(wsaData.wVersion) != maxV || HIBYTE(wsaData.wVersion) != minV)
	{
		WSACleanup();
		return FALSE;
	}

	//Ϊ�˼��PC�����������Ƿ���ȷ����ȡ���ػ���IP��ַ
	CString strClient;
	char *ip;
	char name[155];
	PHOSTENT hostinfo;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)
	{
		if (gethostname(name, sizeof(name)) == 0)
		{
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
				strClient.Format(_T("%s"), ip);
			}
		}
		WSACleanup();
	}
	//�õ����ػ���IP��ַ ����
	if (strClient.Mid(0, 11) != _T("172.17.75.2") && strClient.Mid(0, 9) != _T("127.0.0.1"))//�ж�PC����IP�Ƿ�Ϊ172.17.75.2
	{
		MessageBox(_T("�뽫����IP����Ϊ172.17.75.2,����������Ϊ255.255.255.0,Ĭ����������Ϊ172.17.75.1"));
		return FALSE;
	}
	if (strClient.Mid(0, 9) == _T("127.0.0.1"))
	{
		MessageBox(_T("��򿪳�ǿ�ǣ�"));
		return FALSE;
	}
	else
	{
		nSocketTcp = socket(AF_INET, SOCK_STREAM, 0);
		if (nSocketTcp != -1)
		{
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_addr.s_addr = inet_addr("172.17.75.1");
			addr.sin_port = htons(5555);

			/*			err=connect(nSocketTcp,(struct sockaddr *)&addr,sizeof(addr));*/
			if (SOCKET_ERROR == connect(nSocketTcp, (struct sockaddr *)&addr, sizeof(addr)))
			{
				MessageBox(_T("���ӳ�ǿ��ʧ��"));
				return FALSE;
			}
			else
			{
				int i = 1;
				setsockopt(nSocketTcp, IPPROTO_TCP, TCP_NODELAY, (char*)&i, sizeof(i));
				//��λPR100
				send(nSocketTcp, "*RST\n", strlen("*RST\n"), 0);
				Sleep(100);
				//������
				send(nSocketTcp, "SYST:KLOCK ON\n", strlen("SYST:KLOCK ON\n"), 0);
				Sleep(50);
				//����ϵͳ����Ϊ0
				send(nSocketTcp, "SYST:AUD:VOL 0\n", strlen("SYST:AUD:VOL 0\n"), 0);
				Sleep(50);
				//����Ƶ��ģʽMSC
				send(nSocketTcp, "FREQ:MODE MSC\n", strlen("FREQ:MODE MSC\n"), 0);
				Sleep(50);
				//����MSC ������
				send(nSocketTcp, "MSC:COUN INF\n", strlen("MSC:COUN INF\n"), 0);
				Sleep(50);
				//ͬ��ʱ��
				send(nSocketTcp, "MSC:DWEL 0\n", strlen("MSC:DWEL 0\n"), 0);
				Sleep(50);
				//����ʱ��
				send(nSocketTcp, "MSC:HOLD:TIME 0\n", strlen("MSC:HOLD:TIME 0\n"), 0);
				Sleep(50);
				//ɨ��ģʽ
				send(nSocketTcp, "MSC:LIST STAR 0\n", strlen("MSC:LIST STAR 0\n"), 0);
				Sleep(50);
				send(nSocketTcp, "MSC:LIST STOP 0\n", strlen("MSC:LIST STOP 0\n"), 0);
				Sleep(50);
				//Clear memory lists
				send(nSocketTcp, "MEM:CLE 0\n", strlen("MEM:CLE 0\n"), 0);
				Sleep(50);
				char sendbuff[40];
				sprintf_s(sendbuff, "MEM:CONT 0,%sE6,0,FM,15kHz,6,0,0,0,0,1\n", Pr100freq);
				Sleep(50);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				sprintf_s(sendbuff, "MEM:LABEL 0,\"%s MHz, BW 15kHz\"\n", Pr100freq);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				sprintf_s(sendbuff, "FREQ %sE6\n", Pr100freq);
				//����Ƶ��
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				//���ô���15KHz
				send(nSocketTcp, "BAND 15kHz\n", strlen("BAND 15kHz\n"), 0);
				Sleep(50);
				//���ü�鷽ʽΪMax Peak
				send(nSocketTcp, "SENS:DET PEAK\n", strlen("SENS:DET PEAK\n"), 0);


				Sleep(50);
				//������Ƶ��ʾ��Χ100dbuv
				send(nSocketTcp, "DISP:IFP:LEV:RANG 100\n", strlen("DISP:IFP:LEV:RANG 100\n"), 0);
				Sleep(50);
				//������Ƶ��ʾ���ֵΪ90dbuv
				send(nSocketTcp, "DISP:IFP:LEV:REF 90\n", strlen("DISP:IFP:LEV:REF 90\n"), 0);
				Sleep(50);
				send(nSocketTcp, "DISP:WIND \"Spectrum\"\n", strlen("DISP:WIND \"Spectrum\"\n"), 0);

				Sleep(50);
				//���ò���ģʽΪ���ڲ���
				send(nSocketTcp, "MEAS:MODE PER\n", strlen("MEAS:MODE PER\n"), 0);
				Sleep(50);
				//���ò�������Ϊ0.5ms
				send(nSocketTcp, "MEAS:TIME 0.5 ms\n", strlen("MEAS:TIME 0.5 ms\n"), 0);
				Sleep(50);
				//ɾ�����е�TRAC UdpPath
				send(nSocketTcp, "TRAC:UDP:DEL ALL\n", strlen("TRAC:UDP:DEL ALL\n"), 0);
				Sleep(50);
				//����UdpPath,����ƽ����ֵ���͵�IP��ַΪ172.17.75.2��PC����19000�˿�
				send(nSocketTcp, "TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n", \
					strlen("TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n"), 0);
				Sleep(50);
				//
				send(nSocketTcp, "TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n", \
					strlen("TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n"), 0);
				Sleep(50);
				send(nSocketTcp, "INIT:CONM\n", strlen("INIT:CONM\n"), 0);
				closesocket(nSocketTcp);


				//UDP��ʼ��
				nSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);
				if (INVALID_SOCKET == nSocketUdp)
				{
					MessageBox(_T("�׽��ִ���ʧ�ܣ�"));
					return FALSE;
				}
				struct sockaddr_in addrSock;
				addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
				addrSock.sin_family = AF_INET;
				addrSock.sin_port = htons(19000); //�˶˿�������15��16�еĶ˿�19000����һ��
				int retval;
				retval = bind(nSocketUdp, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));
				if (SOCKET_ERROR == retval)
				{
					closesocket(nSocketUdp);
					MessageBox(_T("�󶨶˿�ʧ�ܣ�"));
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}

	return TRUE;
}
