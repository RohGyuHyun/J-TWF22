
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "AGMDMonitor.h"

#include "MainFrm.h"
#include "Status.h"
#include "MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
	ON_WM_TIMER()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ���� �� ǥ�ñ�
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/�Ҹ�

CMainFrame::CMainFrame()
{
	// TODO: ���⿡ ��� �ʱ�ȭ �ڵ带 �߰��մϴ�.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("���� ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
		return -1;      // ������ ���߽��ϴ�.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH, 0);
	m_wndStatusBar.SetPaneInfo(1, ID_SEPARATOR, SBPS_NORMAL, 300);

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	this->SetWindowText(_T("AGMD"));
	SetTimer(1, 200, NULL);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style = cs.style & (~FWS_ADDTOTITLE);

	if (!CFrameWnd::PreCreateWindow(cs)) return FALSE;
	return TRUE;
}

// CMainFrame ����

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame �޽��� ó����

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// â�� ���̴� ������ ���α׷� ���� �� �������� �Ǻ��ϱ� ���� ����
	static BOOL once = TRUE;

	if (bShow && !IsWindowVisible() && once)
	{
		once = FALSE;

		POINT min, max;
		min.x = 100;
		min.y = 100;
		max.x = 1100;
		max.y = 868;

		WINDOWPLACEMENT lwp;
		lwp.length = sizeof(WINDOWPLACEMENT);
		lwp.flags = SW_SHOWMINIMIZED | WPF_SETMINPOSITION;
		lwp.showCmd = SW_SHOW;
		lwp.ptMinPosition = min;
		lwp.ptMaxPosition = max;
		lwp.rcNormalPosition = CRect(min, max);
		SetWindowPlacement(&lwp);
	}
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// �ּ� ũ���϶� ������
	lpMMI->ptMinTrackSize.x = 1000;
	// �ּ� ũ���϶� ������
	lpMMI->ptMinTrackSize.y = 768;
	// �ִ� ũ���϶� ������
	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
	// �ִ� ũ���϶� ������
	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::SetTitleCaption(CString strCaption)
{
	CString strToView;
	strToView = _T("AGMD-");
	strToView = strToView + strCaption;
	this->SetWindowTextW(strToView);
}

void CMainFrame::SetTitleCaptionReset()
{
	CString strToView;
	strToView = _T("AGMD");
	this->SetWindowTextW(strToView);
}


void CMainFrame::SetStatusBar(int nIndex, LPCTSTR lpszNewText, BOOL bUpdate)
{
	m_wndStatusBar.SetPaneText(nIndex, lpszNewText);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CString strCalibration;
	CString strTemp = CStatus::Instance()->m_strCalDate;

	if (strTemp == _T(""))
	{
		strCalibration = _T("Calibration Date");
		SetStatusBar(1, strCalibration);
	}
	else
	{
		CString strYear = CStatus::Instance()->m_strCalDate.Mid(0, 2);
		CString strMon = CStatus::Instance()->m_strCalDate.Mid(2, 2);
		CString strDay = CStatus::Instance()->m_strCalDate.Mid(4, 2);

		strCalibration = _T("Calibration Date : ") + strYear + _T("-") + strMon + _T("-") + strDay;
		SetStatusBar(1, strCalibration);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (AfxMessageBox(_T("Do you want to exit program ?"), MB_OKCANCEL) == IDOK)
	{
		if (CStatus::Instance()->GetProgramMode() == CStatus::tProgramMode::PM_MONITORING)
		{
			CMonitorDlg* pMonitordlg =  CStatus::Instance()->GetMonitorDlg();
			pMonitordlg->SerialClose();

		}
		CFrameWnd::OnClose();
	}
}


