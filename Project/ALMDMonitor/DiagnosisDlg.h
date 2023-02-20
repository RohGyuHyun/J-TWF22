#pragma once
#include "afxwin.h"
#include "atltypes.h"

class CDiagnosis1Dlg;
class CDiagnosis2Dlg;
class CDiagnosis3Dlg;
class CDiagnosis4Dlg;


// CDiagnosisDlg ��ȭ �����Դϴ�.

class CDiagnosisDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiagnosisDlg)

public:
	CDiagnosisDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDiagnosisDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAGNOSIS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDiagnosis1Dlg *m_pDiagnosis1Dlg;
	CDiagnosis2Dlg *m_pDiagnosis2Dlg;
	CDiagnosis3Dlg *m_pDiagnosis3Dlg;
	CDiagnosis4Dlg *m_pDiagnosis4Dlg;
	int MakeChildWnd();
	int WndRepositioning();
	afx_msg void OnClose();
	void SetStep(int nStep);
	afx_msg void OnBnClickedDiagnext();
	int m_nStep;
	CButton m_cDiagPrev;
	CButton m_cDiagNext;
	afx_msg void OnBnClickedDiagprev();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CPoint m_ptDiagData[4];
	void CalcResult(double * pDblRPResult, double * pDblRRResult, double * pDblPPResult);
	CButton m_cBtnCancel;
};
