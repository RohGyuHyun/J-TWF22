#pragma once
#include "afxwin.h"


// CCGIOMapNTestDlg 대화 상자입니다.

class CCGIOMapNTestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCGIOMapNTestDlg)

public:
	CCGIOMapNTestDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCGIOMapNTestDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGIOMAPNTEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cCBInSensor;
	CComboBox m_cCBOutSensor;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
