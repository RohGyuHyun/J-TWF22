#pragma once
#include "afxwin.h"
#include "resource.h"
#include "SCAlgoBase.h"
#include "SCAlgoDialog.h"
#include "SCButton.h"

// CSCGapGageParamDlg ��ȭ �����Դϴ�.

class CSCGapGageParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCGapGageParamDlg)

public:
	CSCGapGageParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCGapGageParamDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SCGAPGAGEPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static CSCGapGageParamDlg * Instance();
	static CSCGapGageParamDlg *m_pInstance;
	CComboBox m_cSelScanDir;
	virtual BOOL OnInitDialog();
	void LoadParam(CSCAlgoBase * pAlgo);
	afx_msg void OnBnClickedOk();
	CSCButton m_cApply;
	CSCButton m_cCancel;
};
