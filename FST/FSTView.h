
// FSTView.h : CFSTView ��Ľӿ�
//

#pragma once


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
};

#ifndef _DEBUG  // FSTView.cpp �еĵ��԰汾
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

