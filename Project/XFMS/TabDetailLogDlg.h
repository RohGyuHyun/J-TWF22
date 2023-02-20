#pragma once
#include "afxwin.h"
#include "resource.h"
#include <UniUtility.h>

/** \brief Log â�� �����Ͽ� ���� ���� ������ ���¸� ǥ�� ���� �� �ֵ��� �Ͽ����ϴ�.
Log â�� CEdit �� ������� �ۼ��Ͽ����� �⺻ ������ ������ ��� �۾��� ���� �Ͽ����ϴ�.

���� �ؽ�Ʈ �̻� �Ѿ�� �ڵ����� Ŭ����ǵ��� �Ͽ����ϴ�.

X-ray ����, Grib, ���� ��� ���� �鿡�� Ȱ�� �� �� �ֵ��� 

Instance �Լ��� ����� �� �ٸ� Ŭ�������� �޼����� ���� �� �� �ֵ��� �Ͽ����ϴ�.
(�̱��� ���)


\date 2009.12.28
\author Ji Hoon Ahn
*/
class  CTabDetailLogDlg : public CPropertyPage
	

{
	DECLARE_DYNAMIC(CTabDetailLogDlg)

public:

	CTabDetailLogDlg();
	virtual ~CTabDetailLogDlg();

// Dialog Data
	enum { IDD = IDD_TAB_DETAIL_LOG };
     


	static CTabDetailLogDlg* Instance(void);   ///<Instance ���� 

	static CTabDetailLogDlg* m_pInstance;      ///<�ڱ� ���� ������ 
    
	


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);





     CBrush *brush1;
	void SetTextLog(CString Text,COLORREF RGBcolor,BOOL ERR);  ///< Log â�� Text �� ����� �� �ֵ��� �ϴ� �Լ� 
	virtual BOOL OnInitDialog();    
	afx_msg void OnBnClickedTextLogClear();   ///< LogText Clear ��ư 
	
	CEdit m_EditLog;
	CButtonST m_ClearBtn;

//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString m_StringErr;
	CEdit m_LogdataEditControl;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
