#pragma once


// CEnvironmentDlg dialog

class CEnvironmentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEnvironmentDlg)

public:
	CEnvironmentDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEnvironmentDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENVIRONMENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	UINT m_iCvySpeed;
	virtual BOOL OnInitDialog();
	CString m_strG1WSRFile;
	afx_msg void OnBnClickedBtnSaveimgfolder();
	CString m_strDataSaveFolder;
	// 가감속시간
	UINT m_iAccDecel;
	afx_msg void OnEnChangeEdtAccdcel();
	afx_msg void OnBnClickedBtnGp1wpsel();
};
