#pragma once
#include "afxwin.h"


// CConvTestDlg dialog

class CConvTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConvTestDlg)

public:
	CConvTestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CConvTestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONVTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnFwd();
	afx_msg void OnBnClickedBtnBwd();
	afx_msg void OnBnClickedBtnAuto();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_bAuto;
	virtual BOOL OnInitDialog();
	CButton m_ctrlInput;
	CButton m_ctrlOutput;
	afx_msg void OnBnClickedOk();
	int m_Dir;
};
