
// SCInspectorView.cpp : CXFMSView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "XFMS.h"
#endif

#include "XFMSDoc.h"
#include "XFMSView.h"
#include "CamDlg.h"
#include "OpPanelDlg.h"
#include "TeachingDlg.h"
#include "Status.h"
#include "StatusPanelDlg.h"
#include "TabDetailLogDlg.h"
#include "Monitoring.h"
#include "SCProcessCtrl.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCAlignParamDlg.h"
#include "Compo.h"
#include "SCAlgoBase.h"
#include "SCFidParamDlg.h"
#include "MessageWnd.h"
#include "SCHeightParamDlg.h"
#include "SCGapGageParamDlg.h"
#include "GraphDlg.h"
#include "HistoLevellingDlg.h"
#include "MainFrm.h"
#include "Repository.h"
#include "ToolPanelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CXFMSView

IMPLEMENT_DYNCREATE(CXFMSView, CView)

BEGIN_MESSAGE_MAP(CXFMSView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_TOOL_HISTOWINDOWLEVEL, &CXFMSView::OnToolHistowindowlevel)
	ON_UPDATE_COMMAND_UI(ID_TOOL_HISTOWINDOWLEVEL, &CXFMSView::OnUpdateToolHistowindowlevel)
END_MESSAGE_MAP()

// CXFMSView ����/�Ҹ�

CXFMSView::CXFMSView()
	:m_pOpPanelDlg(NULL)
//	, m_pTeachingDlg(NULL)
, m_nCountOfCam(1)
, m_pStatusPanelDlg(NULL)
, m_pGraphDlg(NULL)
, m_pHistoLevellingDlg(NULL)
, m_pToolPanelDlg(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < 4; i++)
		m_pCamDlg[i] = NULL;
}

CXFMSView::~CXFMSView()
{
}

BOOL CXFMSView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CXFMSView �׸���

void CXFMSView::OnDraw(CDC* /*pDC*/)
{
	CXFMSDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CXFMSView �μ�

BOOL CXFMSView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CXFMSView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CXFMSView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CXFMSView ����

#ifdef _DEBUG
void CXFMSView::AssertValid() const
{
	CView::AssertValid();
}

void CXFMSView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CXFMSDoc* CXFMSView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CXFMSDoc)));
	return (CXFMSDoc*)m_pDocument;
}
#endif //_DEBUG


// CXFMSView �޽��� ó����

//! -1 
int CXFMSView::MakeChildWnd()
{
	CRect rectCur;
	GetClientRect(&rectCur);

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		m_pCamDlg[i] = new CCamDlg;
		m_pCamDlg[i]->m_strCamName.Format(_T("%d"),i);
		m_pCamDlg[i]->m_iCamBuildNo = i;
	}


	m_pOpPanelDlg = new COpPanelDlg;
	m_pTeachingDlg = new CTeachingDlg;
	m_pStatusPanelDlg = new CStatusPanelDlg;
	m_pToolPanelDlg = new CToolPanelDlg;
	m_pGraphDlg = new CGraphDlg;
	m_pHistoLevellingDlg = new CHistoLevellingDlg;
	m_pHistoLevellingDlg->m_pView = this;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (m_pCamDlg[i]->GetSafeHwnd() != 0)
			return -1;
	}

	if (m_pOpPanelDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pTeachingDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pStatusPanelDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pToolPanelDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pGraphDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pHistoLevellingDlg->GetSafeHwnd() != 0)
		return -1;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (!m_pCamDlg[i]->Create(IDD_CAM, this))
			return -2;
	}

	if (!m_pOpPanelDlg->Create(IDD_OPPANEL, this))
		return -2;
	if (!m_pTeachingDlg->Create(IDD_TEACHING, this))
		return -2;
	if (!m_pStatusPanelDlg->Create(IDD_STSPANEL, this))
		return -2;
	if (!m_pToolPanelDlg->Create(IDD_TOOLPANEL, this))
		return -2;
	if (!m_pGraphDlg->Create(IDD_GRAPH, this))
		return -2;
	if (!m_pHistoLevellingDlg->Create(IDD_HISTOLEVEL, this))
		return -2;
	m_pHistoLevellingDlg->ShowWindow(SW_HIDE);
	m_pTeachingDlg->ShowWindow(SW_HIDE);

	CSCAlignParamDlg::Instance();
	CSCHEdgeDetParamDlg::Instance();
	CSCFidParamDlg::Instance();
	CSCHeightParamDlg::Instance();
	CSCGapGageParamDlg::Instance();
	CMessageWnd::Instance();
	m_pOpPanelDlg->m_pToolPanelDlg = m_pToolPanelDlg;

	CXFMSApp * pApp = (CXFMSApp *)AfxGetApp();
	CStatus::Instance()->SetRepository(pApp->m_pRepository);

	return 0;
}


int CXFMSView::WndRepositioning()
{
	CRect rectMain;
	CRect rectCam;

	if (!m_pOpPanelDlg)return 0;
	GetClientRect(rectMain);

	CRect rectOpPanel;
	m_pOpPanelDlg->GetWindowRect(&rectOpPanel);
	m_pOpPanelDlg->MoveWindow(0, rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	m_pTeachingDlg->MoveWindow(0, rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	CRect rectStatusPanel;

	double dblAspectRatio = ((double)CAM_HRES) / CAM_VRES;

	m_rectCamFull.SetRect(CPoint(rectOpPanel.Width(), 0), CPoint(rectMain.right, rectMain.bottom));

	m_pStatusPanelDlg->GetClientRect(&rectStatusPanel);
	m_pStatusPanelDlg->MoveWindow(0, rectOpPanel.Height(), rectOpPanel.Width(), rectStatusPanel.Height());

	CRect rectToolPanel;
	m_pToolPanelDlg->GetClientRect(&rectToolPanel);
	m_pToolPanelDlg->MoveWindow(rectOpPanel.Width() + SPACE_CAMX, rectMain.top, rectToolPanel.Width(), rectOpPanel.Height() + rectStatusPanel.Height());

	rectCam.SetRect(rectOpPanel.Width() + rectToolPanel.Width() + SPACE_CAMX*2, 0, rectMain.right, rectOpPanel.Height() + rectStatusPanel.Height());
	if(m_pCamDlg[0])
		m_pCamDlg[0]->MoveWindow(rectCam);

	CRect rectGraphDlg;
	m_pGraphDlg->MoveWindow(0, rectOpPanel.Height() + rectStatusPanel.Height(), rectMain.Width(), rectMain.Height()- rectCam.Height());
	return 0;
}


CRect CXFMSView::CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset)
{
	CRect rectCamera;
	CPoint ptStart, ptEnd;
	switch (nPos)
	{
	case 0:
		ptStart.SetPoint(xOffset+rectMain.left, 0);
		break;
	case 1:
		ptStart.SetPoint(xOffset+rectMain.left + widthCam + SPACE_CAMX, 0);
		break;
	case 2:
		ptStart.SetPoint(xOffset+rectMain.left, heightCam + SPACE_CAMY);
		break;
	case 3:
		ptStart.SetPoint(xOffset+rectMain.left + widthCam + SPACE_CAMX, heightCam + SPACE_CAMY);
		break;
	case 4:
		ptStart.SetPoint(xOffset+rectMain.left + (widthCam * 2) + SPACE_CAMX*2, heightCam + SPACE_CAMY);
		break;
	}
	ptEnd = ptStart;
	ptEnd.Offset(widthCam, heightCam);
	rectCamera.SetRect(ptStart, ptEnd);
	return rectCamera;
}


void CXFMSView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if(MakeChildWnd())exit(-1);
	if (WndRepositioning())exit(-1);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Program StartUp"), RGB(50, 50, 50), FALSE);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("XFMS Inspector"), RGB(50, 50, 50), FALSE);
	if(CStatus::Instance()->m_bIsOffLine)	CTabDetailLogDlg::Instance()->SetTextLog(_T("Offline Mode Start"), RGB(50, 50, 50), FALSE);
	else
	{
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Inline Mode Start"), RGB(50, 50, 50), FALSE);
	}

	if (!CStatus::Instance()->m_bIsOffLine)
	{
		CSCProcessCtrl::Instance()->m_pCamDlg = m_pCamDlg;
		CSCProcessCtrl::Instance()->InitThread();
		CMonitoring::Instance()->Start();//Emg���� �˻��� Thread
	}
	LoadDefaultJob();
}


void CXFMSView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	WndRepositioning();
}


void CXFMSView::SetOPMode(int OPMode)
{
	CStatus::Instance()->SetOPMode((CStatus::tOpMode)OPMode);
	if (OPMode == CStatus::OPTEACHING)
	{
		int nSelCam = CStatus::Instance()->GetSelCamNo();
		m_pOpPanelDlg->ShowWindow(SW_HIDE);
		m_pTeachingDlg->ShowWindow(SW_SHOW);
		m_pGraphDlg->ShowWindow(SW_HIDE);

		int m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
		for (int i = 0; i < m_nCountOfCam; i++)
		{
			if (i == nSelCam)
			{
				m_pCamDlg[i]->MoveWindow(m_rectCamFull);
				m_pCamDlg[i]->ShowWindow(SW_SHOW);
				m_pCamDlg[i]->RefreshView();
				m_pCamDlg[i]->DrawAllTrack();
			}
			else
				m_pCamDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_pTeachingDlg->ShowWindow(SW_HIDE);
		m_pOpPanelDlg->ShowWindow(SW_SHOW);
		m_pGraphDlg->ShowWindow(SW_SHOW);
		WndRepositioning();
		for (int i = 0; i < m_nCountOfCam; i++)m_pCamDlg[i]->ShowWindow(SW_SHOW);
	}
}

BOOL CXFMSView::IsTeachingMode()
{
	return m_pTeachingDlg->IsWindowVisible();
}


void CXFMSView::DoTrial()
{
	int nCamNo = CStatus::Instance()->GetSelCamNo();
	CCamDlg *pSelCamDlg = m_pCamDlg[nCamNo];
	if(CStatus::Instance()->m_bIsOffLine)
		pSelCamDlg->ClearView();
	else
		pSelCamDlg->DoGrab();
	pSelCamDlg->Inspect();
	return;

}


int CXFMSView::LoadDefaultJob()
{
	if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
	{
		AfxMessageBox(_T("COM ERROR\r\nCoInitialize Failed!"));
		return -1;
	}
	CStatus::Instance()->GetModelData()->SetModelPath(CStatus::Instance()->GetModelRepository().GetBuffer());
	CStatus::Instance()->GetModelData()->SetMDName(CStatus::Instance()->GetModelName().GetBuffer());
	CStatus::Instance()->GetModelData()->LoadModelData();
	return 0;
}


int CXFMSView::CloseAll()
{
	CSCAlignParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCHEdgeDetParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCFidParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCHeightParamDlg::Instance()->ShowWindow(SW_HIDE);
	CSCGapGageParamDlg::Instance()->ShowWindow(SW_HIDE);
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

	return 0;
}


BOOL CXFMSView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(234, 239, 245));

	//return TRUE;
	return CView::OnEraseBkgnd(pDC);
}




void CXFMSView::OnToolHistowindowlevel()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CMenu *hMenu = pFrame->GetMenu();

	if (hMenu->GetMenuState(ID_TOOL_HISTOWINDOWLEVEL, MF_BYCOMMAND) != MF_CHECKED) {
		m_pHistoLevellingDlg->ShowWindow(SW_SHOW);
		hMenu->CheckMenuItem(ID_TOOL_HISTOWINDOWLEVEL, MF_CHECKED);
	}
	else {
		m_pHistoLevellingDlg->ShowWindow(SW_HIDE);
		hMenu->CheckMenuItem(ID_TOOL_HISTOWINDOWLEVEL, MF_UNCHECKED);
	}

}


void CXFMSView::OnUpdateToolHistowindowlevel(CCmdUI *pCmdUI)
{
	// TODO: ���⿡ ��� ������Ʈ UI ó���� �ڵ带 �߰��մϴ�.
	if (CStatus::Instance()->m_ulDispMode == CStatus::DISP_MANUAL)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}
