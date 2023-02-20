#pragma once
#include "afxdialogex.h"


// CDialogLogo 대화 상자

class CDialogLogo : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogo)

public:
	CDialogLogo(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogLogo();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_LOGO };
#endif
protected:
	int m_nTimer;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
