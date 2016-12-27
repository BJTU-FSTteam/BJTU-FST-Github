
// FSTView.cpp : CFSTView 类的实现
//
#pragma once
#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FST.h"
#endif

#include "FSTDoc.h"
#include "FSTView.h"
#include "PR100Setting.h"
#include "MainFrm.h"
#include "math.h"
#include <stdio.h>
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
/*********************此处定义全局常量并添加注释********************/   //edit by zwbai 161221
#define nDrawRangeXMin 65
#define nDrawRangeXMax 965
#define nPix500M 30				//本版程序中每30个像素点表示500米
#define nKMDisplayNum 29		//每次画14km:((840Pix/60)*28)
const int nPix100m = 6;
/* Zhou's algorithm*/
#define PI1 3.14159265353846		//PI=3.14159265359;
//WGS84 椭圆参数及转换参数
const double A1 = 6367449.1458;
const double A2 = -16038.508688;
const double A3 = 16.8326;
const double A4 = -0.0220;
const double a = 6378140;
const double f = 298.257;
//unsigned long 32bit

/*********************以上定义全局常量并添加注释********************/

/*********************此处定义全局变量并添加注释********************/   //edit by zwbai 161221
int		Pr100ProcFlag = 0;			//Pr100接收线程工作控制变量  
float	db_value = 0.0;			//场强补偿值
double	unit;					//里程计算相关变量
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
float	AD_value1[30720];//InitScreen中包含
dataType	data1[30720];
int controlstatus = 0;	//0--local control, 1--remote control
struct fusiondataType
{
	int curpos;
	unsigned long pulsenum;
	double lat;
	double lon;
} fusiondata[30720];
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

struct position
{
	double x;
	double y;
};
//坐标转换
struct position BL2xy(double B, double L, double L0)
{
	double e2;
	e2 = 1 - ((f - 1) / f) * ((f - 1) / f);
	double b = B*PI1 / 180.0;
	double l = L*PI1 / 180.0;
	double l0 = L0*PI1 / 180.0;
	double X;
	X = A1 * b + A2 * sin(2 * b) + A3 * sin(4 * b) + A4 * sin(6 * b);
	//子午线的弧长

	double sinb = sin(b);
	double cosb = cos(b);
	double t = tan(b);//辅助变量
	double t2 = t * t;
	double N = a / sqrt(1 - e2 * sinb * sinb);
	double m = cosb * (l - l0);
	double m2 = m * m;
	double ng2 = cosb * cosb * e2 / (1 - e2);//辅助变量
	double x = X + N * t * ((0.5 + ((5 - t2 + 9 * ng2 + 4 * ng2 * ng2) / 24.0 + (61 - 58 * t2 + t2 * t2 + 270 * ng2 - 330 * t2*ng2) * m2 / 720.0) * m2) * m2);
	double  y = N * m * (1 + m2 * ((1 - t2 + ng2) / 6.0 + m2 * (5 - 18 * t2 + t2 * t2 + 14 * ng2 - 58 * ng2 * t2) / 120.0));
	y = y + 500000;
	struct position pp;
	pp.x = x;
	pp.y = y;
	return pp;
}
//两点之间的距离计算
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
	//pr100场强值转换（乌鲁木齐局）by bzw 161116
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
	//本版程序中，每30Pix表示500M
	const int nPix1KM = nPix500M * 2;  //1KM的像素点

	if (sectionLen > 0.0)
	{
		//Display ZhongJi Names and Distance
		pDC->SetTextColor(RGB(0x00, 0xC0, 0x60));
		for (i = 1; i <= zhongjiCount; i++)
		{
			curKM = (zhongjiDis[i] - atof(startKM));
			strKM.Format("(%.1f)", zhongjiDis[i]);
			sz = pDC->GetTextExtent("▲");
			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (zhongjiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[中继] >>"); //◇ 
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [中继]");
				}
			}
		}

		//Display diantai Names and Distance		 
		for (i = 1; i <= diantaiCount; i++)
		{
			curKM = (diantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", diantaiDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (diantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[遥控台] >>"); //Y 
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [遥控台]");
				}
			}
		}

		//Display suidao Names and Distance		 
		for (i = 1; i <= suidaoCount; i++)
		{
			curKM = (suidaoDis[i] - atof(KM1));
			strKM.Format("(%.1f)", suidaoDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 480 + 10, "∩[隧道]");
			}
		}

		//Display ZhiFangZhan Names and Distance
		for (i = 1; i <= zhifangCount; ++i)
		{
			curKM = (zhifangDis[i] - atof(KM1));
			strKM.Format("(%.1f)", zhifangDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (zhifangCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[直放站] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [直放站]");
				}
			}
		}

		//Display QuJianTai Names and Distance
		for (i = 1; i <= qujiantaiCount; ++i)
		{
			curKM = (qujiantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", qujiantaiDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (qujiantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[区间台] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [区间台]");
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
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				if (zhongjiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[中继] >>");   //◇
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [中继]");
				}
			}
		}

		//Display diantai Names and Distance		 
		for (i = 1; i <= diantaiCount; i++)
		{
			curKM = 0.0 - (diantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", diantaiDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				if (diantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[遥控台] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [遥控台]");
				}
			}
		}

		//Display suidao Names and Distance		 
		for (i = 1; i <= suidaoCount; i++)
		{
			curKM = 0.0 - (suidaoDis[i] - atof(KM1));
			strKM.Format("(%.1f)", suidaoDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 480 + 10, "∩");
			}
		}

		//Display ZhiFangZhan Names and Distance
		for (i = 1; i <= zhifangCount; ++i)
		{
			curKM = 0.0 - (zhifangDis[i] - atof(KM1));
			strKM.Format("(%.1f)", zhifangDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (zhifangCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[直放站] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [直放站]");
				}
			}
		}

		//Display QuJianTai Names and Distance
		for (i = 1; i <= qujiantaiCount; ++i)
		{
			curKM = 0.0 - (qujiantaiDis[i] - atof(KM1));
			strKM.Format("(%.1f)", qujiantaiDis[i]);
			sz = pDC->GetTextExtent("▲");

			if ((curKM > 0 - 0.05) && (curKM < sectionLen + 0.05))
			{
				pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM), 460 + 10, "▲");
				//画与车站的归属关系
				if (qujiantaiCode[i] == atoi(nextCode))
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "[区间台] >>");
				}
				else
				{
					pDC->TextOut(nDrawRangeXMin - sz.cx / 2 + (int)(curKM*nPix1KM) - 15, 480 + 10, "<< [区间台]");
				}
			}
		}
	}
}

/*********************以上定义全局变量并添加注释********************/



IMPLEMENT_DYNCREATE(CFSTView, CFormView)

BEGIN_MESSAGE_MAP(CFSTView, CFormView)
	ON_COMMAND(ID_START_BUTTON, &CFSTView::OnStartButton)
	ON_COMMAND(ID_STOP_BUTTON, &CFSTView::OnStopButton)
	ON_COMMAND(ID_UPDATE_BUTTON, &CFSTView::OnUpdateButton)
	ON_COMMAND(ID_SAVEDATA_BUTTON, &CFSTView::OnSavedataButton)
	ON_COMMAND(ID_PR100_SETTING, &CFSTView::OnPr100Setting)
	ON_COMMAND(ID_MODE_BUTTON, &CFSTView::OnModeButton)
	ON_BN_CLICKED(IDC_STATIC_RANGE, &CFSTView::OnBnClickedStaticRange)
END_MESSAGE_MAP()

// CFSTView 构造/析构

CFSTView::CFSTView()
	: CFormView(IDD_FST_FORM)
	, m_controlstatus(_T("本地"))
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
	, m_updown(_T(" 增加"))
{
	// TODO: 在此处添加构造代码
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

	dirName = _T("C:\\场强测试\\");

	CString strTemp, strTempA, strDisp;
	TCHAR szPath[MAX_PATH] = { "c:\\场强测试\\*.TXT" };

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
	::GetPrivateProfileString("TEST_LINE", "Libdir", _T("c:\\里程库\\*.dbf"),
	szPath, MAX_PATH,".\\fsmconfig.ini");
	m_list.InsertString(-1, szPath);
	*/

	//	char pp[50];
	//	sprintf(pp,"%d %d %lf",pulse100M, pulse95, unit);
	//	MessageBox(pp);

	PI = atan(1.0)*4.0;
	unit = atof(m_diameter)*PI*0.001 / pulse;		//meter
	pulse100M = (int)(100.0 / unit + 0.5);
	pulse95 = (int)(5.0 / unit + 0.5);
	pulse90 = (int)(10.0 / unit + 0.5);
	pulse50 = (int)(50.0 / unit + 0.5);

	/*****************************************/
	//add by bzw  161110 start
	InitPR100flag = FALSE;
	stopPR100 = false;
	starRecordLevel = FALSE;
	WorkAreaFlag = false;   //FALSE可读,TRUE 可写
	SampleAreaFlag = true;  //FALSE表示可读,TRUE 可写
	pWorkArea = 0;
	pSampleArea = 0;

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
}

BOOL CFSTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CFSTView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CFSTView 诊断

#ifdef _DEBUG
void CFSTView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFSTView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFSTDoc* CFSTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFSTDoc)));
	return (CFSTDoc*)m_pDocument;
}
#endif //_DEBUG


// CFSTView 消息处理程序


void CFSTView::OnStartButton()
{
	// TODO: 在此添加命令处理程序代码

	// TODO: 在此添加命令处理程序代码
	// TODO: Add your command handler code here
	if (caiji_status == 1)
		return;
	Pr100ProcFlag = 1;
	char pp[200];
	int retv;
	char tmps[5];
	int ii;

	if (!is4403)
		setStatus = 1;
	StartTestFlag = true;

	unsigned char version = 0;
	// 	retv = Ex_Vender_Read(version);		//此处为原版适配器连接判断,之后需要添加新适配器代码。Edit by zwbai 161222
	// 	if (version != 0x21)
	// 	{
	// 		sprintf(pp, "USB连接不成功，请关闭适配器，再重开电源！(%d, Ver %d)", retv, version);
	// 		MessageBox(pp, NULL, MB_ICONEXCLAMATION);
	// 		return;
	// 	}


	RECVPARAM *pRecvParam = new RECVPARAM;
	pRecvParam->sock = nSocketUdp;
	pRecvParam->hwnd = m_hWnd;
	pRecvParam->m_pView = this;
	HANDLE hThread = CreateThread(NULL, 0, RecvProc, (LPVOID)pRecvParam, 0, NULL);
	if (!InitPR100flag)
	{
		MessageBox(_T("      请设置场强仪！"));
		return;
	}
	stopPR100 = false;
	starRecordLevel = FALSE;
	pWorkArea = 0;
	pSampleArea = 0;
	WorkAreaFlag = false;   //FALSE可读,TRUE 可写
	SampleAreaFlag = true;  //FALSE表示可读,TRUE 可写
	/********************以下代码现阶段注释，之后需要恢复*********************/  //Edit by zwbai 161222
	// 	m_line.TrimLeft();
	// 	m_line.TrimRight();
	// 	if (m_line.GetLength()<2)
	// 	{
	// 		MessageBox("请输入测试线路!", NULL, MB_ICONWARNING);
	// 		return;
	// 	}
	/********************以上代码现阶段注释，之后需要恢复*********************/


	/********************以下代码段应为PC与原版适配器的信息交互，此处先注释，*********************///Edit by zwbai 161222
	// 	unsigned char sendbuff[65] = { 0xff, 0x00, 0x02, 0x00, 0x00 };
	// 	unsigned char recvbuff[65];
	// 	unsigned char cmdchkbyte = 0;
	// 
	// 	if (selftestStatus != 1)
	// 	{
	// 		goto tmpl;
	// 		sendbuff[1] = 0x01;
	// 		sendbuff[2] = 0x04;		//4分频，缺省为2
	// 		sendbuff[3] = 0x01;
	// 		cmdchkbyte = 0;
	// 		for (ii = 1; ii<4; ii++)
	// 			cmdchkbyte += sendbuff[ii];
	// 		sendbuff[4] = cmdchkbyte;
	// 		Ex_WriteData(1, sendbuff, 16, 1000);////
	// 
	// 		Ex_Vender_ScanOrPrint(1, 16);
	// 		retv = Ex_ReadData(0, recvbuff, 16, 1000);
	// 		sprintf(pp, "%d:", retv);
	// 		for (ii = 0; ii < 16; ii++)
	// 		{
	// 			sprintf(tmps, "%02X ", recvbuff[ii]);
	// 			strcat(pp, tmps);
	// 		}
	// 		MessageBox(pp, NULL, MB_ICONWARNING);
	// 	tmpl:;
	// 	}
	/********************以上代码段应为PC与原版适配器的信息交互，此处先注释，*********************///Edit by zwbai 161222
	currentSel = 0 - 1;
	currentSavePos = 0;
	firstMapped = 0;

	//OnSelchangeUpdownCombo();此处为增减标志combo消息相应函数，之后需要恢复
	InitScreen();



	caiji_status = 1;

	/********************以下代码段目前无FORM相关窗口，此处先注释，*********************///Edit by zwbai 161222	
	GetDlgItem(IDC_LINE_EDIT)->EnableWindow(FALSE);
 	GetDlgItem(IDC_DISTANCE_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_DIAMETER_EDIT)->EnableWindow(FALSE);  //delete by zgliu 应南昌局要求，测试过程中可能需要修改
 	GetDlgItem(IDC_FILENAME_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_UPDOWN_COMBO)->EnableWindow(FALSE);
 	GetDlgItem(IDC_MAINTANCE_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_RATE_COMBO)->EnableWindow(FALSE);
 	GetDlgItem(IDC_FILENUM_EDIT)->EnableWindow(FALSE);
 	GetDlgItem(IDC_SAVEFILE_SPIN)->EnableWindow(FALSE);
 	GetDlgItem(IDC_LOSSDB_EDIT)->EnableWindow(FALSE);
 	GetDlgItem(IDC_LOSSDB_SPIN)->EnableWindow(FALSE);
 	GetDlgItem(IDC_EDIT20)->EnableWindow(FALSE);
 	SetDlgItemText(IDC_EDIT_FR, Pr100freq);				//输出当前测试频率 add by bzw 161121
	GetDlgItem(IDC_CONTROLSTATUS)->EnableWindow(FALSE);
	/********************以上代码段目前无FORM相关窗口，此处先注释，*********************///Edit by zwbai 161222	

	AD_number = 0;
	for (int i = 0; i<30720; i++)
	{
		AD_value1[i] = 0.0f;
	}

	int m_nTimer;
	m_nTimer = SetTimer(1, 1000, NULL);
	ASSERT(m_nTimer != 0);
	SetTimer(tagAutoSave, 30 * 60 * 1000, NULL);
	//场强数据自动存盘时间间隔 30 mins
	/*
	m_strADdataSavePath = m_strCurDirectory + _T("\\");
	CString strTemp = m_line + _T("场强存盘数据");
	m_strADdataSavePath += strTemp;
	CreateDirectory(m_strADdataSavePath.c_str(), NULL);	  //创建文件夹以保存场强测试数据，但格式有问题，删除先
	strTemp = m_strCurDirectory.c_str();
	strTemp += _T("\\GPSDataFiles");
	CreateDirectory(strTemp, NULL);
	*/
	//delete by bzw 161129	 
	readStatus = 1;
	Pr100flag = 1;
	myThread = AfxBeginThread(MyThreadProc, NULL, THREAD_PRIORITY_HIGHEST);
	myThread->m_bAutoDelete = TRUE;
}


void CFSTView::OnStopButton()
{
	// TODO: 在此添加命令处理程序代码
}


void CFSTView::OnUpdateButton()
{
	// TODO: 在此添加命令处理程序代码
}


void CFSTView::OnSavedataButton()
{
	// TODO: 在此添加命令处理程序代码
}


void CFSTView::OnPr100Setting()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
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
		// 		m_wndStatusBar.SetPaneText(1, _T("正在设置设备参数，请稍等...   "), TRUE);
		Pr100freq = m_PR100SetDlg.m_FR;
		if (TRUE == InitPr100())//PR100初始化
		{
			InitPR100flag = TRUE;
			MessageBox(_T("     设置场强仪成功！"));
			wchar_t buffer[150];
			wprintf(buffer, "场强仪参数 Band:15kHz LU:dBuV LD:5ms AD:FM AT:%ddB Frq:%sMHz", Antenna, Pr100freq);
			//以下设置状态栏宽度
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
	// TODO: 在此添加命令处理程序代码
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
	//WSASYSNOTREADY 指出网络通信依赖的网络子系统还没有准备好.
	//WSAVERNOTSUPPORTED 所需的Windows Sockets API的版本未由特定的Windows Sockets实现提供.
	//WSAEINVAL 应用程序指出的Windows Sockets版本不被该DLL支持
	err = WSAStartup(wVersionRequested, &wsaData);//Windows套接字异步启动命令
	if (err != 0)
	{
		return FALSE;
	}
	//16进制数最低（最右边）字节的内容 || 16进制数最高（最左边）字节的内容
	if (LOBYTE(wsaData.wVersion) != maxV || HIBYTE(wsaData.wVersion) != minV)
	{
		WSACleanup();
		return FALSE;
	}

	//为了检查PC端网络设置是否正确，获取本地机器IP地址
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
	//得到本地机器IP地址 结束
	if (strClient.Mid(0, 11) != _T("172.17.75.2") && strClient.Mid(0, 9) != _T("127.0.0.1"))//判断PC本地IP是否为172.17.75.2
	{
		MessageBox(_T("请将本地IP设置为172.17.75.2,子网掩码设为255.255.255.0,默认网关设置为172.17.75.1"));
		return FALSE;
	}
	if (strClient.Mid(0, 9) == _T("127.0.0.1"))
	{
		MessageBox(_T("请打开场强仪！"));
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
				MessageBox(_T("连接场强仪失败"));
				return FALSE;
			}
			else
			{
				int i = 1;
				setsockopt(nSocketTcp, IPPROTO_TCP, TCP_NODELAY, (char*)&i, sizeof(i));
				//复位PR100
				send(nSocketTcp, "*RST\n", strlen("*RST\n"), 0);
				Sleep(100);
				//按键锁
				send(nSocketTcp, "SYST:KLOCK ON\n", strlen("SYST:KLOCK ON\n"), 0);
				Sleep(50);
				//设置系统音量为0
				send(nSocketTcp, "SYST:AUD:VOL 0\n", strlen("SYST:AUD:VOL 0\n"), 0);
				Sleep(50);
				//设置频率模式MSC
				send(nSocketTcp, "FREQ:MODE MSC\n", strlen("FREQ:MODE MSC\n"), 0);
				Sleep(50);
				//设置MSC 数运行
				send(nSocketTcp, "MSC:COUN INF\n", strlen("MSC:COUN INF\n"), 0);
				Sleep(50);
				//同步时间
				send(nSocketTcp, "MSC:DWEL 0\n", strlen("MSC:DWEL 0\n"), 0);
				Sleep(50);
				//收听时间
				send(nSocketTcp, "MSC:HOLD:TIME 0\n", strlen("MSC:HOLD:TIME 0\n"), 0);
				Sleep(50);
				//扫描模式
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
				//设置频率
				send(nSocketTcp, sendbuff, strlen(sendbuff), 0);
				Sleep(50);
				//设置带宽15KHz
				send(nSocketTcp, "BAND 15kHz\n", strlen("BAND 15kHz\n"), 0);
				Sleep(50);
				//设置检查方式为Max Peak
				send(nSocketTcp, "SENS:DET PEAK\n", strlen("SENS:DET PEAK\n"), 0);


				Sleep(50);
				//设置中频显示范围100dbuv
				send(nSocketTcp, "DISP:IFP:LEV:RANG 100\n", strlen("DISP:IFP:LEV:RANG 100\n"), 0);
				Sleep(50);
				//设置中频显示最大值为90dbuv
				send(nSocketTcp, "DISP:IFP:LEV:REF 90\n", strlen("DISP:IFP:LEV:REF 90\n"), 0);
				Sleep(50);
				send(nSocketTcp, "DISP:WIND \"Spectrum\"\n", strlen("DISP:WIND \"Spectrum\"\n"), 0);

				Sleep(50);
				//设置测量模式为周期测量
				send(nSocketTcp, "MEAS:MODE PER\n", strlen("MEAS:MODE PER\n"), 0);
				Sleep(50);
				//设置测量周期为0.5ms
				send(nSocketTcp, "MEAS:TIME 0.5 ms\n", strlen("MEAS:TIME 0.5 ms\n"), 0);
				Sleep(50);
				//删除所有的TRAC UdpPath
				send(nSocketTcp, "TRAC:UDP:DEL ALL\n", strlen("TRAC:UDP:DEL ALL\n"), 0);
				Sleep(50);
				//设置UdpPath,将电平测量值发送到IP地址为172.17.75.2的PC机的19000端口
				send(nSocketTcp, "TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n", \
					strlen("TRAC:UDP:DEF:FLAG:ON \"172.17.75.2\",19000,\"VOLT:AC\"\n"), 0);
				Sleep(50);
				//
				send(nSocketTcp, "TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n", \
					strlen("TRAC:UDP:DEF:TAG:ON \"172.17.75.2\",19000,MSC\n"), 0);
				Sleep(50);
				send(nSocketTcp, "INIT:CONM\n", strlen("INIT:CONM\n"), 0);
				closesocket(nSocketTcp);


				//UDP初始化
				nSocketUdp = socket(AF_INET, SOCK_DGRAM, 0);
				if (INVALID_SOCKET == nSocketUdp)
				{
					MessageBox(_T("套接字创建失败！"));
					return FALSE;
				}
				struct sockaddr_in addrSock;
				addrSock.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
				addrSock.sin_family = AF_INET;
				addrSock.sin_port = htons(19000); //此端口与命令15、16中的端口19000保持一致
				int retval;
				retval = bind(nSocketUdp, (SOCKADDR*)&addrSock, sizeof(SOCKADDR));
				if (SOCKET_ERROR == retval)
				{
					closesocket(nSocketUdp);
					MessageBox(_T("绑定端口失败！"));
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
	//m_proption = pView->m_rate;			//先注释，后续补

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

	unsigned char inData[64 * 16];
	unsigned long pulseNum, prePulseNum;
	unsigned int  speedNum;
	//unsigned int  vol1;
	short	vol1;
	unsigned char  checkByte;

	//// discard the first 1024 data/////////////
	memset(inData, 0, sizeof(inData));  //add by zgliu 2011.03.03     
										// 	Ex_Vender_ScanOrPrint(1, 16 * 64);
										// 	Ex_ReadData(2, inData, 16 * 64, 600);


	for (int i = 0; i<4096; i++)
	{
		histo1[i] = 0;
	}

	AD_number = 0;
	AD_num100 = 0;

	currentDis = startDis;
	sectionNum = 0;

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

	char dbuffer[9];
	char tbuffer[9];
	int captureno;

	/*read from file gps239.txt*/
	//	simfp=fopen("gps239.txt","r+t");
	/*	simfp=fopen("广坪sim.TXT","r+t");
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
	while (readStatus == 1) //&& AD_number<40000
	{
		/*
		if (pView->selftestStatus)
		{
		for (int kkk=0; kkk<3276; kkk++)
		for (int kkkk=0; kkkk<32767; kkkk++);
		}*/

		memset(inData, 0, sizeof(inData));  //add by zgliu 2011.03.03
// 		Ex_Vender_ScanOrPrint(1, 16 * 64);
// 		Ex_ReadData(2, inData, 16 * 64, 600);

		/****************************************************************/
		//add by bzw 161110 start
		Sleep(250);
		if (false == pView->WorkAreaFlag)
		{
			dataready_flag = 0;
			//以下为读数据操作
			if (pView->pWorkArea>0)
			{
				pView->ReadData(inData, pView->WorkArea, pView->pWorkArea);
				pView->pWorkArea = 0;
				dataready_flag = 1;
				pView->WorkAreaFlag = true;
				pView->SampleAreaFlag = false;
			}
			else if (0 == pView->pWorkArea)// 当前工作区为空
			{

				pView->WorkAreaFlag = true;
				pView->SampleAreaFlag = false;


				if (pView->pSampleArea>0)
				{
					pView->ReadData(inData, pView->SampleArea, pView->pSampleArea);
					pView->pSampleArea = 0;
					dataready_flag = 1;

				}
			}
		}

		else if (false == pView->SampleAreaFlag)
		{

			dataready_flag = 0;
			//以下为读数据操作
			if (pView->pSampleArea>0)
			{
				pView->ReadData(inData, pView->SampleArea, pView->pSampleArea);
				pView->pSampleArea = 0;
				dataready_flag = 1;
				pView->SampleAreaFlag = true;
				pView->WorkAreaFlag = false;
			}
			else if (0 == pView->pSampleArea)// 当前工作区为空
			{
				pView->SampleAreaFlag = true;
				pView->WorkAreaFlag = false;
				if (pView->pWorkArea>0)
				{
					pView->ReadData(inData, pView->WorkArea, pView->pWorkArea);
					pView->pWorkArea = 0;
					dataready_flag = 1;
				}
			}
		}
		if (!dataready_flag)
			continue;
		//add by bzw 161110 end
		/****************************************************************/
		
		//此处删除原版程序gprmc部分代码 edit by zwbai 161227

		for (int jj = 0; jj<16 * 64; jj += 16)
		{
			/*
			if(inData[jj]==0x00)
			{
			speedNum=((unsigned int)inData[jj+5]<<8)+(unsigned int)inData[jj+6];
			sprintf(pp,"%05.1f", speedNum*unit*1.8); //0.9); //???
			pDC.TextOut(915,520,pp);
			}
			//		else if((inData[jj]==0xFF)&&(inData[jj+1] != 0x81)) // for field strength data only
			else*/
			if (inData[jj] == 0xFF) // for field strength data only
			{
				
				checkByte = 0;
				char pptmp[100], pptmp1[5];
				//			sprintf(pptmp, "ID:%02x", inData[jj]);
				sprintf(pptmp, ":");
				for (int ii = 1; ii<15; ii++)
				{
					//				sprintf(pptmp1, " %02x", inData[ii+jj]);
					//				strcat(pptmp, pptmp1);
					checkByte += inData[ii + jj];
				}
				//			sprintf(pptmp1, " %02X", inData[ii+jj]);
				//			strcat(pptmp, pptmp1);

				if (checkByte != inData[jj + 15])
				{
					sprintf(pp, "Checking:%02x %02x,%s", checkByte, inData[jj + 15], pptmp);
					//		    	m_wndStatusBar.SetPaneText(0,pp,TRUE);
				}
				pulseNum = ((unsigned long)inData[jj + 1] << 24) + ((unsigned long)inData[jj + 2] << 16) + ((unsigned long)inData[jj + 3] << 8) + (unsigned long)inData[jj + 4];
				speedNum = ((unsigned int)inData[jj + 5] << 8) + (unsigned int)inData[jj + 6];

				currentPulsenum = pulseNum;

				/**同时将信息显示到文本框与界面上 add by chenbin 161209 start***/
				CString strEidtValue;
				strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
				pView->SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
				strEidtValue.Format(_T("%05.2f"), dbVal1);
				pView->SetDlgItemText(IDC_EDIT_CurDBValue, strEidtValue);
				strEidtValue.Format(_T("%05.1f"), speedNum*unit*0.9 * 2);
				pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);
				sprintf(pp, "%05.1f", speedNum*unit*1.8);//1.8); //???
				pDC.TextOut(915, 525, pp);
				sprintf(pp, "%06.1f", currentDis / 1000.0);
				pDC.TextOut(460, 525, pp);
				sprintf(pp, "%05.2f", dbVal1);
				pDC.TextOut(690, 525, pp);
				/**同时将信息显示到文本框与界面上 add by chenbin 161209 end***/


				if (pulseNum == 0)	//added by Cai, 2007-7-9 for V9.0
				{
					// 				speedNum=((unsigned int)inData[jj+5]<<8)+(unsigned int)inData[jj+6];
					// 				sprintf(pp,"%05.1f", speedNum*unit*1.8);//0.9); 
					// 				pDC.TextOut(915,520,pp);
					//add by zgliu 2011.03.03
					speedNum = ((unsigned int)inData[jj + 5] << 8) + (unsigned int)inData[jj + 6];
					//pView->m_fCurSpeed = speedNum*unit*0.9;
					pView->m_fCurSpeed = speedNum*unit*0.9 * 2;
					///pDC.TextOut(915, 525, pp);

					static unsigned int nCnt = 0;
					nCnt++;
					if (/*20*/5 == nCnt)
					{
						CString strEditValue;
						strEditValue.Format(_T("%05.1f"), pView->m_fCurSpeed);
						pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEditValue);
						nCnt = 0;
					}
					//add end by zgliu 2011.03.03
					continue;
				}
				//vol1=(short)(((inData[jj+7]<<8)&0xff00)|((inData[jj+8])&0xff)) + 100;


				/***处理负的场强值start by chenbin 161209：*/
				/*inData[jj+7]如果等于255，该场强为负值，inData[jj+8]为场强值的绝对值，归一化处理即为：-inData[jj+8]+100**/
				/***Pr100输出场强值为short型，是真实场强值的10倍，归一化为[0,1000]时无需乘以10***/
				if (255 == inData[jj + 7])
				{
					vol1 = 100 - (short)((inData[jj + 8]) & 0xff);
				}
				else
				{
					vol1 = (short)(((inData[jj + 7] << 8) & 0xff00) | ((inData[jj + 8]) & 0xff)) + 100;
				}
				/***处理负的场强值end by chenbin 161209***/


				//add by bzw 161111 start
				if (vol1 < 0)
					vol1 = 0;
				//histo1[vol1]++;	
				if ((vol1 >= 0) && (vol1 < 1000))
				{
					histo1[vol1]++;
					//AD_number++; 
					//samplecount++;
				}
				//if(vol1>=1000)
				//	vol1=1000;
				/*else
				{
				// 					sprintf(pp,"Wrong Data: %06dm  %d", curMilage, vol1);
				// 					pDC.TextOut(400,550,pp);
				continue;
				}*/
				//add by bzw 161111 end


				AD_number++;

				currentDis = startDis + delta*AD_number*unit;

				// add by zgliu 2011.03.13 检查自动控发发送时机
				// 			if (autoKFflag)  //用户选中自动控发功能
				// 			{
				// 				if((currentDis-stationDis[currentStation]*1000.0)*delta > (pView->m_kongfa+50))
				// 				{
				// 					pView->m_bATCSendFlag = true;
				// 					pView->m_bATCSendFlag2 = false;
				// 					pView->m_bATCSendFlag3 = true;
				// 				}
				// 				if (!pView->m_bSendTimeChecking)
				// 				{
				// 					pView->check_SendTime(); 
				// 				}
				// 				//若距车站800M时控发失败，则200M时再次发送自动控发命令
				// 				//实时场强值小于20dB则认为800M控发失败
				// 				if (pView->m_fRealTimeDB <= 20.0)
				// 				{
				// 					pView->check_2ndSendTime();			
				// 				}
				//                 //检查站内停车的控发时机
				// 	           	if (((1 == delta) && pView->TimeSend[currentStation].DownStopFlag) || ((-1 == delta) && pView->TimeSend[currentStation].UpStopFlag))
				// 				{
				// 					pView->check_RestartSendTime();
				// 				}
				// 
				// 			} 
				// add end by zgliu 

				if ((AD_number>2) && (pulseNum - prePulseNum != pulseDivision))   //pulseDivision=2
				{
					sprintf(pp, "Missing: %02x %06d  %08x  %08x", inData[jj], AD_number, pulseNum, prePulseNum);
					//		  	  m_wndStatusBar.SetPaneText(0,pp,TRUE);
				}
				else
				{
					sprintf(pp, "Correct: %06d  %08x  %08x, %02d", AD_number, pulseNum, prePulseNum, pulseNum - prePulseNum);
					//			  pDC.TextOut(400,550,pp);
				}
				prePulseNum = pulseNum;

				if ((AD_number>0) && ((AD_number%pulse100M) == 0))
					//	 	  	if(fabs(AD_number*unit-(AD_num100+1)*100.0)<1.0005*unit)
				{
					kk = 0;
					count = 0;
					//while((count<pulse95)&&(kk<4096))
					while ((count<pulse95) && (kk<1000))
						count += histo1[kk++];
					data1[AD_num100].AD_95 = AD2dB(kk - 1);	//including loss_voltage
															//data1[AD_num100].AD_95= AD2dB(vol1);

															//while((count<pulse90)&&(kk<4096))
					while ((count<pulse90) && (kk<1000))
						count += histo1[kk++];
					data1[AD_num100].AD_90 = AD2dB(kk - 1);
					//data1[AD_num100].AD_90= AD2dB(vol1);

					//while((count<pulse50)&&(kk<4096))
					while ((count<pulse50) && (kk<1000))
						count += histo1[kk++];
					data1[AD_num100].AD_50 = AD2dB(kk - 1);
					//data1[AD_num100].AD_50= AD2dB(vol1);
					/*
					//added for calibration work
					cal_voltage = 10.0*(kk-1)/4095;
					cal_dbval = data1[AD_num100].AD_50;
					sprintf(pp,"%05.2fdB %fV", cal_dbval, cal_voltage);
					m_wndStatusBar.SetPaneText(1,pp,TRUE);
					//end
					*/
					data1[AD_num100].curPos = (int)startDis + (delta*AD_num100) * 100; //???
																					   /// added by cai (2006.3.26)
					sprintf(pp, "dB: %6.2f %6.2f  %6.2f", data1[AD_num100].AD_95,
						data1[AD_num100].AD_90, data1[AD_num100].AD_95);

					fusiondata[AD_num100].curpos = data1[AD_num100].curPos;
					fusiondata[AD_num100].pulsenum = currentPulsenum;
					fusiondata[AD_num100].lat = currentLat;
					fusiondata[AD_num100].lon = currentLon;

					//			  pDC.TextOut(750,550,pp);

					//////////////////////////////////////////////

					if (which == 95)
					{
						dbVal1 = data1[AD_num100].AD_95;
					}
					else if (which == 90)
					{
						dbVal1 = data1[AD_num100].AD_90;
					}
					else
					{
						dbVal1 = data1[AD_num100].AD_50;
					}

					for (int i = 0; i<4096; i++)
					{
						histo1[i] = 0;
					}

					AD_value1[AD_num100] = dbVal1;

					if (0 == sectionNum)
					{
						pDC1.MoveTo(nDrawRangeXMin + sectionNum*nPix100m, 410 - (int)(dbVal1)* 4);
					}




					//////////////// Update position of the train and draw the curve ///////////
					// modified by zgliu 2011.04.14, 将原来的每100米2个像素点改为6个
					// 先覆盖旧车再画当前位置的小车
					pDC1.LineTo(nDrawRangeXMin + nPix100m*sectionNum, 410 - (int)(dbVal1)* 4);
					pDC1.FillSolidRect(nDrawRangeXMin - 30 + sectionNum*nPix100m - nPix100m, 455, 30, 16, 0xFFFFFF);
					pDC1.BitBlt(nDrawRangeXMin - 30 + sectionNum*nPix100m, 458, 30, 13, &pDC1, 345, 30, SRCCOPY);
					AD_num100++;
					sectionNum++;
					// Draw line at the front of train  
					// 		   pOldPen = pDC.SelectObject(&myPenTrainPos);
					// 		   pDC.MoveTo(nDrawRangeXMin+nPix100m*sectionNum-nPix100m, 50);
					// 		   pDC.LineTo(nDrawRangeXMin+nPix100m*sectionNum-nPix100m, 458);
					// 		   pDC.MoveTo(nDrawRangeXMin+nPix100m*sectionNum, 50);
					// 		   pDC.LineTo(nDrawRangeXMin+nPix100m*sectionNum, 458);
					// 		   pDC.SelectObject(pOldPen);

					// modified end by zgliu 
					////////////////////////////////////////////



					// delete by zgliu 2011.03.03                
					// 				pDC.TextOut(400,520,"当前里程:               km");
					// 				pDC.TextOut(630,520,"场强统计:               dB");
					// 		 	    pDC.TextOut(860,520,"当前车速:            km/h");
					// 
					// 				//display current Distance   
					// 				sprintf(pp,"%06.1f", currentDis/1000.0); 
					// 	 			pDC.TextOut(460,520,pp);
					// 
					// 
					// 			//	sprintf(pp,"%ld %d %06.1f",AD_number,pulseNum, pulseNum*unit); 
					// 	 		//	pDC.TextOut(460,545,pp);
					// 
					// 	            sprintf(pp,"%05.2f", dbVal1); 
					// 	 			pDC.TextOut(690,520,pp);
					// 
					// 				sprintf(pp,"%05.1f", speedNum*unit*1.8);	//3.6/pulseDivision=0.9; //1.8); //???
					// 	 			pDC.TextOut(915,520,pp);			
					// delete end by zgliu 

					//add by zgliu 2011.03.03 
					//送往主界面更新显示
					CString strEidtValue;
					strEidtValue.Format(_T("%06.1f"), currentDis / 1000.0);
					pView->SetDlgItemText(IDC_EDIT_CurMileage, strEidtValue);
					strEidtValue.Format(_T("%05.2f"), dbVal1);
					pView->SetDlgItemText(IDC_EDIT_CurDBValue, strEidtValue);
					strEidtValue.Format(_T("%05.1f"), speedNum*unit*0.9 * 2);
					pView->SetDlgItemText(IDC_EDIT_CurSpeed, strEidtValue);
					// add end by zgliu
				}

				//若列车驶出当前显示范围(startKM+15km)，则重新画屏
				// add by zgliu 2011.04.13
				if ((currentDis - (atof(startKM) + 15 * delta)*1000.0)*delta > 0.0)
				{
					// 				pDC.SetBkMode(OPAQUE);
					// 				pDC.SetROP2(nOldDrawMode);
					pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 450 - 50, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

					pDC.SelectObject(&myPen0);
					for (int i = 0; i<10; i++)
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

					// 每1KM显示一个刻度值
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
							if (-1 != pView->m_updown.Find(_T("增加")))
							{
								strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
							}
							else if (-1 != pView->m_updown.Find(_T("减少")))
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

				if (((nextStation>stationCount) && (delta>0)) || ((nextStation<0) && (delta<0)))
				{
					if (overPass == 0)
					{
						pDC.TextOut(360, 180, _T(" 已到该线路的终点站,请停止测试,重新选择线路!"));
						overPass = 1;
						readStatus = 0;
						Pr100ProcFlag = 0;
						AfxMessageBox(_T("!!!!!!!!!!!!!完成测试!!!!!!!!!!!!!!!!!!!!!"));
					}
				}
				else if ((currentDis - stationDis[nextStation] * 1000.0)*delta>0.0) //列车驶入下一站
				{
					// 				pDC.SetBkMode(OPAQUE);
					// 				pDC.SetROP2(nOldDrawMode);

					pDC.FillSolidRect(nDrawRangeXMin + 1, 50, nDrawRangeXMax - nDrawRangeXMin - 2, 400, 0xFFFFFF); // Rectangle(80+1, 48+1, 950-1, 451-1);

					pDC.SelectObject(&myPen0);
					for (int i = 0; i<10; i++)
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
					// 每1KM显示一个刻度值
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
							if (-1 != pView->m_updown.Find(_T("增加")))
							{
								strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
							}
							else if (-1 != pView->m_updown.Find(_T("减少")))
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
			}
		}
	}

	if (sim_flag)
		fclose(simfp);

	tm2 = GetTickCount();
	sprintf(pp, "%ld %d", tm2 - tm1, AD_num100);
	//    m_wndStatusBar.SetPaneText(0,pp,TRUE);

	pDC.SelectObject(pOldPen);
	pDC.SelectObject(pOldFont);


	AfxEndThread(0);
	//    ExitThread(0);

	//
	//	可以利用线程句柄所指的::GetExitCodeThread()函数,如果线程已经结束, 
	//	它将返回一个退出代码,如果还在运行,则返回一个STILL_ACTIVE.不过在之此前,
	//	先将 CWinThread成员对象m_bAutoDelete设置为FALSE.
	//	另外对象在线程结束时会自动检测到.


	return 0;    // thread completed successfully
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
	pDC.TextOut(820, 25, _T("测试日期：") + tNow.Format("%Y.%m.%d"));

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
	// 每1KM显示一个刻度值
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
			// 			if (-1 != m_updown.Find(_T("增加")))						//先注释，后续补
			// 			{
			// 				strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
			// 			}
			// 			else if (-1 != m_updown.Find(_T("减少")))
			// 			{
			// 				strTempKM.Format(_T("%0.2f"), atof(startKM) - i);
			// 			}
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
	pDC.TextOut(380, 20, _T("m_line场强测试曲线"));


	pDC.SelectObject(&myPen2);
	// 	int	maintance = atoi(m_maintance);							//先注释，后续补
	// 	pDC.MoveTo(nDrawRangeXMin, 450 - maintance * 4 - 40);
	// 	pDC.LineTo(nDrawRangeXMax, 450 - maintance * 4 - 40);

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

	////////////////////////	 

	pDC.SelectObject(&myPen3);
	if (offset > 0)
	{
		pDC.MoveTo(nDrawRangeXMin + offset / 2, 445);
		pDC.LineTo(nDrawRangeXMin + offset / 2, 450);
		pDC.MoveTo(nDrawRangeXMin + offset, 50);
		pDC.LineTo(nDrawRangeXMin + offset, 450);
	}
	pDC.SelectObject(pOldFont);

}


void CFSTView::ReadData(unsigned char inData[], unsigned char sharebuffer[], int sizeofBuf)
{
	if (sizeofBuf >= 1024)
	{
		int i;
		int j;
		for (i = 0; i < 1024; i += 16)
		{
			if (0xff == inData[i])
			{
				inData[i + 7] = sharebuffer[i + 7];  //高位场强值数据  测试时场强值赋为常数 /0/
				inData[i + 8] = sharebuffer[i + 8];    //低位场强值数据  /200/

				for (j = 9; j < 16; j++)
				{
					inData[i + j] = 0x00;
				}
			}
		}
	}
	if (sizeofBuf < 1024)
	{
		int i;
		int k = 0;
		for (i = 0; i < sizeofBuf; i += 16)
		{
			if (0xff == inData[i])
			{
				inData[i + 7] = sharebuffer[i + 7];  //高位场强值数据  测试时场强值赋为常数 /0/
				inData[i + 8] = sharebuffer[i + 8];    //低位场强值数据  /200/
				for (int j = 9; j < 16; j++)
				{
					inData[i + j] = 0x00;
				}

			}
		}
		for (k = sizeofBuf; k < 1024; k++)
		{

			inData[k] = 0x00;
		}
	}
}

//PR100 场强值接收线程
DWORD WINAPI CFSTView::RecvProc(LPVOID lpParameter)
{
	SOCKET sock = ((RECVPARAM*)lpParameter)->sock;
	HWND hwnd = ((RECVPARAM*)lpParameter)->hwnd;
	CFSTView *pView = ((RECVPARAM*)lpParameter)->m_pView;
	char cBuffer[33]; //接收数据缓冲区
	memset(cBuffer, 0, 33);
	int len = 0;
	pView->nCountLevel = 0;
	//	MSCpacket *pt;	
	CString str, strtemp, strdisplay = _T("");
	while (TRUE)
	{
		if (0 == Pr100ProcFlag || true == pView->stopPR100)
		{
			return 0;
		}
		//str.Format("%d,%d",pView->WorkAreaFlag,pView->SampleAreaFlag);
		//pView->SetDlgItemText(IDC_EDIT3,str);
		len = recv(sock, cBuffer, sizeof(cBuffer), 0);//接收数据
		pView->nLevel[pView->nCountLevel] = cBuffer[len - 4];
		pView->nLevel[pView->nCountLevel + 1] = cBuffer[len - 3];

		//***  显示原始场强值 调试用   *****//
		float Val;
		Val = ((short)(((pView->nLevel[pView->nCountLevel] << 8) & 0xff00) | (pView->nLevel[pView->nCountLevel + 1]) & 0xff));
		Val = (float)Val / 10;
		//Pr100输出场强值为short型，是真实场强值的10倍

		// 		CString str1;
		// 		str1.Format("%2.1f", Val);
		// 		pView->SetDlgItemText(IDC_EDIT20, str1);
		// 		//*** 显示原始场强值 调试用 end   *****//

		pView->nCountLevel += 2;

		if (true == pView->SampleAreaFlag)
		{
			//写SampleArea
			//	if(TRUE==pView->starRecordLevel)//写场强值操作
			//	{
			for (int i = 0; i<pView->nCountLevel; i += 2)
			{
				pView->SampleArea[pView->pSampleArea] = 0xff;
				/*					for(int j=0;j<6;j++)
				{
				//pView->SampleArea[pView->pSampleArea+j+1]=pView->TaxData[j];  //0-5帧  写入TAX箱里程值
				pView->SampleArea[pView->pSampleArea+j+1]=0x00;
				}
				*/
				pView->SampleArea[pView->pSampleArea + 7] = pView->nLevel[i];
				pView->SampleArea[pView->pSampleArea + 8] = pView->nLevel[i + 1];       //6-7帧  写入场强值
				for (int k = 0; k<7; k++)
				{
					pView->SampleArea[pView->pSampleArea + k + 9] = 0x00;               //8-15帧 写入0x00
				}
				/*************************************************************/
				//pView->SetDlgItemText(IDC_EDIT7,"Sample TAX+LEVEL");
				//str.Format("FF %02x %02x %02x %02x %02x %02x %02x %02x",pView->TaxData[0],pView->TaxData[1],\
														pView->TaxData[2],pView->TaxData[3],pView->TaxData[4],pView->TaxData[5],pView->nLevel[i],\
						pView->nLevel[i+1]);
//pView->SetDlgItemText(IDC_EDIT_TAX,str);  //显示用，可删除
/*************************************************************/
				pView->pSampleArea += 16;

			}

			pView->nCountLevel = 0;
			memset(pView->nLevel, 0, 10000);
			//	}
			// 			if (TRUE==pView->GpsRecordStart)//写GPS数据操作
			// 			{
			//  			pView->GpsRecordStart=false;
			// 				for(int i=0;i<pView->GpsmessageLen;i++)
			// 				{
			// 					pView->SampleArea[pView->pSampleArea+i]=pView->Gpsmessage.GetAt(i);
			// 				}
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT6,"GPS");
			// 				pView->SetDlgItemText(IDC_EDIT_GPS,pView->Gpsmessage);  //显示用，可删除
			// 				/*************************************************************/
			// 
			// 				pView->pSampleArea+=pView->GpsmessageLen;
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT4,"当前SamArea\r\n字节数");
			// 				str.Format("%d",pView->pSampleArea);                            //显示用，可删除
			// 				pView->SetDlgItemText(IDC_EDIT_SAMBYTENUMC,str);
			// 				/*************************************************************/
			// 			}
		}
		if (true == pView->WorkAreaFlag)
		{
			//写WorkArea
			//if(TRUE==pView->starRecordLevel)//写场强值
			//{
			for (int i = 0; i<pView->nCountLevel; i += 2)
			{
				pView->WorkArea[pView->pWorkArea] = 0xff;                          //帧头 0xff
																				   /*					for(int j=0;j<6;j++)
																				   {
																				   pView->WorkArea[pView->pWorkArea+j+1]=pView->TaxData[j];     //1-6帧  写入TAX箱里程值
																				   }
																				   */
				pView->WorkArea[pView->pWorkArea + 7] = pView->nLevel[i];
				pView->WorkArea[pView->pWorkArea + 8] = pView->nLevel[i + 1];          //7-8帧  写入场强值
				for (int k = 0; k<7; k++)
				{
					pView->WorkArea[pView->pWorkArea + k + 9] = 0x00;                  //9-16帧 写入0x00
				}
				pView->pWorkArea += 16;
				/*************************************************************/
				//pView->SetDlgItemText(IDC_EDIT7,"Work TAX+LEVEL");
				//str.Format("FF %02x %02x %02x %02x %02x %02x %02x %02x",pView->TaxData[0],pView->TaxData[1],\
														pView->TaxData[2],pView->TaxData[3],pView->TaxData[4],pView->TaxData[5],pView->nLevel[i],\
						pView->nLevel[i+1]);
//pView->SetDlgItemText(IDC_EDIT_TAX,str); //显示用，可删除
/*************************************************************/

			}

			pView->nCountLevel = 0;
			memset(pView->nLevel, 0, 10000);

			//}
			// 			if (TRUE==pView->GpsRecordStart)//写GPS数据操作
			// 			{
			//  				pView->GpsRecordStart=false;
			// 				for(int i=0;i<pView->GpsmessageLen;i++)
			// 				{
			// 					pView->WorkArea[pView->pWorkArea+i]=pView->Gpsmessage.GetAt(i);
			// 				}
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT6,"GPS");
			// 				pView->SetDlgItemText(IDC_EDIT_GPS,pView->Gpsmessage);  //显示用，可删除
			// 				/*************************************************************/
			// 		
			// 				pView->pWorkArea+=pView->GpsmessageLen;
			// 
			// 				/*************************************************************/
			// 				pView->SetDlgItemText(IDC_EDIT5,"当前WorkArea\r\n字节数");
			// 				str.Format("%d",pView->pWorkArea);                            //显示用，可删除
			// 				pView->SetDlgItemText(IDC_EDIT_WORKBYTENUMC,str);
			// 				/*************************************************************/
			// 			}
		}
		/*if(pView->pSampleArea>ShareBufferSize || pView->pWorkArea>ShareBufferSize)
		{
		AfxMessageBox("内存溢出");
		return 0;
		}*/
		if (pView->pSampleArea>ShareBufferSize)
		{
			AfxMessageBox(_T("内存溢出 pView->pSampleArea") + pView->nCountLevel);
			//	cout << pView->nCountLevel << endl;
			return 0;
		}
		else if (pView->pWorkArea>ShareBufferSize)
		{
			AfxMessageBox(_T("内存溢出 pView->pWorkArea") + pView->nCountLevel);
			//	cout << pView->nCountLevel << endl;
			return 0;
		}
	}
	return 0;

}


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

	// TODO: 在此处为本机数据添加绘制代码
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

		for (int i = 0; i < 128; i++)
		{
			pDC->MoveTo(0, i * 4);
			pDC->LineTo(/*1023*/rcClip.Width(), i * 4);
		}
		for (int i = 1; i < 256; i++)
		{
			pDC->MoveTo(i * 4, 0);
			pDC->LineTo(i * 4, /*508*/rcClip.Height());
		}


		CFont myFont1, myFont2, *pOldFont;
		myFont1.CreateFont(90, 30, 0, 0, FW_HEAVY, 0, 0, 0, DEFAULT_CHARSET,
			OUT_DEVICE_PRECIS, VARIABLE_PITCH | FF_ROMAN, PROOF_QUALITY, 0, _T("隶书"));
		pOldFont = pDC->SelectObject(&myFont1);

		pDC->SetTextColor(RGB(0x00, 0x60, 0x40));
		for (int i = 1; i < 6; i++)
			pDC->TextOut(150 - i, 160 - i, _T("无线列调场强自动测试系统"));
		pDC->SetTextColor(RGB(0x00, 0xC0, 0xA0));
		pDC->TextOut(150, 160, _T("无线列调场强自动测试系统"));
		pDC->SelectObject(pOldFont);

		GetParent()->SetWindowText(_T("              无  线  列  调  场  强  自  动  测  试  系  统"));

		myFont2.CreateFont(50, 16, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET,
			OUT_RASTER_PRECIS, (VARIABLE_PITCH | FF_ROMAN) & 0xFFFD, PROOF_QUALITY, 0, _T("隶书"));

		pOldFont = pDC->SelectObject(&myFont2);


		pDC->SetTextColor(RGB(0x00, 0x80, 0xFF));
		pDC->TextOut(200, 350, _T("研制单位：北京交通大学电子信息工程学院"));
		pDC->TextOut(410, 435, _T("2O16年11月"));
		pDC->SelectObject(pOldFont);

		//ReadLineName("综合库.txt");
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
	 
	 		// add by zgliu 2011.04.15
	 		if (1 == readStatus)
	 		{
	 			// 			CPen  myPenTrainPos(PS_SOLID, 1, RGB(0xFF, 0x00, 0x00));   //draw line for current position of the train
	 			// 			CPen* pOldPen;
	 			// 			int   nOldDrawMode;
	 			// 			nOldDrawMode = pDC->SetROP2(R2_XORPEN);
	 			// 			pDC->SetBkMode(TRANSPARENT);
	 			// 			pOldPen = pDC->SelectObject(&myPenTrainPos);
				// 			pDC->MoveTo(nDrawRangeXMin + sectionNum*nPix100m, 50);
	 			// 			pDC->LineTo(nDrawRangeXMin + sectionNum*nPix100m, 458);
	 			// 			pDC->SelectObject(pOldPen);
	 			// 			pDC->SetROP2(nOldDrawMode);
	 			// 		    pDC->SetBkMode(OPAQUE);
	 		}
	 		// add end by zgliu 2011.04.15
	 
	 		//////////////////////////////////////////		  
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
	 		// 每1KM显示一个刻度值
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
	 				if (-1 != m_updown.Find(_T("增加")))
	 				{
	 					strTempKM.Format(_T("%0.2f"), atof(startKM) + i);
	 				}
	 				else if (-1 != m_updown.Find(_T("减少")))
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
	// TODO: 在此添加控件通知处理程序代码
}
