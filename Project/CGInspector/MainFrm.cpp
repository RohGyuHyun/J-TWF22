
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "CGInspector.h"
#include "Status.h"
#include "Monitoring.h"
#include "TabDetailLogDlg.h"
#include "ConveyorCtrl.h"
#include "SCFidAlgo.h"
#include "SCFidParamDlg.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "repository.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
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
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	cs.style = cs.style & (~FWS_ADDTOTITLE);
	cs.style = cs.style & (~WS_MAXIMIZEBOX);
	cs.style = cs.style | WS_MAXIMIZE;
	cs.style = cs.style & (~WS_SIZEBOX);
	cs.x = 0;
	cs.y = 0;
	cs.cx = 1920;
	cs.cy = 1080;
	//cs.hMenu = NULL; //메뉴를 없앰
	return TRUE;
}

void CMainFrame::SetTitleCaption(CString strCaption)
{
	CString strToView;
	strToView = _T("SCInspector-");
	strToView = strToView + strCaption;
	this->SetWindowTextW(strToView);
}

void CMainFrame::SetTitleCaptionReset()
{
	CString strToView;
	strToView = _T("SCInspector");
	this->SetWindowTextW(strToView);
}
// CMainFrame 진단

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


// CMainFrame 메시지 처리기



void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (AfxMessageBox(_T("Do you want to exit program ?"), MB_OKCANCEL) == IDOK)
	{

		CStatus::Instance()->GetModelData()->RemoveAll();
		CMonitoring::Instance()->Stop();
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Monitoring Stop"), RGB(50, 50, 50), FALSE);
		CConveyorCtrl::Instance()->Stop();
		Sleep(100);
		((CCGInspectorView *)GetActiveView())->CloseAll();
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor Ctrl Stop"), RGB(50, 50, 50), FALSE);
		CSCFidParamDlg::Instance()->CloseMod();
		Sleep(100);
		if (!CStatus::Instance()->m_bIsOffLine)
		{
			CRepository::Instance()->m_Grabber[0].Close();
			CRepository::Instance()->m_Grabber[1].Close();
			Sleep(100);
			CTabDetailLogDlg::Instance()->Puts(_T("Grabber Close"));
		}
		CRepository::Instance()->m_MilLib.Close();
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Vision Library close"), RGB(50, 50, 50), FALSE);
		CFrameWnd::OnClose();
	}
}
