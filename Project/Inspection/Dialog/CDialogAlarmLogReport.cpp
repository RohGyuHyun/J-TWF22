// CDialogAlarmLogReport.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogAlarmLogReport.h"


// CDialogAlarmLogReport 대화 상자

IMPLEMENT_DYNAMIC(CDialogAlarmLogReport, CDialogEx)

CDialogAlarmLogReport::CDialogAlarmLogReport(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ALARM_LOG_REPORT, pParent)
{
	m_dlgAlarmLogView = NULL;
	m_dlgResultReport = NULL;
}

CDialogAlarmLogReport::~CDialogAlarmLogReport()
{
	if (m_dlgAlarmLogView != NULL)
	{
		if (!m_dlgAlarmLogView->IsWindowVisible())
		{
			if (m_dlgAlarmLogView != NULL)
			{
				delete m_dlgAlarmLogView;
				m_dlgAlarmLogView = NULL;
			}
		}
	}

	if (m_dlgResultReport != NULL)
	{
		if (!m_dlgResultReport->IsWindowVisible())
		{
			if (m_dlgResultReport != NULL)
			{
				delete m_dlgResultReport;
				m_dlgResultReport = NULL;
			}
		}
	}
}

void CDialogAlarmLogReport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogAlarmLogReport, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDialogAlarmLogReport 메시지 처리기
BEGIN_EVENTSINK_MAP(CDialogAlarmLogReport, CDialogEx)
	ON_EVENT(CDialogAlarmLogReport, IDC_BTNENHCTRL_ALARM_LOG, DISPID_CLICK, CDialogAlarmLogReport::ClickBtnenhctrlAlarmLog, VTS_NONE)
	ON_EVENT(CDialogAlarmLogReport, IDC_BTNENHCTRL_REPORT, DISPID_CLICK, CDialogAlarmLogReport::ClickBtnenhctrlReport, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogAlarmLogReport::ClickBtnenhctrlAlarmLog()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_dlgAlarmLogView != NULL)
		m_dlgAlarmLogView->ShowWindow(SW_SHOW);
	else
	{
		m_dlgAlarmLogView = new CDialogAlarmLogView();
		m_dlgAlarmLogView->Create(IDD_DIALOG_ALARM_LOG_VIEW, this);
		m_dlgAlarmLogView->ShowWindow(SW_SHOW);
	}

	this->ShowWindow(SW_HIDE);
}


void CDialogAlarmLogReport::ClickBtnenhctrlReport()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_dlgResultReport != NULL)
		m_dlgResultReport->ShowWindow(SW_SHOW);
	else
	{
		m_dlgResultReport = new CDialogResultReport();
		m_dlgResultReport->Create(IDD_DIALOG_RESULT_REPORT, this);
		m_dlgResultReport->ShowWindow(SW_SHOW);
	}

	this->ShowWindow(SW_HIDE);
}


void CDialogAlarmLogReport::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (bShow)
	{
		
	}
	else
	{
		SetTimer(2, 100, NULL);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CDialogAlarmLogReport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CDialogAlarmLogReport::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F5)
			ClickBtnenhctrlAlarmLog();
		else if (pMsg->wParam == VK_F6)
			ClickBtnenhctrlReport();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogAlarmLogReport::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch ((int)nIDEvent)
	{
	case 0:
		if (!m_dlgAlarmLogView->IsWindowVisible())
		{
			if (m_dlgAlarmLogView != NULL)
			{
				delete m_dlgAlarmLogView;
				m_dlgAlarmLogView = NULL;
			}
			KillTimer(0);
		}
		break;
	case 1:
		if (!m_dlgResultReport->IsWindowVisible())
		{
			if (m_dlgResultReport != NULL)
			{
				
			}
			KillTimer(1);
		}
		break;
	case 2:
		if (m_dlgAlarmLogView != NULL)
		{
			if (!m_dlgAlarmLogView->IsWindowVisible())
			{
				if (m_dlgAlarmLogView != NULL)
				{
					delete m_dlgAlarmLogView;
					m_dlgAlarmLogView = NULL;
				}
			}
		}

		if (m_dlgResultReport != NULL)
		{
			if (!m_dlgResultReport->IsWindowVisible())
			{
				if (m_dlgResultReport != NULL)
				{
					delete m_dlgResultReport;
					m_dlgResultReport = NULL;
				}
			}
		}

		if (m_dlgAlarmLogView == NULL && m_dlgResultReport == NULL)
			KillTimer(2);
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}
