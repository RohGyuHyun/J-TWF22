#pragma once
#include "afxcmn.h"


// CTabContainerDlg ��ȭ �����Դϴ�.

class CTabContainerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTabContainerDlg)

public:
	CTabContainerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabContainerDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TABCONTAINER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabPanelContainer;
	virtual BOOL OnInitDialog();
};
