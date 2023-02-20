#pragma once
#include "afxwin.h"
#include "resource.h"
#include "SCAlgoBase.h"
#include "SCAlgoDialog.h"
#include "SCButton.h"

// CSCAlignParamDlg ��ȭ �����Դϴ�.

class CSCAlignParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCAlignParamDlg)

public:
	CSCAlignParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCAlignParamDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SCALIGNPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static CSCAlignParamDlg * Instance();
	static CSCAlignParamDlg *m_pInstance;
	CComboBox m_cSelScanDir;
	virtual BOOL OnInitDialog();
	void LoadParam(CSCAlgoBase * pAlgo);
	afx_msg void OnBnClickedOk();
	CSCButton m_cApply;
	CSCButton m_cCancel;
};
