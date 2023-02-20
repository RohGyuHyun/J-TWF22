// CDialogXRayWarmup.cpp: 구현 파일
//

#include "pch.h"
#include "Inspection.h"
#include "afxdialogex.h"
#include "CDialogXRayWarmup.h"
#include "CXRayClass.h"
#include "CIOClass.h"


// CDialogXRayWarmup 대화 상자

IMPLEMENT_DYNAMIC(CDialogXRayWarmup, CDialogEx)

CDialogXRayWarmup::CDialogXRayWarmup(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_X_RAY_WARMUP, pParent)
{
	m_nWarmupStepTimeSec = 10;
	m_nWarmupStepCnt = 0;
	m_nWarmupStepTimeSecCnt = 0;
	m_nInitCnt = 0;
}

CDialogXRayWarmup::~CDialogXRayWarmup()
{
}

void CDialogXRayWarmup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_WARMUP, m_ProgressWarmup);
}


BEGIN_MESSAGE_MAP(CDialogXRayWarmup, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CDialogXRayWarmup 메시지 처리기


void CDialogXRayWarmup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch((int)nIDEvent)
	{
	case X_RAY_CUR_VALUE_READ:
		if (this->IsWindowVisible())
		{
			X_RAY_INFO info;
			info = CXRayClass::Instance()->GetXrayInfo();
			double dKv, dMa;
			dKv = info.nNoScalingCurKV * info.dScalingCurKV;
			dMa = info.nNoScalingCurMA * info.dScalingCurMA;
			CString strText;
			strText.Format(_T("%0.3f kV"), dKv);
			SetDlgItemText(IDC_BTNENHCTRL_WARMUP_CUR_KV, strText);
			strText.Format(_T("%0.3f mA"), dMa);
			SetDlgItemText(IDC_BTNENHCTRL_WARMUP_CUR_MA, strText);
			strText.Format(_T("%d Step"), m_nWarmupStepCnt + 1);
			SetDlgItemText(IDC_BTNENHCTRL_WARMUP_STEP, strText);

			m_ProgressWarmup.SetPos(m_nWarmupStepCnt + 1);
			int nInterLock = 0;
			if (info.stXray_status.bOverTemperature)
				nInterLock = 1;
			if(info.stXray_status.bOverVoltage)
				nInterLock = 2;
			if(info.stXray_status.bUnderVoltage)
				nInterLock = 3;
			if(info.stXray_status.bOverCurrent)
				nInterLock = 4;
			if(info.stXray_status.bUnderCurrent)
				nInterLock = 5;
			if(info.stXray_status.bWatchdogTimeOut)
				nInterLock = 6;
			if(info.stXray_status.bOpenInterlock)
				nInterLock = 7;

			m_nInitCnt++;
			if (nInterLock > 0 && (m_nInitCnt >= 20))
			{
				KillTimer(X_RAY_WARMUP_START);
				KillTimer(X_RAY_CUR_VALUE_READ);
				CXRayClass::Instance()->SetWarmupState(FALSE);
				CXRayClass::Instance()->SetXrayOnOff(FALSE);
				CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
				CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, FALSE);
				CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, TRUE);
				AfxMessageBox(_T("X-Ray Alarm!!!\nWarmup Stop!!!"));
				KillTimer(X_RAY_WARMUP_TOWER_LAMP);
			}
		}
		break;
	case X_RAY_WARMUP_START:
		if (m_bWarmupStart)
		{
			m_nWarmupStepTimeSecCnt++;
			if (m_nWarmupStepTimeSecCnt >= m_nWarmupStepTimeSec)
			{
				m_nWarmupStepTimeSecCnt = 0;
				m_nWarmupStepCnt++;
				
				switch (m_nWarmupStepCnt)
				{
				case 1:
					CXRayClass::Instance()->SetXrayVolCur(45., 0.4);
					break;
				case 2:
					CXRayClass::Instance()->SetXrayVolCur(50., 0.55);
					break;
				case 3:
					CXRayClass::Instance()->SetXrayVolCur(55., 0.7);
					break;
				case 4:
					CXRayClass::Instance()->SetXrayVolCur(60., 0.85);
					break;
				case 5:
					CXRayClass::Instance()->SetXrayVolCur(65., 1.0);
					break;
				case 6:
					CXRayClass::Instance()->SetXrayVolCur(70., 1.15);
					break;
				case 7:
					CXRayClass::Instance()->SetXrayVolCur(70., 1.25);
					break;
				case 8:
					CXRayClass::Instance()->SetXrayVolCur(75., 1.25);
					break;
				case 9:
					CXRayClass::Instance()->SetXrayVolCur(80., 1.25);
					break;
				case 10:
					KillTimer(X_RAY_CUR_VALUE_READ);
					KillTimer(X_RAY_WARMUP_START);
					CXRayClass::Instance()->SetWarmupState(TRUE);
					CXRayClass::Instance()->SetXrayOnOff(FALSE);
					CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
					
					m_bWarmupEnd = TRUE;
					m_bWarmupStart = FALSE;
					AfxMessageBox(_T("Warmup Success!!"));
					KillTimer(X_RAY_WARMUP_TOWER_LAMP);
					CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, FALSE);
					CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, TRUE);
					ClickBtnenhctrlWarmupExit();
					break;
				}
			}
		}
		else
		{
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);
			KillTimer(X_RAY_WARMUP_START);
		}
		break;
	case X_RAY_WARMUP_INIT:
		KillTimer(X_RAY_WARMUP_INIT);
		SetTimer(X_RAY_WARMUP_START, 1000, NULL);
		break;
	case X_RAY_WARMUP_TOWER_LAMP:
		if (m_bTowerLampOn)
		{
			m_bTowerLampOn = FALSE;
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, FALSE);
		}
		else
		{
			m_bTowerLampOn = TRUE;
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, TRUE);
		}
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CDialogXRayWarmup::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow)
	{
		CString strText;
		strText.Format(_T("%04d.%02d.%02d-%02d:%02d:%02d"), m_cXrayEndTime.nYear, m_cXrayEndTime.nMonth, m_cXrayEndTime.nDay, m_cXrayEndTime.nHour, m_cXrayEndTime.nMinute, m_cXrayEndTime.nSec);
		SetDlgItemText(IDC_BTNENHCTRL_WARMUP_LAST_DATE, strText);
		m_nInitCnt = 0;
		m_ProgressWarmup.SetRange(0, 10);
		m_ProgressWarmup.SetPos(1);
		CXRayClass::Instance()->SetXrayOnOff(TRUE);
		SetTimer(X_RAY_CUR_VALUE_READ, 100, NULL);
		SetWarmnup();
		m_bTowerLampOn = TRUE;
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, FALSE);
		CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, TRUE);
		SetTimer(X_RAY_WARMUP_TOWER_LAMP, 1000, NULL);
	}
}


void CDialogXRayWarmup::SetWarmnup()
{
	m_bWarmupEnd = FALSE;
	m_bWarmupStart = TRUE;
	m_nWarmupStepCnt = 0;
	CTime cTotalTime, cStartTime;
	cStartTime = CTime::GetCurrentTime();
	//cTotalTime = (cStartTime - cXrayEndTime);

	time_t start, end;
	struct tm user_stime;
	int tm_day, tm_hour, tm_min, tm_sec;
	double diff;

	user_stime.tm_year = m_cXrayEndTime.nYear - 1900; // 년도가 1900년부터 시작하기 때문
	user_stime.tm_mon = m_cXrayEndTime.nMonth - 1; //월이 0부터 시작하기 때문
	user_stime.tm_mday = m_cXrayEndTime.nDay;
	user_stime.tm_hour = m_cXrayEndTime.nHour;
	user_stime.tm_min = m_cXrayEndTime.nMinute;
	user_stime.tm_sec = m_cXrayEndTime.nSec;
	user_stime.tm_isdst = 0; //썸머타임 사용안함

	start = mktime(&user_stime);
	time(&end);

	diff = difftime(end, start);

	tm_day = (int)(diff / (60 * 60 * 24));
	diff = (diff - (tm_day * 60 * 60 * 24));

	tm_hour = (int)(diff / (60 * 60));
	diff = diff - (tm_hour * 60 * 60);

	tm_min = (int)(diff / 60);
	diff = diff - (tm_min * 60);

	tm_sec = (int)(diff);


	if (tm_day > 90)
	{
		m_nWarmupStepTimeSec = 60 * 5;
	}
	else if (tm_day > 30)
	{
		m_nWarmupStepTimeSec = 60;
	}
	else if (tm_day > 2)
	{
		m_nWarmupStepTimeSec = 30;
	}
	else
	{
		m_nWarmupStepTimeSec = 10;
	}

	//SetTimer(X_RAY_WARMUP_START, 1000, NULL);
	SetTimer(X_RAY_WARMUP_INIT, 2000, NULL);

	//x_ray on
	//40 kv
	//0.25 ma
	CXRayClass::Instance()->SetXrayOnOff(TRUE);
	CXRayClass::Instance()->SetXrayVolCur(40., 0.25);
}

BEGIN_EVENTSINK_MAP(CDialogXRayWarmup, CDialogEx)
ON_EVENT(CDialogXRayWarmup, IDC_BTNENHCTRL_WARMUP_EXIT, DISPID_CLICK, CDialogXRayWarmup::ClickBtnenhctrlWarmupExit, VTS_NONE)
END_EVENTSINK_MAP()


void CDialogXRayWarmup::ClickBtnenhctrlWarmupExit()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bWarmupStart && !m_bWarmupEnd)
	{
		CString strMsg;
		strMsg.Format(_T("X-Ray Warmup End?"));
		if (AfxMessageBox(strMsg, MB_YESNO) != IDYES)
		{
			return;
		}
	}


	KillTimer(X_RAY_CUR_VALUE_READ);
	KillTimer(X_RAY_WARMUP_START);
	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_RED, FALSE);
	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_TOWER_LAMP_YELLOW, TRUE);
	KillTimer(X_RAY_WARMUP_TOWER_LAMP);
	CXRayClass::Instance()->SetXrayOnOff(FALSE);
	CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_X_RAY_ON, FALSE);

	if(!CXRayClass::Instance()->GetWarmupState() || !m_bWarmupEnd)
		CXRayClass::Instance()->SetWarmupState(FALSE);

	EndDialog(m_bWarmupEnd);
}


BOOL CDialogXRayWarmup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_F12)
			ClickBtnenhctrlWarmupExit();
		else if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
			return TRUE;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
