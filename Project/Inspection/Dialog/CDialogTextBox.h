#pragma once
#include "afxdialogex.h"


// CDialogTextBox 대화 상자

class CDialogTextBox : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTextBox)

public:
	CDialogTextBox(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogTextBox();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN_TEXT_BOX };
#endif

protected:
	CString m_strTextBox;
	BOOL m_bHide;

private:


public:
	void SetTextBoxString(CString strText);
	void SetTimerShowWindow(CString strText, int nMs = 100);
	void SetTimerShowHide(BOOL isHide);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
