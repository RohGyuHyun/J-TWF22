#pragma once
#include "afxdialogex.h"
#include "CDialogAlarmLogView.h"
#include "CDialogResultReport.h"
// CDialogAlarmLogReport 대화 상자

class CDialogAlarmLogReport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAlarmLogReport)

public:
	CDialogAlarmLogReport(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogAlarmLogReport();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ALARM_LOG_REPORT };
#endif

protected:
	CDialogAlarmLogView* m_dlgAlarmLogView;
	CDialogResultReport* m_dlgResultReport;
private:

public:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlAlarmLog();
	void ClickBtnenhctrlReport();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
