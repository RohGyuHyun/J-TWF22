#pragma once


// CXrayPanelDlg ��ȭ �����Դϴ�.

class CXrayPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CXrayPanelDlg)

public:
	CXrayPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CXrayPanelDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_XRAYCTL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
