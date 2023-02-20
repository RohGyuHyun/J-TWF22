
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Repository.h"
#include "SCInspector.h"
#include "Status.h"
#include "SCFidParamDlg.h"
#include "Monitoring.h"
#include "SCInspIO.h"
#include "SCProcessCtrl.h"
#include "TabDetailLogDlg.h"
#include "SCInspectorDoc.h"
#include "SCInspectorView.h"

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
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
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
	//ModifyStyle(WS_CAPTION, NULL); // 타이틀바 없애기

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("도구 모음을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("상태 표시줄을 만들지 못했습니다.\n");
	//	return -1;      // 만들지 못했습니다.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);


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
	cs.hMenu = NULL;
	return TRUE;
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
		CSCProcessCtrl::Instance()->Stop();
		Sleep(100);
		((CSCInspectorView *)GetActiveView())->CloseAll();
		CTabDetailLogDlg::Instance()->SetTextLog(_T("ProcessCtrl Stop"), RGB(50, 50, 50), FALSE);
		CSCFidParamDlg::Instance()->CloseMod();
		Sleep(100);
		if(!CStatus::Instance()->m_bIsOffLine)
			CRepository::Instance()->m_EuresysGrabber.Close();
		CStatus::Instance()->GetComm()->LightOnOff(CSCInspPacket::LIGHT_OFF, CStatus::Instance()->m_LightBrightness);
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Light Off"), RGB(50, 50, 50), FALSE);
		CRepository::Instance()->m_MilLib.Close();
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Vision Library close"), RGB(50, 50, 50), FALSE);
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	lpMMI->ptMinTrackSize.x = 1920;
	// 최소 크기일때 세로폭
	lpMMI->ptMinTrackSize.y = 1030;
	// 최대 크기일때 가로폭
//	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
//	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(1920);
	// 최대 크기일때 세로폭
//	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
//	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(1030);
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
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

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CFrameWnd::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CFrameWnd::OnEraseBkgnd(pDC);
}
