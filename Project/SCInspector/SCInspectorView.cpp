
// SCInspectorView.cpp : CSCInspectorView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "SCInspector.h"
#endif

#include "SCInspectorDoc.h"
#include "SCInspectorView.h"
#include "CamDlg.h"
#include "OpPanelDlg.h"
#include "TeachingDlg.h"
#include "Status.h"
#include "StatusPanelDlg.h"
#include "TabDetailLogDlg.h"
#include "Monitoring.h"
#include "SCProcessCtrl.h"
#include "SCInspIO.h"
#include "SCHEdgeDetParamDlg.h"
#include "SCAlignParamDlg.h"
#include "Compo.h"
#include "SCAlgoBase.h"
#include "SCFidParamDlg.h"
#include "MessageWnd.h"
#include "SCHeightParamDlg.h"
#include "CamRemoteDlg.h"
#include "SCGapGageParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSCInspectorView

IMPLEMENT_DYNCREATE(CSCInspectorView, CView)

BEGIN_MESSAGE_MAP(CSCInspectorView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSCInspectorView ����/�Ҹ�

CSCInspectorView::CSCInspectorView()
	:m_pOpPanelDlg(NULL)
//	, m_pTeachingDlg(NULL)
, m_nCountOfCam(0)
, m_pStatusPanelDlg(NULL)
, m_nCountOfCamRemote(0)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < 4; i++)
		m_pCamDlg[i] = NULL;
	for (int i = 0; i < 2; i++)
		m_pCamRemoteDlg[i] = NULL;
}

CSCInspectorView::~CSCInspectorView()
{
}

BOOL CSCInspectorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}

// CSCInspectorView �׸���

void CSCInspectorView::OnDraw(CDC* /*pDC*/)
{
	CSCInspectorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}


// CSCInspectorView �μ�

BOOL CSCInspectorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CSCInspectorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CSCInspectorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CSCInspectorView ����

#ifdef _DEBUG
void CSCInspectorView::AssertValid() const
{
	CView::AssertValid();
}

void CSCInspectorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSCInspectorDoc* CSCInspectorView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSCInspectorDoc)));
	return (CSCInspectorDoc*)m_pDocument;
}
#endif //_DEBUG


// CSCInspectorView �޽��� ó����

//! -1 
int CSCInspectorView::MakeChildWnd()
{
	CRect rectCur;
	GetClientRect(&rectCur);

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		m_pCamDlg[i] = new CCamDlg;
		m_pCamDlg[i]->m_strCamName.Format(_T("%d"),i);
		m_pCamDlg[i]->m_iCamBuildNo = i;
	}
	for (int i = 0; i < m_nCountOfCamRemote; i++)
	{
		m_pCamRemoteDlg[i] = new CCamRemoteDlg;
		m_pCamRemoteDlg[i]->m_strCamName.Format(_T("%d"), i);
		m_pCamRemoteDlg[i]->m_iCamBuildNo = i;
	}


	m_pOpPanelDlg = new COpPanelDlg;
	m_pTeachingDlg = new CTeachingDlg;
	m_pStatusPanelDlg = new CStatusPanelDlg;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (m_pCamDlg[i]->GetSafeHwnd() != 0)
			return -1;
	}
	for (int i = 0; i < m_nCountOfCamRemote; i++)
	{
		if (m_pCamRemoteDlg[i]->GetSafeHwnd() != 0)
			return -1;
	}

	if (m_pOpPanelDlg->GetSafeHwnd() != 0)
		return -1;
	if (m_pTeachingDlg->GetSafeHwnd() != 0)
		return -1;

	if (m_pStatusPanelDlg->GetSafeHwnd() != 0)
		return -1;

	for (int i = 0; i < m_nCountOfCam; i++)
	{
		if (!m_pCamDlg[i]->Create(IDD_CAM, this))
			return -2;
	}

	for (int i = 0; i < m_nCountOfCamRemote; i++)
	{
		if (!m_pCamRemoteDlg[i]->Create(IDD_CAMREMOTE, this))
			return -2;
	}

	if (!m_pOpPanelDlg->Create(IDD_OPPANEL, this))
		return -2;
	if (!m_pTeachingDlg->Create(IDD_TEACHING, this))
		return -2;
	if (!m_pStatusPanelDlg->Create(IDD_STSPANEL, this))
		return -2;
	m_pTeachingDlg->ShowWindow(SW_HIDE);

	CSCAlignParamDlg::Instance();
	CSCHEdgeDetParamDlg::Instance();
	CSCFidParamDlg::Instance();
	CSCHeightParamDlg::Instance();
	CSCGapGageParamDlg::Instance();
	CMessageWnd::Instance();

	return 0;
}


int CSCInspectorView::WndRepositioning()
{
	CRect rectMain;
	CRect rectCam1, rectCam2, rectCam3, rectCam4;

	if (!m_pOpPanelDlg)return 0;
	GetClientRect(rectMain);

	CRect rectOpPanel;
	m_pOpPanelDlg->GetWindowRect(&rectOpPanel);
	m_pOpPanelDlg->MoveWindow(rectMain.right - rectOpPanel.Width(), rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	m_pTeachingDlg->MoveWindow(rectMain.right - rectOpPanel.Width(), rectMain.top, rectOpPanel.Width(), rectOpPanel.Height());
	CRect rectStatusPanel;

//	double dblAspectRatio = ((double)CAM_HRES) / CAM_VRES;
	double dblAspectRatio = 4.0/3.0;
	int widthCam = (rectMain.Width() - rectOpPanel.Width() - SPACE_CAMX) / 2;
	int heightCam = (rectMain.Height() - SPACE_CAMY) / 2;
	heightCam = widthCam = min(widthCam, heightCam);
	widthCam = (int)(widthCam*dblAspectRatio);


	dblAspectRatio = ((double)CAM_HRES) / CAM_VRES;
	int widthCamRemote = (rectMain.Width() - rectOpPanel.Width() - SPACE_CAMX) / 2;
	int heightCamRemote = (rectMain.Height() - SPACE_CAMY) / 2;
	heightCamRemote = widthCamRemote = min(widthCamRemote, heightCamRemote);
	widthCamRemote = (int)(widthCamRemote*dblAspectRatio);

	m_rectCamFull.SetRect(CPoint(rectMain.left, 0), CPoint(rectMain.left + widthCam * 2 + SPACE_CAMX, heightCam * 2 + SPACE_CAMY));

	m_pStatusPanelDlg->GetClientRect(&rectStatusPanel);
	m_pStatusPanelDlg->MoveWindow(rectMain.right - rectOpPanel.Width(), heightCam - rectStatusPanel.Height(), rectOpPanel.Width(), rectStatusPanel.Height());


	int i = 0;
	CRect rectPosCamera;
	for (; i < m_nCountOfCam; i++)
	{
		rectPosCamera = CameraDlgPos(i, rectMain, widthCam, heightCam);
		m_pCamDlg[i]->MoveWindow(rectPosCamera);
	}


	for (int j = 0; j < m_nCountOfCamRemote; j++, i++)
	{
		rectPosCamera = CameraDlgPos(i, rectMain, widthCamRemote, heightCam, 50);
		m_pCamRemoteDlg[j]->MoveWindow(rectPosCamera);
	}

	return 0;
}


CRect CSCInspectorView::CameraDlgPos(int nPos, CRect rectMain, int widthCam, int heightCam, int xOffset)
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


void CSCInspectorView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	m_nCountOfCamRemote = CStatus::Instance()->m_nCountOfCamRemote;
	if(MakeChildWnd())exit(-1);
	if (WndRepositioning())exit(-1);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Program StartUp"), RGB(50, 50, 50), FALSE);
	CTabDetailLogDlg::Instance()->SetTextLog(_T("Shield Cap Inspector"), RGB(50, 50, 50), FALSE);
	if(CStatus::Instance()->m_bIsOffLine)	CTabDetailLogDlg::Instance()->SetTextLog(_T("Offline Mode Start"), RGB(50, 50, 50), FALSE);
	else
	{
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Inline Mode Start"), RGB(50, 50, 50), FALSE);
	}
	CSCInspIO::Instance()->Init();

	if (!CStatus::Instance()->m_bIsOffLine)
	{
		CSCProcessCtrl::Instance()->m_pCamDlg = m_pCamDlg;
		CSCProcessCtrl::Instance()->InitThread();
		CMonitoring::Instance()->Start();//Emg���� �˻��� Thread
	}
	LoadDefaultJob();
	InitRemoteCam();

}


void CSCInspectorView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	WndRepositioning();
}


void CSCInspectorView::SetOPMode(int OPMode)
{
	CStatus::Instance()->SetOPMode((CStatus::tOpMode)OPMode);
	if (OPMode == CStatus::OPTEACHING)
	{
		int nSelCam = CStatus::Instance()->GetSelCamNo();
		m_pOpPanelDlg->ShowWindow(SW_HIDE);
		m_pTeachingDlg->ShowWindow(SW_SHOW);

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

		for (int i = 0; i < m_nCountOfCamRemote; i++)
		{
			m_pCamRemoteDlg[i]->ShowWindow(SW_HIDE);
		}
	}
	else
	{
		m_pTeachingDlg->ShowWindow(SW_HIDE);
		m_pOpPanelDlg->ShowWindow(SW_SHOW);
		WndRepositioning();
		for (int i = 0; i < m_nCountOfCam; i++)m_pCamDlg[i]->ShowWindow(SW_SHOW);
		for (int i = 0; i < m_nCountOfCamRemote; i++)	m_pCamRemoteDlg[i]->ShowWindow(SW_SHOW);
	}
}

BOOL CSCInspectorView::IsTeachingMode()
{
	return m_pTeachingDlg->IsWindowVisible();
}


void CSCInspectorView::DoTrial()
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


int CSCInspectorView::LoadDefaultJob()
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


int CSCInspectorView::CloseAll()
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



	int nCountOfCamRemote = CStatus::Instance()->m_nCountOfCamRemote;
	for (int i = 0; i < nCountOfCamRemote; i++)
	{
		if (m_pCamRemoteDlg[i])
		{
			m_pCamRemoteDlg[i]->SendMessage(WM_CLOSE);
			Sleep(200);
			delete m_pCamRemoteDlg[i];
		}
	}

	return 0;
}


BOOL CSCInspectorView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	//CRect rect;
	//GetClientRect(rect);
	//pDC->FillSolidRect(rect, RGB(234, 239, 245));

	//return TRUE;
	return CView::OnEraseBkgnd(pDC);
}


void CSCInspectorView::InitRemoteCam()
{
	int nCountOfCamRemote = CStatus::Instance()->m_nCountOfCamRemote;
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (nCountOfCamRemote)
	{
		//Server��
		for (int i = 0; i < nCountOfCamRemote; i++)
		{
		}
	}
	else
	{
		//Send��
		CString strIP = CStatus::Instance()->GetRemoteIPString();
		for (int i = 0; i < nCountOfCam; i++)
		{
		}

	}
}
