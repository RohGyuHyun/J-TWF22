#pragma once
#include "afxcmn.h"

class CInspResult;

// CTabDetailResultDlg dialog
/**
\brief CDetailPropDlg�� �پ �˻����� ���� ������ ��Ÿ���� ���� Logâ.@n 
\date 2010.01.05
\author �� ����
*/
class CTabDetailResultDlg : public CPropertyPage
{
	DECLARE_DYNAMIC(CTabDetailResultDlg)

public:
	CTabDetailResultDlg();
	virtual ~CTabDetailResultDlg();

// Dialog Data
	enum { IDD = IDD_TAB_DETAIL_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	void SetResultDisplay(CInspResult *pInspResult);
    
	virtual BOOL OnInitDialog();
	CListCtrl m_listResult;

	static CTabDetailResultDlg* Instance(void);   ///<Instance ���� 

	static CTabDetailResultDlg* m_pInstance;      ///<�ڱ� ���� ������ 
    
};
