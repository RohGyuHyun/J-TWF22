#pragma once
#include "afxwin.h"


// CConfigDlg ��ȭ �����Դϴ�.

class CConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg)

public:
	CConfigDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CConfigDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
