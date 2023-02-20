#pragma once
#include "afxwin.h"

// CDispUnitsSetDlg ��ȭ �����Դϴ�.
#include "Status.h"
#include "NumEdit.h"

class CDispUnitsSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDispUnitsSetDlg)

public:
	CDispUnitsSetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDispUnitsSetDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DISPUNITSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	int m_iCoordinateSystem;
	int m_iInclinationType;
	virtual BOOL OnInitDialog();
	CComboBox m_cKofAngle;
	CComboBox m_cKofROR;
	afx_msg void OnBnClickedOk();
	double m_dblGoNoGoDegree;
	CString m_strDispTolerance;
	afx_msg void OnCbnSelchangeCbKofangle();
	afx_msg void OnCbnSelchangeCbKofror();
	CString m_strGoNoGoUnit;
	afx_msg void OnBnClickedRdisptype();
	afx_msg void OnBnClickedRdispunit();
	afx_msg void OnBnClickedRror();
	afx_msg void OnBnClickedRmagdir();
	void GoNoGoDispUpdate();
	double m_dblDispTolerance;
	void GetDispData();
	void GetGoNoGoDispInitData();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL GoNoGoEdtChange();
	void GetDispGoNoGo();
	void SetGoNoGoLabel();
	CString m_strGoNoGoRange;
	double m_dblToleranceLow;
	double m_dblToleranceHigh;
	BOOL IsInTheTolRange();
	afx_msg void OnEnUpdateEdtTol();
	CNumEdit m_cTolValue;
};
