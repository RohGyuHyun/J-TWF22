#pragma once
#include "afxdialogex.h"


// CDialogAlarm 대화 상자

class CDialogAlarm : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAlarm)

public:
	CDialogAlarm(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogAlarm();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ALARM };
#endif
protected:
	int m_nAlarmCode;

private:


public:
	void SetAlarmString(int nAlarmCode, CString strAlarm);
	CString GetAlarmString();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonBuzzerOff();
};
