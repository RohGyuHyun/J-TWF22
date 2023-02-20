#pragma once
#include "afxwin.h"


// CConfigDlg 대화 상자입니다.

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CConfigDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CComboBox m_cbCvyrFront;
	CComboBox m_cbCvyrEnd;
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	UINT m_nCGLength;
};
