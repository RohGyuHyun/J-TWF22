#pragma once
#include "TabDetailLogDlg.h"
#include "TabDetailResultDlg.h"


// CDetailPropDlg

/*\brief ���â�� Log�� Result���̱� ���� Container���̾�α�.@n 
\date 2010.01.05
\author �� ����
*/
class CDetailPropDlg : public CPropertySheet
{
	DECLARE_DYNAMIC(CDetailPropDlg)

public:
	CDetailPropDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDetailPropDlg(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDetailPropDlg();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CTabDetailLogDlg *m_pTabDetailLogDlg;
	CTabDetailResultDlg m_TabDetailResultDlg;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBitmap m_background;
};


