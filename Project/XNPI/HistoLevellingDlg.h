#pragma once
#include "afxwin.h"
#include "SCButton.h"
#include "HistoHandle.h"

// CHistoLevellingDlg ��ȭ �����Դϴ�.

class CXNPIView;

class CHistoLevellingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHistoLevellingDlg)

public:
	CHistoLevellingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHistoLevellingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HISTOLEVEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void HistoUpdate();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CXNPIView *m_pView;
	CMilBuffer *m_pOrgMilBuffer;
	CStatic m_cHisotoArea;
	void DrawHistogram();
	MIL_INT m_Histodata[65536];

	afx_msg void OnBnClickedBtnApply();
	CHistoHandle m_cMinHandle;
	CHistoHandle m_cMaxHandle;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CStatic m_cHistoHandleRect;
	LRESULT OnHistoHDLUpdate(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedBtLoadimg();
	// ������׷��߿� ���� ū ��
	afx_msg void OnBnClickedBtnSavwlv();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	CComboBox m_cbxWLPreset;
	afx_msg void OnCbnSelchangeComboPreset();
};
