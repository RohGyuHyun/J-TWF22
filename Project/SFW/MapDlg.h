#pragma once
#include "afxcmn.h"


// CMapDlg ��ȭ �����Դϴ�.

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)

public:
	CMapDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabMapSel;
	virtual BOOL OnInitDialog();
};
