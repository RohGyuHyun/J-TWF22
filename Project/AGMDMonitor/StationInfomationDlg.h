#pragma once


// CStationInfomationDlg ��ȭ �����Դϴ�.

class CStationInfomationDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStationInfomationDlg)

public:
	CStationInfomationDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStationInfomationDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATIONINFOMATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnClear();
	CString m_strOperator;
	CString m_strTool;
	CString m_strStation;
	CString m_strComment;
	afx_msg void OnBnClickedBtnApply();
	virtual BOOL OnInitDialog();
	void Initialize();
	LRESULT WorkUpdate(WPARAM wParam, LPARAM lParam);
};
