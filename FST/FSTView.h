
// FSTView.h : CFSTView 类的接口
//

#pragma once


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
};

#ifndef _DEBUG  // FSTView.cpp 中的调试版本
inline CFSTDoc* CFSTView::GetDocument() const
   { return reinterpret_cast<CFSTDoc*>(m_pDocument); }
#endif

