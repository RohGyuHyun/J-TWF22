#pragma once
#include "afxdialogex.h"

#define LOG_MAX_LIST_COUNT			100
// CDialogAlarmLogView 대화 상자

class CDialogAlarmLogView : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogAlarmLogView)

public:
	CDialogAlarmLogView(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDialogAlarmLogView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ALARM_LOG_VIEW };
#endif

protected:
	deque<CString> m_strMonthAlarmLog;
	deque<CString> m_strDayAlarmLog;
	deque<CString> m_strRawDayAlarmLog;
	deque<CString> m_strRawSeqLog;
	int m_nMonthAlarmCount[8];
	int m_nDayAlarmCount[8];
	SYSTEMTIME	m_SeletelpSystemTime;
	SYSTEMTIME	m_SeletelpSystemTime2;
	CBtnenhctrl m_ctrlSelectMenu[2];
	CWinThread* m_pLogReadThread;
	BOOL m_bLogReadThreadEnd;
	int m_nCurLogIdx;
	int m_nMaxLogIdx;
	int m_nSelectManu;
private:
	void InitList();
	BOOL ReadSeqLog();
	BOOL ReadSeqLogFile(CString strFilePath);
	void ViewSeqLogList();
	BOOL ReadAlarmLog(int nType);
	BOOL ReadAlarmLogFile(int nType, CString strFilePath);
	void AlarmCountCheck(int nType);
	void ViewAlarmCount();
	void ViewAlarmLogList();
	void SelectMenu(int nType);
	void ThreadLogRead();
	BOOL GetLogReadThreadEnd();
	void InitLogReadThread();
public:
	UINT static   LogReadThread(LPVOID pParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP();
	LRESULT OnLogReadEndMessage(WPARAM para0, LPARAM para1);
public:
	CListCtrl m_ctr_list_Month_Alarm;
	CListCtrl m_ctr_list_Day_Alarm;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CDateTimeCtrl m_ctrl_DaySelect;
	CDateTimeCtrl m_ctrl_DaySelect2;
	afx_msg void OnDtnCloseupDatetimepickerDaySelect(NMHDR* pNMHDR, LRESULT* pResult);
	CListCtrl m_ctr_list_AlarmLog;
	DECLARE_EVENTSINK_MAP()
	void ClickBtnenhctrlSelectManu0();
	void ClickBtnenhctrlSelectManu1();
	CListCtrl m_ctr_list_SeqLogList;
	afx_msg void OnDtnCloseupDatetimepickerDaySelect2(NMHDR* pNMHDR, LRESULT* pResult);

	void ClickBtnenhctrlSeqLogListM();
	void ClickBtnenhctrlSeqLogListP();
};
