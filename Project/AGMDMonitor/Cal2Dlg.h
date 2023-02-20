#pragma once
#include "afxcmn.h"
#include "Status.h"


// CCal2Dlg ��ȭ �����Դϴ�.

class CCal2Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCal2Dlg)

public:
	CCal2Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCal2Dlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAL2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cResult;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void MakeChart();
	void MakeChartFrame();
	tCalData m_NewCalData;
	void CalculateCalData();
	int m_iNewCal1;
	int m_iNewCal2;
	int m_iNewCal3;
	int m_iNewJigGap;
	void SendCalSetCmd();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
