#pragma once
#include "SCAlgoDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "SCButton.h"

// CSCFidParamDlg ��ȭ �����Դϴ�.

class CSCFidParamDlg : public CSCAlgoDialog
{
	DECLARE_DYNAMIC(CSCFidParamDlg)

public:
	CSCFidParamDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSCFidParamDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_FIDSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	static CSCFidParamDlg *m_pInstance;
	static CSCFidParamDlg * Instance();
	void GetFidParam();
	void ApplyFidParam();
	void ShowInspResult(void);
	afx_msg void OnBnClickedBtnFidreg();
	virtual BOOL OnInitDialog();
	virtual void LoadParam(CSCAlgoBase * pAlgo);
	CRect GetROIRect();
	void MakeDefaultFid();
	CStatic m_FidImgView;
	CMilDisplay *m_pMilFidDisp;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFidtrial();
	void CloseMod();
	UINT m_iScore;
	CString m_InspXOfs;
	CString m_InspYOfs;
	CNumSpinCtrl m_spFidROIW;
	CNumSpinCtrl m_spFidROIH;
	double m_ROIFactorW;
	double m_ROIFactorH;
	CSCButton m_cReg;
	CSCButton m_cTest;
	CSCButton m_cTrial;
};
