#pragma once
#include "NumEdit.h"


// CStabilizationDlg ��ȭ �����Դϴ�.

class CStabilizationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStabilizationDlg)

public:
	CStabilizationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStabilizationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STABILIZATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	double m_dblStblzSensitivity;
	CNumEdit m_cStblzSensitivity;
	double m_dblSensitivity;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnStblzdefault();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL IsInTheRange();
	BOOL StableEdtChange();
};
