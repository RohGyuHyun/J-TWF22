#pragma once


// CViewDlg ��ȭ �����Դϴ�.

class CViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDlg)

public:
	CViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CViewDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
