#pragma once


// CAnnotateMarkDlg ��ȭ �����Դϴ�.

class CAnnotateMarkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnnotateMarkDlg)

public:
	CAnnotateMarkDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnnotateMarkDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANNOTATE_MARK };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
