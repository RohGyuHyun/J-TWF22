#pragma once
#include "SCButton.h"
//#include <XaGrabber.h>


// CSaveModelData ��ȭ �����Դϴ�.

class CSaveModelDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CSaveModelDataDlg)

public:
	CSaveModelDataDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSaveModelDataDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SAVE_MODELDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnBrowser();
	CString m_strModelName;
	virtual BOOL OnInitDialog();
	CMilBuffer *SavePCBAllBuff;

	CStatus m_pStatus;

	void CopyfileSetuptoModelFloder(CString OriginPath,CString NewPath);
	CSCButton m_cOK;
	CSCButton m_cCancel;
};
