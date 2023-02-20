#pragma once
#include "afxdialogex.h"


// CDialogLogin 대화 상자

class CDialogLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogLogin)

public:
	CDialogLogin(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogLogin();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOGIN };
#endif

protected:

	CString m_strPW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlPw();
	void ClickBtnenhctrlUserLogin();
	void ClickBtnenhctrlAdminLogin();
	void ClickBtnenhctrlExit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ClickBtnenhctrlAdminLogin2();
};
