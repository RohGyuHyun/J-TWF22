
// CGInspectorView.h : CCGInspectorView Ŭ������ �������̽�
//

#pragma once


#include "CtrlPanelDlg.h"
#include "ViewDlg.h"

class CStatusPanelDlg;
class CCamDlg;

class CCGInspectorView : public CView
{
protected: // serialization������ ��������ϴ�.
	CCGInspectorView();
	DECLARE_DYNCREATE(CCGInspectorView)

// Ư���Դϴ�.
public:
	CCGInspectorDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CCGInspectorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	// ���� �����츦 ���ο� �ٿ� ������
	int MakeChildWnd();

	// Control ����� �ν��Ͻ��� ������ �ִ� ����
	CCtrlPanelDlg *m_pCtrlPanelDlg;
	int ReleaseChildWnd();
	int WndRepositioning();
	virtual void OnInitialUpdate();
	CViewDlg *m_pViewDlg;
	CStatusPanelDlg *m_pStatusPanelDlg;
	afx_msg void OnSetupConv();
	afx_msg void OnSetupDataaqu();
	afx_msg void OnSetupRev();
	afx_msg void OnSetupI();
	void CloseAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CCamDlg *m_pCamDlg[2];
	void SetOPMode(int OPMode);
	void SetCameraOPMode(int OPMode);
	afx_msg void OnSetupEnv();
	afx_msg void OnUpdateSetupEnv(CCmdUI *pCmdUI);
	int ViDiInit();
	int ViDiClose();
	afx_msg void OnSetupDrawdefect();
};

#ifndef _DEBUG  // CGInspectorView.cpp�� ����� ����
inline CCGInspectorDoc* CCGInspectorView::GetDocument() const
   { return reinterpret_cast<CCGInspectorDoc*>(m_pDocument); }
#endif

