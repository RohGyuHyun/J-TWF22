#pragma once
#include "afxwin.h"

class CCal1Dlg;
class CCal2Dlg;

// CCalibrationDlg ��ȭ �����Դϴ�.

class CCalibrationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibrationDlg)

public:
	CCalibrationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCalibrationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CALIBRATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int MakeChildWnd();
	CCal1Dlg *m_pCal1Dlg;
	CCal2Dlg *m_pCal2Dlg;
	int WndRepositioning();
	CButton m_cNext;
	CButton m_cPrev;
	int m_nStep;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnNext();
	void SetStep(int nStep);
	afx_msg void OnBnClickedBtnPrev();
	CButton m_cBtnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};
