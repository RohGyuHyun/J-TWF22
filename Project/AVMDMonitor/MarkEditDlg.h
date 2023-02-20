#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMarkEditDlg ��ȭ �����Դϴ�.
class CViewOptionDlg;
class CMarkEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMarkEditDlg)

public:
	CMarkEditDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMarkEditDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MARKEDIT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_strMarkEdit;
	afx_msg void OnBnClickedBtnAdd();
	CListBox m_lstbMarks;
	afx_msg void OnBnClickedBtnItemdel();
	afx_msg void OnBnClickedBtnItemins();
	afx_msg void OnBnClickedBtnItemupdate();
	afx_msg void OnLbnSelchangeLstbMark();
	afx_msg void OnBnClickedBtnItemup();
	afx_msg void OnBnClickedBtnItempdown();
	afx_msg void OnBnClickedBtnItemload();
	afx_msg void OnBnClickedBtnItemsave();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOk();
	void SetViewOptionDlg(CViewOptionDlg * pViewOptionDlg);
	CViewOptionDlg *m_pViewOptionDlg;
	void LoadMark();
};
