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
	enum { IDD = IDD_ENVSETTINGDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strSaveFolder;
	CString m_strDataFileName;
	afx_msg void OnBnClickedBtnSetfolder();
	afx_msg void OnBnClickedOk();
};
