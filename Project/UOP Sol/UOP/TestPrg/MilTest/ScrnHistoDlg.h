#pragma once
#include "HistoSlider.h"
#include "histoGraph.h"
#include <matrox.h>

// CScrnHistoDlg ��ȭ �����Դϴ�.



class CScrnHistoDlg : public CDialog
{
	DECLARE_DYNAMIC(CScrnHistoDlg)

public:
	CScrnHistoDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CScrnHistoDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SCRN_HISTO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	CHistoSlider m_HistoSlider;
	CHistoGraph m_HistoOrg;
	CHistoGraph m_HistoModified;
	virtual BOOL OnInitDialog();
	CMilBuffer *m_pImage;
	double m_dXValPro[65536];

//protected:
	double *m_pHistoValueOrg;
	double *m_pHistoValueModified;
public:
	long m_SliderLeft;
	long m_SliderRight;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL m_IsManual;
	LRESULT OnAdjModeUpdated(UINT wParam, LONG lParam);
	LRESULT OnAdjRangeUpdated(UINT wParam, LONG lParam);
	LRESULT OnAdjRangeChanged(UINT wParam, LONG lParam);
};
