#pragma once


// CXMLTestDlg ��ȭ �����Դϴ�.

class CXMLTestDlg : public CDialog
{
	DECLARE_DYNAMIC(CXMLTestDlg)

public:
	CXMLTestDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CXMLTestDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_XML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnXmlRead();
	afx_msg void OnBnClickedBtnXmlWrite();
};
