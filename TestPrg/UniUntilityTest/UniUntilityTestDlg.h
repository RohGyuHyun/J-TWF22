
// UniUntilityTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include <UniUtility.h>


// CUniUntilityTestDlg ��ȭ ����
class CUniUntilityTestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CUniUntilityTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_UNIUNTILITYTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBxmltest();
	afx_msg void OnBnClickedBtnCfg();
	CButtonST m_ImgBtn;
	afx_msg void OnBnClickedBtnImg();
	afx_msg void OnBnClickedBtnParamparser();
	int GetResult(TCHAR * szData, double & dblScore, CPtrList& listPoints);
};
