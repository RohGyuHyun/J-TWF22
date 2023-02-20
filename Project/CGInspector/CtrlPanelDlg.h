#pragma once
#include "afxwin.h"
#include "SCButton.h"


// CCtrlPanelDlg 대화 상자입니다.

class CCtrlPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCtrlPanelDlg)

public:
	CCtrlPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCtrlPanelDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTRLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
	// Top/Bottom카메라 작업설정
	int m_nCamSel;
	int m_nTopSideInspType;
	afx_msg void OnBnClickedSaveinspimg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
