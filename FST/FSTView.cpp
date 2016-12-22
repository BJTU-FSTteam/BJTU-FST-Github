
// FSTView.cpp : CFSTView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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

// CFSTView ����/����

CFSTView::CFSTView()
	: CFormView(IDD_FST_FORM)
{
	// TODO: �ڴ˴���ӹ������

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
	// TODO: �ڴ���������������
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
}


void CFSTView::OnModeButton()
{
	// TODO: �ڴ���������������
}
