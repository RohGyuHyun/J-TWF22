#pragma once


// CEnvSettingDlg ��ȭ �����Դϴ�.

class CEnvSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvSettingDlg)

public:
	CEnvSettingDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEnvSettingDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVSETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strSaveFolder;
	afx_msg void OnBnClickedBtnSetfolder();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strDataFileName;
};
