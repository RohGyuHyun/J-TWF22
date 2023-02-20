
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "AVMDMonitor.h"

#include "MainFrm.h"
#include "Status.h"
#include "Repository.h"

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
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
	ON_WM_GETMINMAXINFO()
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

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.x = 100;
	cs.y = 50;
	cs.cx = 1248;
	cs.cy = 1024;
	// �̹� ������ ������ �Ӽ� �������� FWS_ADDTOTITLE �Ӽ��� ����.. 
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



void CMainFrame::GetStatusBarRect(CRect &rectStatusBar)
{
	m_wndStatusBar.GetClientRect(&rectStatusBar);
}


void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (CStatus::Instance()->GetProgramMode() == CStatus::PM_REVIEW)
	{
		CFrameWnd::OnClose();
		return;
	}

	if (AfxMessageBox(_T("Do you want to exit program ?"), MB_OKCANCEL) == IDOK)
	{
		CStatus::Instance()->SetProgramMode(CStatus::PM_REVIEW);
		CStatus::Instance()->GetRepository()->TerminateFileSaveThread();
		CFrameWnd::OnClose();
	}
	else
	{

	}

}


void CMainFrame::SetTitleCaption(CString strCaption)
{
	CString strToView;
	strToView = _T("AVMD-");
	strToView = strToView + strCaption;
	this->SetWindowTextW(strToView);
}

void CMainFrame::SetTitleCaptionReset()
{
	CString strToView;
	strToView = _T("AVMD");
	this->SetWindowTextW(strToView);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	static BOOL once = TRUE;

	// â�� ���̴� ������ ���α׷� ������ �������� �˻��ϴ� ����
	if (bShow && !IsWindowVisible() && once)
	{
		// ���α׷� ������ �̰��� �ѹ��� ����ǵ��� once�� false��...
		once = FALSE;
		this->ShowWindow(SW_SHOWMAXIMIZED);

		this->UpdateWindow();

		CStatus::Instance()->UpdateScrn();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	// �ּ� ũ���϶� ������
	lpMMI->ptMinTrackSize.x = 1092;
	// �ּ� ũ���϶� ������
	lpMMI->ptMinTrackSize.y = 1080;
	// �ִ� ũ���϶� ������
	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
	// �ִ� ũ���϶� ������
	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}
