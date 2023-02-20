
// APMDMonitorView.h : CAPMDMonitorView Ŭ������ �������̽�
//

#pragma once

#include "ViewOptionDlg.h"
#include "GraphDlg.h"
#include "ModeDlg.h"
#include "StatusDlg.h"
#include "ComSetDlg.h"
#include "MarkEditDlg.h"
#include "WarningSetDlg.h"
#include "EnvSettingDlg.h"
#include "afxcoll.h"
#include "StatusCompareDlg.h"

class CAPMDMonitorView : public CView
{
protected: // serialization������ ��������ϴ�.
	CAPMDMonitorView();
	DECLARE_DYNCREATE(CAPMDMonitorView)

// Ư���Դϴ�.
public:
	CAPMDMonitorDoc* GetDocument() const;

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
	virtual ~CAPMDMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	int MakeChildWnd();
	CViewOptionDlg *m_pViewOptionDlg;
	CGraphDlg *m_pGraphDlg;
	CModeDlg *m_pModeDlg;
	CStatusDlg *m_pStatusDlg;
	CComSetDlg *m_pComsetDlg;

	afx_msg void OnComset();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void WndRepositioning();
	afx_msg void OnWarnset();
	afx_msg void OnMarkeditor();
	afx_msg void OnEnvset();
	CPtrList m_lstMarks;
	int MakeExcelReport(CString strFileName);
	afx_msg void OnViewComparedata();
	afx_msg void OnOverlay();
	void DemendComparedataView();
	void ApplyCompareMode();
	CStatusCompareDlg *m_pStatusCompareDlg;
	afx_msg void OnUpdateLoadComparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateViewComparedata(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOverlay(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // APMDMonitorView.cpp�� ����� ����
inline CAPMDMonitorDoc* CAPMDMonitorView::GetDocument() const
   { return reinterpret_cast<CAPMDMonitorDoc*>(m_pDocument); }
#endif

