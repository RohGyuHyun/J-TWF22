#pragma once
#include "afxdialogex.h"


// CDialgInterLockStatus 대화 상자

class CDialgInterLockStatus : public CDialogEx
{
	DECLARE_DYNAMIC(CDialgInterLockStatus)

public:
	CDialgInterLockStatus(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialgInterLockStatus();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_INTER_LOCK_STATUS };
#endif

public:


private:
	BOOL GetInterLockStatus(int nIndex);


protected:
	CBtnenhctrl m_btnInterLockStatus[8];
	BOOL m_bInterLockStatus[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP();
	DECLARE_EVENTSINK_MAP();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
