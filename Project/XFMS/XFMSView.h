
// XFMSView.h : CXFMSView Ŭ������ �������̽�
//

#pragma once
#include "atltypes.h"

class CCamDlg;
class COpPanelDlg;
class CTeachingDlg;
class CStatusPanelDlg;
class CGraphDlg;
class CHistoLevellingDlg;
class CToolPanelDlg;

class CXFMSView : public CView
{
protected: // serialization������ ��������ϴ�.
	CXFMSView();
	DECLARE_DYNCREATE(CXFMSView)

// Ư���Դϴ�.
public:
	CXFMSDoc* GetDocument() const;

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
	virtual ~CXFMSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	int MakeChildWnd();
	CCamDlg *m_pCamDlg[4];
	COpPanelDlg *m_pOpPanelDlg;
	CTeachingDlg *m_pTeachingDlg;
	int WndRepositioning();
	CRect CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset = 0);
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetOPMode(int OPMode);
	BOOL IsTeachingMode();
	CRect m_rectCamFull;
	int m_nCountOfCam;
	CStatusPanelDlg *m_pStatusPanelDlg;
	void DoTrial();
	int LoadDefaultJob();
	int CloseAll();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CGraphDlg *m_pGraphDlg;
	CHistoLevellingDlg *m_pHistoLevellingDlg;
	afx_msg void OnToolHistowindowlevel();
	afx_msg void OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI);
	CToolPanelDlg *m_pToolPanelDlg;
};

#ifndef _DEBUG  // SCInspectorView.cpp�� ����� ����
inline CXFMSDoc* CXFMSView::GetDocument() const
   { return reinterpret_cast<CXFMSDoc*>(m_pDocument); }
#endif

