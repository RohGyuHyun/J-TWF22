// HistoLevelingDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XFMS.h"
#include "HistoLevellingDlg.h"
#include "afxdialogex.h"
#include "XFMSDoc.h"
#include "XFMSView.h"
#include "CamDlg.h"
#include "Status.h"

#define TIME_UPDATEHIOSTO 25
// CHistoLevellingDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHistoLevellingDlg, CDialogEx)

CHistoLevellingDlg::CHistoLevellingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HISTOLEVEL, pParent)
	, m_pView(NULL)
	, m_pOrgMilBuffer(NULL)
{

}

CHistoLevellingDlg::~CHistoLevellingDlg()
{
}

void CHistoLevellingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTOAREA, m_cHisotoArea);
	DDX_Control(pDX, IDC_BTN_HMIN, m_cMinHandle);
	DDX_Control(pDX, IDC_BTN_HMAX, m_cMaxHandle);
	DDX_Control(pDX, IDC_HISTO_HDLRECT, m_cHistoHandleRect);
}


BEGIN_MESSAGE_MAP(CHistoLevellingDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_HISTO_UPDATE, &CHistoLevellingDlg::OnHistoHDLUpdate)
	ON_BN_CLICKED(IDC_BTN_APPLY, &CHistoLevellingDlg::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BT_LOADIMG, &CHistoLevellingDlg::OnBnClickedBtLoadimg)
	ON_BN_CLICKED(IDC_BTN_SAVWLV, &CHistoLevellingDlg::OnBnClickedBtnSavwlv)
END_MESSAGE_MAP()


// CHistoLevellingDlg �޽��� ó�����Դϴ�.

void CHistoLevellingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case TIME_UPDATEHIOSTO:
		HistoUpdate();
		DrawHistogram();
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CHistoLevellingDlg::HistoUpdate()
{
	CMilIm::Histogram(m_pOrgMilBuffer, 65535, m_Histodata);
}




void CHistoLevellingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	int iLow, iHigh;
	iLow = CStatus::Instance()->m_wlvLow;
	iHigh = CStatus::Instance()->m_wlvHigh;
	if (bShow)
	{
		m_pOrgMilBuffer = m_pView->m_pCamDlg[0]->m_pMilCamProc;
		SetDlgItemInt(IDC_EDT_LVLLOW, iLow);
		SetDlgItemInt(IDC_EDT_LVLHIGH, iHigh);
		OnBnClickedBtnApply();
		SetTimer(TIME_UPDATEHIOSTO, 500, NULL);
	}
	else KillTimer(TIME_UPDATEHIOSTO);
}


void CHistoLevellingDlg::DrawHistogram()
{
	CRect rectHisto;
	CDC *pDC = m_cHisotoArea.GetDC();
	m_cHisotoArea.GetClientRect(&rectHisto);
	int heightHisto = rectHisto.Height();
	int widthHisto = rectHisto.Width();
	int *pQuantazation = new int[widthHisto];

	for (int i = 0; i < widthHisto; i++)
		pQuantazation[i] = 0;

	int posQuantazation;
	int iHighestQV = 0;
	int iHighestQ = 0;
	for (int i = 0; i < 65535; i++)
	{
		int iCount = m_Histodata[i];
		posQuantazation =(i * widthHisto)/65535.0;
		pQuantazation[posQuantazation] += iCount;
		if (iHighestQV < pQuantazation[posQuantazation])
		{
			iHighestQV = pQuantazation[posQuantazation];
			iHighestQ = posQuantazation;
		}
	}
	if (!iHighestQV)iHighestQV = CAM_HRES*CAM_VRES;
	pDC->MoveTo(0, heightHisto);
//	double rateHeight = (double)heightHisto / (CAM_HRES*CAM_VRES);
	double rateHeight = (double)heightHisto / iHighestQV;
	for (int i = 0; i < widthHisto; i++)
	{
		pDC->LineTo(i, (int)(heightHisto - (rateHeight * pQuantazation[i])));
//		pDC->LineTo(i, (int)(heightHisto - (rateHeight * (CAM_HRES*CAM_VRES))));
		pDC->MoveTo(i + 1, heightHisto);
	}
}


void CHistoLevellingDlg::OnBnClickedBtnApply()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int iLow, iHigh;
	int uiLvlLow = GetDlgItemInt(IDC_EDT_LVLLOW);
	int uiLvlHigh = GetDlgItemInt(IDC_EDT_LVLHIGH);
	if (uiLvlLow <= uiLvlHigh)
	{
		iLow = uiLvlLow;
		iHigh =uiLvlHigh;
	}
	else
	{
		iLow = uiLvlHigh;
		iHigh = uiLvlLow;
		SetDlgItemInt(IDC_EDT_LVLLOW, iLow);
		SetDlgItemInt(IDC_EDT_LVLHIGH, iHigh);
	}
	if(!CStatus::Instance()->m_bIsOffLine)m_pView->m_pCamDlg[0]->m_pGrabChannel->SetWndLvl(iLow, iHigh);
	CStatus::Instance()->m_wlvLow = iLow;
	CStatus::Instance()->m_wlvHigh = iHigh;
	m_cMinHandle.SetPos(iLow);
	m_cMaxHandle.SetPos(iHigh);
}


BOOL CHistoLevellingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnApply();
			return TRUE;
		}
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}



BOOL CHistoLevellingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CRect rectHistoHDLRect;
	m_cHistoHandleRect.GetWindowRect(&rectHistoHDLRect);
	this->ScreenToClient(&rectHistoHDLRect);
	m_cMinHandle.SetHistoHDLRect(&rectHistoHDLRect);
	m_cMaxHandle.SetHistoHDLRect(&rectHistoHDLRect);
	SetDlgItemInt(IDC_EDT_LVLLOW, 0);
	SetDlgItemInt(IDC_EDT_LVLHIGH, 65535);
	m_cMinHandle.SetPos(0);
	m_cMaxHandle.SetPos(65535);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


LRESULT CHistoLevellingDlg::OnHistoHDLUpdate(WPARAM wparam, LPARAM lparam)
{
	SetDlgItemInt(IDC_EDT_LVLLOW, m_cMinHandle.GetPos());
	SetDlgItemInt(IDC_EDT_LVLHIGH, m_cMaxHandle.GetPos());
	OnBnClickedBtnApply();
	return LRESULT();
}



void CHistoLevellingDlg::OnBnClickedBtLoadimg()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pOrgMilBuffer->BufLoad(_T("D:\\test.tif"));
}


void CHistoLevellingDlg::OnBnClickedBtnSavwlv()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CStatus::Instance()->SaveCFG();
}
