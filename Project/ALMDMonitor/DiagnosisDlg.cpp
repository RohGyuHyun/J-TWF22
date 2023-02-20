// DiagnosisDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ALMDMonitor.h"
#include "DiagnosisDlg.h"
#include "afxdialogex.h"
#include "Diagnosis1Dlg.h"
#include "Diagnosis2Dlg.h"
#include "Diagnosis3Dlg.h"
#include "Diagnosis4Dlg.h"
#include "Stabilization.h"
#include "Status.h"


// CDiagnosisDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CDiagnosisDlg, CDialogEx)

CDiagnosisDlg::CDiagnosisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIAGNOSIS, pParent)
	, m_pDiagnosis1Dlg(NULL)
	, m_pDiagnosis2Dlg(NULL)
	, m_pDiagnosis3Dlg(NULL)
	, m_pDiagnosis4Dlg(NULL)
	, m_nStep(1)
{

}

CDiagnosisDlg::~CDiagnosisDlg()
{
}

void CDiagnosisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DIAGPREV, m_cDiagPrev);
	DDX_Control(pDX, IDC_DIAGNEXT, m_cDiagNext);
	DDX_Control(pDX, IDCANCEL, m_cBtnCancel);
}


BEGIN_MESSAGE_MAP(CDiagnosisDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_DIAGNEXT, &CDiagnosisDlg::OnBnClickedDiagnext)
	ON_BN_CLICKED(IDC_DIAGPREV, &CDiagnosisDlg::OnBnClickedDiagprev)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDiagnosisDlg �޽��� ó�����Դϴ�.


BOOL CDiagnosisDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	MakeChildWnd();
	WndRepositioning();
	SetStep(1);
	SetTimer(22, 100, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


int CDiagnosisDlg::MakeChildWnd()
{
	m_pDiagnosis1Dlg = new CDiagnosis1Dlg;
	m_pDiagnosis2Dlg = new CDiagnosis2Dlg;
	m_pDiagnosis3Dlg = new CDiagnosis3Dlg;
	m_pDiagnosis4Dlg = new CDiagnosis4Dlg;

	if (m_pDiagnosis1Dlg->GetSafeHwnd() != 0) return -1;
	if (m_pDiagnosis2Dlg->GetSafeHwnd() != 0) return -1;
	if (m_pDiagnosis3Dlg->GetSafeHwnd() != 0) return -1;
	if (m_pDiagnosis4Dlg->GetSafeHwnd() != 0) return -1;

	if (!m_pDiagnosis1Dlg->Create(IDD_DIAG1, this))
		return -2;
	if (!m_pDiagnosis2Dlg->Create(IDD_DIAG2, this))
		return -2;
	if (!m_pDiagnosis3Dlg->Create(IDD_DIAG3, this))
		return -2;
	if (!m_pDiagnosis4Dlg->Create(IDD_DIAG4, this))
		return -2;

	return 0;
}


int CDiagnosisDlg::WndRepositioning()
{
	CRect parentRect;
	CRect rectDiag1, rectDiag2, rectDiag3, rectDiag4;

	GetClientRect(parentRect);
	m_pDiagnosis1Dlg->GetClientRect(&rectDiag1);
	m_pDiagnosis2Dlg->GetClientRect(&rectDiag2);
	m_pDiagnosis3Dlg->GetClientRect(&rectDiag3);
	m_pDiagnosis4Dlg->GetClientRect(&rectDiag4);

	m_pDiagnosis1Dlg->MoveWindow(CRect(0, 0, rectDiag1.Width(), rectDiag1.Height()));
	m_pDiagnosis2Dlg->MoveWindow(CRect(0, 0, rectDiag2.Width(), rectDiag2.Height()));
	m_pDiagnosis3Dlg->MoveWindow(CRect(0, 0, rectDiag3.Width(), rectDiag3.Height()));
	m_pDiagnosis4Dlg->MoveWindow(CRect(0, 0, rectDiag4.Width(), rectDiag4.Height()));

	m_pDiagnosis1Dlg->ShowWindow(SW_SHOW);
	m_pDiagnosis2Dlg->ShowWindow(SW_HIDE);
	m_pDiagnosis3Dlg->ShowWindow(SW_HIDE);
	m_pDiagnosis4Dlg->ShowWindow(SW_HIDE);

	return 0;
}


void CDiagnosisDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	KillTimer(22);
	delete m_pDiagnosis1Dlg;
	delete m_pDiagnosis2Dlg;
	delete m_pDiagnosis3Dlg;
	delete m_pDiagnosis4Dlg;
	CDialogEx::OnClose();
}


void CDiagnosisDlg::SetStep(int nStep)
{
	double dblRPResult = 0.0;
	double dblRRResult = 0.0;
	double dblPPResult = 0.0;
	switch (nStep)
	{
	case 1:
		m_cDiagPrev.ShowWindow(SW_HIDE);
		this->SetWindowTextW(_T("Step 1 of 4: 0 Degree Reading"));
		m_pDiagnosis1Dlg->ShowWindow(SW_SHOW);
		m_pDiagnosis2Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis3Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis4Dlg->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_cDiagPrev.ShowWindow(SW_SHOW);
		this->SetWindowTextW(_T("Step 2 of 4: 90 Degree Reading"));
		m_pDiagnosis1Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis2Dlg->ShowWindow(SW_SHOW);
		m_pDiagnosis3Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis4Dlg->ShowWindow(SW_HIDE);
		break;
	case 3:
		this->SetWindowTextW(_T("Step 3 of 4: 180 Degree Reading"));
		m_pDiagnosis1Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis2Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis3Dlg->ShowWindow(SW_SHOW);
		m_pDiagnosis4Dlg->ShowWindow(SW_HIDE);
		break;
	case 4:
		this->SetWindowTextW(_T("Step 4 of 4: 270 Degree Reading"));
		m_pDiagnosis1Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis2Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis3Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis4Dlg->ShowWindow(SW_SHOW);
		m_pDiagnosis4Dlg->HideResult();
		m_cBtnCancel.SetWindowTextW(_T("Cancel"));
		break;
	case 5:
		this->SetWindowTextW(_T("Result"));
		m_cBtnCancel.SetWindowTextW(_T("Done"));
		m_pDiagnosis1Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis2Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis3Dlg->ShowWindow(SW_HIDE);
		m_pDiagnosis4Dlg->ShowWindow(SW_SHOW);
		CalcResult(&dblRPResult, &dblRRResult, &dblPPResult);
		m_pDiagnosis4Dlg->SetResult(dblRPResult, dblRRResult, dblPPResult);
		m_pDiagnosis4Dlg->ShowResult();
	}
}


void CDiagnosisDlg::OnBnClickedDiagnext()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nStep != 5)
	{
		tALMDRecord *pALMDRecord = CStatus::Instance()->GetCurMeasureData();
		m_ptDiagData[m_nStep - 1].SetPoint(pALMDRecord->sRawRoll, pALMDRecord->sRawPitch);
	}
	if (m_nStep < 5)m_nStep++;
	SetStep(m_nStep);
}


void CDiagnosisDlg::OnBnClickedDiagprev()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (m_nStep > 1)m_nStep--;
	SetStep(m_nStep);
}


void CDiagnosisDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
	case 22:
		if ( CStabilization::Instance()->IsStable() && (m_nStep != 5) )
		{
			m_cDiagNext.EnableWindow(true);
		}
		else
		{
			m_cDiagNext.EnableWindow(false);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDiagnosisDlg::CalcResult(double * pDblRPResult, double * pDblRRResult, double * pDblPPResult)
{
	//�ݽð�������� 0��->m_ptDiagData[0] 90��->m_ptDiagData[1] 180��->m_ptDiagData[2] 270��->m_ptDiagData[3]
	//! 0���� 180�� ������ Roll�� ���밪�� ���� ��ȣ�� �ݴ�. Pitch���� ���밪�� ���� ��ȣ�� �ݴ�.
	//! 0���� 90�� ������ Roll�� Pitch 
	*pDblRPResult = (abs(m_ptDiagData[0].x + m_ptDiagData[1].y)) / 1000.;
	*pDblRRResult = (abs(m_ptDiagData[0].x + m_ptDiagData[2].x)) / 1000.;
	*pDblPPResult = (abs(m_ptDiagData[0].y + m_ptDiagData[2].y)) / 1000.;
	//*pDblRPResult = (abs(m_ptDiagData[0].x + m_ptDiagData[1].y)) / 1000.;
	//*pDblRRResult = (abs(m_ptDiagData[1].x + m_ptDiagData[3].x)) / 1000.;
	//*pDblPPResult = (abs(m_ptDiagData[0].y + m_ptDiagData[2].y)) / 1000.;
}
