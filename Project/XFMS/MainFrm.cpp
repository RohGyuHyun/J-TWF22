
// MainFrm.cpp : CMainFrame Ŭ������ ����
//

#include "stdafx.h"
#include "MainFrm.h"
#include "Repository.h"
#include "XFMS.h"
#include "Status.h"
#include "SCFidParamDlg.h"
#include "Monitoring.h"
#include "SCProcessCtrl.h"
#include "TabDetailLogDlg.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "OpPanelDlg.h"

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
	//ModifyStyle(WS_CAPTION, NULL); // Ÿ��Ʋ�� ���ֱ�

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("���� ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}

	//if (!m_wndStatusBar.Create(this))
	//{
	//	TRACE0("���� ǥ������ ������ ���߽��ϴ�.\n");
	//	return -1;      // ������ ���߽��ϴ�.
	//}
	//m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: ���� ������ ��ŷ�� �� ���� �Ϸ��� �� �� ���� �����Ͻʽÿ�.
//	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
//	EnableDocking(CBRS_ALIGN_ANY);
//	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	cs.style = cs.style & (~FWS_ADDTOTITLE);
	cs.style = cs.style & (~WS_MAXIMIZEBOX);
	cs.style = cs.style | WS_MAXIMIZE;
	cs.style = cs.style & (~WS_SIZEBOX);
	cs.x = 0;
	cs.y = 0;
	cs.cx = 1920;
	cs.cy = 1080;
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

void CMainFrame::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (AfxMessageBox(_T("Do you want to exit program ?"), MB_OKCANCEL) == IDOK)
	{
		((CXFMSView *)GetActiveView())->m_pOpPanelDlg->CloseProcess();
		Sleep(100);
		CStatus::Instance()->GetModelData()->RemoveAll();
		CMonitoring::Instance()->Stop();
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Monitoring Stop"), RGB(50, 50, 50), FALSE);
		CSCProcessCtrl::Instance()->Stop();
		Sleep(100);
		((CXFMSView *)GetActiveView())->CloseAll();
		CTabDetailLogDlg::Instance()->SetTextLog(_T("ProcessCtrl Stop"), RGB(50, 50, 50), FALSE);
		CSCFidParamDlg::Instance()->CloseMod();
		Sleep(100);
		if(!CStatus::Instance()->m_bIsOffLine)
			CRepository::Instance()->m_Grabber.Close();
		Sleep(100);
		CRepository::Instance()->m_MilLib.Close();
		Sleep(100);
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Vision Library close"), RGB(50, 50, 50), FALSE);
		CFrameWnd::OnClose();
	}
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	lpMMI->ptMinTrackSize.x = 1920;
	// �ּ� ũ���϶� ������
	lpMMI->ptMinTrackSize.y = 1030;
	// �ִ� ũ���϶� ������
//	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXSCREEN);
//	lpMMI->ptMaxTrackSize.x = GetSystemMetrics(1920);
	// �ִ� ũ���϶� ������
//	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYSCREEN);
//	lpMMI->ptMaxTrackSize.y = GetSystemMetrics(1030);
	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}


void CMainFrame::SetTitleCaption(CString strCaption)
{
	CString strToView;
	strToView = _T("XFMS-");
	strToView = strToView + strCaption;
	this->SetWindowTextW(strToView);
}

void CMainFrame::SetTitleCaptionReset()
{
	CString strToView;
	strToView = _T("XFMS");
	this->SetWindowTextW(strToView);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	return CFrameWnd::PreTranslateMessage(pMsg);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	return CFrameWnd::OnEraseBkgnd(pDC);
}
