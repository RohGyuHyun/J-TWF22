#pragma once


// CStatusPanelDlg ��ȭ �����Դϴ�.
class CDetailInfoDlg;

class CStatusPanelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusPanelDlg)

public:
	CStatusPanelDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStatusPanelDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STSPANEL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDetailInfoDlg *m_pDetailInfoDlg;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void WndRePositioning();
};
