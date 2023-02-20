#pragma once
#include "afxcmn.h"
#include "SCButton.h"


// CLightSetDlg ��ȭ �����Դϴ�.

class CLightSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLightSetDlg)

public:
	CLightSetDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLightSetDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIGHTSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CSpinButtonCtrl m_spL0;
	CSpinButtonCtrl m_spL1;
	CSpinButtonCtrl m_spL2;
	CSpinButtonCtrl m_spL3;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	long m_ChBrightness[4];
	afx_msg void OnBnClickedChkl0();
	afx_msg void OnBnClickedChkl1();
	afx_msg void OnBnClickedChkl2();
	afx_msg void OnBnClickedChkl3();
	void LightSet(int ch, bool bStatus);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDeltaposSpl0(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpl1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpl2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpl3(NMHDR *pNMHDR, LRESULT *pResult);
	CSCButton m_cOK;
	CSCButton m_cCancel;
};
