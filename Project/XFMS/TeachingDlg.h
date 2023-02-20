#pragma once
#include "SCButton.h"


// CTeachingDlg ��ȭ �����Դϴ�.

class CTeachingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTeachingDlg)

public:
	CTeachingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTeachingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEACHING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRetopmode();
	int m_iTeachingMode;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedModes();
	afx_msg void OnBnClickedBtnTrial();
	CSCButton m_cOPMode;
	CSCButton m_cTrial;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
