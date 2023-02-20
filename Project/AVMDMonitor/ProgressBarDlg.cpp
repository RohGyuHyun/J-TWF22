// ProgressBarDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AVMDMonitor.h"
#include "ProgressBarDlg.h"
#include "afxdialogex.h"
#include "Status.h"

// CProgressBarDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CProgressBarDlg, CDialogEx)

BOOL CProgressBarDlg::m_bIsActive = FALSE;
int CProgressBarDlg::m_iValue = 0;
CProgressBarDlg::CProgressBarDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PROGRESSBARDLG, pParent)
	, m_pThread(NULL)
	, m_strMessage(_T(""))
	, m_nDataCount(0)
	, m_iTimeSecond(0)
{

}

CProgressBarDlg::~CProgressBarDlg()
{
}

void CProgressBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_WAIT, m_ctrl_Progress);
	DDX_Text(pDX, IDC_STATIC_PROGRESSBAR_TEXT, m_strMessage);
}


BEGIN_MESSAGE_MAP(CProgressBarDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDCANCEL, &CProgressBarDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CProgressBarDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CProgressBarDlg �޽��� ó�����Դϴ�.
BOOL CProgressBarDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::EXPORT_EXCEL)
	{
		int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
		m_nDataCount = nDataCount;
		SetTextMesaage(_T("Be Saving Excel data. \nPlease Wait."));
		CString strSecond;
		strSecond = "Progress Time : 0 Second";
		SetDlgItemText(IDC_STATIC_TIMEVIEW, strSecond);
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(TRUE);
		SetMaxRange(nDataCount);
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::PRATICAL_SAVE)
	{
		int nDataCount = CStatus::Instance()->m_iPartialSaveCount;
		m_nDataCount = nDataCount;
		SetTextMesaage(_T("Be Saving Partial Data. \nPlease Wait."));
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(FALSE);
		SetMaxRange(nDataCount);
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::SAVE_AS)
	{
		int nDataCount = CStatus::Instance()->GetRepository()->GetDataCount(0);
		m_nDataCount = nDataCount;
		SetTextMesaage(_T("Saving to another file. \nPlease Wait."));
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(FALSE);
		SetMaxRange(nDataCount);
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::FILTER)
	{
		SetTextMesaage(_T("The time required for the filter conversion operation is up to 60 seconds.Please Wait."));
		CString strSecond;
		strSecond = "Progress Time : 0 Second";
		SetDlgItemText(IDC_STATIC_TIMEVIEW, strSecond);
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(TRUE);
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::COMPARE_FILTER)
	{
		SetTextMesaage(_T("The time required for the filter conversion operation is up to 60 seconds.Please Wait."));
		CString strSecond;
		strSecond = "Progress Time : 0 Second";
		SetDlgItemText(IDC_STATIC_TIMEVIEW, strSecond);
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(TRUE);
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::REPORT)
	{
		SetTextMesaage(_T("Making to Report. \nPlease Wait."));
		CString strSecond;
		strSecond = "Progress Time : 0 Second";
		SetDlgItemText(IDC_STATIC_TIMEVIEW, strSecond);
		GetDlgItem(IDC_PROGRESS_WAIT)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIMEVIEW)->ShowWindow(TRUE);
	}
	ProgressBarStart();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void CProgressBarDlg::SetTextMesaage(CString strMessage)
{
	SetDlgItemText(IDC_STATIC_PROGRESSBAR_TEXT, strMessage);
//	m_strMessage = strMessage;
}

void CProgressBarDlg::SetMaxRange(int nMaxCount)
{
	//CStatus::Instance()->m_nProgressBarCount = 0;
	m_ctrl_Progress.SetRange32(0, nMaxCount);
}

void CProgressBarDlg::ProgressBarStart()
{
	m_bIsActive = TRUE;
	m_iTimeSecond = 0;
	SetTimer(0, 1000, NULL);
	if (CStatus::Instance()->GetProgressBarMode() != CStatus::tProgressBarMode::FILTER)
	{
		m_pThread = AfxBeginThread(ProcessThread, this);
	}
}

void CProgressBarDlg::ProgressBarFinish()
{
	m_bIsActive = FALSE;
	KillTimer(0);
	CDialogEx::OnCancel();
}

UINT CProgressBarDlg::ProcessThread(LPVOID lParam)
{
	while (m_bIsActive)
	{
		m_iValue = CStatus::Instance()->m_nProgressBarCount;
	}
	return 0;
}

void CProgressBarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	switch (nIDEvent)
	{
	case 0:
		if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::FILTER
			|| CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::COMPARE_FILTER
			|| CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::REPORT)
		{
			m_iTimeSecond++;
			CString strSecond;
			strSecond.Format(_T("Progress Time : %d Second"), m_iTimeSecond);
			SetDlgItemText(IDC_STATIC_TIMEVIEW, strSecond);
			if(CStatus::Instance()->m_bIsFiltering == FALSE)
				ProgressBarFinish();
		}
		else
		{
			if (m_nDataCount == m_iValue)
			{
				m_ctrl_Progress.SetPos(m_iValue);
				ProgressBarFinish();
			}
			m_ctrl_Progress.SetPos(m_iValue);
		}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CProgressBarDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::EXPORT_EXCEL)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::PRATICAL_SAVE)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::SAVE_AS)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::FILTER)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
		CStatus::Instance()->ReviewDataClear(TRUE, TRUE);
		CStatus::Instance()->SetFilterType(FALSE, FALSE, FALSE);
		CStatus::Instance()->m_strFilterStatus = _T("disable");
		CStatus::Instance()->DisplayDlgUpdateUI();
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::COMPARE_FILTER)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
		CStatus::Instance()->ReviewDataClear(FALSE, TRUE);
		CStatus::Instance()->DisplayDlgUpdateUI();
	}
	if (CStatus::Instance()->GetProgressBarMode() == CStatus::tProgressBarMode::REPORT)
	{
		CStatus::Instance()->m_bIsThreadStop = TRUE;
	}

	ProgressBarFinish();
	CDialogEx::OnCancel();
}

void CProgressBarDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetTimer(0, 10, NULL);
}



