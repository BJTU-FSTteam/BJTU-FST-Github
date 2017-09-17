
// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	// TODO: add member initialization code here
	//add by sgw
	//保留原屏幕分辨率，以便系统关闭时恢复
	m_width = GetSystemMetrics(SM_CXSCREEN);
	m_height = GetSystemMetrics(SM_CYSCREEN);
	//改变系统分辨率
	DEVMODE dm;
	ZeroMemory(&dm, sizeof(dm));//add by shao(清空内存)
	dm.dmSize = sizeof(dm);//add by shao(设置大小)
	dm.dmBitsPerPel = 32;
	dm.dmPelsWidth = 1024;
	dm.dmPelsHeight = 768;
	//dm.dmDisplayFrequency=75; 
	dm.dmFields = DM_PELSWIDTH | DM_BITSPERPEL | DM_PELSHEIGHT/*|DM_DISPLAYFREQUENCY*/;
	LONG res;
	res = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
	//add end

	//add by sgw
	// 任务栏自动隐藏
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
	//恢复原来屏幕分辨率
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
	//任务栏自动隐藏恢复
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
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

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


// CMainFrame 消息处理程序



void CMainFrame::OnClose()
{
	if (1 == readStatus)
	{
		MessageBox(_T("\r\n请先停止当前线路的测试!"), _T("操作提示"));
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
