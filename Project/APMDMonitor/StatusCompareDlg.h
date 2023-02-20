#pragma once
#include "afxcmn.h"

#include "Status.h"
// CStatusCompareDlg ��ȭ �����Դϴ�.

class CStatusCompareDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStatusCompareDlg)

public:
	CStatusCompareDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CStatusCompareDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STATUS_COMPARE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstStatChart;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int SetDispData(tAPMDDispData & dispMain, tAPMDDispData & dispCompare);
	void SetDispCore(tAPMDDispData * pAPMDDispData, int KindOfData);
};
