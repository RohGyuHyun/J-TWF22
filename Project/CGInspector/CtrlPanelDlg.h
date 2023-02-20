#pragma once
#include "afxwin.h"
#include "SCButton.h"


// CCtrlPanelDlg ��ȭ �����Դϴ�.

class CCtrlPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCtrlPanelDlg)

public:
	CCtrlPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCtrlPanelDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTRLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRunstopbtn();
	CSCButton m_btnRun;
	CSCButton m_btnLoadImg;
	CSCButton m_btnSaveImg;
	CSCButton m_btnSaveInspImg;
	CSCButton m_btnTrial;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedLoadimg();
	int m_nDispayType;
	afx_msg void OnBnClickedRadio(UINT uiID);
	void OnBnClickedTopSideInspType(UINT uiID);
	afx_msg void OnBnClickedTrial();
	afx_msg void OnBnClickedSaveimg();
	// Top/Bottomī�޶� �۾�����
	int m_nCamSel;
	int m_nTopSideInspType;
	afx_msg void OnBnClickedSaveinspimg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
