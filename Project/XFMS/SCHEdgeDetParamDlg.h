#pragma once

#include "SCAlgoDialog.h"
#include "afxwin.h"
#include "SCButton.h"
// CSCHEdgeDetParamDlg ��ȭ �����Դϴ�.

class CSCHEdgeDetParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCHEdgeDetParamDlg)

public:
	CSCHEdgeDetParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCHEdgeDetParamDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCHEDGEDETPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static CSCHEdgeDetParamDlg * Instance();
	static CSCHEdgeDetParamDlg *m_pInstance;
	virtual BOOL OnInitDialog();
	virtual void LoadParam(CSCAlgoBase * pAlgo);
	CComboBox m_cHEDetDir;
	afx_msg void OnBnClickedOk();
	CSCButton m_cApply;
	CSCButton m_cCancel;
};
