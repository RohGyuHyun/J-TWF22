#pragma once

#include "resource.h"
#include "SCButton.h"
// CRenameModelDataDlg ��ȭ �����Դϴ�.

class CRenameModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CRenameModelDataDlg)

public:
	CRenameModelDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRenameModelDataDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RENAME_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRename();
	CString m_strDirName;
	CSCButton m_cModify;
	CSCButton m_cCancel;
};
