#pragma once
#include "afxwin.h"


// CSetTimeDomainFormatDlg ��ȭ �����Դϴ�.

class CSetTimeDomainFormatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetTimeDomainFormatDlg)

public:
	CSetTimeDomainFormatDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSetTimeDomainFormatDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_TIME_DOMAIN_FORMAT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_ctrl_combo_TimeDomain;
	afx_msg void OnBnClickedOk();
};
