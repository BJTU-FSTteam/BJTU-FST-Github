
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
#include "conio.h"
#include <stdio.h>
#include <iostream>
#include "ModeSet.h"
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
/* Zhou's algorithm*/
#define PI 3.14159265353846		//PI=3.14159265359;
//#define PI1 3.14159265353846		//PI=3.14159265359;
//WGS84 ��Բ������ת������
const double A1 = 6367449.1458;
const double A2 = -16038.508688;
const double A3 = 16.8326;
const double A4 = -0.0220;
const double a = 6378140;
const double f = 298.257;
//unsigned long 32bit
double pi = atan(1.0)*4.0;      //���ڼ����־���pi

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
int     Tax_number = 0;//��������tax���ݼ������
int		AD_num100 = 0;
int		currentStation;
int		nextStation;
int		delta;
float	AD_value1[30720];//InitScreen�а���
dataType	data1[30720];
int controlstatus = 0;	//0--local control, 1--remote control
bool m_FSTbLocked = true;
float dbvalue_global = 0.0;
int    which = 95;

struct fusiondataType
{
	int curpos;
	unsigned long pulsenum;
	double lat;
	double lon;
} fusiondata[30720];
CString gprmcdata[30720];
int gprmcCount;
int gprmcfileCount;
int GPSThreshold = 50;

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
double currentLat;
double currentLon;

unsigned long currentPulsenum;
CString	filename;
float	loss_voltage = 0.0;

//LiCheng
int			stationCount;
char		stationName[512][12];
double		stationDis[512];
int			stationNum[512];
//ZhongJi
int			zhongjiCount;
char		zhongjiName[512][12];
double		zhongjiDis[512];
int         zhongjiCode[512];
//DianTai 
int			diantaiCount;
char		diantaiName[512][12];
double		diantaiDis[512];
int         diantaiCode[512];
//SuiDao
int			suidaoCount;
char		suidaoName[512][12];
double		suidaoDis[512];
//add by zgliu 2011.04.13
//ZhiFangZhan
int         zhifangCount;
char        zhifangName[512][12];
double      zhifangDis[512];
int         zhifangCode[512];
//QuJianTai
int         qujiantaiCount;
char        qujiantaiName[512][12];
double      qujiantaiDis[512];
int         qujiantaiCode[512];

int nWorkMode;//add by yjh 161125
bool modeFlag = false;//add by yjh 161125
SOCKET nSocketUdpMs;//add by yjh 161125
char gpsData[100];  //add by yjh 161125
char taxData[100]; //add by yjh 161125
char odoData[100]; //add by yjh 161125
char kilomter[6]; // add by yjh
unsigned long preGonglibiao = 0; //add by yjh 161125 �����˹���꣬ͨ��ODO��TAX�õ������
unsigned long curGonglibiao = 0; //add by yjh 161125
unsigned long tempGonglibiao = 0;//add by yjh 161125
								 
unsigned int  global_odoSpeedData = 0;//����ȫ�ִ��͵�odo�ٶ�ֵ
unsigned long global_odoTotalData = 0;//����ȫ�ִ��͵�odo��������
unsigned int global_taxSpeedData = 0;//����ȫ�ִ��͵�taxspeed��Ϣ
unsigned int global_taxPosData = 0;//����ȫ�ִ��͵�taxposition��Ϣ


//������̵�GPS��������
int			gpscorCount;
char		gpsName[512][12];
double		gpsDis[512];
double		gpsLon[512];
double		gpsLat[512];
int			gpsmmflag[512];

int gpscor_flag;
int gpscapture_flag;
double gpscaptureDis;

int gpssampleCount = 0;
int gpscaptureCount = 0;
/* Zhou's algorithm*/
//#define PI 3.14159265353846		//PI=3.14159265359;

//unsigned long 32bit
//����ṹ��,ƽ������x,y
struct position
{
	double x;
	double y;
};
//����ת��
struct position BL2xy(double B, double L, double L0)
{
	double e2;
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	double b = B*PI / 180.0;
	double l = L*PI / 180.0;
	double l0 = L0*PI / 180.0;
	double X;
	X = A1 * b + A2 * sin(2 * b) + A3 * sin(4 * b) + A4 * sin(6 * b);
	//�����ߵĻ���

	double sinb = sin(b);
	double cosb = cos(b);
	double t = tan(b);//��������
	double t2 = t * t;
	double N = a / sqrt(1 - e2 * sinb * sinb);
	double m = cosb * (l - l0);
	double m2 = m * m;
	double ng2 = cosb * cosb * e2 / (1 - e2);//��������
	double x = X + N * t * ((0.5 + ((5 - t2 + 9 * ng2 + 4 * ng2 * ng2) / 24.0 + (61 - 58 * t2 + t2 * t2 + 270 * ng2 - 330 * t2*ng2) * m2 / 720.0) * m2) * m2);
	double  y = N * m * (1 + m2 * ((1 - t2 + ng2) / 6.0 + m2 * (5 - 18 * t2 + t2 * t2 + 14 * ng2 - 58 * ng2 * t2) / 120.0));
	y = y + 500000;
	struct position pp;
	pp.x = x;
	pp.y = y;
	return pp;
}
//����֮��ľ������
double Distance2(double B1, double L1, double B2, double L2)
{
	double L0, d;
	L0 = 0.5*(floor(L1) + floor(L2));
	struct position p1, p2;
	p1 = BL2xy(B1, L1, L0);
	p2 = BL2xy(B2, L2, L0);
	d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	return d;
}


float  AD2dB(int AD_val)          //AD_val is the A/D convertor value 0-4095
{

	float		dbValue = 0.0f;
	float		Voltage = 0.0f;
	//pr100��ǿֵת������³ľ��֣�by bzw 161116
	dbValue = (float)(AD_val*0.1 - 10.0);	  // change from 0--900 to -10dB--80dB
	dbValue = dbValue + loss_voltage + db_value;

	if (dbValue<0 - 10.0)
		dbValue = -10.0;
	else if (dbValue>90.0)
		dbValue = 90.0;


	return (dbValue);
}


float  AD2dB5(int AD_val)          //AD_val is the A/D convertor value 0-4095
{

	float		dbValue = 0.0f;
	float		Voltage = 0.0f;
	Voltage = (float)(AD_val / 819.0);//*5.0/4095.0);	  //  0-5V

	dbValue = (float)(Voltage*24.0 - 10.0); //120.0/5.0  //-10dB--110dB

	dbValue = dbValue + loss_voltage + db_value;

	return (dbValue);

}


void  DisplayOthers(CDC *pDC, CString KM1, CString KM2)
{
	int		i = 0;
	double		curKM = 0.0;
	double     sectionLen = atof(KM2) - atof(KM1);
	CString	strKM = _T("        ");
	CSize		sz;
	//��������У�ÿ30Pix��ʾ500M
	const int nPix1KM = nPix500M * 2;  //1KM�����ص�

	if (sectionLen > 0.0)
	{
		//Display ZhongJi Names and Distance
		pDC->SetTextColor(RGB(0x00, 0xC0, 0x60));
		for (i = 1; i <= zhongjiCount; i++)
		{
			curKM = (zhongjiDis[i] - atof(startKM));
			strKM.Format("(%.1f)", zhongjiDis[i]);
			sz = pDC->GetTextExtent("��");
			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (zhongjiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[�м�] >>"); //�� 
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [�м�]");
				}
			}
		}

		//Display diantai Names and Distance		 
		for (i = 1; i <= diantaiCount; i++)
		{
			curKM = (diantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", diantaiDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (diantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[ң��̨] >>"); //Y 
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [ң��̨]");
				}
			}
		}

		//Display suidao Names and Distance		 
		for (i = 1; i <= suidaoCount; i++)
		{
			curKM = (suidaoDis[i] - atof(KM1));
			strKM.Format("(%.1f)", suidaoDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 480 + 10, "��[���]");
			}
		}

		//Display ZhiFangZhan Names and Distance
		for (i = 1; i <= zhifangCount; ++i)
		{
			curKM = (zhifangDis[i] - atof(KM1));
			strKM.Format("(%.1f)", zhifangDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (zhifangCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[ֱ��վ] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [ֱ��վ]");
				}
			}
		}

		//Display QuJianTai Names and Distance
		for (i = 1; i <= qujiantaiCount; ++i)
		{
			curKM = (qujiantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", qujiantaiDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (qujiantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[����̨] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [����̨]");
				}
			}
		}
	}
	else
	{
		//Display ZhongJi Names and Distance
		sectionLen = fabs(sectionLen);

		pDC->SetTextColor(RGB(0x00, 0xC0, 0x60));
		for (i = 1; i <= zhongjiCount; i++)
		{
			curKM = 0.0 - (zhongjiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", zhongjiDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				if (zhongjiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[�м�] >>");   //��
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [�м�]");
				}
			}
		}

		//Display diantai Names and Distance		 
		for (i = 1; i <= diantaiCount; i++)
		{
			curKM = 0.0 - (diantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", diantaiDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				if (diantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[ң��̨] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [ң��̨]");
				}
			}
		}

		//Display suidao Names and Distance		 
		for (i = 1; i <= suidaoCount; i++)
		{
			curKM = 0.0 - (suidaoDis[i] - atof(KM1));
			strKM.Format("(%.1f)", suidaoDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 480 + 10, "��");
			}
		}

		//Display ZhiFangZhan Names and Distance
		for (i = 1; i <= zhifangCount; ++i)
		{
			curKM = 0.0 - (zhifangDis[i] - atof(KM1));
			strKM.Format("(%.1f)", zhifangDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (zhifangCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[ֱ��վ] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [ֱ��վ]");
				}
			}
		}

		//Display QuJianTai Names and Distance
		for (i = 1; i <= qujiantaiCount; ++i)
		{
			curKM = 0.0 - (qujiantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", qujiantaiDis[i]);
			sz = pDC->GetTextExtent("��");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "��");
				//���복վ�Ĺ�����ϵ
				if (qujiantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[����̨] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [����̨]");
				}
			}
		}
	}
}

/*********************���϶���ȫ�ֱ��������ע��********************/



IMPLEMENT_DYNCREATE(CFSTView, CFormView)

BEGIN_MESSAGE_MAP(CFSTView, CFormView)
	ON_COMMAND(ID_START_BUTTON, &CFSTView::OnStartButton)
	ON_COMMAND(ID_STOP_BUTTON, &CFSTView::OnStopButton)
	ON_COMMAND(ID_UPDATE_BUTTON, &CFSTView::OnUpdateButton)
	ON_COMMAND(ID_SAVEDATA_BUTTON, &CFSTView::OnSavedataButton)
	ON_COMMAND(ID_PR100_SETTING, &CFSTView::OnPr100Setting)
	ON_COMMAND(ID_MODE_BUTTON, &CFSTView::OnModeButton)
	ON_BN_CLICKED(IDC_STATIC_RANGE, &CFSTView::OnBnClickedStaticRange)
	ON_COMMAND(ID_FILE_OPEN, &CFSTView::OnFileOpen)
	ON_COMMAND(IDC_STATUS_MENU, &CFSTView::OnStatusMenu)
	ON_EN_CHANGE(IDC_DISTANCE_EDIT, &CFSTView::OnChangeDistanceEdit)
	ON_CBN_SELCHANGE(IDC_UPDOWN_COMBO, &CFSTView::OnSelchangeUpdownCombo)
	ON_CBN_SELCHANGE(IDC_MAINTANCE_COMBO, &CFSTView::OnSelchangeMaintanceCombo)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_START_BUTTON, &CFSTView::OnUpdateStartButton)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DIAMETER_SET, &CFSTView::OnDeltaposDiameterSet)
	ON_NOTIFY(UDN_DELTAPOS, IDC_MODIFY_SPIN, &CFSTView::OnDeltaposModifySpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_DBBC_SPIN, &CFSTView::OnDeltaposDbbcSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SAVEFILE_SPIN, &CFSTView::OnDeltaposSavefileSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPEED_SPIN, &CFSTView::OnDeltaposSpeedSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_LOSSDB_SPIN, &CFSTView::OnDeltaposLossdbSpin)
END_MESSAGE_MAP()

// CFSTView ����/����

CFSTView::CFSTView()
	: CFormView(IDD_FST_FORM)
	, m_controlstatus(_T("����"))
	, m_dbbcvalue(0.0)
	, m_diameter(_T("915.0"))
	, m_distance(_T("0.0"))
	, m_filename(_T(""))
	, m_filenum(0)
	, m_line(_T(""))
	, m_lossdb(0)
	, m_maintance(_T(" 10"))
	, m_modify(0)
	, m_rate(_T(" 95"))
	, m_speed(110)
	, m_updown(_T(" ����"))
	, m_gpscapture(_T(""))
	, m_gpssample(_T(""))
	, m_gpsstr(_T(""))
{
	// TODO: �ڴ˴���ӹ������
	controlstatus = 0;	//

	selftestStatus = 0;
	caiji_status = 0;
	firstTime = 1;
	currentName = _T("");
	nextName = _T("");
	offset = 0;
	startDis = 0.0;
	currentDis = 0.0;
	currentStation = 0;
	nextStation = 1;
	delta = 1;

	currentLat = 0.0;
	currentLon = 0.0;

	currentSel = 0 - 1;

	currentSavePos = 0;

	firstMapped = 0;

	dirName = _T("C:\\��ǿ����\\");

	CString strTemp, strTempA, strDisp;
	TCHAR szPath[MAX_PATH] = { "c:\\��ǿ����\\*.TXT" };

	//GPSThreshold = ::GetPrivateProfileInt("GPS_PARAMETERS", "GPSThreshold", 50, dirName + "fsmconfig.ini");
	//strTempA.Format("%d", GPSThreshold);

	::GetPrivateProfileString("TEST_PARAMETERS", "Diameter", m_diameter,
		strTemp.GetBuffer(20), 20, dirName + "fsmconfig.ini");
	m_diameter = strTemp;

	//::GetPrivateProfileString("Receiver_PARAMETERS", "Frequency", m_frequency,
	//	strTemp.GetBuffer(20), 20, dirName + "fsmconfig.ini");
	//m_frequency = strTemp;

	m_lossdb = ::GetPrivateProfileInt("TEST_PARAMETERS", "LossdB", 1, dirName + "fsmconfig.ini");
	strTempA.Format("%d", m_lossdb);
	loss_voltage = (float)m_lossdb;

	pulse = ::GetPrivateProfileInt("TEST_PARAMETERS", "Pulse", pulse, dirName + "fsmconfig.ini");
	strTempA.Format("%d", pulse);

	pulseDivision = ::GetPrivateProfileInt("TEST_PARAMETERS", "PulseDivision", pulseDivision, dirName + "fsmconfig.ini");
	strTempA.Format("%d", pulseDivision);

	// 	::GetPrivateProfileString("TEST_PARAMETERS", "Maintance", m_maintance, 
	// 		strTemp.GetBuffer(20), 20, dirName+"fsmconfig.ini");
	// 	m_maintance = strTemp;
	// 	m_maintance =_T(" ")+m_maintance;

	//strDisp.Format("%d %s %s %d %d %d %s", GPSThreshold, m_diameter, m_frequency,
	//	m_lossdb, pulse, pulseDivision, m_maintance);
	//	MessageBox(strDisp);
	/*
	::GetPrivateProfileString("TEST_LINE", "Libdir", _T("c:\\��̿�\\*.dbf"),
	szPath, MAX_PATH,".\\fsmconfig.ini");
	m_list.InsertString(-1, szPath);
	*/

	//	char pp[50];
	//	sprintf(pp,"%d %d %lf",pulse100M, pulse95, unit);
	//	MessageBox(pp);

	unit = atof(m_diameter)*pi*0.001 / pulse;		//meter
	pulse100M = (int)(100.0 / unit + 0.5);
	pulse95 = (int)(5.0 / unit + 0.5);
	pulse90 = (int)(10.0 / unit + 0.5);
	pulse50 = (int)(50.0 / unit + 0.5);

	/*****************************************/
	//add by bzw  161110 start
	InitPR100flag = FALSE;
	stopPR100 = true;

	//add by bzw  161110 end
	/*****************************************/

	// add by zgliu 2011.03.03
	bSecondTime = false;
	m_bReadLib = false;
	m_bAtcConfig = false;
	m_fCurSpeed = 0.0;
	m_strADdataSavePath = _T("");
	m_strAutoSavePath = _T("");
	m_curAutoSavePos = 0;
	char chDirBuf[MAX_PATH];
	memset(chDirBuf, 0, sizeof(chDirBuf));
	::GetCurrentDirectory(MAX_PATH, chDirBuf);
	m_strCurDirectory = chDirBuf;
	// add end by zgliu 
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
//	DDX_Text(pDX, IDC_GPSCAPTURE, m_gpscapture);
//	DDX_Text(pDX, IDC_GPSSAMPLE, m_gpssample);
//  DDX_Text(pDX, IDC_GPSSTR, gpsstr);
//  DDX_Text(pDX, IDC_GPSSTR, m_gpsstr);
//	DDX_Text(pDX, IDC_GPSSTR, m_gpsstr);
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
	theApp.pView = this;

	CRect rectWnd;
	GetClientRect(&rectWnd);
	listRect.AddTail(&rectWnd);
	CWnd *pWndChild = GetWindow(GW_CHILD);
	while (pWndChild)
	{
		pWndChild->GetWindowRect(&rectWnd);
		ScreenToClient(&rectWnd);
		listRect.AddTail(&rectWnd);
		pWndChild = pWndChild->GetNextWindow();
	}

	GetDlgItem(IDC_EDIT_GridDis)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CurMileage)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CurDBValue)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CurSpeed)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_FR)->EnableWindow(FALSE);
	SetDlgItemText(IDC_EDIT_GridDis, _T("0.5"));  //ÿ��0.5����
												  //	GetDlgItem(IDC_BTN_TrainPass)->EnableWindow(FALSE);
												  //	GetDlgItem(IDC_BTN_TrainStop)->EnableWindow(FALSE);

	Invalidate(TRUE);
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
	// TODO: �ڴ���������������

	// TODO: �ڴ���������������
	// TODO: Add your command handler code here
	if (caiji_status == 1)
		return;
	char pp[200];
	int retv;
	char tmps[5];
	int ii;

	if (!is4403)
		setStatus = 1;
	StartTestFlag = true;

	Pr100flag = 1;
	Pr100ProcFlag = 1;
	readStatus = 1;
	stopPR100 = false;

	RECVPARAM *pRecvParam = new RECVPARAM;
	pRecvParam->sock = nSocketUdp;
	pRecvParam->hwnd = m_hWnd;
	pRecvParam->m_pView = this;
	HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);
	//add by yjh 161115 start
	RECVPARAM *pRecvParam_MS = new RECVPARAM;
	pRecvParam_MS->sock = nSocketUdpMs;
	pRecvParam_MS->hwnd = m_hWnd;
	pRecvParam_MS->m_pView = this;
	HANDLE hThread_MS = CreateThread(NULL, 0, RecvProc_MS, (LPVOID)pRecvParam_MS, 0, NULL);
	//����һ���µ��̣߳�����UDp����ODO GPS TAX �������
	//add by yjh  161115 end
	if (!InitPR100flag)
	{
		MessageBox(_T("      �����ó�ǿ�ǣ�"));				//pr100ģ����������⣬�˴���ע�� Edit by zwbai 161228
		return;
	}
	m_line.TrimLeft();
	m_line.TrimRight();
    if (m_line.GetLength()<2)
	 {
	 		MessageBox(_T("�����������·!"), NULL, MB_ICONWARNING);
	 		return;
	 }
	currentSel = 0 - 1;
	currentSavePos = 0;
	firstMapped = 0;
	OnSelchangeUpdownCombo();
	InitScreen();
	caiji_status = 1;


	GetDlgItem(IDC_LINE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DISTANCE_EDIT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_DIAMETER_EDIT)->EnableWindow(FALSE);  //delete by zgliu Ӧ�ϲ���Ҫ�󣬲��Թ����п�����Ҫ�޸�
	GetDlgItem(IDC_FILENAME_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_UPDOWN_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_MAINTANCE_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RATE_COMBO)->EnableWindow(FALSE);
	//add by liuang 2011.03.11
	GetDlgItem(IDC_FILENUM_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SAVEFILE_SPIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOSSDB_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_LOSSDB_SPIN)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT20)->EnableWindow(FALSE);
	GetDlgItem(IDC_MODIFY_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_DBBC)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPEED_EDIT)->EnableWindow((FALSE));
	GetDlgItem(IDC_DIAMETER_EDIT)->EnableWindow(FALSE);//add by yjh
													   //add end 2011.03.11

	//GetDlgItem(IDC_EDIT_RealSpeed)->EnableWindow((FALSE));//���ʵʱ�ٶ�
	SetDlgItemText(IDC_EDIT_FR, Pr100freq);   //�����ǰ����Ƶ�� add by bzw 161121

											  // add by zgliu 2011.03.13
	GetDlgItem(IDC_CONTROLSTATUS)->EnableWindow(FALSE);
	AD_number = 0;
	for (int i = 0; i<30720; i++)
	{
		AD_value1[i] = 0.0f;
	}

	int m_nTimer;
	m_nTimer = SetTimer(1, 1000, NULL);
	ASSERT(m_nTimer != 0);
	SetTimer(tagAutoSave, 30 * 60 * 1000, NULL);
	//��ǿ�����Զ�����ʱ���� 30 mins
	/*
	m_strADdataSavePath = m_strCurDirectory + _T("\\");
	CString strTemp = m_line + _T("��ǿ��������");
	m_strADdataSavePath += strTemp;
	CreateDirectory(m_strADdataSavePath.c_str(), NULL);	  //�����ļ����Ա��泡ǿ�������ݣ�����ʽ�����⣬ɾ����
	strTemp = m_strCurDirectory.c_str();
	strTemp += _T("\\GPSDataFiles");
	CreateDirectory(strTemp, NULL);
	*/
	//delete by bzw 161129	 
	myThread = AfxBeginThread(MyThreadProc, NULL, THREAD_PRIORITY_HIGHEST);
	myThread->m_bAutoDelete = TRUE;
}


void CFSTView::OnStopButton()
{
	// TODO: �ڴ���������������
	if (caiji_status == 0)
		return;
	Pr100ProcFlag = 0;         //stop Pr100Proc Edit by bzw 161129
	KillTimer(1);
	KillTimer(tagAutoSave);		//ֹͣ�Զ�����timer add by bzw 161124
	caiji_status = 0;
	readStatus = 0;
	Pr100flag = 0;
	/*****************************************/
	//add by bzw  161110 start
	stopPR100 = true;
	//add by bzw  161110 end
	/*****************************************/
	modeFlag = false;//add by yjh 161125ֹͣ���Ե�ʱ��modeFlag�ü�

	global_odoTotalData = 0;
	global_odoSpeedData = 0;


	Sleep(100);
	TerminateThread(myThread->m_hThread, 0);
	GetDlgItem(IDC_LINE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DISTANCE_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_DIAMETER_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_FILENAME_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_UPDOWN_COMBO)->EnableWindow(TRUE);

	GetDlgItem(IDC_MAINTANCE_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DIAMETER_EDIT)->EnableWindow(TRUE);//add by yjh
	GetDlgItem(IDC_RATE_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_LOSSDB_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_MODIFY_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPEED_EDIT)->EnableWindow((FALSE));
	OnSavedataButton();   //save data

	CPen	myPen1(PS_SOLID, 1, RGB(0x00, 0x80, 0xFF));
	CPen	myPen3(PS_SOLID, 1, RGB(0xFF, 0xFF, 0xFF));
	CPen	*pOldPen;
	CClientDC   pDC(this);
	pOldPen = pDC.SelectObject(&myPen3);
	if (offset > 0)
	{
		pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
		pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
		pDC.MoveTo(nDrawRangeXMin + offset, 50);
		pDC.LineTo(nDrawRangeXMin + offset, 450);
	}
	pDC.SelectObject(pOldPen);

	pOldPen = pDC.SelectObject(&myPen1);
	pDC.SelectStockObject(NULL_BRUSH);
	pDC.Rectangle(nDrawRangeXMin, 48, nDrawRangeXMax, 451);
	pDC.SelectObject(pOldPen);

	char pp[20];
	sprintf(pp, "%5.1f", currentDis / 1000.0);
	m_distance = _T(pp);
	UpdateData(FALSE);
}


void CFSTView::OnUpdateButton()
{
	// TODO: Add your command handler code here
	if (firstTime == 1)
	{
		MessageBox("û�в�������!", NULL, MB_ICONERROR);
		return;
	}

	if ((offset > 0) && (readStatus == 1))
	{
		CClientDC   pDC(this);
		CPen	myPen0(PS_DOT, 1, RGB(0xD0, 0xD0, 0xD0));
		CPen	myPen11(PS_SOLID, 2, RGB(0x00, 0x00, 0xC0));
		CPen	myPen22(PS_SOLID, 1, RGB(0xC0, 0x00, 0xC0));
		CPen	myPen2(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));

		pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 400, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);
		pDC.SelectObject(&myPen0);
		for (int i = 0; i < 10; i++)
		{
			pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
			pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
		}
		for (int i = 1; i <= nKMDisplayNum; i++)
		{
			pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
			pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
		}

		pDC.SelectObject(&myPen2);
		int	maintance = atoi(m_maintance);
		pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
		pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);


		int	jj = AD_num100 - sectionNum;

		pDC.SelectObject(&myPen11);
		pDC.MoveTo(nDrawRangeXMin, 410 - (int)AD_value1[jj] * 4);
		for (int ii = 1; ii <= sectionNum; ii++)
			pDC.LineTo(nDrawRangeXMin + (ii*(nPix500M / 5)), 410 - (int)AD_value1[jj + ii - 1] * 4);

		//////////////////////////////////////////////////////	


		CFont	myFont, *pOldFont;
		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myFont);

		pDC.FillSolidRect(nDrawRangeXMin - 20, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

		pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
		pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);
		if (_T("") != currentName)
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
		}
		else
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
		}
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));

		DisplayOthers(&pDC, startKM, nextKM);
		SetDlgItemText(IDC_EDIT_FR, Pr100freq);   //�����ǰ����Ƶ�� add by bzw 161121

												  // add by zgliu 2011.04.13 
												  // ÿ1KM��ʾһ���̶�ֵ
		CFont myKMFont;
		myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myKMFont);
		const int nPix1KM = nPix500M * 2;
		const int nDeltaKM = offset / nPix1KM;
		CString strTempKM;
		for (int i = 1; i <= nDeltaKM; ++i)
		{
			if (i <= (nKMDisplayNum + 1))
			{
				if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
				}
				else if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
				}
				pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
				pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
			}
		}
		pDC.SelectObject(pOldFont);
		// add end by zgliu 


		pDC.SelectObject(&myPen22);
		pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
		pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
		pDC.MoveTo(nDrawRangeXMin + offset, 50);
		pDC.LineTo(nDrawRangeXMin + offset, 450);

		pDC.SelectObject(pOldFont);
	}
}


void CFSTView::OnSavedataButton()
{
	// TODO: �ڴ���������������
	char	pp[40];
	FILE	*fp1, *fp2;
	int	currentNumber;
	int strLen;
	CString tmp, fusionfilename;
	int kk1;

	//write to gprmc files
	FILE	*fp4;

	strLen = m_filename.GetLength();
	tmp = m_filename.Mid(0, strLen - 4);
	CString gprmcfilename;

	gprmcfilename.Format("%s-%03d.TXT", tmp, gprmcfileCount);	//дGps��Ϣ������.TXT�ļ��� Edit by zwbai 20170720

	fp4 = fopen(gprmcfilename, "a+t");
	if (fp4 != NULL)
	{
		for (int iii = 0; iii < gprmcCount; iii++)
			fprintf(fp4, "%s", gprmcdata[iii]);
		fclose(fp4);
	}
	gprmcCount = 0;

	
	currentNumber = AD_num100;

	if (currentNumber <= currentSavePos)
	{
		CString error_message;
		error_message.Format("û�в������ݿɴ�! ");
		MessageBox(error_message);
		return;
	}

	fp1 = fopen(m_filename, "w+b");
	if (fp1 == NULL)
	{
		MessageBox("�ļ���ʧ��!", NULL, MB_ICONERROR);
		return;
	}


	strLen = m_filename.GetLength();
	tmp = m_filename.Mid(0, strLen - 4);
	fusionfilename = tmp + ".fus";

	fp2 = fopen(fusionfilename, "w+b");
	if (fp1 == NULL)
	{
		MessageBox("�ļ���ʧ��!", NULL, MB_ICONERROR);
	}
	else
	{
		kk1 = fwrite(&fusiondata[currentSavePos], sizeof(fusiondataType), currentNumber - currentSavePos, fp2);
		//дodoλ����gps��γ����Ϣ������.fus�ļ��� Edit by zwbai 20170720
		fclose(fp2);
	}


	kk1 = fwrite(&data1[currentSavePos], sizeof(dataType), currentNumber - currentSavePos, fp1);
	//д��ǿ��λ����Ϣ������.dat�ļ��� Edit by zwbai 20170720
	fclose(fp1);


	if (kk1 != currentNumber - currentSavePos)
	{
		sprintf(pp, "�ļ�����ʧ��(%d  %d)", kk1, currentNumber - currentSavePos);
		MessageBox(pp, NULL, MB_ICONERROR);
	}
	else
	{
		strLen = m_filename.GetLength();
		tmp = m_filename.Mid(0, strLen - 7);
		m_filenum = m_filenum + 1;                  //add one
		sprintf(pp, "%03d", m_filenum);
		m_filename = tmp + pp + _T(".dat");
		UpdateData(FALSE);

		currentSavePos = currentNumber;
	}

	//////////////////////////////////////////////  	  

// 	gprmcfileCount = 0;

	return;
}


void CFSTView::OnPr100Setting()
{
	// TODO: �ڴ���������������
	// TODO: �ڴ���������������
	if (caiji_status == TRUE)//add by yjh 
	{
		AfxMessageBox("���������в���������PR100!!");
		return;
	}
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
	CModeSet CModeSeting;
	//	0:ODO
	//	1:TAX
	//	2:GPS+ODO
	//	3:GPS+TAX
	//	4:GPS+ODO+TAX


	if (IDOK == CModeSeting.DoModal())
	{
		if (CModeSeting.m_nMode == "0:ODO")
			nWorkMode = 0;
		if (CModeSeting.m_nMode == "1:TAX")
			nWorkMode = 1;
		if (CModeSeting.m_nMode == "2:GPS+ODO")
			nWorkMode = 2;
		if (CModeSeting.m_nMode == "3:GPS+TAX")
			nWorkMode = 3;
		if (CModeSeting.m_nMode == "4:GPS+ODO+TAX")
		{
			nWorkMode = 4;
		}
		if (nWorkMode == 0 || nWorkMode == 1 || nWorkMode == 2 || nWorkMode == 3 || nWorkMode == 4)
		{

			AfxMessageBox("ģʽ���óɹ�");
			//memset(odoDataTosend, 0, sizeof(odoDataTosend));//��odo������г�ʼ��
			WORD wVersionRequested;
			WSADATA wsaDataMs;
			int err;
			int maxV = 2;
			int minV = 0;

			wVersionRequested = MAKEWORD(maxV, minV);
			//WSASYSNOTREADY ָ������ͨ��������������ϵͳ��û��׼����.
			//WSAVERNOTSUPPORTED �����Windows Sockets API�İ汾δ���ض���Windows Socketsʵ���ṩ.
			//WSAEINVAL Ӧ�ó���ָ����Windows Sockets�汾������DLL֧��
			err = WSAStartup(wVersionRequested, &wsaDataMs);//Windows�׽����첽��������
			if (err != 0)
			{
				MessageBox("�׽��ִ���ʧ�ܣ�");
				return;
			}
			//16��������ͣ����ұߣ��ֽڵ����� || 16��������ߣ�����ߣ��ֽڵ�����
			if (LOBYTE(wsaDataMs.wVersion) != maxV || HIBYTE(wsaDataMs.wVersion) != minV)
			{
				WSACleanup();
				MessageBox("�׽��ִ���ʧ�ܣ�");
				return;
			}
			nSocketUdpMs = socket(AF_INET, SOCK_DGRAM, 0);
			if (INVALID_SOCKET == nSocketUdpMs)
			{
				int tempASocket = WSAGetLastError();
				if (tempASocket == WSANOTINITIALISED)
				{
				}
				MessageBox("�׽��ִ���ʧ�ܣ�");
				return;
			}
			struct sockaddr_in addrSockMs;
			addrSockMs.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
			addrSockMs.sin_family = AF_INET;
			addrSockMs.sin_port = htons(8234); //��ʱ��ʵ��ѡ��Ķ˿ں��Ƕ��٣����޸�����˿ں�
			int retval;
			retval = bind(nSocketUdpMs, (SOCKADDR*)&addrSockMs, sizeof(SOCKADDR));
			if (SOCKET_ERROR == retval)
			{
				int i = WSAGetLastError();
				if (i == WSANOTINITIALISED)
				{
				}
				closesocket(nSocketUdpMs);
				MessageBox("��λ����Ϣ�˿�ʧ�ܣ�");
				return;
			}

		}
		else
		{
			AfxMessageBox("���������������ʧ��");
			return;
		}
	}
	modeFlag = true;

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
				strClient.Format("%s", ip);
			}
		}
		WSACleanup();
	}
	//�õ����ػ���IP��ַ ����
	//if (strClient.Mid(0, 11) != "172.17.75.2"&&strClient.Mid(0, 9) != "127.0.0.1")//�ж�PC����IP�Ƿ�Ϊ172.17.75.2
	/*if (strClient.Mid(0, 11) != "192.168.0.201"&&strClient.Mid(0, 9) != "127.0.0.1")//�ж�PC����IP�Ƿ�Ϊ172.17.75.2

	{
		MessageBox("�뽫����IP����Ϊ192.168.0.201,����������Ϊ255.255.255.0,Ĭ����������Ϊ192.168.0.1");
		return FALSE;
	}
	*/
	if (strClient.Mid(0, 9) == "127.0.0.1")
	{
		MessageBox("��򿪳�ǿ�ǣ�");
		return FALSE;
	}
	else
	{
		nSocketTcp = socket(AF_INET, SOCK_STREAM, 0);
		if (nSocketTcp != -1)
		{
			memset(&addr, 0, sizeof(addr));
			addr.sin_family = AF_INET;
			//addr.sin_addr.s_addr = inet_addr("192.168.0.5");//����PR100
			addr.sin_addr.s_addr = inet_addr("127.0.0.1");//�����ػ���ַ
			addr.sin_port = htons(5555);
		
			/*			err=connect(nSocketTcp,(struct sockaddr *)&addr,sizeof(addr));*/
			if (SOCKET_ERROR == connect(nSocketTcp, (struct sockaddr *)&addr, sizeof(addr)))
			{
				MessageBox("���ӳ�ǿ��ʧ��");
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
				char sendbuff[100];
				sprintf(sendbuff, "MEM:CONT 0,%sE6,0,FM,15kHz,6,0,0,0,0,1\n", Pr100freq);
				Sleep(50);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				sprintf(sendbuff, "MEM:LABEL 0,\"%s MHz, BW 15kHz\"\n", Pr100freq);
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				sprintf(sendbuff, "FREQ %sE6\n", Pr100freq);
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
				/*
				send(nSocketTcp, "TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n", \
					strlen("TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n"), 0);
				*/
				send(nSocketTcp, "TRAC:UDP:DEF:FLAG:ON \"192.168.0.201\",19000,\"VOLT:AC\"\n", \
					strlen("TRAC:UDP:DEF:FLAG:ON \"192.168.0.201\",19000,\"VOLT:AC\"\n"), 0);
				Sleep(50);
				//
				/*send(nSocketTcp, "TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n", \
					strlen("TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n"), 0);*/
				send(nSocketTcp, "TRAC:UDP:DEF:TAG:ON \"192.168.0.201\",19000,MSC\n", \
					strlen("TRAC:UDP:DEF:TAG:ON \"192.168.0.201\",19000,MSC\n"), 0);
				Sleep(50);
				send(nSocketTcp, "INIT:CONM\n", strlen("INIT:CONM\n"), 0);
				closesocket(nSocketTcp);


				//UDP��ʼ��
				nSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);
				if (INVALID_SOCKET == nSocketUdp)
				{
					MessageBox("�׽��ִ���ʧ�ܣ�");
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
					MessageBox("�󶨶˿�ʧ�ܣ�");
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

UINT MyThreadProc(LPVOID pParam)
{
	unsigned long	histo1[4100];
	int				speed0 = 80;
	CString			m_proption;

	CMDIFrameWnd *pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	CMDIChildWnd *pChild = (CMDIChildWnd *)pFrame->GetActiveFrame();
	CFSTView *pView = (CFSTView *)pChild->GetActiveView();
	m_proption = pView->m_rate;			

	CClientDC  pDC(pView), pDC1(pView);
	CPen	myPen3(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));   //Maintenance line color
	CPen	myPen2(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));   //Text color
	CPen	myPen11(PS_SOLID, 2, RGB(0x00, 0x00, 0xC0));  //data display color
	CPen	myPen0(PS_DOT, 1, RGB(0xD0, 0xD0, 0xD0));     //grid color
	CPen    myPenTrainPos(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));   //draw line for current position of the train
	CPen	*pOldPen;
	//	int     nOldDrawMode;

	CFont	myFont, *pOldFont;
	myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
	pOldFont = pDC.SelectObject(&myFont);

	pDC1.SelectObject(&myPen11);
	pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));

	// Display train and move pen to start point 
	pDC1.FillSolidRect(nDrawRangeXMin - 30, 455, 30, 16, 0xFFFFFF);
	pDC1.BitBlt(nDrawRangeXMin - 30, 458, 30, 13, &pDC1, 345, 30, SRCCOPY);
	pDC1.MoveTo(nDrawRangeXMin, 450);
	// 	pDC.SetBkMode(TRANSPARENT);
	// 	nOldDrawMode = pDC.SetROP2(R2_XORPEN);
	// 	pDC.SelectObject(&myPenTrainPos);
	// 	pDC.MoveTo(nDrawRangeXMin, 50);
	// 	pDC.LineTo(nDrawRangeXMin, 458);
	// 	pDC.SelectObject(pOldFont);


	char	pp[1200];
	char	gprmc[1200];
	int		gpsdata_flag;

	int		count, kk;
	float	dbVal1, cal_voltage, cal_dbval;

	unsigned char inData[640 * 16];
	long pulseNum = 0;
	long prePulseNum = 0;
	int taxPosNum = 0;
	int preTaxPosNum = 0;
	int taxSpeedNum = 0;
	unsigned int  speedNum;
	//unsigned int  vol1;
	short	vol1;
	unsigned char  checkByte;
	bool flagNum = false;
	//// discard the first 1024 data/////////////
	unsigned long curMilage, preMilage;

	int getinitdis_flag;

	for (int i = 0; i<4096; i++)
	{
		histo1[i] = 0;
	}
	AD_number = 0;
	AD_num100 = 0;
	currentDis = startDis;
	sectionNum = 0;
	getinitdis_flag = 0;
	int samplecount = 0;
	double preDis;

	preDis = currentDis;

	int	sendFlag = 0;
	int    which = 95;
	int    overPass = 0;

	DWORD	tm1, tm2;
	tm1 = GetTickCount();

	if (m_proption.Find(_T("95")) != 0 - 1)
		which = 95;
	else if (m_proption.Find(_T("90")) != 0 - 1)
		which = 90;
	else   which = 50;


	///open an existing file to reand and simulate to get gps data
	FILE *simfp, *fp2;
	int sim_flag = 0;
	long simcount = 0;
	int matchcount = 0;
	int kkk;
	int diffNum = 0;
	char dbuffer[9];
	char tbuffer[9];
	int captureno;

	/*read from file gps239.txt*/
	//	simfp=fopen("gps239.txt","r+t");
	/*	simfp=fopen("��ƺsim.TXT","r+t");
	if(simfp!=NULL)
	{
	sim_flag = 1;
	}
	else
	sim_flag = 0;

	//*/
	////////////////////////////////////////////////
	//add by bzw 161111 start
	int dataready_flag = 0;
	int pre_global_odoTotalData = 0;
	int odoCountNum = 0;
	while (readStatus == 1) 
	{
		
				gpsdata_flag = 0;	//no gps data available	//get gps data edit by zwbai 20170503
				sprintf(pp, "%s", gpsData);
				pDC.TextOut(60, 570, pp);	//output gps data
				if (!strncmp((char *)&gpsData[0], "$GNRMC", 6))
				{
					sprintf(gprmc, "");
					gpsdata_flag = 1;
					if (sim_flag)
					{
						while (!feof(simfp))
						{
							fscanf(simfp, "%s", gprmc);
							if (!strncmp(gprmc, "$GNRMC", 6))
							{
								strcpy((char *)&gpsData[0], gprmc);
								break;
							}
						}
					}

				}
				/*if (pulseNum == prePulseNum)
				{
					pView->m_fCurSpeed = speedNum*unit*1.8/1000;
					static unsigned int nCnt = 0;
					nCnt++;
					if (20 == nCnt)
					{
						CString strEditValue;
						strEditValue.Format(_T("%05.1f"), pView->m_fCurSpeed);
						pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEditValue);
						nCnt = 0;
					}
					continue;
				}*/
				if (nWorkMode==0 || nWorkMode==2)
				{
					pulseNum = global_odoTotalData;				//get odo data edit by zwbai 20170503
					speedNum = global_odoSpeedData;
					for (long i = prePulseNum; i < pulseNum; i++)
					{
						if ((i%2)==0)//����Ƶ
						{
							AD_number++;
							currentDis = startDis + delta*AD_number*unit;
							sprintf(pp, "Correct:%06d  %06d  %08x  %08x, %02d", speedNum, AD_number, pulseNum, prePulseNum, pulseNum - prePulseNum);
							pDC.TextOut(400, 350, pp);

							currentDis = startDis + delta*AD_number*unit;
							prePulseNum = pulseNum;
							if (((AD_number%pulse100M) == 0))
							{
								//����100�׿�ʼ��ͼ
								//m_FSTbLocked = true;		//ȡ��ǿֵ������д
								flagNum = false;
								data1[AD_num100].curPos = (int)startDis + (delta*AD_num100) * 100;
								sprintf(pp, "dB: %6.2f %6.2f  %6.2f", data1[AD_num100].AD_95,
									data1[AD_num100].AD_90, data1[AD_num100].AD_50);

								fusiondata[AD_num100].curpos = data1[AD_num100].curPos;
								fusiondata[AD_num100].pulsenum = currentPulsenum;
								fusiondata[AD_num100].lat = currentLat;
								fusiondata[AD_num100].lon = currentLon;

								pDC.TextOut(750, 350, pp);
								dbVal1 = dbvalue_global;
								AD_value1[AD_num100] = dbVal1;

								//m_FSTbLocked = false;			//ȡ����ǿֵ����дadd by zwbai 170224
								if (0 == sectionNum)
								{
									pDC1.MoveTo(nDrawRangeXMin + sectionNum*nPix100m, 410 - (int)(dbVal1)* 4);
								}

								AD_num100++;
								sectionNum++;
								//////////////// Update position of the train and draw the curve ///////////
								// modified by zgliu 2011.04.14, ��ԭ����ÿ100��2�����ص��Ϊ6��
								// �ȸ��Ǿɳ��ٻ���ǰλ�õ�С��
								pDC1.LineTo(nDrawRangeXMin + nPix100m*sectionNum, 410 - (int)(dbVal1)* 4);
								pDC1.FillSolidRect(nDrawRangeXMin - 30 + sectionNum*nPix100m - nPix100m, 455, 30, 16, 0xFFFFFF);
								pDC1.BitBlt(nDrawRangeXMin - 30 + sectionNum*nPix100m, 458, 30, 13, &pDC1, 345, 30, SRCCOPY);

								//���������������ʾ
								CString strEidtValue;
								strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
								pView->SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
								strEidtValue.Format(_T("%05.2f"), dbVal1);
								pView->SetDlgItemText(IDC_EDIT_CurDBValue, strEidtValue);
								strEidtValue.Format(_T("%05.1f"), speedNum*unit*18);   //��������odoЭ��
								pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);
								// add end by zgliu
							}
							/*odo speed display edit by zwbai 170307*/
							CString strEidtValue;
							strEidtValue.Format(_T("%05.1f"), speedNum*unit*18);   //��������odoЭ��
							pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);

							//���г�ʻ����ǰ��ʾ��Χ(startKM+15km)�������»���
							// add by zgliu 2011.04.13
							if ((currentDis - (atof(startKM) + 15 * delta)*1000.0)*delta > 0.0)
							{
								// 				pDC.SetBkMode(OPAQUE);
								// 				pDC.SetROP2(nOldDrawMode);
								pView->Invalidate(TRUE);
								pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 450 - 50, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

								pDC.SelectObject(&myPen0);
								for (int i = 0; i < 10; i++)
								{
									pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
									pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
								}
								const int nPix1KM = nPix500M * 2;
								for (int i = 1; i <= nKMDisplayNum; i++)
								{
									pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
									pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
								}

								pDC.SelectObject(&myPen3);
								int	maintance = atoi(pView->m_maintance);
								pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
								pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);

								pDC.FillSolidRect(nDrawRangeXMin - 35, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

								sectionNum = 0;
								currentName = _T("");
								nextName = CString(stationName[nextStation]);
								nextCode.Format("%d", stationNum[nextStation]);
								float fRangeStart = currentDis / 1000;
								startKM.Format("%6.2f", fRangeStart);
								nextKM.Format("%6.2f", stationDis[nextStation]);
								offset = (int)(fabs(fRangeStart - stationDis[nextStation])*nPix1KM);

								pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + nextCode);

								DisplayOthers(&pDC, startKM, nextKM);

								// ÿ1KM��ʾһ���̶�ֵ
								CFont myKMFont;
								myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
									OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
								pOldFont = pDC.SelectObject(&myKMFont);
								const int nDeltaKM = offset / nPix1KM;
								CString strTempKM;
								for (int i = 1; i <= nDeltaKM; ++i)
								{
									if (i <= (nKMDisplayNum + 1) / 2)
									{
										if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
										}
										else if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
										}
										pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
										pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
									}
								}
								pDC.SelectObject(pOldFont);

								pOldPen = pDC.SelectObject(&myPen2);
								pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
								pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
								pDC.MoveTo(nDrawRangeXMin + offset, 50);
								pDC.LineTo(nDrawRangeXMin + offset, 450);

								pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
								// 				pDC.SetBkMode(TRANSPARENT);
								// 				pDC.SetROP2(R2_XORPEN);
							}
							// add end by zgliu 

							if (((nextStation > stationCount) && (delta > 0)) || ((nextStation < 0) && (delta < 0)))
							{
								if (overPass == 0)
								{
									pDC.TextOut(360, 180, _T(" �ѵ�����·���յ�վ,��ֹͣ����,����ѡ����·!"));
									overPass = 1;
									readStatus = 0;
									Pr100ProcFlag = 0;
									AfxMessageBox(_T("!!!!!!!!!!!!!��ɲ���!!!!!!!!!!!!!!!!!!!!!"));
								}
							}
							else if ((currentDis - stationDis[nextStation] * 1000.0)*delta > 0.0) //�г�ʻ����һվ
							{
								// 				pDC.SetBkMode(OPAQUE);
								// 				pDC.SetROP2(nOldDrawMode);

								pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 400, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

								pDC.SelectObject(&myPen0);
								for (int i = 0; i < 10; i++)
								{
									pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
									pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
								}
								const int nPix1KM = nPix500M * 2;
								for (int i = 1; i <= nKMDisplayNum; i++)
								{
									pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
									pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
								}

								pDC.SelectObject(&myPen3);
								int	maintance = atoi(pView->m_maintance);
								pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
								pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);


								pDC.FillSolidRect(nDrawRangeXMin - 35, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

								currentStation += delta;
								nextStation += delta;

								sectionNum = 0;
								currentName = CString(stationName[currentStation]);
								nextName = CString(stationName[nextStation]);
								currentCode.Format("%d", stationNum[currentStation]);
								nextCode.Format("%d", stationNum[nextStation]);
								startKM.Format("%6.2f", stationDis[currentStation]);
								nextKM.Format("%6.2f", stationDis[nextStation]);
								offset = (int)(fabs(stationDis[currentStation] - stationDis[nextStation])*nPix1KM);


								pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);

								pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + currentCode);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + nextCode);

								DisplayOthers(&pDC, startKM, nextKM);

								// add by zgliu 2011.04.13 
								// ÿ1KM��ʾһ���̶�ֵ
								CFont myKMFont;
								myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
									OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
								pOldFont = pDC.SelectObject(&myKMFont);
								const int nDeltaKM = offset / nPix1KM;
								CString strTempKM;
								for (int i = 1; i <= nDeltaKM; ++i)
								{
									if (i <= (nKMDisplayNum + 1) / 2)
									{
										if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
										}
										else if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
										}
										pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
										pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
									}
								}
								pDC.SelectObject(pOldFont);
								// add end by zgliu 

								pOldPen = pDC.SelectObject(&myPen2);
								pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
								pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
								pDC.MoveTo(nDrawRangeXMin + offset, 50);
								pDC.LineTo(nDrawRangeXMin + offset, 450);
								pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
								// 				pDC.SetBkMode(TRANSPARENT);
								// 				pDC.SetROP2(R2_XORPEN);
								sendFlag = 0;   //allow send timeCode and addressCode again
							}

						}else{
								continue;
						}
					}
				}else if (nWorkMode==1||nWorkMode==3)
				{
					taxPosNum = global_taxPosData;				//get odo data edit by zwbai 20170503
					taxSpeedNum = global_taxSpeedData;
					
					if (!getinitdis_flag)
					{
						curMilage = taxPosNum;

						//��ֹ���Թ�����TAX��������뵼������
						if (curMilage > stationDis[stationCount] * 1000.0)
						{
							curMilage = preMilage;
						}

						preMilage = curMilage;
						getinitdis_flag = 1;

						//update the init distance
						pView->m_distance.Format("%.1f", (int)((curMilage / 100)) / 10.0);
						pView->OnChangeInitDistance();

						//	sprintf(pp,"Init Dis: %06dm (%s) %d", curMilage, pView->m_distance, vol1);
						//	pDC.TextOut(400,550,pp);

					}
					else
					{
						curMilage = taxPosNum;
						//sprintf(pp, "ʵʱ��Ϣ: %06dm (%s) %4.1f", curMilage, pView->m_distance, (vol1 - 100) / 10.0);
						//pDC.TextOut(150, 530, pp);
					}
					currentDis = (int)(curMilage / 1.0);		//unit: meters  //modified by yhb 100320
					///////////////////ʵʱTax������ʾ////////////////////////
					sprintf(pp, "TaxSpeed:%06d  TaxPosition:%06d ", global_taxSpeedData, global_taxPosData);
					pDC.TextOut(400, 350, pp);
					///////////////////ʵʱTax������ʾ////////////////////////
					//for (long i = preTaxPosNum; i < pulseNum; i++)
					//{
							
							if ((curMilage - preMilage)*delta >= 100)
							{
								//����100�׿�ʼ��ͼ
								//m_FSTbLocked = true;		//ȡ��ǿֵ������д
								flagNum = false;
								preMilage = curMilage;
								preDis = currentDis;                           

								data1[AD_num100].curPos = (int)currentDis;
								sprintf(pp, "dB: %6.2f %6.2f  %6.2f", data1[AD_num100].AD_95,
									data1[AD_num100].AD_90, data1[AD_num100].AD_50);
								pDC.TextOut(750, 350, pp);
								fusiondata[AD_num100].curpos = data1[AD_num100].curPos;
								fusiondata[AD_num100].pulsenum = taxPosNum;
								fusiondata[AD_num100].lat = currentLat;
								fusiondata[AD_num100].lon = currentLon;

								
								dbVal1 = dbvalue_global;
								AD_value1[AD_num100] = dbVal1;

								//m_FSTbLocked = false;			//ȡ����ǿֵ����дadd by zwbai 170224
								if (0 == sectionNum)
								{
									pDC1.MoveTo(nDrawRangeXMin + sectionNum*nPix100m, 410 - (int)(dbVal1)* 4);
								}

								AD_num100++;
								sectionNum++;
								//////////////// Update position of the train and draw the curve ///////////
								// modified by zgliu 2011.04.14, ��ԭ����ÿ100��2�����ص��Ϊ6��
								// �ȸ��Ǿɳ��ٻ���ǰλ�õ�С��
								pDC1.LineTo(nDrawRangeXMin + nPix100m*sectionNum, 410 - (int)(dbVal1)* 4);
								pDC1.FillSolidRect(nDrawRangeXMin - 30 + sectionNum*nPix100m - nPix100m, 455, 30, 16, 0xFFFFFF);
								pDC1.BitBlt(nDrawRangeXMin - 30 + sectionNum*nPix100m, 458, 30, 13, &pDC1, 345, 30, SRCCOPY);

								//���������������ʾ
								CString strEidtValue;
								strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
								pView->SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
								strEidtValue.Format(_T("%05.2f"), dbVal1);
								pView->SetDlgItemText(IDC_EDIT_CurDBValue, strEidtValue);
								strEidtValue.Format(_T("%05.1f"), taxSpeedNum*1.0);   //��������odoЭ��
								pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);
								// add end by zgliu
							}
							/*odo speed display edit by zwbai 170307*/
							CString strEidtValue;
							strEidtValue.Format(_T("%05.1f"), taxSpeedNum*1.0);   //��������odoЭ��
							pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);

							//���г�ʻ����ǰ��ʾ��Χ(startKM+15km)�������»���
							// add by zgliu 2011.04.13
							if ((currentDis - (atof(startKM) + 15 * delta)*1000.0)*delta > 0.0)
							{
								// 				pDC.SetBkMode(OPAQUE);
								// 				pDC.SetROP2(nOldDrawMode);
								pView->Invalidate(TRUE);
								pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 450 - 50, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

								pDC.SelectObject(&myPen0);
								for (int i = 0; i < 10; i++)
								{
									pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
									pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
								}
								const int nPix1KM = nPix500M * 2;
								for (int i = 1; i <= nKMDisplayNum; i++)
								{
									pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
									pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
								}

								pDC.SelectObject(&myPen3);
								int	maintance = atoi(pView->m_maintance);
								pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
								pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);

								pDC.FillSolidRect(nDrawRangeXMin - 35, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

								sectionNum = 0;
								currentName = _T("");
								nextName = CString(stationName[nextStation]);
								nextCode.Format("%d", stationNum[nextStation]);
								float fRangeStart = currentDis / 1000;
								startKM.Format("%6.2f", fRangeStart);
								nextKM.Format("%6.2f", stationDis[nextStation]);
								offset = (int)(fabs(fRangeStart - stationDis[nextStation])*nPix1KM);

								pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + nextCode);

								DisplayOthers(&pDC, startKM, nextKM);

								// ÿ1KM��ʾһ���̶�ֵ
								CFont myKMFont;
								myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
									OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
								pOldFont = pDC.SelectObject(&myKMFont);
								const int nDeltaKM = offset / nPix1KM;
								CString strTempKM;
								for (int i = 1; i <= nDeltaKM; ++i)
								{
									if (i <= (nKMDisplayNum + 1) / 2)
									{
										if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
										}
										else if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
										}
										pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
										pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
									}
								}
								pDC.SelectObject(pOldFont);

								pOldPen = pDC.SelectObject(&myPen2);
								pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
								pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
								pDC.MoveTo(nDrawRangeXMin + offset, 50);
								pDC.LineTo(nDrawRangeXMin + offset, 450);

								pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
								// 				pDC.SetBkMode(TRANSPARENT);
								// 				pDC.SetROP2(R2_XORPEN);
							}
							// add end by zgliu 

							if (((nextStation > stationCount) && (delta > 0)) || ((nextStation < 0) && (delta < 0)))
							{
								if (overPass == 0)
								{
									pDC.TextOut(360, 180, _T(" �ѵ�����·���յ�վ,��ֹͣ����,����ѡ����·!"));
									overPass = 1;
									readStatus = 0;
									Pr100ProcFlag = 0;
									AfxMessageBox(_T("!!!!!!!!!!!!!��ɲ���!!!!!!!!!!!!!!!!!!!!!"));
								}
							}
							else if ((currentDis - stationDis[nextStation] * 1000.0)*delta > 0.0) //�г�ʻ����һվ
							{
								// 				pDC.SetBkMode(OPAQUE);
								// 				pDC.SetROP2(nOldDrawMode);

								pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 400, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

								pDC.SelectObject(&myPen0);
								for (int i = 0; i < 10; i++)
								{
									pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
									pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
								}
								const int nPix1KM = nPix500M * 2;
								for (int i = 1; i <= nKMDisplayNum; i++)
								{
									pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
									pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
								}

								pDC.SelectObject(&myPen3);
								int	maintance = atoi(pView->m_maintance);
								pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
								pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);


								pDC.FillSolidRect(nDrawRangeXMin - 35, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

								currentStation += delta;
								nextStation += delta;

								sectionNum = 0;
								currentName = CString(stationName[currentStation]);
								nextName = CString(stationName[nextStation]);
								currentCode.Format("%d", stationNum[currentStation]);
								nextCode.Format("%d", stationNum[nextStation]);
								startKM.Format("%6.2f", stationDis[currentStation]);
								nextKM.Format("%6.2f", stationDis[nextStation]);
								offset = (int)(fabs(stationDis[currentStation] - stationDis[nextStation])*nPix1KM);


								pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);

								pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + currentCode);
								pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + nextCode);

								DisplayOthers(&pDC, startKM, nextKM);

								// add by zgliu 2011.04.13 
								// ÿ1KM��ʾһ���̶�ֵ
								CFont myKMFont;
								myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
									OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
								pOldFont = pDC.SelectObject(&myKMFont);
								const int nDeltaKM = offset / nPix1KM;
								CString strTempKM;
								for (int i = 1; i <= nDeltaKM; ++i)
								{
									if (i <= (nKMDisplayNum + 1) / 2)
									{
										if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
										}
										else if (-1 != pView->m_updown.Find(_T("����")))
										{
											strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
										}
										pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
										pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
									}
								}
								pDC.SelectObject(pOldFont);
								// add end by zgliu 

								pOldPen = pDC.SelectObject(&myPen2);
								pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
								pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
								pDC.MoveTo(nDrawRangeXMin + offset, 50);
								pDC.LineTo(nDrawRangeXMin + offset, 450);
								pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
								// 				pDC.SetBkMode(TRANSPARENT);
								// 				pDC.SetROP2(R2_XORPEN);
								sendFlag = 0;   //allow send timeCode and addressCode again
						}
						else
						{
							continue;
						}
					}
				//}

	//GPS���ݴ洢
		if (gpsdata_flag)
		{
			//sprintf(pp, "%s,[%d]", gprmc, strlen(gprmc));
			//pDC.TextOut(100,570,gprmc);	//output gps data
			///$GPRMC,213900.00,A,3611.8869946,N,09447.0742503,E,27.971,215.9,080406,0.0,E*66
			CString gprmc1, gprmc2;
			CString utc, gpsvalidstr, latstr, lonstr, tmpstr, gspeedstr;
			int comma, comma1, minflag;
			double lat, lon, disval, mindisval, gspeed;

			gprmcdata[gprmcCount] = gprmc;
			gprmcCount++;

			if (gprmcCount > 32000)
			{
				//write to gprmc files
				FILE	*fp4;

				int strLen = pView->m_filename.GetLength();
				CString tmp = pView->m_filename.Mid(0, strLen - 4);
				CString gprmcfilename;

				gprmcfilename.Format("%s-%03d.TXT", tmp, gprmcfileCount);
				fp4 = fopen(gprmcfilename, "a+t");
				if (fp4 != NULL)
				{
					for (int iii = 0; iii < gprmcCount; iii++)
						fprintf(fp4, "%s", gprmcdata[iii]);
					fclose(fp4);
				}
				gprmcCount = 0;
				gprmcfileCount++;
			}

			gprmc2.Format("%s", gprmc);
			int strLen = gprmc2.GetLength();
			gprmc1 = gprmc2;
			comma = gprmc2.Find(",");
			gprmc1 = gprmc2.Mid(comma + 1, strLen - comma - 1);
			comma1 = gprmc1.Find(",");
			utc = gprmc1.Mid(0, comma1);

			strLen = gprmc1.GetLength();
			gprmc2 = gprmc1.Mid(comma1 + 1, strLen - comma1 - 1);
			comma = gprmc2.Find(",");
			gpsvalidstr = gprmc2.Mid(0, comma);

			strLen = gprmc2.GetLength();
			gprmc1 = gprmc2.Mid(comma + 1, strLen - comma - 1);
			comma1 = gprmc1.Find(",");
			latstr = gprmc1.Mid(0, comma1);

			strLen = gprmc1.GetLength();
			gprmc2 = gprmc1.Mid(comma1 + 1, strLen - comma1 - 1);
			comma = gprmc2.Find(",");
			tmpstr = gprmc2.Mid(0, comma);

			strLen = gprmc2.GetLength();
			gprmc1 = gprmc2.Mid(comma + 1, strLen - comma - 1);
			comma1 = gprmc1.Find(",");
			lonstr = gprmc1.Mid(0, comma1);

			strLen = gprmc1.GetLength();
			gprmc2 = gprmc1.Mid(comma1 + 1, strLen - comma1 - 1);
			comma = gprmc2.Find(",");
			tmpstr = gprmc2.Mid(0, comma);

			strLen = gprmc2.GetLength();
			gprmc1 = gprmc2.Mid(comma + 1, strLen - comma - 1);
			comma1 = gprmc1.Find(",");
			gspeedstr = gprmc1.Mid(0, comma1);

			gspeed = atof(gspeedstr)*1.853245;	//convert knot to km/h

			strLen = latstr.GetLength();
			gprmc1 = latstr.Left(2);
			gprmc2 = latstr.Right(strLen - 2);
			lat = atof(gprmc1) + atof(gprmc2) / 60.0;
			strLen = lonstr.GetLength();
			gprmc1 = lonstr.Left(3);
			gprmc2 = lonstr.Right(strLen - 3);
			lon = atof(gprmc1) + atof(gprmc2) / 60.0;

			currentLat = lat;
			currentLon = lon;

			if (gpscorCount > 0)
			{
				minflag = 1;
				captureno = -1;
				for (kkk = 0; kkk < gpscorCount; kkk++)
				{
					if (gpsmmflag[kkk] == 1)
						continue;
					disval = Distance2(lat, lon, gpsLat[kkk], gpsLon[kkk]);
					if (minflag == 1)
					{
						mindisval = disval;
						minflag = 0;
					}
					else
					{
						if (disval < mindisval)
							mindisval = disval;
					}
					//sprintf(pp, "%lf, %lf, %lf, %lf, %lf, %s",
					//lat, lon, gpsLat[kkk], gpsLon[kkk], disval, gpsName[kkk]);
					//pDC.TextOut(100, 100+kkk*20, pp);	//output gps data
					if (disval < GPSThreshold)	//default: 50m
					{
						gpscaptureCount++;
						captureno = kkk;
						gpsmmflag[kkk] = 1;
						gpscapture_flag = 1;
						gpscaptureDis = gpsDis[kkk];

						//pView->m_gpscapture.Format("%d/%d, %.1lf, %d",
						//gpscaptureCount, gpscorCount, disval, gpscapture_flag);

						fp2 = fopen("MMResults.txt", "a+t");
						if (fp2 != NULL)
						{
							_strdate(dbuffer);
							_strtime(tbuffer);
							fprintf(fp2, "%s %s %.2lf, %.2lf, %lf, %lf, %lf, %lf, %.1lf, %s, %d\n",
								dbuffer, tbuffer, currentDis, gpscaptureDis, lat, lon,
								gpsLat[captureno], gpsLon[captureno], disval, gpsName[captureno], captureno);
							fclose(fp2);
						}

					}
					pView->m_gpscapture.Format("%d/%d, %.1lf(%d), %d",
						gpscaptureCount, gpscorCount, mindisval, captureno, gpscapture_flag);
					if (gpscapture_flag)
						break;

				}

			}

			//sprintf(pp, "(%s)UTCʱ��:%s γ�ȣ�%lf, ���ȣ�%lf, %d, %lf",
			//gpsvalidstr, utc, lat, lon, gpscorCount, disval);
			//pDC.TextOut(60, 550, pp);	//output gps data
			pView->m_gpsstr.Format("(%s)UTCʱ��:%s γ��:%lf, ����:%lf, %.1lfkm/h",
				gpsvalidstr, utc, lat, lon, gspeed);
			//pView->m_gpscapture.Format("%d, %lf", gpscorCount, disval);
			pView->m_gpssample.Format("%d", gpssampleCount);
			pView->UpdateData(FALSE);	//UpdateGPSDisplay();

			sprintf(pp, "(%s)UTCʱ��:%s γ�ȣ�%s, ���ȣ�%s",
				gpsvalidstr, utc, latstr, lonstr);
			//pDC.TextOut(60, 570, pp);	//output gps data

			gpsdata_flag = 0;
		}

	}
	if (sim_flag) {
		fclose(simfp);
	}

	tm2 = GetTickCount();
	sprintf(pp, "%ld %d", tm2 - tm1, AD_num100);
	//    m_wndStatusBar.SetPaneText(0,pp,TRUE);

	//pDC.SelectObject(pOldPen);
 	//pDC.SelectObject(pOldFont);//�˴������⣬��ע��


	AfxEndThread(0);
	// ExitThread(0);

	//
	//	���������߳̾����ָ��::GetExitCodeThread()����,����߳��Ѿ�����, 
	//	��������һ���˳�����,�����������,�򷵻�һ��STILL_ACTIVE.������֮��ǰ,
	//	�Ƚ� CWinThread��Ա����m_bAutoDelete����ΪFALSE.
	//	����������߳̽���ʱ���Զ���⵽.

	
	return 0;    // thread completed successfully
}

void CFSTView::OnChangeInitDistance()
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function to send the EM_SETEVENTMASK message to the control
	// with the ENM_CHANGE flag ORed into the lParam mask.

	// TODO: Add your control notification handler code here
	if ( /*UpdateData(TRUE)  &&*/ m_updown.GetLength() > 3)
	{

		CClientDC pDC(this);
		//???	pDC.SetROP2(R2_XORPEN);
		pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
		CPen	myPen1(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
		CPen	*pOldPen;
		pOldPen = pDC.SelectObject(&myPen1);

		CFont	myFont, *pOldFont;
		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myFont);


		if (offset > 0)
		{
			pDC.MoveTo(80 + offset / 2, 445);
			pDC.LineTo(80 + offset / 2, 450);
			pDC.MoveTo(80 + offset, 445);
			pDC.LineTo(80 + offset, 450);
		}

		pDC.FillSolidRect(60, 455, 800, 50, 0xFFFFFF);


		offset = 0;

		startDis = atof(m_distance)*1000.0;
		startKM = m_distance;
		nextKM = _T("");

		currentStation = 0 - 1;
		currentName = _T("");
		nextName = _T("");
		currentCode = _T("");
		nextCode = _T("");

		firstMapped = 0;

		for (int i = 1; i <= stationCount; i++)
		{
			if (fabs(stationDis[i] * 1000.0 - startDis) < 100.0)
			{
				currentStation = i;
				currentName = CString(stationName[i]);
				currentCode.Format("% d", stationNum[i]);

				if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i + 1]);
					nextName = CString(stationName[i + 1]);
					nextCode.Format("% d", stationNum[i + 1]);
					nextStation = i + 1;
					offset = (int)((stationDis[i + 1] - stationDis[i])*20.0);
					delta = 1;
				}
				else if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i - 1]);
					nextName = CString(stationName[i - 1]);
					nextCode.Format("% d", stationNum[i - 1]);
					nextStation = i - 1;
					offset = (int)((stationDis[i] - stationDis[i - 1])*20.0);
					delta = 0 - 1;
				}

				firstMapped = 1;
				i = stationCount + 1;
			}

		}

		if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = 1; i <= stationCount; i++)
				if (stationDis[i] * 1000.0 > startDis)
				{
					currentStation = i - 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("% d", stationNum[i]);
					nextStation = i;
					offset = (int)((stationDis[i] - atof(startKM))*20.0);
					delta = 1;
					i = stationCount + 1;
				}
		}

		else if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = stationCount; i >= 1; i--)
				if (stationDis[i] * 1000.0 < startDis)
				{
					currentStation = i + 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("% d", stationNum[i]);
					nextStation = i;
					offset = (int)((atof(startKM) - stationDis[i])*20.0);
					delta = 0 - 1;
					i = 0;
				}
		}

		pDC.TextOut(80 - 5 - 15, 460 + 10, startKM);
		pDC.TextOut(80 - 5 + offset, 460 + 10, nextKM);

		pDC.TextOut(80 - 5 - 15, 480 + 10, currentName + currentCode);
		pDC.TextOut(80 - 5 + offset, 480 + 10, nextName + nextCode);

		DisplayOthers(&pDC, startKM, nextKM);

		if (offset > 0)
		{
			pDC.MoveTo(80 + offset / 2, 445);
			pDC.LineTo(80 + offset / 2, 450);
			pDC.MoveTo(80 + offset, 445);
			pDC.LineTo(80 + offset, 450);
		}

		pDC.SelectObject(pOldPen);
		pDC.SelectObject(pOldFont);

	}

}
void CFSTView::InitScreen()
{

	//Init screen for drawing	 
	CFont myFont1, myFont2, myFont3, myKMFont, *pOldFont;

	CClientDC pDC(this);
	CRect rcClip;
	GetClientRect(&rcClip);
	pDC.FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom - 140, 0xffffff);

	firstTime = 0;

	CPen	myPen0(PS_DOT, 1, RGB(0xD0, 0xD0, 0xD0));
	CPen	myPen1(PS_SOLID, 1, RGB(0x00, 0x80, 0xFF));
	CPen	myPen2(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));
	CPen	myPen3(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
	CPen	myPen11(PS_SOLID, 2, RGB(0x00, 0x00, 0xC0));

	char	pp[30];

	pDC.SelectObject(&myPen0);

	for (int i = 0; i < 10; i++)//�������еĺ���
	{
		pDC.MoveTo(nDrawRangeXMin, 50 + i * 40);
		pDC.LineTo(nDrawRangeXMax, 50 + i * 40);
	}

	for (int i = 1; i <= nKMDisplayNum; i++)//�������е�����
	{
		pDC.MoveTo(nDrawRangeXMin + i*nPix500M, 50);
		pDC.LineTo(nDrawRangeXMin + i*nPix500M, 450);
	}


	if (sectionNum > 0)
	{
		int	jj = AD_num100 - sectionNum;

		pDC.SelectObject(&myPen11);
		pDC.MoveTo(nDrawRangeXMin, 410 - (int)AD_value1[jj] * 4);
		for (int ii = 1; ii <= sectionNum; ii++)
			pDC.LineTo(nDrawRangeXMin + (ii*(nPix100m)), 410 - (int)AD_value1[jj + ii - 1] * 4);
	}

	pDC.SelectObject(&myPen1);
	pDC.SelectStockObject(NULL_BRUSH);
	pDC.Rectangle(nDrawRangeXMin, 48, nDrawRangeXMax, 451);

	myFont1.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
	pOldFont = pDC.SelectObject(&myFont1);

	CTime tNow;
	tNow = CTime::GetCurrentTime();
	pDC.TextOut(820, 25, _T("�������ڣ�") + tNow.Format("%Y.%m.%d"));

	pDC.SetTextColor(RGB(0x00, 0x00, 0x80));

	for (int i = 0; i < 10; i++)
	{
		sprintf(pp, "%02d dB", 90 - i * 10);
		pDC.TextOut(nDrawRangeXMin - 45, 50 + i * 40 - 10, pp);
		pDC.TextOut(nDrawRangeXMax + 10, 50 + i * 40 - 10, pp);
	}

	pDC.TextOut(nDrawRangeXMin - 50, 50 + 10 * 40 - 14, _T("-10 dB"));
	pDC.TextOut(nDrawRangeXMax + 5, 50 + 10 * 40 - 14, _T("-10 dB"));

	pDC.SelectObject(pOldFont);

	myFont2.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
	pOldFont = pDC.SelectObject(&myFont2);

	pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
	pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
	pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);

	if (_T("") != currentName)
	{
		pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
	}
	else
	{
		pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
	}
	pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));

	DisplayOthers(&pDC, startKM, nextKM);

	pDC.SelectObject(pOldFont);
	// add by zgliu 2011.04.13 
	// ÿ1KM��ʾһ���̶�ֵ
	myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
	pOldFont = pDC.SelectObject(&myKMFont);
	const int nPix1KM = nPix500M * 2;
	const int nDeltaKM = offset / nPix1KM;
	CString strTempKM;
	for (int i = 1; i <= nDeltaKM; ++i)
	{
		if (i <= (nKMDisplayNum + 1) / 2)
		{
			 			if (-1 != m_updown.Find(_T("����")))						
			 			{
							strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
						}
						else if (-1 != m_updown.Find(_T("����")))
						{
		 				strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
			 			}
			pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
			pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
		}
	}
	pDC.SelectObject(pOldFont);
	// add end by zgliu 

	myFont3.CreateFont(24, 10, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("ROMAN"));
	pOldFont = pDC.SelectObject(&myFont3);

	pDC.SetTextColor(RGB(0x00, 0xC0, 0x80));
	pDC.TextOut(380, 20, m_line+_T("��ǿ��������"));


	pDC.SelectObject(&myPen2);
	 	int	maintance = atoi(m_maintance);							
	pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
 	pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);

	//Draw Train before the line name	 
	pDC.FillSolidRect(370, 30, 5, 4, 0x00C000);
	pDC.FillSolidRect(345, 33, 30, 8, 0x0000FF);
	pDC.SelectStockObject(WHITE_BRUSH);
	pDC.Ellipse(369, 37, 375, 43);
	pDC.Ellipse(363, 37, 369, 43);
	//		pDC.Ellipse(357,37,363,43);
	pDC.Ellipse(351, 37, 357, 43);
	pDC.Ellipse(345, 37, 351, 43);
	pDC.SelectStockObject(NULL_BRUSH);

	pDC.SelectObject(&myPen3);
	if (offset > 0)//��վ�յ����
	{
		pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
		pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
		pDC.MoveTo(nDrawRangeXMin + offset, 50);
		pDC.LineTo(nDrawRangeXMin + offset, 450);
	}
	pDC.SelectObject(pOldFont);

}


//void CFSTView::ReadData(unsigned char inData[], unsigned char sharebuffer[], unsigned char odoData[], int sizeofBuf)
//{
//	if (sizeofBuf >= 1024)
//	{
//		int i;
//		int j;
//		for (i = 0; i < 1024; i += 16)
//		{
//			if (0xff == inData[i])
//			{
//				inData[i + 1] = 0x00;
//				inData[i + 2] = '5';//odoData[i + 2];//odo data
//				inData[i + 3] = '0';//odoData[i + 2];
//				inData[i + 4] = '0';//odoData[i + 2];
// 				inData[i + 5] = sharebuffer[i + 4];
// 				inData[i + 6] = sharebuffer[i + 5];
//				inData[i + 7] = sharebuffer[i + 7];  //��λ��ǿֵ����  ����ʱ��ǿֵ��Ϊ���� /0/
//				inData[i + 8] = sharebuffer[i + 8];    //��λ��ǿֵ����  /200/	
//				for (j = 9; j < 16; j++)
//				{
//					inData[i + j] = 0x00;
//				}
//			}
//		}
//	}
//	if (sizeofBuf < 1024)
//	{
//		int i = 0;
//		int k = 0;
//		for (i = 0; i < sizeofBuf; i += 16)
//		{		
//
//			if (0xff == inData[i])
//			{
//				inData[i + 7] = sharebuffer[i + 7];  //��λ��ǿֵ����  ����ʱ��ǿֵ��Ϊ���� /0/
//				inData[i + 8] = sharebuffer[i + 8];    //��λ��ǿֵ����  /200/
//				for (int j = 9; j < 16; j++)
//				{
//					inData[i + j] = 0x00;
//				}
//
//			}
//		}
//		for (k = sizeofBuf; k < 1024; k++)
//		{
//
//			inData[k] = 0x00;
//		}
//	}
//}

//PR100 ��ǿֵ�����߳�
DWORD WINAPI CFSTView::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	CFSTView *pView = ((RECVPARAM*)lpParameter)->m_pView;
	char cBuffer[33]; //�������ݻ�����
	memset(cBuffer, 0, 33);
	int len = 0;
	unsigned int countNum = 0;
	unsigned long fst[4096];
	unsigned long fstPlus = 0;
	unsigned long fstvalue_plus = 0;
	unsigned int dbNum = 0;
	pView->nCountLevel = 0;
	//	MSCpacket *pt;	
	CString str, strtemp, strdisplay = "";
	while (TRUE)
	{
		memset(cBuffer, 0, 33);
		if (0 == Pr100ProcFlag || true == pView->stopPR100)
		{
			return 0;
		}
		len = recv(sock, cBuffer, sizeof(cBuffer), 0);//��������
		pView->nLevel[pView->nCountLevel] = cBuffer[len - 4];
		pView->nLevel[pView->nCountLevel + 1] = cBuffer[len - 3];
		//***  ��ʾԭʼ��ǿֵ ������   *****//
		float Val;
		Val = ((short)((pView->nLevel[pView->nCountLevel] << 8) & 0xff00) | (pView->nLevel[pView->nCountLevel + 1] & 0xff));
		Val = (float)Val / 10;
		CString str1;
		str1.Format("%2.1f", Val);
		pView->SetDlgItemText(IDC_EDIT20, str1);
		//*** ��ʾԭʼ��ǿֵ ������ end   *****//
		////////////////////////////////��ǿͳ��add by zwbai 170224 start/////////////////////////////////////////

		if (255 == pView->nLevel[pView->nCountLevel])
		{
			dbNum = 100 - (short)((pView->nLevel[pView->nCountLevel + 1]) & 0xff);
		}
		else
		{
			dbNum = (short)(((pView->nLevel[pView->nCountLevel] << 8) & 0xff00) | (pView->nLevel[pView->nCountLevel + 1] & 0xff)) + 100;
		}
		if (dbNum < 0)
		{
			dbNum = 0;
		}
		if ((dbNum >= 0) && (dbNum < 1000))
		{
			fst[dbNum]++;
			countNum++;
		}
		/*����һ����100��˲ʱ��ǿ�ľ�ֵ start*/
		/*
		if (countNum==100)
		{

		for (int i = 0;i<countNum;i++)
		{
		fstPlus += fst[i];
		}
		fstvalue_plus = fstPlus/100;
		if(m_FSTbLocked == false)
		dbvalue_global = AD2dB(fstvalue_plus);
		countNum = 0;
		fstPlus = 0;
		}
		*/
		/*����һ����100��˲ʱ��ǿ�ľ�ֵ end*/

		/*�����������ղ�����Ҫ��õ�200��˲ʱ��ǿ��ͳ��ֵ start*/

		if (countNum == 100)
		{

			int kk = 0;
			int count = 0;
			pulse95 = 0.05*countNum;
			pulse90 = 0.1*countNum;
			pulse50 = 0.5*countNum;

			while ((count<pulse95) && (kk<1000))
				count += fst[kk++];
			data1[AD_num100].AD_95 = AD2dB(kk - 1);	//including loss_voltage

			while ((count<pulse90) && (kk<1000))
				count += fst[kk++];
			data1[AD_num100].AD_90 = AD2dB(kk - 1);

			while ((count<pulse50) && (kk<1000))
				count += fst[kk++];
			data1[AD_num100].AD_50 = AD2dB(kk - 1);

			//if (m_FSTbLocked == false)
			//{

				if (which == 95)
				{
					dbvalue_global = data1[AD_num100].AD_95;
				}
				else if (which == 90)
				{
					dbvalue_global = data1[AD_num100].AD_90;
				}
				else
				{
					dbvalue_global = data1[AD_num100].AD_50;
				}
			//}
			countNum = 0;
			for (int i = 0; i<4096; i++)
			{
				fst[i] = 0;
			}
			//	memset(pView->nLevel,0,10000);
		}
		/*�����������ղ�����Ҫ��õ�100��˲ʱ��ǿ��ͳ��ֵ end*/
		////////////////////////////////��ǿͳ��add by zwbai 170224 end//////////////////////////////////////////



	}
	return 0;

}

/*****************************************/
//add by yjh 161125
DWORD WINAPI CFSTView::RecvProc_MS(LPVOID lpParameter)
{
	SOCKET sockMs = ((RECVPARAM*)lpParameter)->sock;
	HWND hwndMs = ((RECVPARAM*)lpParameter)->hwnd;
	CFSTView *pViewMs = ((RECVPARAM*)lpParameter)->m_pView;
	char msBuffer[1024];//���ݽ��ջ�����
	int strLen;
	char tempSerialNumSearch[32];//�жϽ�����ˮ��
	static	int tempSerialNum = 0;
	int tempSearch = 0;
	int i, j;
	CString strStored;
	memset(msBuffer, 0, 1024);
	bool tempSerialNumFlag = false;
	bool speedFlag = false;
	bool odoTotalFlag = false;
	bool taxDataFlag = false;
	bool GpsFlag = false;
	int flagNum = 0;//���ڼ�¼ȡ���Ķ��Ÿ���
	int workMode = 0;
	char odoSpeedDataTosend[10];// odo�ٶ����� add by yjh
	char odoTotalDataTosend[32];// odo��������� add by yjh
	char gpsSpeedDataTosend[32];
	char gpsLatDataTosend[32];
	char gpsLonDataTosend[32];
	char TaxPosTosend[40];
	char TaxSpeedTosend[10];
	int odoSpeedDataTosend_num = 0;
	int odoTotalDataTosend_num = 0;
	int gpsSpeedDataTosend_num = 0;
	int gpsLatDataTosend_num = 0;
	int gpsLonDataTosend_num = 0;
	int TaxPosTosend_num = 0;
	int TaxSpeedTosend_num = 0;
	memset(tempSerialNumSearch, 0,32);
	memset(odoSpeedDataTosend, 0, 10);
	memset(odoTotalDataTosend, 0, 32);
	memset(TaxSpeedTosend, 0, 10);
	memset(TaxPosTosend, 0, 40);
	memset(odoTotalDataTosend, 0, 32);
	memset(gpsSpeedDataTosend, 0, 32);
	memset(gpsLatDataTosend, 0, 32);
	memset(gpsLonDataTosend, 0, 32);


	int serialNum = 0;
	int odoSpeedData_num = 0;
	int odoTotalData_num = 0;
	int taxSpeedData_num = 0;
	int taxPosData_num = 0;
	float gpsSpeedData_num = 0.0;
	float gpsLatData_num = 0.0;
	float gpsLonData_num = 0.0;
	//int preoOdoTotalData_num = 0;
	int count_odo = 0;
	int diff_odo = 0;

	while (true)
	{
		strLen = recv(sockMs, msBuffer, 1024, 0);
		if (0 == strLen)
		{
			AfxMessageBox("��������ʧ��!");
			return 0;
		}
		strStored.Format("%s", msBuffer);
		nWorkMode = 2;
		switch (nWorkMode)
		{
		case 0://ODO
			//if (-1 != strStored.Find("$DATA"))
			if(msBuffer[1] == 'D')
			{
				for (j = 0, i = 0; msBuffer[i] != 0x0d; i++, j++)
				{
					odoData[j] = msBuffer[i + 5];//��$DATA,֮��ʼ��ֵ
				}
				//memset(tempSerialNumSearch, 0, 32);
				for (j = 0; odoData[j] != 0x0d; j++)//�˴������ݽ��н�����жϻس�\r
				{
					if (0x2c == odoData[j])   //�ж϶���
					{
						j++;
						flagNum++;
					}	
					if (flagNum == 1)//����ˮ��
					{
						tempSerialNumSearch[tempSearch] = odoData[j];
						serialNum = atoi(tempSerialNumSearch);
						tempSearch++;
					}
					if (flagNum == 2)//set mode
					{
						workMode = (int)(odoData[j]-'0');
					}
					if (flagNum == 3)//��odoSpeed
					{
						odoSpeedDataTosend[odoSpeedDataTosend_num] = odoData[j];
						odoSpeedData_num = atoi(odoSpeedDataTosend);
						odoSpeedDataTosend_num++;
					}
					if (flagNum == 4)//��odo��������
					{
						odoTotalDataTosend[odoTotalDataTosend_num] = odoData[j];
						odoTotalData_num = atoi(odoTotalDataTosend);
						odoTotalDataTosend_num++;
					}
				}

				global_odoSpeedData = odoSpeedData_num;//����ȫ�ִ��͵�odo�ٶ�ֵ
				global_odoTotalData = odoTotalData_num;
				flagNum = 0;
				odoSpeedDataTosend_num = 0;
				odoTotalDataTosend_num = 0;
				tempSearch = 0;

			}
			break;
		case 1://TAX
			//if (-1 != strStored.Find("$TAX"))
			if (msBuffer[1] == 'T')
			{
				for (i = 0, j = 0; msBuffer[i] != 0x0d; i++, j++)
				{
					taxData[j] = msBuffer[i + 4];
				}
				for (j = 0; taxData[j] != 0x0d; j++)//�˴������ݽ��н�����жϻس�\r
				{
					if (0x2c == taxData[j])   //�ж϶���
					{
						j++;
						flagNum++;
					}
					if (flagNum == 1)//����ˮ��
					{
						tempSerialNumSearch[tempSearch] = taxData[j];
						serialNum = atoi(tempSerialNumSearch);
						tempSearch++;
					}
					if (flagNum == 2)//��tax_speed
					{
						TaxSpeedTosend[TaxSpeedTosend_num] = taxData[j];
						taxSpeedData_num = atoi(TaxSpeedTosend);
						TaxSpeedTosend_num++;
					}
					if (flagNum == 3)//��tax_pos
					{
						TaxPosTosend[TaxPosTosend_num] = taxData[j];
						taxPosData_num = atoi(TaxPosTosend);
						TaxPosTosend_num++;
					}
					
				}
				memset(TaxSpeedTosend, 0, 10);
				memset(TaxPosTosend, 0, 40);

				global_taxSpeedData = taxSpeedData_num;//����ȫ�ִ��͵�odo�ٶ�ֵ
				global_taxPosData = taxPosData_num;
				flagNum = 0;
				taxSpeedData_num = 0;
				taxPosData_num = 0;
				tempSearch = 0;
				TaxSpeedTosend_num = 0;
				TaxPosTosend_num = 0;

			}
			break;
		case 2://ODO+GPS
			//ODO
			if (msBuffer[1] == 'D')
			{
				for (j = 0, i = 0; msBuffer[i] != 0x0d; i++, j++)
				{
					odoData[j] = msBuffer[i + 5];//��$DATA,֮��ʼ��ֵ
				}
				//memset(tempSerialNumSearch, 0, 32);
				for (j = 0; odoData[j] != 0x0d; j++)//�˴������ݽ��н��
				{
					if (0x2c == odoData[j])
					{
						j++;
						flagNum++;
					}
					if (flagNum == 1)//����ˮ��
					{
						tempSerialNumSearch[tempSearch] = odoData[j];
						serialNum = atoi(tempSerialNumSearch);
						tempSearch++;
					}
					if (flagNum == 2)//set mode
					{
						workMode = (int)(odoData[j] - '0');
					}
					if (flagNum == 3)//��odoSpeed
					{
						odoSpeedDataTosend[odoSpeedDataTosend_num] = odoData[j];
						odoSpeedData_num = atoi(odoSpeedDataTosend);
						odoSpeedDataTosend_num++;
					}
					if (flagNum == 4)//��odo��������
					{
						odoTotalDataTosend[odoTotalDataTosend_num] = odoData[j];
						odoTotalData_num = atoi(odoTotalDataTosend);
						odoTotalDataTosend_num++;
					}
					if (flagNum == 5)//��tax_glb
					{

					}
					if (flagNum == 6)//��tax_speed
					{

					}
					if (flagNum == 7)//��gps_lat
					{
						gpsLatDataTosend[gpsLatDataTosend_num] = odoData[j];
						gpsLatData_num = atof(gpsLatDataTosend);
						gpsLatDataTosend_num++;
					}
					if (flagNum == 8)//��gps_lon
					{
						gpsLonDataTosend[gpsLonDataTosend_num] = odoData[j];
						gpsLonData_num = atof(gpsLonDataTosend);
						gpsLonDataTosend_num++;
					}
					if (flagNum == 9)//��gps_speed
					{
						gpsSpeedDataTosend[gpsSpeedDataTosend_num] = odoData[j];
						gpsSpeedData_num = atof(gpsSpeedDataTosend);
						gpsSpeedDataTosend_num++;
					}
				}
				memset(odoTotalDataTosend, 0, 32);
				memset(odoSpeedDataTosend, 0, 10);
				memset(gpsLatDataTosend, 0, 32);
				memset(gpsLonDataTosend, 0, 32);
				memset(gpsSpeedDataTosend, 0, 32);

				global_odoSpeedData = odoSpeedData_num;//����ȫ�ִ��͵�odo�ٶ�ֵ
				global_odoTotalData = odoTotalData_num;
				flagNum = 0;
				odoSpeedDataTosend_num = 0;
				odoTotalDataTosend_num = 0;
				gpsLatDataTosend_num = 0;
				gpsLonDataTosend_num = 0;
				gpsSpeedDataTosend_num = 0;
				tempSearch = 0;

			}
			if (-1 != strStored.Find("$GNRMC"))
			{
				for (j = 0, i = strStored.Find("$GNRMC"); strStored[i] != 0x0d; i++, j++)
					gpsData[j] = msBuffer[i];//�����ȡ����GPS������

			}

			break;
		case 3://TAX+GPS
			   //GPS
			if (-1 != strStored.Find("$GPRMC"))
			{
				for (j = 0, i = strStored.Find("$GPRMC"); strStored[i] != 0x0d; i++, j++)
					gpsData[j] = msBuffer[i + 6];
			}
			//TAX
			if (msBuffer[1] == 'T')
			{
				for (i = 0, j = 0; msBuffer[i] != 0x0d; i++, j++)
				{
					taxData[j] = msBuffer[i + 4];
				}
				for (j = 0; taxData[j] != 0x0d; j++)//�˴������ݽ��н�����жϻس�\r
				{
					if (0x2c == taxData[j])   //�ж϶���
					{
						j++;
						flagNum++;
					}
					if (flagNum == 1)//����ˮ��
					{
						tempSerialNumSearch[tempSearch] = taxData[j];
						serialNum = atoi(tempSerialNumSearch);
						tempSearch++;
					}
					if (flagNum == 2)//��tax_speed
					{
						TaxSpeedTosend[TaxSpeedTosend_num] = taxData[j];
						taxSpeedData_num = atoi(TaxSpeedTosend);
						TaxSpeedTosend_num++;
					}
					if (flagNum == 3)//��tax_pos
					{
						TaxPosTosend[TaxPosTosend_num] = taxData[j];
						taxPosData_num = atoi(TaxPosTosend);
						TaxPosTosend_num++;
					}

				}
				memset(TaxSpeedTosend, 0, 10);
				memset(TaxPosTosend, 0, 40);

				global_taxSpeedData = taxSpeedData_num;//����ȫ�ִ��͵�odo�ٶ�ֵ
				global_taxPosData = taxPosData_num;
				flagNum = 0;
				taxSpeedData_num = 0;
				taxPosData_num = 0;
				tempSearch = 0;
				TaxSpeedTosend_num = 0;
				TaxPosTosend_num = 0;

			}
			break;
		case 4://TAX+ODO+GPS
			break;
		}
	}
	return 0;
}
//add by yjh 161125

BOOL CFSTView::EnumChildProc(HWND hwndChild, LPARAM lParam)
{
	if (CtrlHide == lParam)
	{
		::ShowWindow(hwndChild, SW_HIDE);
	}
	else if (CtrlShow == lParam)
	{
		::ShowWindow(hwndChild, SW_SHOW);
	}
	return TRUE;
}


void CFSTView::OnDraw(CDC* pDC)
{
	CFSTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	CRect rcClip;
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(255, 255, 255));

	if (firstTime == 1)
	{
		// add by shao 2011.03.03
		EnumChildWindows(this->m_hWnd, EnumChildProc, CtrlHide);
		//m_picture.ShowWindow(SW_SHOW);
		GetClientRect(&rcClip);
		pDC->FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom/*-140*/, 0xFFFFFF);
		bSecondTime = true;
		// add end by shao

		CPen myPen3(PS_DOT, 1, RGB(0xE0, 0xE0, 0xFF));
		pDC->SelectObject(&myPen3);
	
// 		for (int i = 0; i < 128; i++)
// 		{
// 			pDC->MoveTo(0, i * 4);
// 			pDC->LineTo(/*1023*/rcClip.Width(), i * 4);
// 		}
// 		for (int i = 1; i < 256; i++)
// 		{
// 			pDC->MoveTo(i * 4, 0);
// 			pDC->LineTo(i * 4, /*508*/rcClip.Height());
// 		}
		

		CFont myFont1, myFont2, *pOldFont;
		myFont1.CreateFont(90, 30, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("����"));
		pOldFont = pDC->SelectObject(&myFont1);

		pDC->SetTextColor(RGB(0x00, 0x60, 0x40));
		//pDC->SetTextColor(RGB(0x00, 0x60, 0x30));

		//for (int i = 1; i < 6; i++)
		//	pDC->TextOut(150 - i, 160 - i, _T("���������е���ǿ����ϵͳ"));

		//pDC->SetTextColor(RGB(0x00, 0xC0, 0xA0));
		pDC->TextOut(150, 160, _T("���������е���ǿ����ϵͳ"));
		pDC->SelectObject(pOldFont);

		GetParent()->SetWindowText(_T("              ��  ��  ��  ��  ��  ��  ��  ǿ  ��  ��  ϵ  ͳ"));

		//myFont2.CreateFont(50, 16, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
		//	OUT_RASTER_PRECIS, (VARIABLE_PITCH | FF_ROMAN) & 0xFFFD, PROOF_QUALITY, 0, _T("����"));
		myFont2.CreateFont(30, 15, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
			OUT_RASTER_PRECIS, (VARIABLE_PITCH | FF_ROMAN) & 0xFFFD, PROOF_QUALITY, 0, _T("����"));

		pOldFont = pDC->SelectObject(&myFont2);


		pDC->SetTextColor(RGB(0x00, 0x80, 0xFF));
		pDC->TextOut(200, 350, _T("���Ƶ�λ��������ͨ��ѧ������Ϣ����ѧԺ"));
		pDC->TextOut(410, 435, _T("2O17��4��"));
		pDC->SelectObject(pOldFont);

		//ReadLineName("�ۺϿ�.txt");
	}
	 	else /*if((offset>0) && (readStatus==1))*/
	 	{
	 		// add by zgliu 2011.03.03
	 		if (bSecondTime)
	 		{
	 			CRect rcDrawRange;
	 			GetDlgItem(IDC_STATIC_RANGE)->GetWindowRect(&rcDrawRange);
	 			ScreenToClient(&rcDrawRange);
	 			EnumChildWindows(this->m_hWnd, EnumChildProc, CtrlShow);
	 			//			m_picture.ShowWindow(FALSE);
	 			GetDlgItem(IDC_INQUIRY_LIST)->ShowWindow(FALSE);
	 			GetClientRect(&rcClip);
	 			pDC->FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom - rcDrawRange.Height()/*140*/, 0xFFFFFF);
	 			bSecondTime = false;
	 			Invalidate(TRUE);
	 		}
	 		// add end by zgliu 
	 
	 		InitScreen();
	 
	 		CPen	myPen11(PS_SOLID, 2, RGB(0x00, 0x00, 0xC0));
	 		CPen	myPen0(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
	 
	 		int	jj = AD_num100 - sectionNum;
	 
	 		pDC->SelectObject(&myPen11);
	 		pDC->MoveTo(nDrawRangeXMin, 410 - (int)AD_value1[jj] * 4);
	 		for (int ii = 1; ii <= sectionNum; ii++)
	 		{
	 			pDC->LineTo(nDrawRangeXMin + (ii*(nPix100m)), 410 - (int)AD_value1[jj + ii - 1] * 4);
	 		}		  
	 		CFont	myFont, *pOldFont;
	 		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
	 			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
	 		pOldFont = pDC->SelectObject(&myFont);
	 
	 
	 		pDC->SetTextColor(RGB(0xFF, 0x00, 0xFF));
	 		pDC->TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
	 		pDC->TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);
	 		if (_T("") != currentName)
	 		{
	 			pDC->TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
	 		}
	 		else
	 		{
	 			pDC->TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
	 		}
			pDC->TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));
	 
	 		DisplayOthers(pDC, startKM, nextKM);
	 
	 		// add by zgliu 2011.04.13 
	 		// ÿ1KM��ʾһ���̶�ֵ
	 		CFont myKMFont;
	 		myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
	 			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
	 		pOldFont = pDC->SelectObject(&myKMFont);
	 		const int nPix1KM = nPix500M * 2;
	 		const int nDeltaKM = offset / nPix1KM;
	 		CString strTempKM;
	 		for (int i = 1; i <= nDeltaKM; ++i)
	 		{
	 			if (i <= (nKMDisplayNum + 1) / 2)
	 			{
	 				if (-1 != m_updown.Find(_T("����")))
	 				{
	 					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
	 				}
	 				else if (-1 != m_updown.Find(_T("����")))
	 				{
	 					strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
	 				}
	 				pDC->SetTextColor(RGB(0x00, 0x00, 0x00));
	 				pDC->TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
	 			}
	 		}
	 		pDC->SelectObject(pOldFont);
	 		// add end by zgliu 
	 
	 
	 		pDC->SelectObject(&myPen0);
	 		pDC->MoveTo(nDrawRangeXMin + offset / 2, 445);
	 		pDC->LineTo(nDrawRangeXMin + offset / 2, 450);
	 		pDC->MoveTo(nDrawRangeXMin + offset, 50);
	 		pDC->LineTo(nDrawRangeXMin + offset, 450);
	 
	 		pDC->SelectObject(pOldFont);
	 	}
}


//void CFSTView::variableInti()
//{
//	
//}


void CFSTView::OnBnClickedStaticRange()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CFSTView::OnFileOpen()
{
	// TODO: �ڴ���������������
	if (caiji_status)
	{
		MessageBox(_T("����ֹͣ����!"), _T("��������"));
		return;
	}
	if (firstTime == 1)
	{
		CClientDC pDC(this);
		CRect rcClip;
		GetClientRect(&rcClip);
		pDC.FillSolidRect(rcClip.left, rcClip.top, rcClip.right, rcClip.bottom - 140, 0xFFFFFF);
		firstTime = 0;
	}
	CFileDialog dlgFile(TRUE, "TXT", filename, OFN_HIDEREADONLY | OFN_NOCHANGEDIR, "(TXT) *.txt |*.txt||", this);
	CString strTemp = m_strCurDirectory.c_str();
	strTemp += _T("\\������ݿ�");
	dlgFile.m_ofn.lpstrInitialDir = strTemp;
	if (dlgFile.DoModal() == IDOK)
	{
		CString	tmp_name = dlgFile.GetPathName();
		CString	tt_name = dlgFile.GetFileName();

		int	pos = tmp_name.Find(tt_name);
		dirName = tmp_name.Left(pos);
		int gpscorcount = ReadGPSCor(tmp_name);
				if (gpscorcount > 0)
					gpscor_flag = 1;
				else
					gpscor_flag = 0;
		
		int count = ReadLib(tmp_name);
		if (count > 0)
		{
			stationCount = count;
			filename = tmp_name;

			librayDlg.stationCount = stationCount + zhongjiCount + diantaiCount + suidaoCount + zhifangCount + qujiantaiCount;
			librayDlg.filename = filename;

			int  index = 0;
			for (int kk = 1; kk <= stationCount; kk++)
			{
				index++;
				strcpy(librayDlg.stationName[kk], stationName[index]);
				librayDlg.stationDis[kk] = stationDis[index];
				librayDlg.stationNum[kk] = stationNum[index];
				librayDlg.type[kk] = 0;
			}

			index = 0;
			for (int kk = stationCount + 1; kk <= stationCount + zhongjiCount; kk++)
			{
				index++;
				strcpy(librayDlg.stationName[kk], zhongjiName[index]);
				librayDlg.stationDis[kk] = zhongjiDis[index];
				librayDlg.stationNum[kk] = zhongjiCode[index];
				librayDlg.type[kk] = 1;

			}

			index = 0;
			for (int kk = stationCount + zhongjiCount + 1; kk <= stationCount + zhongjiCount + diantaiCount; kk++)
			{
				index++;
				strcpy(librayDlg.stationName[kk], diantaiName[index]);
				librayDlg.stationDis[kk] = diantaiDis[index];
				librayDlg.stationNum[kk] = diantaiCode[index];
				librayDlg.type[kk] = 2;

			}

			index = 0;
			for (int kk = stationCount + zhongjiCount + diantaiCount + 1; kk <= stationCount + zhongjiCount + diantaiCount + suidaoCount; kk++)
			{
				index++;
				strcpy(librayDlg.stationName[kk], suidaoName[index]);
				librayDlg.stationDis[kk] = suidaoDis[index];
				librayDlg.stationNum[kk] = 0;
				librayDlg.type[kk] = 3;

			}

			index = 0;
			int nTempNum = stationCount + zhongjiCount + diantaiCount + suidaoCount;
			for (int kk = nTempNum + 1; kk <= nTempNum + zhifangCount; ++kk)
			{
				index++;
				strcpy(librayDlg.stationName[kk], zhifangName[index]);
				librayDlg.stationDis[kk] = zhifangDis[index];
				librayDlg.stationNum[kk] = zhifangCode[index];
				librayDlg.type[kk] = 4;
			}

			index = 0;
			nTempNum = stationCount + zhongjiCount + diantaiCount + suidaoCount + zhifangCount;
			for (int kk = nTempNum + 1; kk <= nTempNum + qujiantaiCount; ++kk)
			{
				index++;
				strcpy(librayDlg.stationName[kk], qujiantaiName[index]);
				librayDlg.stationDis[kk] = qujiantaiDis[index];
				librayDlg.stationNum[kk] = qujiantaiCode[index];
				librayDlg.type[kk] = 5;
			}




			if (librayDlg.DoModal() == IDOK)
			{
				stationCount = 0;
				zhongjiCount = 0;
				diantaiCount = 0;
				suidaoCount = 0;
				zhifangCount = 0;
				qujiantaiCount = 0;


				for (int ii = 0; ii < 512; ii++)
				{
					strcpy(stationName[ii], "");
					stationDis[ii] = 0.0;
					stationNum[ii] = 0;

					strcpy(zhongjiName[ii], "");
					zhongjiDis[ii] = 0.0;
					zhongjiCode[ii] = 0;

					strcpy(diantaiName[ii], "");
					diantaiDis[ii] = 0.0;
					diantaiCode[ii] = 0;

					strcpy(suidaoName[ii], "");
					suidaoDis[ii] = 0.0;

					strcpy(zhifangName[ii], "");
					zhifangDis[ii] = 0.0;
					zhifangCode[ii] = 0;

					strcpy(qujiantaiName[ii], "");
					qujiantaiDis[ii] = 0.0;
					qujiantaiCode[ii] = 0;
				}

				for (int kk = 1; kk <= librayDlg.stationCount; kk++)
				{
					if (librayDlg.type[kk] == 0)
					{
						stationCount++;
						strcpy(stationName[stationCount], librayDlg.stationName[kk]);
						stationDis[stationCount] = librayDlg.stationDis[kk];
						stationNum[stationCount] = librayDlg.stationNum[kk];
					}
					else if (librayDlg.type[kk] == 1)
					{
						zhongjiCount++;
						strcpy(zhongjiName[zhongjiCount], librayDlg.stationName[kk]);
						zhongjiDis[zhongjiCount] = librayDlg.stationDis[kk];
						zhongjiCode[zhongjiCount] = librayDlg.stationNum[kk];
					}
					else if (librayDlg.type[kk] == 2)
					{
						diantaiCount++;
						strcpy(diantaiName[diantaiCount], librayDlg.stationName[kk]);
						diantaiDis[diantaiCount] = librayDlg.stationDis[kk];
						diantaiCode[diantaiCount] = librayDlg.stationNum[kk];
					}
					else if (librayDlg.type[kk] == 3)
					{
						suidaoCount++;
						strcpy(suidaoName[suidaoCount], librayDlg.stationName[kk]);
						suidaoDis[suidaoCount] = librayDlg.stationDis[kk];
					}
					else if (4 == librayDlg.type[kk])
					{
						zhifangCount++;
						strcpy(zhifangName[zhifangCount], librayDlg.stationName[kk]);
						zhifangDis[zhifangCount] = librayDlg.stationDis[kk];
						zhifangCode[zhifangCount] = librayDlg.stationNum[kk];
					}
					else if (5 == librayDlg.type[kk])
					{
						qujiantaiCount++;
						strcpy(qujiantaiName[qujiantaiCount], librayDlg.stationName[kk]);
						qujiantaiDis[qujiantaiCount] = librayDlg.stationDis[kk];
						qujiantaiCode[qujiantaiCount] = librayDlg.stationNum[kk];
					}

				}

				currentSel = librayDlg.selNum;

				if (currentSel > 0)
				{
					m_distance.Format("%.2f", librayDlg.stationDis[currentSel]);
					int	len = tt_name.GetLength();

					m_line = tt_name.Left(len - 4);
					m_filename = tt_name.Left(len - 6) + "000.dat";
					m_filenum = 0;
					UpdateData(FALSE);

					//		OnSelchangeLineCombo() ;

					OnChangeDistanceEdit();
					InitScreen();

					//		MessageBox("��ѡ�������ļ���ţ�",NULL, MB_ICONEXCLAMATION);
					OnStatusMenu();
				}
			}
			//add by zgliu 2011.03.06
			Invalidate(TRUE);

		}
		else if (count == 0)
		{
			MessageBox("�����ļ���", NULL, MB_ICONERROR);
			return;
		}
		Sleep(200);
		InitScreen();	 //2005-6-2

						 // add by zgliu 2011.03.10
		m_bReadLib = true;
	}
}


int	CFSTView::ReadLib(CString fname)
{
	//starts from the index 1
	char	title[40];
	FILE	*fp;

	fp = fopen(fname, "r+t");

	if (fp == NULL)
		return 0;

	stationCount = 0;
	zhongjiCount = 0;
	diantaiCount = 0;
	suidaoCount = 0;
	zhifangCount = 0;
	qujiantaiCount = 0;

	for (int ii = 0; ii < 512; ii++)
	{
		strcpy(stationName[ii], "");
		stationDis[ii] = 0.0;
		stationNum[ii] = 0;

		strcpy(zhongjiName[ii], "");
		zhongjiDis[ii] = 0.0;
		zhifangCode[ii] = 0;

		strcpy(diantaiName[ii], "");
		diantaiDis[ii] = 0.0;
		diantaiCode[ii] = 0;

		strcpy(suidaoName[ii], "");
		suidaoDis[ii] = 0.0;

		strcpy(zhifangName[ii], "");
		zhifangDis[ii] = 0.0;
		zhifangCode[ii] = 0;

		strcpy(qujiantaiName[ii], "");
		qujiantaiDis[ii] = 0.0;
		qujiantaiCode[ii] = 0;
	}

	//First Line is title
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);

	char		name[12];
	double	dis = 0.0 - 1.0;
	int       num = 0 - 1;
	int       type = 0 - 1;

	while (!feof(fp))
	{
		strcpy(name, "");
		dis = 0.0 - 1.0;
		num = 0 - 1;
		type = 0 - 1;

		fscanf(fp, "%s", name);
		fscanf(fp, "%lf", &dis);
		fscanf(fp, "%d", &num);
		fscanf(fp, "%d", &type);

		//		char  pp[90];
		//	    sprintf(pp,"%s  %lf  %d  %d",name,dis,num,type);
		//		MessageBox(pp);

		//Station
		if (type == 0)
		{
			stationCount++;
			strcpy(stationName[stationCount], name);
			stationDis[stationCount] = dis;
			stationNum[stationCount] = num;
		}
		else if (type == 1)    //Zhongji
		{
			zhongjiCount++;
			strcpy(zhongjiName[zhongjiCount], name);
			zhongjiDis[zhongjiCount] = dis;
			zhongjiCode[zhongjiCount] = num;
		}
		else if (type == 2)   	//DianTai
		{
			diantaiCount++;
			strcpy(diantaiName[diantaiCount], name);
			diantaiDis[diantaiCount] = dis;
			diantaiCode[diantaiCount] = num;
		}
		else if (type == 3)  	//SuiDao
		{
			suidaoCount++;
			strcpy(suidaoName[suidaoCount], name);
			suidaoDis[suidaoCount] = dis;
		}
		else if (4 == type) //ZhiFangZhan
		{
			zhifangCount++;
			strcpy(zhifangName[zhifangCount], name);
			zhifangDis[zhifangCount] = dis;
			zhifangCode[zhifangCount] = num;
		}
		else if (5 == type) //QuJianTai
		{
			qujiantaiCount++;
			strcpy(qujiantaiName[qujiantaiCount], name);
			qujiantaiDis[qujiantaiCount] = dis;
			qujiantaiCode[qujiantaiCount] = num;
		}
	}
	fclose(fp);

	return stationCount;
}


void CFSTView::OnStatusMenu()
{
	// TODO: �ڴ���������������
	if (UpdateData(TRUE))
	{
		if (m_line.GetLength() < 6)
		{
			MessageBox("����ѡ����·!", "��������", MB_ICONEXCLAMATION);
			return;
		}

		char  pp[180];
		int   len = m_line.GetLength();
		sprintf(pp, "%s%s???.dat", dirName, m_line.Left(len - 2));

		m_inquiry_list.ResetContent();
		if (DlgDirList(pp, m_inquiry_list.GetDlgCtrlID(), 0, DDL_READWRITE) == 0)
		{
			MessageBox("����·�޲�������!", "", MB_ICONEXCLAMATION);
			UpdateData(FALSE);
			return;
		}

		int totalFiles = m_inquiry_list.GetCount();
		if (totalFiles < 1)
		{
			MessageBox("����·�޲�������!", NULL, MB_ICONEXCLAMATION);
			UpdateData(FALSE);
			return;
		}


		if (m_inquiry_list.GetCount()>0)
		{
			int totalFiles = m_inquiry_list.GetCount();

			CString curFilename = _T("");
			int		i = 0, dataLength = 0;
			FILE	*fp;
			char	pp[80];
			int		itemNum = 0;

			for (int index = 0; index < totalFiles; index++)
			{
				m_inquiry_list.GetText(index, curFilename);

				if (index == totalFiles - 1)
				{
					CString  tt1 = _T("");
					CString  tt2 = _T("");
					curFilename.MakeUpper();
					int Pos = curFilename.Find(".DAT");
					curFilename.MakeLower();
					int fNum = atoi(curFilename.Mid(Pos - 3, Pos));

					tt1.Format("%03d.dat", fNum + 1);
					tt2 = curFilename.Left(Pos - 3);

					m_filenum = fNum + 1;
					m_filename.Format("%s%s", tt2, tt1);
					UpdateData(FALSE);
				}


				if ((fp = fopen(curFilename, "r+b")) != NULL)
				{
					dataLength = fread(tmpData, sizeof(dataType), 30720, fp);

					if (tmpData[0].curPos > tmpData[1].curPos)
					{
						sprintf(pp, "%13s    ����    %6.1f ---> %-6.1f", curFilename, tmpData[0].curPos / 1000.0, tmpData[dataLength - 1].curPos / 1000.0);
						strcpy(inquiryDlg.item[itemNum++], pp);
					}

					else if (tmpData[0].curPos < tmpData[1].curPos)
					{
						sprintf(pp, "%13s    ����    %6.1f ---> %-6.1f", curFilename, tmpData[0].curPos / 1000.0, tmpData[dataLength - 1].curPos / 1000.0);
						strcpy(inquiryDlg.item[itemNum++], pp);
					}
					fclose(fp);
				}
			}  //end of for

			inquiryDlg.itemNum = itemNum;
			if (itemNum > 0)
				inquiryDlg.DoModal();
		}
	}
}


void CFSTView::OnChangeDistanceEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE) && m_updown.GetLength() > 3)
	{
		CClientDC pDC(this);
		//???	pDC.SetROP2(R2_XORPEN);
		pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
		CPen	myPen1(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
		CPen	*pOldPen;
		pOldPen = pDC.SelectObject(&myPen1);

		CFont	myFont, *pOldFont;
		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myFont);


		if (offset > 0)
		{
			pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
			pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
			pDC.MoveTo(nDrawRangeXMin + offset, 50);
			pDC.LineTo(nDrawRangeXMin + offset, 450);
		}

		pDC.FillSolidRect(nDrawRangeXMin - 20, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

		offset = 0;

		startDis = atof(m_distance)*1000.0;
		startKM = m_distance;
		nextKM = _T("");

		currentStation = 0 - 1;
		currentName = _T("");
		nextName = _T("");
		currentCode = _T("");
		nextCode = _T("");

		firstMapped = 0;
		const int nPix1KM = nPix500M * 2;  //ÿ30Pix��ʾ500M add by zgliu 2011.04.13

		for (int i = 1; i <= stationCount; i++)
		{
			if (fabs(stationDis[i] * 1000.0 - startDis) < 100.0)
			{
				currentStation = i;
				currentName = CString(stationName[i]);
				currentCode.Format("%d", stationNum[i]);

				if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i + 1]);
					nextName = CString(stationName[i + 1]);
					nextCode.Format("%d", stationNum[i + 1]);
					nextStation = i + 1;
// 					KFnextstation = i + 1;  // add by shao 2011.03.08
					offset = (int)((stationDis[i + 1] - stationDis[i])*nPix1KM);
					delta = 1;
				}
				else if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i - 1]);
					nextName = CString(stationName[i - 1]);
					nextCode.Format("%d", stationNum[i - 1]);
					nextStation = i - 1;
// 					KFnextstation = i - 1;  // add by shao 2011.03.08
					offset = (int)((stationDis[i] - stationDis[i - 1])*nPix1KM);
					delta = 0 - 1;
				}

				firstMapped = 1;
				i = stationCount + 1;
			}

		}

		if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = 1; i <= stationCount; i++)
				if (stationDis[i] * 1000.0 > startDis)
				{
					currentStation = i - 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("%d", stationNum[i]);
					nextStation = i;
// 					KFnextstation = i;   // add by shao 2011.03.08
					offset = (int)((stationDis[i] - atof(startKM))*nPix1KM);
					delta = 1;
					i = stationCount + 1;
				}
		}
		else if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = stationCount; i >= 1; i--)
			{
				if (stationDis[i] * 1000.0 < startDis)
				{
					currentStation = i + 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("%d", stationNum[i]);
					nextStation = i;
// 					KFnextstation = i;  // add by shao 2011.03.08
					offset = (int)((atof(startKM) - stationDis[i])*nPix1KM);
					delta = 0 - 1;
					i = 0;
				}
			}
		}

		pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);

		if (_T("") != currentName)
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
		}
		else
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
		}
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));

		DisplayOthers(&pDC, startKM, nextKM);

		// add by zgliu 2011.04.13 
		// ÿ1KM��ʾһ���̶�ֵ
		CFont myKMFont;
		myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myKMFont);
		const int nDeltaKM = offset / nPix1KM;
		CString strTempKM;
		for (int i = 1; i <= nDeltaKM; ++i)
		{
			if (i <= (nKMDisplayNum + 1) / 2)
			{
				if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
				}
				else if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
				}
				pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
				pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
			}
		}
		pDC.SelectObject(pOldFont);
		// add end by zgliu 


		if (offset > 0)
		{
			pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
			pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
			pDC.MoveTo(nDrawRangeXMin + offset, 50);
			pDC.LineTo(nDrawRangeXMin + offset, 450);
		}

		pDC.SelectObject(pOldPen);
		pDC.SelectObject(pOldFont);

	}
}


void CFSTView::OnSelchangeUpdownCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE) && m_updown.GetLength() > 3)
	{
		CClientDC pDC(this);
		//pDC.SetROP2(R2_XORPEN);
		pDC.SetTextColor(RGB(0xFF, 0x00, 0xFF));
		CPen	myPen1(PS_SOLID, 1, RGB(0xFF, 0x00, 0xFF));
		CPen	*pOldPen;
		pOldPen = pDC.SelectObject(&myPen1);

		CFont	myFont, *pOldFont;
		myFont.CreateFont(18, 6, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myFont);

		if (offset > 0)
		{
			pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
			pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
			pDC.MoveTo(nDrawRangeXMin + offset, 50);
			pDC.LineTo(nDrawRangeXMin + offset, 450);
		}

		pDC.FillSolidRect(nDrawRangeXMin - 20, 455, nDrawRangeXMax - nDrawRangeXMin + 20, 55, 0xFFFFFF);

		offset = 0;

		startDis = atof(m_distance)*1000.0;
		startKM = m_distance;
		nextKM = _T("");

		currentStation = 0 - 1;
		currentName = _T("");
		nextName = _T("");
		currentCode = _T("");
		nextCode = _T("");

		firstMapped = 0;
		const int nPix1KM = nPix500M * 2;  //ÿ30Pix��ʾ500M add by zgliu 2011.04.13

		for (int i = 1; i <= stationCount; i++)
		{
			if (fabs(stationDis[i] * 1000.0 - startDis) < 100.0)
			{
				currentStation = i;
				currentName = CString(stationName[i]);
				currentCode.Format("%d", stationNum[i]);

				if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i + 1]);
					nextName = CString(stationName[i + 1]);
					nextCode.Format("%d", stationNum[i + 1]);
					nextStation = i + 1;
// 					KFnextstation = i + 1;
					offset = (int)((stationDis[i + 1] - stationDis[i])*nPix1KM);
					delta = 1;
				}
				else if (m_updown.Find("����") != 0 - 1)
				{
					nextKM.Format("%6.2f", stationDis[i - 1]);
					nextName = CString(stationName[i - 1]);
					nextCode.Format("%d", stationNum[i - 1]);
					nextStation = i - 1;
// 					KFnextstation = i - 1;
					offset = (int)((stationDis[i] - stationDis[i - 1])*nPix1KM);
					delta = 0 - 1;
				}

				firstMapped = 1;
				i = stationCount + 1;
			}

		}

		if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = 1; i <= stationCount; i++)
				if (stationDis[i] * 1000.0 > startDis)
				{
					currentStation = i - 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("%d", stationNum[i]);
					nextStation = i;
// 					KFnextstation = i;
					offset = (int)((stationDis[i] - atof(startKM))*nPix1KM);
					delta = 1;
					i = stationCount + 1;
				}
		}
		else if ((firstMapped == 0) && (m_updown.Find("����") != 0 - 1))
		{
			for (int i = stationCount; i >= 1; i--)
				if (stationDis[i] * 1000.0 < startDis)
				{
					currentStation = i + 1;
					currentName = _T("");
					currentCode = _T("");

					nextKM.Format("%6.2f", stationDis[i]);
					nextName = CString(stationName[i]);
					nextCode.Format("%d", stationNum[i]);
					nextStation = i;
// 					KFnextstation = i;
					offset = (int)((atof(startKM) - stationDis[i])*nPix1KM);
					delta = 0 - 1;
					i = 0;
				}
		}

		pDC.TextOut(nDrawRangeXMin - 5 - 15, 460 + 10, startKM);
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 460 + 10, nextKM);

		if (_T("") != currentName)
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, currentName + _T(" (") + currentCode + _T(")"));
		}
		else
		{
			pDC.TextOut(nDrawRangeXMin - 5 - 15, 480 + 10, _T(""));
		}
		pDC.TextOut(nDrawRangeXMin - 5 + offset, 480 + 10, nextName + _T(" (") + nextCode + _T(")"));

		DisplayOthers(&pDC, startKM, nextKM);

		// add by zgliu 2011.04.13 
		// ÿ1KM��ʾһ���̶�ֵ
		CFont myKMFont;
		myKMFont.CreateFont(15, 5, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, "ROMAN");
		pOldFont = pDC.SelectObject(&myKMFont);
		const int nDeltaKM = offset / nPix1KM;
		CString strTempKM;
		for (int i = 1; i <= nDeltaKM; ++i)
		{
			if (i <= (nKMDisplayNum + 1) / 2)
			{
				if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
				}
				else if (-1 != m_updown.Find(_T("����")))
				{
					strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
				}
				pDC.SetTextColor(RGB(0x00, 0x00, 0x00));
				pDC.TextOut(nDrawRangeXMin - 8 + nPix1KM*i, 455, strTempKM);
			}
		}
		pDC.SelectObject(pOldFont);
		// add end by zgliu 


		if (offset > 0)
		{
			pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
			pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
			pDC.MoveTo(nDrawRangeXMin + offset, 50);
			pDC.LineTo(nDrawRangeXMin + offset, 450);
		}

		pDC.SelectObject(pOldPen);
		pDC.SelectObject(pOldFont);

	}
}


void CFSTView::OnSelchangeMaintanceCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString	preStr = m_maintance;

	UpdateData(TRUE);

	//	MessageBox(preStr+m_maintance);

	if (preStr != m_maintance)
		InitScreen();
}


void CFSTView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	if (1 == nIDEvent)
	{
		int ii, loopcount, Delta;	// 0-pNMUpDown->iDelta;
		double tmpdis;

		if (gpscapture_flag)
		{
			tmpdis = gpscaptureDis * 1000;
			if (currentDis>tmpdis)
			{
				loopcount = (int)((currentDis - tmpdis) / 100);
				if (delta>0)
					Delta = -1;
				else
					Delta = 1;
			}
			else
			{
				loopcount = (int)((tmpdis - currentDis) / 100);
				if (delta>0)
					Delta = 1;
				else
					Delta = -1;
			}

			if (loopcount>0)
			{
				for (ii = 0; ii < loopcount; ii++)
				{
					if ((Delta<0) && (AD_num100>0) && (sectionNum>0))
					{
						currentDis = currentDis - 100.0*delta;
						AD_number = AD_number - pulse100M;
						AD_num100--;
						sectionNum--;
					}
					else if ((Delta>0) && (AD_num100>0) && (fabs(currentDis - atof(nextKM)*1000.0)>100.0))
					{
						currentDis = currentDis + 100.0*delta;
						AD_number = AD_number + pulse100M;

						AD_value1[AD_num100] = AD_value1[AD_num100 - 1];
						data1[AD_num100].AD_95 = data1[AD_num100 - 1].AD_95;
						data1[AD_num100].AD_90 = data1[AD_num100 - 1].AD_90;
						data1[AD_num100].AD_50 = data1[AD_num100 - 1].AD_50;
						data1[AD_num100].curPos = data1[AD_num100 - 1].curPos + delta * 100;
						////////////////////////////////////////////////////////
						AD_num100++;
						sectionNum++;
					}
				}
			}
			gpscapture_flag = 0;	//reset gpscapture flag after processing
		}
	}
	
	//GPS���ִ��룬�˴���ע�� ��Edit by zwbai 170104
	if (tagAutoSave == nIDEvent)
	{
		//	ADdataAutoSave();   //���������ļ��Զ����� ��������5mins
		if (readStatus == 1)
		{
			OnSavedataButton();
			/*
			n_ontimer++;					//������   edit by bzw 161125
			CString error_ontimer;
			error_ontimer.Format("OnSavedataButton();���ô�����%i��,readStatus:%i",n_ontimer,readStatus);
			MessageBox(error_ontimer);
			*/
		}
	}
	CFormView::OnTimer(nIDEvent);
}

int	CFSTView::ReadLineName(CString fname)
{
	int		kk = 0;
	char		head[20];
	//	  CComboBox *nameCombo;
	FILE		*fp;

	fp = fopen(fname, "r+t");

	if (fp == NULL)
		return 0;

	fscanf(fp, "%s", head);	//Line name
	fscanf(fp, "%s", head);	//File name

							//	  nameCombo=(CComboBox*)GetDlgItem(IDC_LINE_COMBO);
							// 	  nameCombo->ResetContent( );

	while ((kk < 256) && !feof(fp))
	{
		fscanf(fp, "%s", lineName[kk]);
		fscanf(fp, "%s", lineFile[kk]);
		//		nameCombo->AddString(lineName[kk]);
		kk++;
	}
	fclose(fp);

	//    nameCombo->SetCurSel(0);
	m_line = lineName[0];
	m_filename = CString(lineFile[0]) + "000.dat";
	UpdateData(FALSE);
	return kk;
}

bool CFSTView::InitMode()
{
	
	return false;
}


void CFSTView::OnUpdateStartButton(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetCheck(caiji_status);

}


void CFSTView::OnDeltaposDiameterSet(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if (UpdateData(TRUE))
	{
		int Delta = 0 - pNMUpDown->iDelta;
		if (Delta < 0)
		{
			float temp;
			temp = atof(m_diameter);
			temp = temp - 0.1;
			m_diameter.Format("%0.1f", temp);
			unit = atof(m_diameter)*pi*0.001 / pulse;
			pulse100M = (int)(100.0 / unit + 0.5);
			pulse95 = (int)(5.0 / unit + 0.5);
			pulse90 = (int)(10.0 / unit + 0.5);
			pulse50 = (int)(50.0 / unit + 0.5);
			UpdateData(FALSE);
		}
		else if (Delta > 0)
		{
			float temp;
			temp = atof(m_diameter);
			temp = temp + 0.1;
			m_diameter.Format("%0.1f", temp);
			unit = atof(m_diameter)*pi*0.001 / pulse;
			pulse100M = (int)(100.0 / unit + 0.5);
			pulse95 = (int)(5.0 / unit + 0.5);
			pulse90 = (int)(10.0 / unit + 0.5);
			pulse50 = (int)(50.0 / unit + 0.5);
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}


void CFSTView::OnDeltaposModifySpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))
	{
		int Delta = 0 - pNMUpDown->iDelta;

		if ((Delta < 0) && (AD_num100>0) && (sectionNum > 0))
		{
			currentDis = currentDis - 100.0*delta;
			AD_number = AD_number - pulse100M;
			AD_num100--;
			sectionNum--;
			m_modify -= 100;
			//			UpdateData(FALSE);
			// add by zgliu 2011.03.03
			CString strEidtValue;
			strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
			SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
			strEidtValue.Format(_T("%d"), m_modify);
			SetDlgItemText(IDC_MODIFY_EDIT, strEidtValue);
			// add end by zgliu 
		}
		else if ((Delta > 0) && (AD_num100 > 0) && (fabs(currentDis - atof(nextKM)*1000.0) > 100.0))
		{
			currentDis = currentDis + 100.0*delta;
			AD_number = AD_number + pulse100M;

			AD_value1[AD_num100] = AD_value1[AD_num100 - 1];
			data1[AD_num100].AD_95 = data1[AD_num100 - 1].AD_95;
			data1[AD_num100].AD_90 = data1[AD_num100 - 1].AD_90;
			data1[AD_num100].AD_50 = data1[AD_num100 - 1].AD_50;
			data1[AD_num100].curPos = data1[AD_num100 - 1].curPos + delta * 100;
			////////////////////////////////////////////////////////
			AD_num100++;
			sectionNum++;
			m_modify += 100;
			//			UpdateData(FALSE);
			// add by zgliu 2011.03.03
			CString strEidtValue;
			strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
			SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
			strEidtValue.Format(_T("%d"), m_modify);
			SetDlgItemText(IDC_MODIFY_EDIT, strEidtValue);
			// add end by zgliu 
		}
	}
	Invalidate(TRUE);
	//Invalidate();
	*pResult = 0;
}


void CFSTView::OnDeltaposDbbcSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))
	{
		int Delta = 0 - pNMUpDown->iDelta;
		if (Delta < 0)
		{
			m_dbbcvalue -= 1.0;
			db_value--;
			UpdateData(FALSE);
		}
		else if (Delta>0)
		{
			m_dbbcvalue += 1.0;
			db_value++;
			UpdateData(FALSE);
		}

	}
	*pResult = 0;
}


void CFSTView::OnDeltaposSavefileSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))
	{
		CString	tmp;
		char	pp[40];
		int		Delta = 0 - pNMUpDown->iDelta;

		int	strLen = m_filename.GetLength();
		if (strLen > 7)
		{
			tmp = m_filename.Mid(0, strLen - 7);
			if ((m_filenum > 0) && (Delta < 0))
			{
				m_filenum = m_filenum - 1;
				sprintf(pp, "%03d", m_filenum);
				m_filename = tmp + pp + ".dat";
				UpdateData(FALSE);
			}
			else if (Delta > 0)
			{
				m_filenum = m_filenum + 1;
				sprintf(pp, "%03d", m_filenum);
				m_filename = tmp + pp + ".dat";
				UpdateData(FALSE);
			}
		}
	}
	*pResult = 0;
}


void CFSTView::OnDeltaposSpeedSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))
	{
		int Delta = 0 - pNMUpDown->iDelta;

		if ((Delta < 0) && (m_speed>30))
		{
			m_speed -= 10;
			UpdateData(FALSE);
		}
		else if ((Delta > 0) && (m_speed < 200))
		{
			m_speed += 10;
			UpdateData(FALSE);
		}
	}

	*pResult = 0;
}


void CFSTView::OnDeltaposLossdbSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (UpdateData(TRUE))
	{
		int Delta = 0 - pNMUpDown->iDelta;

		if ((Delta < 0) && (m_lossdb>0 - 10))
		{
			m_lossdb -= 1;
			loss_voltage = (float)m_lossdb;
			UpdateData(FALSE);
		}
		else if ((Delta > 0) && (m_lossdb < 30))
		{
			m_lossdb += 1;
			loss_voltage = (float)m_lossdb;
			UpdateData(FALSE);
		}
	}
	*pResult = 0;
}


int CFSTView::ReadGPSCor(CString fname)
{
	//starts from the index 1
	char	title[40];
	CString gpscorfname, tmpprompt;
	FILE	*fp;

	fname.MakeUpper();
	int pos = fname.Find(".TXT");
	gpscorfname = fname.Left(pos) + ".COR";

	fp = fopen(gpscorfname, "rt");

	if (fp == NULL)
	{
		tmpprompt.Format("ע�⣺�����ڸ��ߵ�GPS�����ļ����Կɽ����������ԡ�");
		MessageBox(tmpprompt, _T("����"), MB_ICONWARNING);
		return 0;
	}

	tmpprompt.Format("���ڸ��ߵ�GPS�����ļ�%s����������GPS��Ϣ��������Զ�У����", gpscorfname);
	MessageBox(tmpprompt, _T("��Ϣ"), MB_ICONINFORMATION);

	gpscorCount = 0;

	for (int ii = 0; ii<512; ii++)
	{
		strcpy(gpsName[ii], "");
		gpsDis[ii] = 0.0;
		gpsLat[ii] = 0.0;
		gpsLon[ii] = 0.0;
		gpsmmflag[ii] = 0;
	}

	//First Line is title
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);
	fscanf(fp, "%s", title);


	char		name[12];
	double	dis = 0.0 - 1.0;
	double	lat = 0.0;
	double	lon = 0.0;

	while (!feof(fp))
	{
		strcpy(name, "");
		dis = 0.0 - 1.0;
		lat = 0.0;
		lon = 0.0;

		fscanf(fp, "%s", name);
		fscanf(fp, "%0.lf", &dis);
		fscanf(fp, "%0.lf", &lat);
		fscanf(fp, "%0.lf", &lon);
		strcpy(gpsName[gpscorCount], name);
		gpsDis[gpscorCount] = dis;
		gpsLat[gpscorCount] = lat;
		gpsLon[gpscorCount] = lon;
		/*
		char  pp[90];
		sprintf(pp,"%s  %lf  %lf  %lf\n%s  %lf  %lf  %lf ",
		name,dis,lat,lon, gpsName[gpscorCount], gpsDis[gpscorCount],
		gpsLat[gpscorCount], gpsLon[gpscorCount]);
		MessageBox(pp);
		*/
		gpscorCount++;

	}
	fclose(fp);
	m_gpscapture.Format("��ֵ%dm����%d��У����", GPSThreshold, gpscorCount);
	UpdateData(FALSE);

	return gpscorCount;
}
