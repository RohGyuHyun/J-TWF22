#include "pch.h"
#include "SequenceModule.h"
//#include "DialogAlarmMsg.h"
#include "InspectionDlg.h"

//Multi media Timer
DWORD _TimerCounter;
UINT _TimerID;
TIMECAPS _Caps;
BOOL _TimerPost;

static void CALLBACK TimerReadFunc(UINT wID, UINT wUser, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (_TimerPost == FALSE)
	{
		_TimerCounter++;
	}
	else
	{
		AfxMessageBox(_T("Error : high resolution timer event loss!"));
	}
}

CMotionData::CMotionData()
{
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);
	memset(&m_ProSts, NULL, sizeof(typeMotionProcessSts) * MOT_STATUS_MAX_NUM * MOT_STATUS_SUB_MAX_NUM);
}


CMotionData::~CMotionData()
{
	ResetSeq();
}

void CMotionData::ResetSeq()
{
	memset(&m_Seq, NULL, sizeof(typeMotionSeq) * MOT_STATUS_MAX_NUM);
	memset(&m_ProSts, NULL, sizeof(typeMotionProcessSts) * MOT_STATUS_MAX_NUM * MOT_STATUS_SUB_MAX_NUM);
}

CSequenceModule::CSequenceModule(void)
{
	memset(&m_Flag, NULL, sizeof(typeMotionFlag));
	memset(&m_SeqFlag, NULL, sizeof(typeSequenceFlag));
	m_bInitSeq = FALSE;
	m_bReadyState = FALSE;
	m_bSeqEnd = FALSE;
	m_bswStart[0] = FALSE;
	m_bswStart[1] = FALSE;

	m_hWndMain = NULL;
	m_nDstDeqIdx = 0;
	m_nGrabStepIndex = 0;
	m_nMaxStepCnt = 0;
	m_nPrc_Idx = 0;
	m_nSeqChannel = 0;
	m_nSeqIdx = 0;
	m_nSeq_Idx = 0;
	m_nSeq_Sub_Idx = 0;

	m_bInspecting = FALSE;
	m_nInSensingTime = 3;
	m_nOutSensingTime = 3;
}


CSequenceModule::~CSequenceModule(void)
{
	
}

void CSequenceModule::Open(HWND hWnd, int nChannel, CListCtrl* listLog)
{
	CString strText;

	m_hWndMain = hWnd;

	_TimerPost = FALSE;
	timeGetDevCaps(&_Caps, sizeof(_Caps));
	_Caps.wPeriodMin = 1;
	timeBeginPeriod(_Caps.wPeriodMin);
	_TimerID = timeSetEvent(_Caps.wPeriodMin, _Caps.wPeriodMin, (LPTIMECALLBACK)&TimerReadFunc, 0, (UINT)TIME_PERIODIC);

	if (!_TimerID)
	{
		AfxMessageBox(_T("Error : high resolution timer set fail!"));
	}
	//wchar_t strTemp[MAX_CHAR_LENG];
	//GetCurrentDirectory(MAX_CHAR_LENG, strTemp);
	//strText.Format(_T("%s\\%sSequence\\"), strTemp, LOG_FILE_PATH);
	//m_pMotLog = new CLogFile(strText, _T("Sequence.log"), listLog, _T("Log"), 500, 50, TRUE);

	memset(&m_SeqFlag, NULL, sizeof(typeSequenceFlag));

	m_SeqFlag.m_bInitialized = FALSE;
	m_Flag.m_bProcessThread = TRUE;

	SetSeqChannel(nChannel);

	//20190703 ngh
	//SetLamp(COLOR_LAMP_YELLOW);//, FALSE);
	//SetLamp(COLOR_LAMP_RED);//, FALSE);
}

void CSequenceModule::Close()
{
	m_Flag.m_bProcessThread = FALSE;

	//SetLamp(COLOR_LAMP_OFF);//, FALSE);
}

void CSequenceModule::Reset()
{
	int i, j;
	int prc_idx, seq_idx, sub_seq_idx;
	prc_idx = 0;
	seq_idx = 0;
	sub_seq_idx = 0;

	CString strText;

	for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			if (m_MDat[i].m_Seq[j].m_bAlarm)
			{
				prc_idx = i;
				seq_idx = j;
				sub_seq_idx = m_MDat[i].m_Seq[j].m_iSubSeqIdx;

			}
			m_MDat[i].m_Seq[j].m_bAlarm = FALSE;
		}
	}

	switch (seq_idx)
	{
	case 0:

		break;
	case 1:
		switch(sub_seq_idx)
		{
		case 110:

			break;
		}
		break;
	case 2:
		switch (sub_seq_idx)
		{
		case 5030:
		case 5040:
		case 5050:
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 5000;
			break;
		case 5100:
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 5090;
			break;
		case 8000:	//수평 검사
		case 8010:	//수직 검사 실패시 시퀀스 종료
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 1000;
			break;
		default:
			m_MDat[0].m_Seq[seq_idx].m_iSubSeqIdx = 1000;	
			break;
		}
		break;
	}

	m_Flag.m_bAlarm = FALSE;
	m_Flag.m_bAutoStart = FALSE;

	strText.Format(_T("Alarm Reset"));		//LOG
	WriteLog(strText);

	//SetLamp(COLOR_LAMP_YELLOW);//, FALSE);
}

BOOL CSequenceModule::Start()
{
	int i, j;
	CString strText;
	BOOL rslt = TRUE;

	//검사시작
	if (!m_Flag.m_bFirstStart)
		InitSeq();

	for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_Flag.m_bAutoStart = TRUE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	strText.Format(_T("AutoRun Start"));		//LOG
	WriteLog(strText);

	//SetLamp(COLOR_LAMP_GREEN);//, FALSE);

	return rslt;
}
void CSequenceModule::Stop()
{
	int i, j;
	CString strText;

	for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
	{
		for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
		{
			m_Flag.m_bAutoStart = FALSE;
			m_MDat[i].m_Seq[j].m_bAutoStart = m_Flag.m_bAutoStart;
		}
	}

	CInspectionDlg *pDlgMain = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
	//pDlgMain->SetInitSeqStepPara(m_nSeqChannel);
	//pDlgMain->SetSeqStepIndex(m_nSeqChannel, -1);
	//pDlgMain->SetStopInitSeq(m_nSeqChannel);
	m_nGrabStepIndex = 0;
	m_bInspecting = FALSE;
#ifdef HW_USE
	CAtDetector::Instance()->GrabStop();
#endif
	strText.Format(_T("AutoRun End"));		//LOG
	WriteLog(strText);

	//SetLamp(COLOR_LAMP_RED);//, FALSE);
}

void CSequenceModule::SetLamp(int nColor)//, BOOL bBuzz)
{
	/*switch (nColor)
	{
	case COLOR_LAMP_RED:
		break;
	case COLOR_LAMP_YELLOW:
		break;
	case COLOR_LAMP_GREEN:
		break;
	case COLOR_LAMP_OFF:
		break;
	default:
		
		break;
	}*/
}

void CSequenceModule::InitSeq()
{
	CString strText;
	strText.Format(_T("Initialize sequence ?"));
	//if (AfxMessageBox(strText, MB_YESNO) == IDYES)
	{
		CInspectionDlg *pDlgMain = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;

		m_Flag.m_bFirstStart = TRUE;
		m_Flag.m_bAreaSensor = FALSE;

		m_MDat[0].m_Seq[1].m_iSubSeqIdx = 0;
		m_MDat[0].m_Seq[2].m_iSubSeqIdx = 100;

		int nTimerCnt = 0;

		//pDlgMain->KillTimer(MAIN_TIMER_AUTO_SEQ_END_CHECK_CHANNEL_0 + nTimerCnt);
		m_bSeqProgress = FALSE;			

		strText.Format(_T("Sequence Initialize"));		//LOG
		WriteLog(strText);
	}
}

int CSequenceModule::CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type)
{

	if (0 == m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if (0 <m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE = _TimerCounter;
			if (m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeout < (m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_dTimeoutS))
			{
				return 0;
			}
		}
	}

	sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_cTypeStatus, "%s", status_type);//축번호

	return 1;
}

//시퀀스 인덱스 넣기(리셋후 다음동작 않함)
void CSequenceModule::SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx)
{
	m_MDat[prc_idx].m_ProSts[seq_idx][seq_sub_idx].m_iNGSeqIdx = 0;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_iNextSubSeqIdx = seq_sub_idx;
	m_MDat[prc_idx].m_Seq[seq_idx].m_bOpSts = 0;

	m_nPrc_Idx = prc_idx;;
	m_nSeq_Idx = seq_idx;
	m_nSeq_Sub_Idx = seq_sub_idx;
}

void CSequenceModule::GetSeqIdx(int *prc_idx, int *seq_idx, int *seq_sub_idx)
{
	*prc_idx = m_nPrc_Idx;
	*seq_idx = m_nSeq_Idx;
	*seq_sub_idx = m_nSeq_Sub_Idx;
}

int CSequenceModule::GetSeqSubIdx(int prc_idx, int seq_idx)
{
	return m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
}

int CSequenceModule::CheckDelay(int prc_idx, int seq_idx, int delay)
{
	if (0 == m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx)
	{
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_iNGSeqIdx++;
		m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS = _TimerCounter;
	}
	else
	{
		//Compare timeout
		if (0 < delay)
		{
			m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE = _TimerCounter;
			if (delay < (m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutE - m_MDat[prc_idx].m_ProSts[seq_idx][m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx].m_dTimeoutS))
			{
				return 1;
			}
		}
		else
		{
			return 1;
		}
	}

	return 0;
}

//정지 상태에서의 동작을 지정
int CSequenceModule::StopModeOperation(int *prc_idx, int *seq_idx)
{	
	BOOL rslt = TRUE;

	*prc_idx = 0;
	*seq_idx = 0;

	if (m_MDat[*prc_idx].m_Seq[*seq_idx].m_bAlarm)
	{
		return 1;
	}

	CInspectionDlg *pDlgMain = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;
#ifdef HW_USE
	//Safe and limit sensor check
	/*if (!pDlgMain->GetEMG())
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = 0;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
		sprintf_s(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, ALARM_MSG_EMG_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_EMG_INTER_LOCK.GetString());
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, "1");

		return 0;
	}

	if (!pDlgMain->GetDoor())
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = 2;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
		sprintf_s(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, ALARM_MSG_DOOR_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_DOOR_INTER_LOCK.GetString());
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, "3");

		return 0;
	}

	if (pDlgMain->GetTubeAlarm() || pDlgMain->GetXRayInterLock())
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = 4;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
		sprintf_s(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, ALARM_MSG_TUBE_ALARM_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_TUBE_ALARM_INTER_LOCK.GetString());
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, "5");

		return 0;
	}*/
#endif
	//스톱 모드에서는 에어리어 센서는 무시한다.
	/*
	if (m_Axis.ReadInput(AXIS_Y, INPUT_003_AREA_SENSOR))
	{
		m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx = OP_AREA_SENSOR_CHECK;
		m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_dTimeout = TIMEOUT_SAFE_SENSOR;//ms
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGStatus, ALARM_MSG_AREA_SENSOR);
		sprintf(m_MDat[*prc_idx].m_ProSts[*seq_idx][m_MDat[*prc_idx].m_Seq[*seq_idx].m_iSubSeqIdx].m_cNGCode, ALARM_CODE_AREA_SENSOR);

		return 0;
	}
	*/

	return TRUE;
}

int CSequenceModule::MotionProcess(int prc_idx, int seq_idx)
{
	int rslt = 1;
	int sub_seq_idx = 0;

	long lTotalTime = 0;

	BOOL chk_timeout = TRUE;
	BOOL not_delay = TRUE;

	char status_type[20];
	memset(status_type, NULL, sizeof(status_type));

	CString strText;
	CTime CurTime;
	CurTime = CTime::GetCurrentTime();

	CInspectionDlg *pDlgMain = (CInspectionDlg*)AfxGetApp()->m_pMainWnd;

	switch (seq_idx)
	{
		case 0:	//Stop mode check
			chk_timeout = FALSE;
			
			break;

		case 1:	//CASE 1: Safe and limit sensor(inter lock)
			sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;

			switch (sub_seq_idx)
			{
			case 0:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_EMG_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_EMG_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "1"); 

				if(pDlgMain->GetEMG())
					SetSeqIdx(prc_idx, seq_idx, 1);

				break;
			case 1:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_KEY_SW_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_KEY_SW_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "2");

				if (pDlgMain->GetKeySw())
					SetSeqIdx(prc_idx, seq_idx, 2);

				break;
			case 2:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_DOOR_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_DOOR_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "3");

				if (pDlgMain->GetDoor())
					SetSeqIdx(prc_idx, seq_idx, 3);

				break;
			case 3:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_INVERTER_ALARM_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_INVERTER_ALARM_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "4");

				if (!pDlgMain->GetInverterAlarm())
					SetSeqIdx(prc_idx, seq_idx, 4);

				break;

			case 4:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_TUBE_ALARM_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_TUBE_ALARM_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "5");

				if (!pDlgMain->GetTubeAlarm() && !pDlgMain->GetXRayInterLock())
					SetSeqIdx(prc_idx, seq_idx, 5);

				break;
			case 5:
				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_X_RAY_LAMP_ALARM_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "6");

				if (!pDlgMain->GetXRayLampALM())
					SetSeqIdx(prc_idx, seq_idx, 6);

				break;
			case 6:
				if (m_nInSensingTime == 0)
				{
					SetSeqIdx(prc_idx, seq_idx, 7);
					break;
				}

				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = m_nInSensingTime * 1000.;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_IN_SENSER_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_IN_SENSER_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "7");

				if (!pDlgMain->GetInSenser())
					SetSeqIdx(prc_idx, seq_idx, 7);

				break;
			case 7:
				if (m_nOutSensingTime == 0)
				{
					SetSeqIdx(prc_idx, seq_idx, 9);
					break;
				}

				m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = m_nOutSensingTime * 1000.;
				sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_OUT_SENSER_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_OUT_SENSER_INTER_LOCK.GetString());
				sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "8");

				if (!pDlgMain->GetOutSenser())
					SetSeqIdx(prc_idx, seq_idx, 9);

				break;
			case 8:
				//m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_dTimeout = TIMEOUT_SAFE_INTERLOCK;
				//sprintf_s(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGStatus, ALARM_MSG_DOOR_INTER_LOCK.GetLength() + 1, "%S", ALARM_MSG_DOOR_INTER_LOCK.GetString());
				//sprintf(m_MDat[prc_idx].m_ProSts[seq_idx][sub_seq_idx].m_cNGCode, "2");

				//if (!pDlgMain->GetDoorInterLock())
				//	SetSeqIdx(prc_idx, seq_idx, 2);

				break;
			case 9:
				SetSeqIdx(prc_idx, seq_idx, 0);
				break;
			}
			break;
		case 2:	//CASE 2: Main sequence
			sub_seq_idx = m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx;
			
			switch (sub_seq_idx)
			{
				case 100:
					//READY INITIALIZE
					//Sleep(2000);
					strText.Format(_T("Sequence Loop Start"));
					WriteLog(strText);
					m_SeqFlag.m_bInitialized = FALSE;
					m_SeqFlag.m_nMotSubSeqIdx = 100;
					m_SeqFlag.m_nSeqIndex = SEQ_STATUS_READY;
					m_bSeqProgress = FALSE;
					m_bReadyState = TRUE;
					m_bInspecting = FALSE;
					SetSeqIdx(prc_idx, seq_idx, 110);
					break;
				case 110:
					if (pDlgMain->GetXRayOnCheck())
					{
						if (pDlgMain->GetStopWait())
						{
							pDlgMain->ShowTextBox(_T(""), FALSE);
						}
						strText.Format(_T("Sequence Init X-Ray On Check End"));
						WriteLog(strText);
						SetSeqIdx(prc_idx, seq_idx, 120);
					}
					break;
				case 120://init
					//if(pDlgMain->GetGrabEnd() && pDlgMain->GetInspectionEnd())
					{
						m_bReadyState = FALSE;
						strText.Format(_T("Sequence Init Start"));
						WriteLog(strText);
						m_bSeqProgress = TRUE;
						m_nGrabStepIndex = 0;
						m_bSeqEnd = FALSE;
						pDlgMain->SetInitInspection();
#ifdef HW_USE
						SetSeqIdx(prc_idx, seq_idx, 130);
#else
						SetSeqIdx(prc_idx, seq_idx, 125);
#endif
						strText.Format(_T("Sequence Init End"));
						WriteLog(strText);
						strText.Format(_T("Sequence Grab Start Wait"));
						WriteLog(strText);
						}
					break;
#ifndef HW_USE
				case 125:
					{
						long lStart = (long)GetTickCount64();
						pDlgMain->SetTotalStartTime(lStart);
						if(pDlgMain->LoadSqImage())
							SetSeqIdx(prc_idx, seq_idx, 130);
					}
					break;
#endif
				case 130:
					if (pDlgMain->GetGrabEnd() && !pDlgMain->GetInspectionStart())// && !pDlgMain->GetInspectionEnd())
					{
						strText.Format(_T("Sequence Grab End"));
						WriteLog(strText);
						strText.Format(_T("Sequence Inspection Start"));
						WriteLog(strText);
						pDlgMain->SetInspectionStart();
						SetSeqIdx(prc_idx, seq_idx, 135);
					}
					break;
				case 135:
					//안정화시간
					not_delay = CheckDelay(prc_idx, seq_idx, 50);

					if (not_delay)
						SetSeqIdx(prc_idx, seq_idx, 140);
					break;
				case 140:
					//if (!pDlgMain->GetInspectionStart() && pDlgMain->GetInspectionEnd())
					if (pDlgMain->GetInspectionEnd())
					{
						strText.Format(_T("Sequence Inspection End"));
						WriteLog(strText);
						SetSeqIdx(prc_idx, seq_idx, 150);
					}
					break;
				case 150:
					{
						long lEnd = (long)GetTickCount64();
						double dTackTime = (double)(lEnd - pDlgMain->GetInspStartTime()) / 1000.;
						pDlgMain->SetDispTackTime(dTackTime);

						strText.Format(_T("Sequence Inspection Result Display Start"));
						WriteLog(strText);
						pDlgMain->SetResultDisplay();
						SetSeqIdx(prc_idx, seq_idx, 170);
					}
					break; 
				case 160:
					//안정화시간
					not_delay = CheckDelay(prc_idx, seq_idx, 200);

					if (not_delay)
						SetSeqIdx(prc_idx, seq_idx, 170);
					break;
				case 170:
					if (pDlgMain->GetResultDisplayEnd())
					{
						long lEnd = (long)GetTickCount64();
						double dTackTime = (double)(lEnd - pDlgMain->GetTotalStartTime()) / 1000.;
						pDlgMain->SetDispTackTime(dTackTime, 1);
						
						SetSeqIdx(prc_idx, seq_idx, 180);

						strText.Format(_T("Sequence Inspection Result Display End"));
						WriteLog(strText);
						//Sleep(1500);
					}
#ifdef HW_USE
					if(pDlgMain->GetStopWait())
						SetSeqIdx(prc_idx, seq_idx, 190);
#endif
					break;
				case 175:
				{
#ifndef HW_USE
					//안정화시간
					not_delay = CheckDelay(prc_idx, seq_idx, 500);

					if (not_delay)
						SetSeqIdx(prc_idx, seq_idx, 180);
#endif
				}
					break;
				case 180:
					strText.Format(_T("Sequence Inspection Result Image Save Start"));
					WriteLog(strText);
					pDlgMain->SetInspImageSave();
					strText.Format(_T("Sequence Inspection Result Image Save End"));
					WriteLog(strText);
#ifdef HW_USE
					SetSeqIdx(prc_idx, seq_idx, 200);
#else
					SetSeqIdx(prc_idx, seq_idx, 190);
#endif
					break;
				case 190:
				{
#ifndef HW_USE
					//안정화시간
					not_delay = CheckDelay(prc_idx, seq_idx, 800);

					if (not_delay)
						SetSeqIdx(prc_idx, seq_idx, 200);
#endif
				}
					break;
				case 200:
				{
					SetSeqIdx(prc_idx, seq_idx, 120);

					if (pDlgMain->GetStopWait())
					{
						pDlgMain->ShowTextBox(_T(""), FALSE, TRUE);
						//Stop();
					}
				}
				break;
			}

			break;
	}

	if (chk_timeout)
		rslt = CheckTimeout(prc_idx, seq_idx, m_MDat[prc_idx].m_Seq[seq_idx].m_iSubSeqIdx, status_type);

	strText.Empty();
	return rslt;
}

void CSequenceModule::SequenceLoop()
{
	int rslt[MOT_PROCESS_MAX_NUM][MOT_STATUS_MAX_NUM] = { NULL, };
	int i, j, prc_idx, seq_idx;

	BOOL chk = FALSE;
	CString strText;

	while (m_Flag.m_bProcessThread)
	{
		//쓰레드 루틴이 돌 때마다 m_bAutoStart를 체크한다.
		chk = TRUE;

		for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
		{
			for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
			{
				if (!m_MDat[i].m_Seq[j].m_bAutoStart)
				{
					chk = FALSE;
				}
			}
		}

		if (chk)		//Auto mode
		{
			//Proccessing
			for (i = 0; i < MOT_PROCESS_MAX_NUM; i++)
			{
				for (j = 0; j < MOT_STATUS_MAX_NUM; j++)
				{
					rslt[i][j] = MotionProcess(i, j);
				
					if (1 == rslt[i][j])
					{
						//Auto status
					}
					else if (0 == rslt[i][j])
					{
						Stop();

						m_MDat[i].m_Seq[j].m_bAlarm = TRUE;
						m_Flag.m_bAlarm = TRUE; // Display Status 참조용

						//리셋시 다음번 동작예약
						m_MDat[i].m_Seq[j].m_iSubSeqIdx = m_MDat[i].m_Seq[j].m_iNextSubSeqIdx;

						//Error
						//SetLamp(COLOR_LAMP_RED);//, TRUE);

						strText.Format(_T("Alarm has occurred. (%S, %S)"), m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGCode, m_MDat[i].m_ProSts[j][m_MDat[i].m_Seq[j].m_iSubSeqIdx].m_cNGStatus);	//LOG
						WriteLog(strText);

						//알람처리
						::SendMessage(m_hWndMain, USE_MSG_ALARM_SEQUENCE_TO_MAIN, 0, 0);
						break;
					}
				}
			}
		}
		else
		{
			//Stop Mode
			if (1 == StopModeOperation(&prc_idx, &seq_idx))
			{

			}
			else
			{
				Stop();	//정지

				//Error
				m_MDat[prc_idx].m_Seq[seq_idx].m_bAlarm = TRUE;
				m_Flag.m_bAlarm = TRUE; // Display Status 참조용

				//SetLamp(COLOR_LAMP_RED);//, TRUE);

				strText.Format(_T("Alarm has occurred. (%S)"), m_MDat[0].m_ProSts[0][m_MDat[0].m_Seq[0].m_iSubSeqIdx].m_cNGStatus);	//LOG
				WriteLog(strText);

				//알람처리
				//::SendMessage(m_hWndMain, USE_MSG_MOTION_TO_MAIN_DIALOG, 0, 0);
			}
			Sleep(1);
		}

		Sleep(1);
	}

	strText.Empty();
}

void CSequenceModule::WriteLog(CString strLog)
{
	SYSTEMTIME	lpSystemTime;
	GetLocalTime(&lpSystemTime);
	CString strText;

	strText.Format(_T("%02d:%02d:%02d:%03d - %s"), lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds, strLog);

	::SendMessage(m_hWndMain, USE_MSG_LOG_TO_MAIN, (WPARAM)strText.GetBuffer(), strText.GetLength());
	//m_strMainLogString.push_back(strText);
}