#pragma once
#include "afxcmn.h"


// CInfoPanelDlg ��ȭ �����Դϴ�.

class CInfoPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInfoPanelDlg)

public:
	CInfoPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInfoPanelDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_INFOPANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_tabInfoSel;
};
