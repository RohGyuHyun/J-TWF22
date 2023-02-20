#include "pch.h"
#include "CXRayClass.h"

CXRayClass* m_This;
CXRayClass* CXRayClass::m_pInstance = NULL;
CXRayClass::CXRayClass()
{
	m_nPollingInterval = 100;
	m_bThreadEnd = FALSE;
	m_SendCommand = NON;
	memset(&m_XrayInfo, NULL, sizeof(X_RAY_INFO));
	m_nSendDataValue = -1;
	m_bWarmUpState = FALSE;
	m_nSendCmdIndex = 0;
	m_SendCommand = NON;
	m_pStatusPollingThread = NULL;
}

CXRayClass::~CXRayClass()
{
	
}

CXRayClass* CXRayClass::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = DBG_NEW CXRayClass();
	}
	return m_pInstance;
}

void XrayCallBackEvent(BYTE* pData, int nDataCnt)
{	
	m_This->SetReadCommand(pData, nDataCnt);
}

UINT CXRayClass::ThreadStatusPolling(LPVOID pParam)
{
	CXRayClass* pdlg = (CXRayClass*)pParam;

	pdlg->StatusPollingThread();

	return 0;
}

void CXRayClass::StatusPollingThread()
{

	while (TRUE)
	{
		if (m_bThreadEnd)
			break;

		//if (m_Comport.IsOpen())
		if(m_Comport.m_bPortOpen)
		{
#ifdef _DEBUG
			continue;
#endif
			if (m_nSendCmdIndex > 4)
				m_nSendCmdIndex = 0;

			if (m_SendCommand == NON && m_nSendCmdIndex == -1)
			{
				m_nSendCmdIndex--;
				SendCommand(REQUEST_VOLTAGE_SCALING, -1);
			}

			if (m_SendCommand == NON && m_nSendCmdIndex == -2)
			{
				m_nSendCmdIndex = 0;
				SendCommand(REQUEST_CURRENT_SCALING, -1);
			}

			if (m_SendCommand == NON && m_nSendCmdIndex == 0)
			{
				m_nSendCmdIndex++;
				SendCommand(REQUEST_FAULTS, -1);
			}
	
			if (m_SendCommand == NON && m_nSendCmdIndex == 1)
			{
				m_nSendCmdIndex++;
				SendCommand(REQUEST_KV_MONITOR, -1);
			}
			//Sleep(10);
			if (m_SendCommand == NON && m_nSendCmdIndex == 2)
			{
				m_nSendCmdIndex++;
				SendCommand(REQUEST_MA_MONITOR, -1);
			}
			//Sleep(10);
			if (m_SendCommand == NON && m_nSendCmdIndex == 3)
			{
				m_nSendCmdIndex++;
				SendCommand(REQUEST_TANK_TEMPERATURE, -1);
			}
			//Sleep(10);
			if (m_SendCommand == NON && m_nSendCmdIndex == 4)
			{
				m_nSendCmdIndex++;
				SendCommand(READ_X_RAY_STATUS, -1);
			}
			//Sleep(10);

			if (m_TimeOutCheck.IsTimeOver())
			{
				m_Comport.m_bPortOpen = FALSE;
			}
			else
			{
				/*Delay(10);
				BYTE* byData;
				byData = new BYTE[2048];
				DWORD dwLen = m_Comport.ReadComm(byData, 2048);
				if(dwLen > 0)
					SetReadCommand(byData, dwLen);
				delete[] byData;*/
			}
			if (m_bThreadEnd)
				break;
		}
		else
		{
			if (m_bThreadEnd)
				break;

			m_Comport.ClosePort();
			m_Comport.OpenPort(NULL, m_strPortName, m_dwBaud, _wtoi(m_strPortName.Right(1)));
			m_SendCommand = NON;
			m_nSendCmdIndex = -1;
		}

		Sleep(1);
	}
}

void CXRayClass::OpenPort(CString strPortName, DWORD dwBaud, int nReadInterval)
{
	m_This = this;
	m_Comport.SetCallBack(XrayCallBackEvent);
	//m_Comport.Open(_wtoi(strPortName.Mid(3, strPortName.GetLength() - 3)), dwBaud);
	m_Comport.OpenPort(NULL, strPortName, dwBaud, _wtoi(strPortName.Right(1)));
	m_strPortName.Format(_T("%s"), strPortName);
	m_dwBaud = dwBaud;
	//m_Comport.OpenPort(_T("\\\\.\\") + strPortName, dwBaud);//
	m_TimeOutCheck.SetCheckTime(3000);
	m_TimeOutCheck.StartTimer();
	m_nSendCmdIndex = -1;
	m_nPollingInterval = nReadInterval;
	m_pStatusPollingThread = AfxBeginThread(ThreadStatusPolling, this, THREAD_PRIORITY_NORMAL);
	SetThreadAffinityMask(m_pStatusPollingThread, 1);
	if (m_pStatusPollingThread != NULL)
	{
		m_pStatusPollingThread->m_bAutoDelete = FALSE;
		m_pStatusPollingThread->ResumeThread();
	}
}

void CXRayClass::Close()
{
	m_bThreadEnd = TRUE;

	BOOL rslt = TRUE;
	while (TRUE)
	{
		if (m_pStatusPollingThread != NULL)
		{
			rslt = TRUE;
			DWORD dwRet = WaitForSingleObject(m_pStatusPollingThread->m_hThread, 1000);

			if (dwRet == WAIT_FAILED)
			{
				rslt = FALSE;
			}
			else if (dwRet == WAIT_ABANDONED)
			{
				rslt = FALSE;
			}
			else if (dwRet == WAIT_TIMEOUT)
			{
				rslt = FALSE;
			}
		}

		if (rslt)
		{
			if (m_pStatusPollingThread)
				delete m_pStatusPollingThread;

			m_pStatusPollingThread = NULL;
			break;
		}
	}

	m_Comport.m_bThreadEnd = TRUE;
	m_Comport.ClosePort();
}

BYTE CXRayClass::Checksums(BYTE* pData, int nCnt)
{
	BYTE byResult = 0x00, byTemp1 = 0x00;
	WORD dSum = 0, sb1, sb2;

	for (int i = 1; i < nCnt; i++)
		dSum += pData[i];

	sb1 = 4095 ^ dSum;
	sb2 = sb1 + 1;

	byTemp1 = (BYTE)sb2;

	byTemp1 = byTemp1 & 0x7F;
	byResult = byTemp1 | 0x40;

	return byResult;
}

BOOL CXRayClass::SetReadCommand(BYTE* pData, int nCnt)
{
	ReadCommand(pData, nCnt);

	return TRUE;
}

int CXRayClass::ReadData(BYTE* pData, void* nReadData, int nCnt, int nReadType)
{
	int nReturn = 0;
	CString strData, strReqData;
	int nCheckSumIdx;
	BYTE byChecksum;
	int nReqData;
	double dReqData;
	BOOL bReqData[8];
	char cReqData[128];

	for (int i = 0; i < nCnt; i++)
		strData.AppendFormat(_T("%c"), pData[i]);

	nCheckSumIdx = strData.Find(_T(";")) + 1;
	byChecksum = pData[nCheckSumIdx];
	strReqData.Format(_T("%s"), strData.Mid(1, nCheckSumIdx - 2));
	switch (nReadType)
	{
	case 0://Int
		nReqData = _wtoi(strReqData);
		memcpy(nReadData, &nReqData, sizeof(int));
		break;
	case 1://Double
		dReqData = _wtof(strReqData);
		memcpy(nReadData, &dReqData, sizeof(double));
		break;
	case 2://BOOL Array
		for (int i = 0; i < 8; i++)
		{
			bReqData[i] = _wtoi(strReqData.Mid(i, 1));// << i;
		}
		memcpy(nReadData, bReqData, sizeof(BOOL) * 8);
		break;
	case 3://string
		sprintf_s(cReqData, strData.Mid(1, nCheckSumIdx - 2).GetLength() + 1, "%S", strData.Mid(1, nCheckSumIdx - 2).GetBuffer());
		nReturn = strData.Mid(1, nCheckSumIdx - 2).GetLength() + 1;
		memcpy(nReadData, cReqData, sizeof(char) * nReturn);
		break;
	}

	return nReturn;
}

BOOL CXRayClass::ReadCommand(BYTE* pData, int nCnt)
{
	//m_SendCommand = REQUEST_VOLTAGE_SCALING;
	BOOL rslt = TRUE;
	BOOL bNoData = FALSE;
	CString strData;

	int nReqData = 0;
	double dReqData = 0.;
	BOOL bReqData[8] = { FALSE, };
	char cReqData[128];
	int z = 0;
	int nStringCnt = 0;

	for (int i = 0; i < nCnt; i++)
		strData.AppendFormat(_T("%c"), pData[i]);

	//strData.Format(_T("%S"), pData);
	if(m_SendCommand == PROGRAM_KV)
		bNoData = TRUE;
	else if(m_SendCommand == PROGRAM_MA)
		bNoData = TRUE;
	else if (m_SendCommand == REQUEST_KV_SEPOINT)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.nNoScalingSetKV = nReqData;
	}
	else if (m_SendCommand == REQUEST_MA_SEPOINT)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.nNoScalingSetMA = nReqData;
	}
	else if (m_SendCommand == REQUEST_KV_MONITOR)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.nNoScalingCurKV = nReqData;
	}
	else if (m_SendCommand == REQUEST_MA_MONITOR)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.nNoScalingCurMA = nReqData;
	}
	else if (m_SendCommand == REQUEST_FILAMENT_MONITOR)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.nFilament = nReqData;
	}
	else if (m_SendCommand == TURN_X_RAY_ON_OFF)
		bNoData = TRUE;
	else if (m_SendCommand == ENABLE_COMM_WATCHDOG)
		bNoData = TRUE;
	else if (m_SendCommand == TICKLE_COMM_WATCHDOG)
		bNoData = TRUE;
	else if (m_SendCommand == RESET_FAULTS)
		bNoData = TRUE;
	else if (m_SendCommand == REQUEST_FAULTS)
	{
		ReadData(pData, &bReqData, nCnt, 2);
		m_XrayInfo.stXray_status.bArc = bReqData[z++];
		m_XrayInfo.stXray_status.bOverTemperature = bReqData[z++];
		m_XrayInfo.stXray_status.bOverVoltage = bReqData[z++];
		m_XrayInfo.stXray_status.bUnderVoltage = bReqData[z++];
		m_XrayInfo.stXray_status.bOverCurrent = bReqData[z++];
		m_XrayInfo.stXray_status.bUnderCurrent = bReqData[z++];
		m_XrayInfo.stXray_status.bWatchdogTimeOut = bReqData[z++];
		m_XrayInfo.stXray_status.bOpenInterlock = bReqData[z++];
	}
	else if (m_SendCommand == READ_X_RAY_STATUS)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.isOnOff = nReqData;
	}
	else if (m_SendCommand == REQUEST_SOFTWARE_VERSION)
	{
		nStringCnt = ReadData(pData, &cReqData, nCnt, 3);
		memcpy(m_XrayInfo.strSWVersion, cReqData, sizeof(char) * nStringCnt);
	}
	else if (m_SendCommand == REQUEST_VOLTAGE_SCALING)
	{
		ReadData(pData, &dReqData, nCnt, 1);
		m_XrayInfo.dScalingCurKV = (dReqData / 4095) / 100.;
	}
	else if (m_SendCommand == REQUEST_CURRENT_SCALING)
	{
		ReadData(pData, &dReqData, nCnt, 1);
		m_XrayInfo.dScalingCurMA = (dReqData / 4095) / 1000.;
	}
	else if (m_SendCommand == REQUEST_MODEL_NUMBER)
	{
		nStringCnt = ReadData(pData, &cReqData, nCnt, 3);
		memcpy(m_XrayInfo.strModelNumber, cReqData, sizeof(char) * nStringCnt);
	}
	else if (m_SendCommand == REQUEST_HARDWARE_VERSION)
	{
		nStringCnt = ReadData(pData, &cReqData, nCnt, 3);
		memcpy(m_XrayInfo.strHWVersion, cReqData, sizeof(char) * nStringCnt);
	}
	else if (m_SendCommand == REQUEST_SOFTWARE_BULID_VERSION)
	{
		nStringCnt = ReadData(pData, &cReqData, nCnt, 3);
		memcpy(m_XrayInfo.strSWBuildVersion, cReqData, sizeof(char) * nStringCnt);
	}
	else if (m_SendCommand == REQUEST_15V_LVPS)
		ReadData(pData, &nReqData, nCnt, 0);
	else if (m_SendCommand == REQUEST_TANK_TEMPERATURE)
	{
		ReadData(pData, &nReqData, nCnt, 0);
		m_XrayInfo.dTemp = (double)nReqData * (70.036 / 956);
	}
	else if (m_SendCommand == PROGRAM_BAUD_RATE)
		bNoData = TRUE;
	
	m_SendCommand = NON;

	return rslt;
}

BOOL CXRayClass::SendCommand(X_RAY_CMD cmd, int nData)
{
#ifdef _DEBUG
	return TRUE;
#endif
	BOOL rslt = TRUE;
	BYTE pData[128] = {0x00,};
	char pData2[128] = { 0x00, };
	pData[0] = 0x02;
	int nIdx = 1;
	CString strData;
	m_SendCommand = cmd;
	switch (cmd)
	{
		case PROGRAM_KV:
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'R';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'F';
			pData[nIdx++] = 0x20;
			break;
		case PROGRAM_MA:
			pData[nIdx++] = 'I';
			pData[nIdx++] = 'R';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'F';
			pData[nIdx++] = 0x20;
			break;
		case REQUEST_KV_SEPOINT:
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_MA_SEPOINT:
			pData[nIdx++] = 'I';
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_KV_MONITOR:
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'M';
			pData[nIdx++] = 'O';
			pData[nIdx++] = 'N';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_MA_MONITOR:
			pData[nIdx++] = 'I';
			pData[nIdx++] = 'M';
			pData[nIdx++] = 'O';
			pData[nIdx++] = 'N';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_FILAMENT_MONITOR:
			pData[nIdx++] = 'F';
			pData[nIdx++] = 'M';
			pData[nIdx++] = 'O';
			pData[nIdx++] = 'N';
			//pData[nIdx++] = 0x20;
			break;
		case TURN_X_RAY_ON_OFF:
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'N';
			pData[nIdx++] = 'B';
			pData[nIdx++] = 'L';
			pData[nIdx++] = 0x20;
			break;
		case ENABLE_COMM_WATCHDOG:
			pData[nIdx++] = 'W';
			pData[nIdx++] = 'D';
			pData[nIdx++] = 'T';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 0x20;
			break;
		case TICKLE_COMM_WATCHDOG:
			pData[nIdx++] = 'W';
			pData[nIdx++] = 'D';
			pData[nIdx++] = 'T';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case RESET_FAULTS:
			pData[nIdx++] = 'C';
			pData[nIdx++] = 'L';
			pData[nIdx++] = 'R';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_FAULTS:
			pData[nIdx++] = 'F';
			pData[nIdx++] = 'L';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case READ_X_RAY_STATUS:
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'T';
			pData[nIdx++] = 'A';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_SOFTWARE_VERSION:
			pData[nIdx++] = 'F';
			pData[nIdx++] = 'R';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'V';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_VOLTAGE_SCALING:
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'L';
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'R';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_CURRENT_SCALING:
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'L';
			pData[nIdx++] = 'I';
			pData[nIdx++] = 'R';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_MODEL_NUMBER:
			pData[nIdx++] = 'M';
			pData[nIdx++] = 'O';
			pData[nIdx++] = 'D';
			pData[nIdx++] = 'R';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_HARDWARE_VERSION:
			pData[nIdx++] = 'H';
			pData[nIdx++] = 'W';
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'R';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_SOFTWARE_BULID_VERSION:
			pData[nIdx++] = 'S';
			pData[nIdx++] = 'O';
			pData[nIdx++] = 'F';
			pData[nIdx++] = 'T';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_15V_LVPS:
			pData[nIdx++] = 'L';
			pData[nIdx++] = 'V';
			pData[nIdx++] = 'P';
			pData[nIdx++] = 'S';
			//pData[nIdx++] = 0x20;
			break;
		case REQUEST_TANK_TEMPERATURE:
			pData[nIdx++] = 'T';
			pData[nIdx++] = 'E';
			pData[nIdx++] = 'M';
			pData[nIdx++] = 'P';
			//pData[nIdx++] = 0x20;
			break;
		case PROGRAM_BAUD_RATE:
			pData[nIdx++] = 'B';
			pData[nIdx++] = 'A';
			pData[nIdx++] = 'U';
			pData[nIdx++] = 'D';
			pData[nIdx++] = 0x20;
			break;
		default:
			rslt = FALSE;
			break;
	}
	
	if (rslt)
	{
		if (nData >= 0)
		{
			if (nData > 4095)
				nData = 4095;

			if (nData < 0)
				nData = 0;

			strData.Format(_T("%d"), nData);
			sprintf_s(pData2, strData.GetLength() + 1, "%S", strData.GetBuffer());

			for (int i = 0; i < strData.GetLength(); i++)
				pData[nIdx++] = pData2[i];
		}


		pData[nIdx++] = 0x3B;

		pData[nIdx++] = Checksums(pData, nIdx);

		pData[nIdx++] = 0x0D;
		pData[nIdx++] = 0x0A;
		pData[100] = '_';
		DWORD dRet = m_Comport.WriteComm(pData, nIdx);

		if (dRet != 0)
		{
			//m_TimeOutCheck.SetCheckTime(1000);
			m_TimeOutCheck.StartTimer();
		}
	}

	return rslt;
}

BOOL CXRayClass::SetXrayOnOff(BOOL isOnOff)
{
	BOOL rslt = FALSE;

	rslt = SendCommand(TURN_X_RAY_ON_OFF, isOnOff);

	return rslt;
}

BOOL CXRayClass::SetXrayVolCur(double dVol, double dCur)
{
	BOOL rslt = FALSE;
	int nVol = 0, nCur = 0;
	if(dVol > 0.)
		nVol = (int)(dVol / m_XrayInfo.dScalingCurKV);

	if(dCur > 0.)
		nCur = (int)(dCur / m_XrayInfo.dScalingCurMA);

	if ((0 <= nVol && nVol < 4096) && (0 <= nCur && nCur < 4096))
		rslt = TRUE;

	if(rslt)
		rslt = SendCommand(PROGRAM_KV, nVol);
	if (rslt)
		rslt = SendCommand(PROGRAM_MA, nCur);

#ifndef HW_USE
	m_XrayInfo.dScalingCurKV = dVol;
	m_XrayInfo.dScalingCurMA = dCur;
#endif

	return rslt;
}

BOOL CXRayClass::SetXrayRestFault()
{
	BOOL rslt = FALSE;

	rslt = SendCommand(RESET_FAULTS);

	return rslt;
}

BOOL CXRayClass::GetXrayWarmupCheck()
{
	BOOL rslt = FALSE;

	CTime cTotalTime, cStartTime;
	cStartTime = CTime::GetCurrentTime();
	//cTotalTime = (cStartTime - cXrayEndTime);

	time_t start, end;
	struct tm user_stime;
	int tm_day, tm_hour, tm_min, tm_sec;
	double diff;

	user_stime.tm_year = m_XrayEndTime.nYear - 1900; // 년도가 1900년부터 시작하기 때문
	user_stime.tm_mon = m_XrayEndTime.nMonth - 1; //월이 0부터 시작하기 때문
	user_stime.tm_mday = m_XrayEndTime.nDay;
	user_stime.tm_hour = m_XrayEndTime.nHour;
	user_stime.tm_min = m_XrayEndTime.nMinute;
	user_stime.tm_sec = m_XrayEndTime.nSec;
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

	if(tm_day > 0)
	{
		rslt = TRUE;
	}


	return rslt;
}

#ifndef HW_USE
void CXRayClass::SetTestXRayStatus(int nIdx, BOOL bVal)
{
	switch (nIdx)
	{
	case 0:
		m_XrayInfo.stXray_status.bArc = bVal;
		break;
	case 1:
		m_XrayInfo.stXray_status.bOverTemperature = bVal;
		break;
	case 2:
		m_XrayInfo.stXray_status.bOverVoltage = bVal;
		break;
	case 3:
		m_XrayInfo.stXray_status.bUnderVoltage = bVal;
		break;
	case 4:
		m_XrayInfo.stXray_status.bOverCurrent = bVal;
		break;
	case 5:
		m_XrayInfo.stXray_status.bUnderCurrent = bVal;
		break;
	case 6:
		m_XrayInfo.stXray_status.bWatchdogTimeOut = bVal;
		break;
	case 7:
		m_XrayInfo.stXray_status.bOpenInterlock = bVal;
		break;
	}	
}
#endif