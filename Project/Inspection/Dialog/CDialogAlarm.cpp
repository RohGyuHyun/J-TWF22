// CDialogAlarm.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogAlarm.h"
#include "CIOClass.h"
#include "InspectionDlg.h"
// CDialogAlarm 대화 상자

IMPLEMENT_DYNAMIC(CDialogAlarm, CDialogEx)

CDialogAlarm::CDialogAlarm(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ALARM, pParent)
{

}

CDialogAlarm::~CDialogAlarm()
{
}

void CDialogAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogAlarm, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDialogAlarm::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_BUZZER_OFF, &CDialogAlarm::OnBnClickedButtonBuzzerOff)
END_MESSAGE_MAP()


// CDialogAlarm 메시지 처리기


void CDialogAlarm::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
	
	if (m_nAlarmCode == 3)
	{
		//pdlg->SetInverterAlarmReset();
	}
	
	pdlg->SetAlarmStatus(FALSE);
	
	this->ShowWindow(SW_HIDE);
}

void CDialogAlarm::SetAlarmString(int nAlarmCode, CString strAlarm)
{
	this->ShowWindow(SW_SHOW);
	m_nAlarmCode = nAlarmCode;
	CString strText;
	strText.Format(_T("%04d"), nAlarmCode);
	SetDlgItemText(IDC_EDIT_ALARM_0, strText);
	strText.Format(_T("%s"), strAlarm);
	SetDlgItemText(IDC_EDIT_ALARM_1, strText);

	if (nAlarmCode == 1)
	{
		SetDlgItemText(IDC_BUTTON1, _T("Reset"));
	}
	else
		SetDlgItemText(IDC_BUTTON1, _T("Close"));
}

CString CDialogAlarm::GetAlarmString()
{
	CString strText;
	GetDlgItemText(IDC_EDIT_ALARM_1, strText);

	return strText;
}

BOOL CDialogAlarm::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDialogAlarm::OnBnClickedButtonBuzzerOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CInspectionDlg* pdlg = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
	pdlg->SetAlarmBuzzerOff(TRUE);
}
