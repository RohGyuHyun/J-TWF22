
// CGInspectorView.h : CCGInspectorView 클래스의 인터페이스
//

#pragma once


#include "CtrlPanelDlg.h"
#include "ViewDlg.h"

class CStatusPanelDlg;
class CCamDlg;

class CCGInspectorView : public CView
{
protected: // serialization에서만 만들어집니다.
	CCGInspectorView();
	DECLARE_DYNCREATE(CCGInspectorView)

// 특성입니다.
public:
	CCGInspectorDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CCGInspectorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	// 각종 윈도우를 메인에 붙여 생성함
	int MakeChildWnd();

	// Control 페널의 인스턴스를 가지고 있는 변수
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

#ifndef _DEBUG  // CGInspectorView.cpp의 디버그 버전
inline CCGInspectorDoc* CCGInspectorView::GetDocument() const
   { return reinterpret_cast<CCGInspectorDoc*>(m_pDocument); }
#endif

