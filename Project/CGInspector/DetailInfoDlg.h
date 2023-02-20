#pragma once
#include "afxcmn.h"
#include "detailpropdlg.h"
#include <UniUtility.h>
#include "TabDetailLogDlg.h"
#include "TabDetailResultDlg.h"


// CDetailInfoDlg dialog

class CDetailInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CDetailInfoDlg)

public:
	CDetailInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDetailInfoDlg();

// Dialog Data
	enum { IDD = IDD_DETAIL_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CDetailPropDlg *m_pDetailPropDlg;
public:

	     /********mil ���� �κ�************/
	/*	CMilDisplay *m_pMyDisplay;
		CMilBuffer *m_DisplayAllMap;
		CMilBuffer *m_DisplayOnePice;

		int DisplayAllMapInfo();
*/

public:
	/*********�̱���**************/

	
    static CDetailInfoDlg* Instance(void);                 //<�̱��� �ν��Ͻ� �κ�(.)
    static CDetailInfoDlg* m_pInstance;
    
	CTabDetailResultDlg *m_pTabDetailResultDlg;
    CTabDetailLogDlg *m_pTabDetailLogDlg;  


	afx_msg void OnBnClickedOkbtn();
	afx_msg void OnBnClickedOkbtn20();
	void ShowResultPage(void);
	CImageList m_ImageList;
	CXTabCtrl m_TabCtrl;
	 void initTabContain();
	 afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	 afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	 afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	 void WndRepositioning();
	 afx_msg void OnSize(UINT nType, int cx, int cy);
};
