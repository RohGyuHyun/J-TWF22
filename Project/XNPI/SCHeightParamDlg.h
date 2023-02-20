#pragma once

#include "SCAlgoDialog.h"
#include "afxwin.h"
#include "SCButton.h"
// CSCHeightParamDlg ��ȭ �����Դϴ�.

class CSCHeightParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCHeightParamDlg)

public:
	CSCHeightParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCHeightParamDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEIGHTPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static CSCHeightParamDlg * Instance();
	static CSCHeightParamDlg *m_pInstance;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual void LoadParam(CSCAlgoBase * pAlgo);
	CComboBox m_cScanDir;
	CSCButton m_cApply;
};
