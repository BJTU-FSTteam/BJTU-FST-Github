
// FSTView.cpp : CFSTView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FST.h"
#endif

#include "FSTDoc.h"
#include "FSTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFSTView

IMPLEMENT_DYNCREATE(CFSTView, CFormView)

BEGIN_MESSAGE_MAP(CFSTView, CFormView)
	ON_COMMAND(ID_START_BUTTON, &CFSTView::OnStartButton)
	ON_COMMAND(ID_STOP_BUTTON, &CFSTView::OnStopButton)
	ON_COMMAND(ID_UPDATE_BUTTON, &CFSTView::OnUpdateButton)
	ON_COMMAND(ID_SAVEDATA_BUTTON, &CFSTView::OnSavedataButton)
	ON_COMMAND(ID_PR100_SETTING, &CFSTView::OnPr100Setting)
	ON_COMMAND(ID_MODE_BUTTON, &CFSTView::OnModeButton)
END_MESSAGE_MAP()

// CFSTView 构造/析构

CFSTView::CFSTView()
	: CFormView(IDD_FST_FORM)
{
	// TODO: 在此处添加构造代码

}

CFSTView::~CFSTView()
{
}

void CFSTView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
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
}


void CFSTView::OnModeButton()
{
	// TODO: 在此添加命令处理程序代码
}
