// CalibrationDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AGMDMonitor.h"
#include "CalibrationDlg.h"
#include "afxdialogex.h"
#include "Cal1Dlg.h"
#include "Cal2Dlg.h"
#include "Status.h"
#include "MonitorDlg.h"


// CCalibrationDlg ��ȭ �����Դϴ�.
#define MAINREF_TIME 1

IMPLEMENT_DYNAMIC(CCalibrationDlg, CDialogEx)

CCalibrationDlg::CCalibrationDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CALIBRATION, pParent)
	, m_pCal1Dlg(NULL)
	, m_pCal2Dlg(NULL)
	, m_nStep(0)
{

}

CCalibrationDlg::~CCalibrationDlg()
{
}

void CCalibrationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_NEXT, m_cNext);
	DDX_Control(pDX, IDC_BTN_PREV, m_cPrev);
}


BEGIN_MESSAGE_MAP(CCalibrationDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_NEXT, &CCalibrationDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREV, &CCalibrationDlg::OnBnClickedBtnPrev)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CCalibrationDlg �޽��� ó�����Դϴ�.


BOOL CCalibrationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	MakeChildWnd();
	WndRepositioning();
	CStatus::Instance()->m_pMonitorDlg->m_AGMDPacket.SendCalInfoCmd();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


int CCalibrationDlg::MakeChildWnd()
{
	m_pCal1Dlg = new CCal1Dlg;
	m_pCal2Dlg = new CCal2Dlg;

	if (m_pCal1Dlg->GetSafeHwnd() != 0) return -1;
	if (m_pCal2Dlg->GetSafeHwnd() != 0) return -1;
	if (!m_pCal1Dlg->Create(IDD_CAL1, this))
		return -2;
	if (!m_pCal2Dlg->Create(IDD_CAL2, this))
		return -2;
	return 0;
}


int CCalibrationDlg::WndRepositioning()
{
	CRect parentRect;
	CRect rectCal1, rectCal2;

	m_nStep = 0;
	GetClientRect(parentRect);
	m_pCal1Dlg->GetClientRect(&rectCal1);
	m_pCal2Dlg->GetClientRect(&rectCal2);

	m_pCal1Dlg->MoveWindow(CRect(0, 0, rectCal1.Width(), rectCal1.Height()));
	m_pCal2Dlg->MoveWindow(CRect(0, 0, rectCal2.Width(), rectCal2.Height()));

	m_pCal1Dlg->ShowWindow(SW_SHOW);
	m_pCal2Dlg->ShowWindow(SW_HIDE);

	return 0;
}


void CCalibrationDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	m_nStep = 0;
	SetStep(m_nStep);
	SetTimer(MAINREF_TIME, 300, NULL);
}


void CCalibrationDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
		case MAINREF_TIME:
			switch (m_nStep)
			{
			case 0:
				if (m_pCal1Dlg->m_bAbleToNext)
					m_cNext.EnableWindow(TRUE);
				else
					m_cNext.EnableWindow(FALSE);
				break;
			case 1:
				break;
			}
			break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CCalibrationDlg::OnBnClickedBtnNext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_nStep==0)
	{ 
		CStatus::Instance()->m_iNewJigGap = m_pCal1Dlg->GetJigGap();
	}

	if (m_nStep == 1)
	{
		m_pCal2Dlg->SendCalSetCmd();
		CDialogEx::OnOK();
		return;
	}
	if (m_nStep < 1)m_nStep++;
	SetStep(m_nStep);
}


void CCalibrationDlg::OnBnClickedBtnPrev()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nStep > 0)m_nStep--;
	SetStep(m_nStep);
}


void CCalibrationDlg::SetStep(int nStep)
{
	switch (nStep)
	{
	case 0:
		m_cPrev.ShowWindow(SW_HIDE);
		m_pCal1Dlg->ShowWindow(SW_SHOW);
		m_pCal2Dlg->ShowWindow(SW_HIDE);
		SetDlgItemTextW(IDC_BTN_NEXT, _T("Next"));
		break;
	case 1:
		m_pCal1Dlg->ShowWindow(SW_HIDE);
		m_pCal2Dlg->ShowWindow(SW_SHOW);
		SetDlgItemTextW(IDC_BTN_NEXT, _T("Done"));
		m_cPrev.ShowWindow(SW_SHOW);
		m_cNext.ShowWindow(SW_SHOW);
		break;
	}
}



BOOL CCalibrationDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (pMsg->message == WM_KEYDOWN)
	{
		if ((pMsg->wParam == VK_RETURN) || (pMsg->wParam == VK_ESCAPE))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCalibrationDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CDialogEx::OnClose();
}


void CCalibrationDlg::OnDestroy()
{
	delete m_pCal1Dlg;
	delete m_pCal2Dlg;
	CDialogEx::OnDestroy();

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}
