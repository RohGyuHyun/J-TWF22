#pragma once
#include "afxwin.h"


// CBrowserDlg ��ȭ �����Դϴ�.

class CBrowserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBrowserDlg)

public:
	CBrowserDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrowserDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BROWSERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strBrowser;
	CString m_strDisplayNum;
	CString m_strTotalDisplayNum;
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
private:
	void BrowserUpdate();
public:
	CComboBox m_ctrl_CBBrowser;
	LRESULT afx_msg OnInitBrowser(WPARAM wParam, LPARAM lParam);
	LRESULT afx_msg OnDlgInit(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeCbBrowser();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
