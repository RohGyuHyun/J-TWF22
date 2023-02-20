#pragma once
#include "afxwin.h"
#include "resource.h"
#include <UniUtility.h>

// CMessageWnd ��ȭ �����Դϴ�.

class CMessageWnd : public CDialog
{
	DECLARE_DYNAMIC(CMessageWnd)

public:
	CMessageWnd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMessageWnd();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MESSAGE_WND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CLabel m_StarMessageLabel;
	afx_msg void OnBnClickedOk();
	void SetMessageText(CString str);
	void UIIint();
	virtual BOOL OnInitDialog();

	BOOL m_bSelectWnd;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	CString m_StarMessageLabelText;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOkmessagebt();
	afx_msg void OnBnClickedCancelMessagebt();
	CButton m_StarPrjExitButton;
	afx_msg void OnBnClickedButtonExitstar();
	int m_iTextID;
	static CMessageWnd *m_pInstance;
	static CMessageWnd * Instance();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
