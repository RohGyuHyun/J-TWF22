#pragma once
#include "afxwin.h"


// CCGIOMapNTestDlg ��ȭ �����Դϴ�.

class CCGIOMapNTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCGIOMapNTestDlg)

public:
	CCGIOMapNTestDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCGIOMapNTestDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGIOMAPNTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cCBInSensor;
	CComboBox m_cCBOutSensor;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
