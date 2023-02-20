#pragma once
#include "afxwin.h"


// CWarningSetDlg ��ȭ �����Դϴ�.

class CWarningSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWarningSetDlg)

public:
	CWarningSetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CWarningSetDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WARN_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_cSampleTimeSelect;
	double m_dblTemp;
	BOOL m_bUseTemp;
	BOOL m_bUseP01;
	BOOL m_bUseP03;
	BOOL m_bUseP05;
	BOOL m_bUseWarning;
	virtual BOOL OnInitDialog();
	int m_iThresP01;
	int m_iThresP03;
	int m_iThresP05;
};
