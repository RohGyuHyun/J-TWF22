#pragma once


// CReportDlg ��ȭ �����Դϴ�.

class CReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReportDlg)

public:
	CReportDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CReportDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnload();
	afx_msg void OnBnClickedBtnDataapply();
	virtual BOOL OnInitDialog();
	int m_nReportData;
	void Initialize(CString strExcelFile=_T(""));
	CString m_strExcelFile;
	afx_msg void OnBnClickedOk();
	BOOL m_bIsSelectLog;
	afx_msg void OnBnClickedCancel();
};
