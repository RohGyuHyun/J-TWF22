
// CGInspectorView.cpp : CCGInspectorView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "CGInspector.h"
#endif

#include "CtrlPanelDlg.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "MainFrm.h"
#include "StatusPanelDlg.h"
#include "TabDetailLogDlg.h"
#include "Status.h"
#include "CGIOMapNTestDlg.h"
#include "ProcessLogicCtrl.h"
#include "Monitoring.h"
#include "ConveyorCtrl.h"
#include "MessageWnd.h"
#include "CamDlg.h"
#include "EnvironmentDlg.h"
#include <vidi_runtime.h>
#include <iostream>
#include <fstream>
#include <atlconv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCGInspectorView

IMPLEMENT_DYNCREATE(CCGInspectorView, CView)

BEGIN_MESSAGE_MAP(CCGInspectorView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_SETUP_CONV, &CCGInspectorView::OnSetupConv)
	ON_COMMAND(ID_SETUP_DATAAQU, &CCGInspectorView::OnSetupDataaqu)
	ON_COMMAND(ID_SETUP_REV, &CCGInspectorView::OnSetupRev)
	ON_COMMAND(ID_SETUP_I, &CCGInspectorView::OnSetupI)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_SETUP_ENV, &CCGInspectorView::OnSetupEnv)
	ON_UPDATE_COMMAND_UI(ID_SETUP_ENV, &CCGInspectorView::OnUpdateSetupEnv)
	ON_COMMAND(ID_SETUP_DRAWDEFECT, &CCGInspectorView::OnSetupDrawdefect)
END_MESSAGE_MAP()

// CCGInspectorView ����/�Ҹ�

CCGInspectorView::CCGInspectorView()
//	: m_pCtrlPanelDlg(NULL)
	: m_pViewDlg(NULL)
	, m_pStatusPanelDlg(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CCGInspectorView::~CCGInspectorView()
{
}

BOOL CCGInspectorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CCGInspectorView �׸���

void CCGInspectorView::OnDraw(CDC* /*pDC*/)
{
	CCGInspectorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CCGInspectorView �μ�

BOOL CCGInspectorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CCGInspectorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CCGInspectorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CCGInspectorView ����

#ifdef _DEBUG
void CCGInspectorView::AssertValid() const
{
	CView::AssertValid();
}

void CCGInspectorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGInspectorDoc* CCGInspectorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGInspectorDoc)));
	return (CCGInspectorDoc*)m_pDocument;
}
#endif //_DEBUG


// CCGInspectorView �޽��� ó����


// ���� �����츦 ���ο� �ٿ� ������
int CCGInspectorView::MakeChildWnd()
{
	m_pCtrlPanelDlg = new CCtrlPanelDlg;
//	m_pViewDlg = new CViewDlg;
	m_pStatusPanelDlg = new CStatusPanelDlg;

	int nOfCam = CStatus::Instance()->m_nCountOfCam;
#ifndef _NOMILKEY
	//����
	if (nOfCam >= 1)
	{
		m_pCamDlg[0] = new CCamDlg(CCamDlg::CAMTYPE_TOP);
		m_pCamDlg[0]->m_strCamName = _T("CG Top");
	}
	//�ĸ�
	if (nOfCam >= 2)
	{
		m_pCamDlg[1] = new CCamDlg(CCamDlg::CAMTYPE_BOTTOM);
		m_pCamDlg[1]->m_strCamName = _T("CG Bottom");
	}
	//Left side
	//m_pCamDlg[2] = new CCamDlg(CCamDlg::CAMTYPE_LEFTSIDE);
	//m_pCamDlg[0]->m_strCamName = _T("CamTapeLeftSide");
	////Right side
	//m_pCamDlg[3] = new CCamDlg(CCamDlg::CAMTYPE_RIGHTSIDE);
	//m_pCamDlg[0]->m_strCamName = _T("CamTapeRightSide");

	for (int i = 0; i < nOfCam; i++)
	{
		if (m_pCamDlg[i]->GetSafeHwnd() != 0 )return -1;
		if (!m_pCamDlg[i]->Create(IDD_CAM, this))return -2;
	}
#endif

	if (m_pCtrlPanelDlg->GetSafeHwnd() != 0)
			return -1;
	
	//if (m_pViewDlg->GetSafeHwnd() != 0)
	//	return -1;

	if (m_pStatusPanelDlg->GetSafeHwnd() != 0)
		return -1;

	if (!m_pCtrlPanelDlg->Create(IDD_CTRLDLG, this))
		return -2;

	//if (!m_pViewDlg->Create(IDD_VIEWDLG, this))
	//	return -2;

	if (!m_pStatusPanelDlg->Create(IDD_STSPANEL, this))
		return -2;
	CMessageWnd::Instance();
	return 0;
}


int CCGInspectorView::ReleaseChildWnd()
{
	delete m_pCtrlPanelDlg;
	return 0;
}


int CCGInspectorView::WndRepositioning()
{
	CRect rectMain, rectCtrlPanel, rectView, rectStatusPanel;
	CMainFrame *pMainFrame = (CMainFrame *)AfxGetMainWnd();
	if (pMainFrame == NULL)
		return 0;

	GetClientRect(&rectMain);
	m_pCtrlPanelDlg->GetClientRect(&rectCtrlPanel);
	m_pStatusPanelDlg->GetClientRect(&rectStatusPanel);

	m_pCtrlPanelDlg->MoveWindow(0, 0, rectCtrlPanel.Width(), rectMain.Height() - rectStatusPanel.Height() -5 );

//	m_pViewDlg->MoveWindow(rectCtrlPanel.Width()+10, 0, rectMain.Width()-10, rectMain.Height() - rectStatusPanel.Height() -5 );

	m_pStatusPanelDlg->MoveWindow(0, rectMain.Height()- rectStatusPanel.Height(), rectMain.Width(), rectMain.Height());

	int nOfCam = CStatus::Instance()->m_nCountOfCam;
#ifndef _NOMILKEY
	int iSpace = 10;
	int iLeft = rectCtrlPanel.Width() + iSpace;
	int iCamDlgWidth = (rectMain.Width() - (iLeft + iSpace * 2)) / 2;
	int iRight = iLeft + iCamDlgWidth;
	int iTop = 0;
	int iHeight = rectMain.Height() - rectStatusPanel.Height() - 5;
	if (nOfCam >= 1)
		m_pCamDlg[0]->MoveWindow(iLeft, iTop, iCamDlgWidth, iHeight);
	if (nOfCam >= 2)
		m_pCamDlg[1]->MoveWindow(iRight+iSpace, 0, iCamDlgWidth, iHeight);
#endif
	return 0;
}


void CCGInspectorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	MakeChildWnd();
	WndRepositioning();
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Program StartUp"), RGB(50, 50, 50), FALSE);
	CMessageWnd::Instance()->SetMessageText(_T("Program StartUp"));

	BOOL bValue = CStatus::Instance()->m_bIsOffLine;
	if (!CStatus::Instance()->m_bIsOffLine)
	{
		CConveyorCtrl::Instance()->InitThread();
//		CProcessLogicCtrl::Instance()->m_pCamDlg = NULL;
//		CProcessLogicCtrl::Instance()->InitThread();
		CMonitoring::Instance()->Start();//Emg���� �˻��� Thread
	}
	
	if (ViDiInit() == 0)CTabDetailLogDlg::Instance()->Puts(_T("Deep learning Engine Init!"));
	else
	{
		AfxMessageBox(_T("Deep learning Engine Init Error!!!"));
		exit(0);
	}

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (CStatus::Instance()->IsDrawDefect())
		hMenu->CheckMenuItem(ID_SETUP_DRAWDEFECT, MF_CHECKED);
	else
		hMenu->CheckMenuItem(ID_SETUP_DRAWDEFECT, MF_UNCHECKED);
}


void CCGInspectorView::OnSetupConv()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	hMenu->GetMenuState(ID_SETUP_CONV, MF_BYCOMMAND);

	if (hMenu->GetMenuState(ID_SETUP_CONV, MF_BYCOMMAND) != MF_CHECKED) {
		CStatus::Instance()->SetRepConvMode(TRUE);
		hMenu->CheckMenuItem(ID_SETUP_CONV, MF_CHECKED);

	}

	else {
		CStatus::Instance()->SetRepConvMode(FALSE);
		hMenu->CheckMenuItem(ID_SETUP_CONV, MF_UNCHECKED);

	}
}


void CCGInspectorView::OnSetupDataaqu()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	hMenu->GetMenuState(ID_SETUP_DATAAQU, MF_BYCOMMAND);

	if (hMenu->GetMenuState(ID_SETUP_DATAAQU, MF_BYCOMMAND) != MF_CHECKED) {

		CStatus::Instance()->SetDataAquMode(TRUE);

		hMenu->CheckMenuItem(ID_SETUP_DATAAQU, MF_CHECKED);

	}

	else {

		CStatus::Instance()->SetDataAquMode(FALSE);
		hMenu->CheckMenuItem(ID_SETUP_DATAAQU, MF_UNCHECKED);

	}
}


void CCGInspectorView::OnSetupRev()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CStatus::Instance()->SetActiveRevInsertMode();
	CTabDetailLogDlg::Instance()->Puts(_T("Insert the board in the opposite direction !"));
}

void CCGInspectorView::OnSetupDrawdefect()
{
	// TODO: Add your command handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	hMenu->GetMenuState(ID_SETUP_DRAWDEFECT, MF_BYCOMMAND);

	if (hMenu->GetMenuState(ID_SETUP_DRAWDEFECT, MF_BYCOMMAND) != MF_CHECKED) {

		CStatus::Instance()->SetDrawDefect(TRUE);

		hMenu->CheckMenuItem(ID_SETUP_DRAWDEFECT, MF_CHECKED);

	}

	else {

		CStatus::Instance()->SetDrawDefect(FALSE);
		hMenu->CheckMenuItem(ID_SETUP_DRAWDEFECT, MF_UNCHECKED);

	}
}

void CCGInspectorView::OnSetupI()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CCGIOMapNTestDlg myDlg;
	myDlg.DoModal();
}


void CCGInspectorView::CloseAll()
{
	CMessageWnd::Instance()->ShowWindow(SW_HIDE);

	int nCamCount = CStatus::Instance()->m_nCountOfCam;
	for (int i = 0; i < nCamCount; i++)
	{
		if (m_pCamDlg[i])
		{
			m_pCamDlg[i]->SendMessage(WM_CLOSE);
			Sleep(200);
			delete m_pCamDlg[i];
		}
	}


}


BOOL CCGInspectorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	return CView::OnEraseBkgnd(pDC);
}


void CCGInspectorView::SetOPMode(int OPMode)
{
	CStatus::Instance()->SetOPMode((CStatus::tOpMode)OPMode);
	if (OPMode == CStatus::OPSTOP)
		SetCameraOPMode(CStatus::OPSTOP);
}

void CCGInspectorView::SetCameraOPMode(int OPMode)
{
	int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	for (int i = 0; i < m_nCountOfCam; i++)
	{
		m_pCamDlg[i]->SetRunStop(OPMode);
	}
	if(OPMode == CStatus::OPAUTO)
		CTabDetailLogDlg::Instance()->Puts(_T("Camera Start"));
	else
		CTabDetailLogDlg::Instance()->Puts(_T("Camera Stop"));

}


void CCGInspectorView::OnSetupEnv()
{
	// TODO: Add your command handler code here
	CEnvironmentDlg dlg;
	dlg.DoModal();
}


void CCGInspectorView::OnUpdateSetupEnv(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (CStatus::Instance()->GetOpMode() == CStatus::OPAUTO)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);

}


int CCGInspectorView::ViDiInit()
{
#ifndef _NOVIDI
	USES_CONVERSION;
	CString strRuntimeWS = CStatus::Instance()->m_strWorkSpaceRuntime;
	VIDI_UINT status = vidi_debug_infos(VIDI_DEBUG_SINK_FILE, "vidi_messages.log");
	if (status != VIDI_SUCCESS)
	{
		std::cerr << "failed to enable debug infos" << endl;
		return -1;
	}

	// initialize the libary to run with one GPU per tool
	status = vidi_initialize(VIDI_GPU_SINGLE_DEVICE_PER_TOOL, "");
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to initialize library" << endl;
		return -1;
	}

	// create and initialize a buffer to be used whenever data is returned from the library
	VIDI_BUFFER buffer;
	status = vidi_init_buffer(&buffer);
	if (status != VIDI_SUCCESS)
	{
		clog << "failed to initialize vidi buffer" << endl;
		return -1;
	}

	// open the given workspace
	CString strRutimeFullName = CAppInfo::GetAppDir() + _T("\\") + strRuntimeWS;
	CW2A pszRunTimeWS(strRutimeFullName);
	char* szWSName = CStatus::Instance()->m_szWSName;
	status = vidi_runtime_open_workspace_from_file(szWSName, pszRunTimeWS);
	if (status != VIDI_SUCCESS)
	{
		vidi_get_error_message(status, &buffer);
		// if you get this error, it's possible that the resources were not extracted in the path
		clog << "failed to open workspace: " << buffer.data << endl;
		vidi_deinitialize();
		return -1;
	}
	vidi_free_buffer(&buffer);
#endif
	return 0;
}


int CCGInspectorView::ViDiClose()
{
	return 0;
}

