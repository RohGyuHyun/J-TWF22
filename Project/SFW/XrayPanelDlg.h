#pragma once


// CXrayPanelDlg 대화 상자입니다.

class CXrayPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXrayPanelDlg)

public:
	CXrayPanelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CXrayPanelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_XRAYCTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
