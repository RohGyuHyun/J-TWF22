#pragma once


// CTempDlg ��ȭ �����Դϴ�.

class CTempDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTempDlg)

public:
	CTempDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTempDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEMPDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
