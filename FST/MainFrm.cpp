
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "FST.h"
#include "FSTView.h"
#include "MainFrm.h"
#include "afxwin.h"
#include "exitFrame.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
extern int readStatus;
CStatusBar  m_wndStatusBar;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	// TODO: add member initialization code here
	//add by sgw
	//����ԭ��Ļ�ֱ��ʣ��Ա�ϵͳ�ر�ʱ�ָ�
	m_width = GetSystemMetrics(SM_CXSCREEN);
	m_height = GetSystemMetrics(SM_CYSCREEN);
	//�ı�ϵͳ�ֱ���
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));//add by shao(����ڴ�)
	dm.dmSize = sizeof(dm);//add by shao(���ô�С)
	dm.dmBitsPerPel = 32;
	dm.dmPelsWidth = 1024;
	dm.dmPelsHeight = 768;
	//dm.dmDisplayFrequency=75; 
	dm.dmFields = DM_PELSWIDTH | DM_BITSPERPEL | DM_PELSHEIGHT/*|DM_DISPLAYFREQUENCY*/;
	LONG res;
	res = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
	//add end

	//add by sgw
	// �������Զ�����
	APPBARDATA abd;
	HWND hTaskBar;
	abd.cbSize = sizeof(abd);
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
	hTaskBar = ::FindWindowEx(NULL, NULL, "Shell_TrayWnd", NULL);
	if (hTaskBar != 0)
	{
		abd.hWnd = hTaskBar;
		abd.lParam = ABS_ALWAYSONTOP | ABS_AUTOHIDE;
		SHAppBarMessage(ABM_SETSTATE, &abd);
	}
	//add end
}

CMainFrame::~CMainFrame()
{
	//add by sgw
	//�ָ�ԭ����Ļ�ֱ���
	DEVMODE dm;
	dm.dmBitsPerPel = 32;
	dm.dmPelsWidth = m_width;
	dm.dmPelsHeight = m_height;
	//dm.dmDisplayFrequency=75;
	dm.dmSize = sizeof(dm);
	dm.dmFields = DM_PELSWIDTH | DM_BITSPERPEL | DM_PELSHEIGHT/*|DM_DISPLAYFREQUENCY*/;
	LONG res;
	res = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
	//add end

	//add by sgw
	//�������Զ����ػָ�
	APPBARDATA abd;
	HWND hTaskBar;
	abd.cbSize = sizeof(abd);
	SHAppBarMessage(ABM_GETTASKBARPOS, &abd);
	hTaskBar = ::FindWindowEx(0, 0, "Shell_TrayWnd", NULL);
	if (hTaskBar != 0)
	{
		abd.hWnd = hTaskBar;
		abd.lParam = ABS_ALWAYSONTOP;
		SHAppBarMessage(ABM_SETSTATE, &abd);
	}
	//add end
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������



void CMainFrame::OnClose()
{
	if (1 == readStatus)
	{
		MessageBox(_T("\r\n����ֹͣ��ǰ��·�Ĳ���!"), _T("������ʾ"));
		return;
	}
	else
	{
		exitFrame cExit;
		if (IDOK == cExit.DoModal())
		{
			//	AfxGetMainWnd()->SendMessage(WM_CLOSE);
			CFrameWnd::OnClose();
		}

	}
}
