#pragma once
#include "afxcmn.h"


// CCamViewDlg ��ȭ �����Դϴ�.

class CCamViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCamViewDlg)

public:
	CCamViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCamViewDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_CAMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabCamSel;
	virtual BOOL OnInitDialog();
};
