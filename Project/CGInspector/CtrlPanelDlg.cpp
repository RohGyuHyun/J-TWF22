// CtrlPanelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CGInspector.h"
#include "CtrlPanelDlg.h"
#include "afxdialogex.h"
#include "Status.h"
#include "TabDetailLogDlg.h"
#include "ConvTestDlg.h"
#include "MainFrm.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "repository.h"
#include "CamDlg.h"
#include <UniUtility.h>

// CCtrlPanelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCtrlPanelDlg, CDialogEx)

CCtrlPanelDlg::CCtrlPanelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CTRLDLG, pParent)
	, m_nDispayType(0)
	, m_nCamSel(0)
	, m_nTopSideInspType(0)
{

}

CCtrlPanelDlg::~CCtrlPanelDlg()
{
}

void CCtrlPanelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RUNSTOPBTN, m_btnRun);
	DDX_Control(pDX, IDC_LOADIMG, m_btnLoadImg);
	DDX_Control(pDX, IDC_SAVEIMG, m_btnSaveImg);
	DDX_Control(pDX, IDC_SAVEINSPIMG, m_btnSaveInspImg);
	DDX_Control(pDX, IDC_TRIAL, m_btnTrial);
	DDX_Radio(pDX, IDC_RBDISPTYPE, m_nDispayType);
	DDX_Radio(pDX, IDC_RSELCAM, m_nCamSel);
	DDX_Radio(pDX, IDC_RBTOPINSPTYPE, m_nTopSideInspType);
}


BEGIN_MESSAGE_MAP(CCtrlPanelDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RUNSTOPBTN, &CCtrlPanelDlg::OnBnClickedRunstopbtn)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_TEST, &CCtrlPanelDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_LOADIMG, &CCtrlPanelDlg::OnBnClickedLoadimg)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RBDISPTYPE, IDC_RADIO3, &CCtrlPanelDlg::OnBnClickedRadio)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RBTOPINSPTYPE, IDC_RADIO5, &CCtrlPanelDlg::OnBnClickedTopSideInspType)
	ON_BN_CLICKED(IDC_TRIAL, &CCtrlPanelDlg::OnBnClickedTrial)
	ON_BN_CLICKED(IDC_SAVEIMG, &CCtrlPanelDlg::OnBnClickedSaveimg)
	ON_BN_CLICKED(IDC_SAVEINSPIMG, &CCtrlPanelDlg::OnBnClickedSaveinspimg)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCtrlPanelDlg 메시지 처리기입니다.


void CCtrlPanelDlg::OnBnClickedRunstopbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();

	if (CStatus::OPAUTO == CStatus::Instance()->GetOpMode())
	{
		m_btnRun.SetWindowTextW(_T("RUN"));
		pView->SetOPMode(CStatus::OPSTOP);
		CTabDetailLogDlg::Instance()->Puts(_T("Manual Mode"));

	}
	else
	{
		m_btnRun.SetWindowTextW(_T("STOP"));
		pView->SetOPMode(CStatus::OPAUTO);
		CTabDetailLogDlg::Instance()->Puts(_T("Auto Mode Start"));
	}
	
}


BOOL CCtrlPanelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(234, 239, 245));

	return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CCtrlPanelDlg::OnBnClickedBtnTest()
{
	// TODO: Add your control notification handler code 
	CConvTestDlg myDlg;
	myDlg.DoModal();
}


void CCtrlPanelDlg::OnBnClickedLoadimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog LoadFile(TRUE, _T(""), _TEXT("*.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _TEXT("BMP|*.BMP|PNG|*.PNG|"));
	if (LoadFile.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();

		CString strExt;
		strExt = CFileInfo::strClip(LoadFile.GetPathName(), 2);
		strExt.MakeUpper();
		CMilBuffer *pImage = CRepository::Instance()->m_MilLib.NewImage();
		pImage->Restore(LoadFile.GetPathName());
		if(m_nCamSel==0)
			pView->m_pCamDlg[0]->LoadBufImage(pImage);
		else
			pView->m_pCamDlg[m_nCamSel]->LoadBufImage(pImage, m_nDispayType);

//		pView->m_pCamDlg[0]->m_pMilDisp->FilWindow();
//		pView->m_pCamDlg[1]->LoadBufImage(pImage);
//		pView->m_pCamDlg[1]->m_pMilDisp->FilWindow();
		pImage->Close();
	}


}

void CCtrlPanelDlg::OnBnClickedSaveimg()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CFileDialog SaveFile(FALSE, _T(""), _T("*.bmp"), OFN_OVERWRITEPROMPT, _TEXT("BMP|*.bmp|JPG|*.jpg|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();
		if (m_nCamSel == 0)
			pView->m_pCamDlg[0]->m_ppSplitLineImg[m_nDispayType]->Export(SaveFile.GetPathName(), M_BMP);
		else
			pView->m_pCamDlg[m_nCamSel]->m_ppSplitLineImg[0]->Export(SaveFile.GetPathName(), M_BMP);
	}

}


void CCtrlPanelDlg::OnBnClickedSaveinspimg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CFileDialog SaveFile(FALSE, _T(""), _T("*.jpg"), OFN_OVERWRITEPROMPT, _TEXT("|JPG|*.jpg|"));
	if (SaveFile.DoModal() == IDOK)
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();
		if (m_nCamSel == 0)
			pView->m_pCamDlg[0]->m_pMilCamView->Export(SaveFile.GetPathName(), M_JPEG_LOSSY);
		else
			pView->m_pCamDlg[m_nCamSel]->m_pMilCamView->Export(SaveFile.GetPathName(), M_JPEG_LOSSY);
	}
}

void CCtrlPanelDlg::OnBnClickedRadio(UINT uiID)
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();

	switch (uiID)
	{
	case IDC_RBDISPTYPE:
		pView->m_pCamDlg[0]->SwitchLightImage(0);
		break;
	case IDC_RADIO2:
		pView->m_pCamDlg[0]->SwitchLightImage(1);
		break;
	case IDC_RADIO3:
		pView->m_pCamDlg[0]->SwitchLightImage(2);
		break;
	}
}

void CCtrlPanelDlg::OnBnClickedTopSideInspType(UINT uiID)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();

	switch (uiID)
	{
	case IDC_RBTOPINSPTYPE:
		pView->m_pCamDlg[0]->SetTopSideInspType(0);
		break;
	case IDC_RADIO5:
		pView->m_pCamDlg[0]->SetTopSideInspType(1);
		break;
	}
}

void CCtrlPanelDlg::OnBnClickedTrial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();

	switch (m_nCamSel)
	{
	case 0:
		pView->m_pCamDlg[m_nCamSel]->Inspect(NULL, m_nDispayType);
		break;
	default:
		pView->m_pCamDlg[m_nCamSel]->Inspect();
		break;
	}
	
}




HBRUSH CCtrlPanelDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (nCtlColor)
	{
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(90, 107, 135));  // static text 글자색 변경
		return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
