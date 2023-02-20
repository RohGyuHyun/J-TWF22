#pragma once


// CStationInformationDlg ��ȭ �����Դϴ�.

class CStationInformationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStationInformationDlg)

public:
	CStationInformationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStationInformationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATION_INFORMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnApply();
	CString m_strOperator;
	CString m_strTool;
	CString m_strStation;
	CString m_strComment;
	afx_msg void OnBnClickedBtnClear();
	void GetWorkEnvData();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
