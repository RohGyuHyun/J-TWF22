#include "pch.h"
#include "CInverterClass.h"
#include "CIOClass.h"
#ifdef INVERTER_COM
CInverterClass* m_This;
CInverterClass* CInverterClass::m_pInstance = NULL;
CInverterClass::CInverterClass()
{
	m_bReadCommandEnd = FALSE;
	m_bThreadEnd = FALSE;

	memset(m_bStatusInfo1, NULL, sizeof(m_bStatusInfo1));
	memset(m_bStatusInfo2, NULL, sizeof(m_bStatusInfo2));
	memset(m_bStatusInfo3, NULL, sizeof(m_bStatusInfo3));
	memset(m_bAlarmInfo, NULL, sizeof(m_bAlarmInfo));
	memset(m_bySendCommand, NULL, sizeof(m_bySendCommand));

	m_nDirection = 0;
	m_dSpeed = 0.;
	m_bRun = FALSE;
	m_bTimeOutError = FALSE;
	m_dInverterSpeed = 0.;
	m_bReConnect = FALSE;
}

CInverterClass::~CInverterClass()
{
	
}

void InverterCallBackEvent(BYTE* pData, int nDataCnt)
{
	m_This->SetReadCommand(pData, nDataCnt);
}

CInverterClass* CInverterClass::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = DBG_NEW CInverterClass();
	}
	return m_pInstance;
}

BOOL CInverterClass::SetInverterSpeed(double dVal)
{
	BOOL rslt = FALSE;
	//dVal = 40 m/min
	int nSpeedVal = 0;
	double dRPM = 1800 / 5.;//회전수/min
	double dMax1 = 55.;// ((50.1 * M_PI) * dRPM) / 1000.;//56.5486 m/min
	double dMax2 = (1800 * 4.) / 12.;
	double dMax3 = 60.;
	double dRet1 = dMax1 / dMax2;//0.091
	double dRet2 = dVal / dMax1;//0.2818
	double dTempVal1 = 1800. * dRet2;
	nSpeedVal = (int)(dTempVal1);// *100.);//507

	rslt = SetWriteCommand(INVERTER_WRITE_SPEED, nSpeedVal);

	return rslt;
}

BOOL CInverterClass::SetInverterMove(int nDirection, double dSpeed, BOOL isRun)
{
	m_nDirection = nDirection;
	m_dSpeed = dSpeed;
	m_bRun = isRun;
	BOOL rslt = FALSE;

	rslt = SetWriteCommand(INVERTER_WRITE_RUN, m_nDirection);
	
	if (isRun)
		rslt = SetInverterSpeed(dSpeed + 1.5);
	//else
		rslt = TRUE;


	return rslt;
}

BOOL CInverterClass::SetWriteCommand(INVERTER_WRITE_CMD write_cmd, int nWriteData)
{
	BOOL rslt = TRUE;
#ifdef _DEBUG
	return rslt;
#endif
	WRITE_DATA write_data;
	write_data.nDataLenght = 0;
	memset(write_data.byData, NULL, sizeof(write_data.byData));
	write_data.byData[write_data.nDataLenght++] = 0x2F;
	write_data.byData[write_data.nDataLenght++] = 0x50;
	CString strTemp;

	switch (write_cmd)
	{
	case INVERTER_WRITE_CMD::INVERTER_WRITE_SPEED://FA13
		strTemp.Format(_T("FA13"));
		write_data.byData[write_data.nDataLenght++] = 0xFA;
		write_data.byData[write_data.nDataLenght++] = 0x13;
		break;
	case INVERTER_WRITE_CMD::INVERTER_WRITE_RUN:
	case INVERTER_WRITE_CMD::INVERTER_WRITE_ALARM_RESET:
	case INVERTER_WRITE_CMD::INVERTER_WRITE_EMG_STOP://FA00
		write_data.byData[write_data.nDataLenght++] = 0xFA;
		write_data.byData[write_data.nDataLenght++] = 0x00;
		break;
	default:
		rslt = FALSE;
		break;
	}
	if (rslt)
	{
		int k = 0;
		for (int i = write_data.nDataLenght - 2; i < write_data.nDataLenght; i++)
		{
			m_bySendCommand[k++] = write_data.byData[i];
		}
		char* str = NULL;
		
		if (write_cmd == INVERTER_WRITE_CMD::INVERTER_WRITE_SPEED)
		{
			//Data Input
			int nCnt = 0;
			strTemp.Format(_T("%04X"), nWriteData);
			nCnt = strTemp.GetLength();

			write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 0, 2);
			write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 2, 2);
		}
		else
		{
			BOOL bCmdData[16];
			memset(bCmdData, NULL, sizeof(bCmdData));

			bCmdData[INVERTER_COMMUNICATION_CMD_1_PRESET_SPEED_FREQUENCIES_1];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_PRESET_SPEED_FREQUENCIES_2];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_PRESET_SPEED_FREQUENCIES_3];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_PRESET_SPEED_FREQUENCIES_4];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_MOTOR_SELECTION];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_PID_CONTROL];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_ACC_DEC_PATTERN_SELECTION];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_DC_BRAKING];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_JOG_RUN];
			
			bCmdData[INVERTER_COMMUNICATION_CMD_1_COAST_STOP];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_FAULT_RESET];
			bCmdData[INVERTER_COMMUNICATION_CMD_1_FREQUENCY_PRIORITY_SELECTION] = TRUE;
			bCmdData[INVERTER_COMMUNICATION_CMD_1_COMMAND_PRIORITY_SELECTION];// = TRUE;


			if (write_cmd == INVERTER_WRITE_CMD::INVERTER_WRITE_RUN)
			{
				bCmdData[INVERTER_COMMUNICATION_CMD_1_FORWARD_REVERSE_RUN] = nWriteData;
				bCmdData[INVERTER_COMMUNICATION_CMD_1_RUN_STOP] = m_bRun;
			}

			if (write_cmd == INVERTER_WRITE_CMD::INVERTER_WRITE_EMG_STOP)
				bCmdData[INVERTER_COMMUNICATION_CMD_1_EMERGENCY_STOP] = nWriteData;

			if(write_cmd == INVERTER_WRITE_CMD::INVERTER_WRITE_ALARM_RESET)
				bCmdData[INVERTER_COMMUNICATION_CMD_1_FAULT_RESET] = TRUE;

			BYTE byCmdData[4];
			memset(byCmdData, NULL, sizeof(byCmdData));
			strTemp.Format(_T("%02X"), BitToByte(&bCmdData[8]));
			strTemp.AppendFormat(_T("%02X"), BitToByte(bCmdData));
			
			write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 0, 2);
			write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 2, 2);
		}
		
		//sum
		int nCheckSumIdx = write_data.nDataLenght;
		long lCheckSum = 0;
		for (int i = 0; i < nCheckSumIdx; i++)
		{
			lCheckSum += write_data.byData[i];
		}
		strTemp.Format(_T("%04X"), lCheckSum);

		write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 2, 2);

		m_queWriteData.push_back(write_data);
	}

	return rslt;
}

BOOL CInverterClass::SetReadCommand(INVERTER_READ_CMD read_cmd)
{
	BOOL rslt = TRUE;
#ifdef _DEBUG
	return rslt;
#endif
	WRITE_DATA write_data;
	write_data.nDataLenght = 0;
	memset(write_data.byData, NULL, sizeof(BYTE) * 512);
	write_data.byData[write_data.nDataLenght++] = 0x2F;
	write_data.byData[write_data.nDataLenght++] = 0x52;

	switch (read_cmd)
	{
	case INVERTER_READ_CMD::INVERTER_READ_STATUS1:
		write_data.byData[write_data.nDataLenght++] = 0xFD;
		write_data.byData[write_data.nDataLenght++] = 0x01;
		break;
	case INVERTER_READ_CMD::INVERTER_READ_STATUS2:
		write_data.byData[write_data.nDataLenght++] = 0xFD;
		write_data.byData[write_data.nDataLenght++] = 0x42;
		break;
	case INVERTER_READ_CMD::INVERTER_READ_STATUS3:
		write_data.byData[write_data.nDataLenght++] = 0xFD;
		write_data.byData[write_data.nDataLenght++] = 0x49;
		break;
	case INVERTER_READ_CMD::INVERTER_READ_ALARM_INFO:
		write_data.byData[write_data.nDataLenght++] = 0xFC;
		write_data.byData[write_data.nDataLenght++] = 0x91;
		break;
	case INVERTER_READ_CMD::INVERTER_READ_MOTOR_SPEED:
		write_data.byData[write_data.nDataLenght++] = 0xFE;
		write_data.byData[write_data.nDataLenght++] = 0x00;
		break;
	default:
		rslt = FALSE;
		break;
	}
	
	if (rslt)
	{
		switch (read_cmd)
		{
		case INVERTER_READ_CMD::INVERTER_READ_STATUS1:

			break;
		case INVERTER_READ_CMD::INVERTER_READ_STATUS2:

			break;
		case INVERTER_READ_CMD::INVERTER_READ_STATUS3:

			break;
		case INVERTER_READ_CMD::INVERTER_READ_ALARM_INFO:

			break;
		case INVERTER_READ_CMD::INVERTER_READ_MOTOR_SPEED:

			break;
		}

		CString strTemp;

		//sum
		int nCheckSumIdx = write_data.nDataLenght;
		long lCheckSum = 0;
		for (int i = 0; i < nCheckSumIdx; i++)
		{
			lCheckSum += write_data.byData[i];
		}
		strTemp.Format(_T("%04X"), lCheckSum);

		write_data.byData[write_data.nDataLenght++] = TextToByte(strTemp, 2, 2);

		m_queWriteData.push_back(write_data);
	}
	return TRUE;
}

BOOL CInverterClass::WriteCommand(BYTE* byData, int nLenght)
{
#ifdef _DEBUG
	return TRUE;
#endif
	DWORD dRslt = m_Comport.WriteComm(byData, nLenght);
	if (dRslt != nLenght)
		return FALSE;

	return TRUE;
}

BOOL CInverterClass::ReadCommand(BYTE* byData, int nLenght)
{
	BOOL rslt = TRUE;

	if (rslt)
	{
		if (byData[1] == 0x4E || byData[1] == 0x6E)
		{
			AfxMessageBox(_T("Inverter Command Error"));
		}
		else
		{
			//4~5 Read Data

			//status1 read(FD01)
			if (byData[2] == 0xFD && byData[3] == 0x01)
			{
				BOOL bData[16] = { FALSE, };
				ByteToBit(byData[5], bData);
				ByteToBit(byData[4], &bData[8]);
				for (int i = 0; i < 16; i++)
					StatusInfoSet(0, i, bData[i]);
			}
			//status2 read(FD42)
			else if (byData[2] == 0xFD && byData[3] == 0x42)
			{
				BOOL bData[16] = { FALSE, };
				ByteToBit(byData[5], bData);
				ByteToBit(byData[4], &bData[8]);
				for (int i = 0; i < 16; i++)
					StatusInfoSet(1, i, bData[i]);
			}
			//status3 read(FD49)
			else if (byData[2] == 0xFD && byData[3] == 0x49)
			{
				BOOL bData[16] = { FALSE, };
				ByteToBit(byData[5], bData);
				ByteToBit(byData[4], &bData[8]);
				for (int i = 0; i < 16; i++)
					StatusInfoSet(2, i, bData[i]);
			}
			//alarm read(FC91)
			else if (byData[2] == 0xFC && byData[3] == 0x91)
			{
				BOOL bData[16] = { FALSE, };
				ByteToBit(byData[5], bData);
				ByteToBit(byData[4], &bData[8]);
				for (int i = 0; i < 16; i++)
					StatusInfoSet(3, i, bData[i]);
			}
			//speed read(FD90)
			else if (byData[2] == 0xFE && byData[3] == 0x00)
			{
				//16진수 -> 10진수 변환
				BYTE byRcvData[4] = { NULL, };
				CString strTemp;
				strTemp.Format(_T("%02x%02x"), byData[4], byData[5]);
				WideCharToMultiByteChar(strTemp, (char*)byRcvData);
				if (HexToDec(byRcvData, 4) > 0)
				{
					//실제 15m/min
					// 51.639
					//64564
					double dVal = ((double)HexToDec(byRcvData, 4));// *4.) / 120.;// hz 1690
					double dRPM = 1800 / 5.;//회전수/min
					double dMax1 = 55.;// ((50. * M_PI) * dRPM) / 1000.;//56.5486 m/min
					double dMax2 = (1800 * 4.) / 12.;
					double dRet1 = dVal / 6000.;//0.091
					double dRet2 = dVal / 1800; 
					double dTempVal1 = dRet1 * dMax1;
					m_dInverterSpeed = dTempVal1;
				}
				else
				{
					m_dInverterSpeed = 0;
				}
			}
		}
	}
	if(m_queWriteData.size() > 0)
		m_queWriteData.pop_front();

	m_bReadCommandEnd = TRUE;
	return TRUE;
}

BOOL CInverterClass::StatusInfoSet(int nStatusIndex, int nStatusSectionIdx, BOOL bData)
{
	BOOL bRslt = TRUE;
	switch (nStatusIndex)
	{
	case 0:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INFO_1_FALLUE_FL:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_FALLUE:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_ALARM:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_UNDER_VOLTAGE:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_MOTOR_SECTION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_PID_CONTROL_OFF:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_ACC_DEC_PATTERN_SELECTION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_DC_BRAKING:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_JOG_RUN:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_FORWARD_REVERSE_RUN:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_RUN_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_COAST_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_EMERGENCY_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_STANDBY_ST:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_STANDBY:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			m_bStatusInfo1[nStatusSectionIdx] = bData;
		}
	}
		break;
	case 1:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INFO_2_MAX_DEC_FORCED_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_ACC_DEC_PATTERN_SELECTION_1:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_ACC_DEC_PATTERN_SELECTION_2:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_OC_STALL_LEVEL:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			m_bStatusInfo2[nStatusSectionIdx] = bData;
		}
	}
		break;
	case 2:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INF_3_RY_TERMINAL_OUTPUT_HOLD:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_OUT_TERMINAL_OUTPUT_HOLD:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_CONST:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_HEALTHY_SIGNAL:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_ACC_DEC_COMPLETION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_SPECIFIED_SPEED_REACH:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_ACC:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_DEC:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			m_bStatusInfo3[nStatusSectionIdx] = bData;
		}
	}
		break;
	case 3:
	{
		m_bAlarmInfo[nStatusSectionIdx] = bData;
	}
		break;
	default:
		bRslt = FALSE;
		break;
	}

	return bRslt;
}

BOOL CInverterClass::StatusInfoGet(int nStatusIndex, int nStatusSectionIdx, BOOL* bData)
{
	BOOL bRslt = TRUE;
	switch (nStatusIndex)
	{
	case 0:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INFO_1_FALLUE_FL:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_FALLUE:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_ALARM:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_UNDER_VOLTAGE:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_MOTOR_SECTION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_PID_CONTROL_OFF:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_ACC_DEC_PATTERN_SELECTION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_DC_BRAKING:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_JOG_RUN:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_FORWARD_REVERSE_RUN:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_RUN_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_COAST_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_EMERGENCY_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_STANDBY_ST:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_1_STANDBY:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			*bData = m_bStatusInfo1[nStatusSectionIdx];
		}
	}
	break;
	case 1:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INFO_2_MAX_DEC_FORCED_STOP:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_ACC_DEC_PATTERN_SELECTION_1:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_ACC_DEC_PATTERN_SELECTION_2:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INFO_2_OC_STALL_LEVEL:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			*bData = m_bStatusInfo2[nStatusSectionIdx];
		}
	}
	break;
	case 2:
	{
		bRslt = FALSE;
		switch (nStatusSectionIdx)
		{
		case INVERTER_STATUS_INF_3_RY_TERMINAL_OUTPUT_HOLD:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_OUT_TERMINAL_OUTPUT_HOLD:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_CONST:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_HEALTHY_SIGNAL:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_ACC_DEC_COMPLETION:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_SPECIFIED_SPEED_REACH:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_ACC:
			bRslt = TRUE;
			break;
		case INVERTER_STATUS_INF_3_RUNNING_DEC:
			bRslt = TRUE;
			break;
		}

		if (bRslt)
		{
			*bData = m_bStatusInfo3[nStatusSectionIdx];
		}
	}
	break;
	case 3:
	{
		*bData = m_bAlarmInfo[nStatusSectionIdx];
	}
	break;
	default:
		bRslt = FALSE;
		break;
	}

	return bRslt;
}

BOOL CInverterClass::GetInverterRunning()
{
	BOOL bRslt;
	if(m_nDirection)
		bRslt = m_bStatusInfo3[INVERTER_STATUS_INF_3_RUNNING_ACC];
	else
		bRslt = m_bStatusInfo3[INVERTER_STATUS_INF_3_RUNNING_DEC];

	return bRslt;
}

BOOL CInverterClass::GetInverterRun()
{
	return m_bStatusInfo1[INVERTER_STATUS_INFO_1_RUN_STOP];
}

BOOL CInverterClass::GetInverterAlarm()
{
	BOOL bRslt = FALSE;
	for (int i = 0; i < 16; i++)
	{
		if (m_bAlarmInfo[i])
		{
			bRslt = TRUE;
			break;
		}
	}

	return bRslt;
}

BOOL CInverterClass::SetInverterAlarmReset()
{
	return SetWriteCommand(INVERTER_WRITE_ALARM_RESET);
}

BOOL CInverterClass::SetInverterEMGStop(BOOL isStop)
{
	return SetWriteCommand(INVERTER_WRITE_EMG_STOP, isStop);
}

void CInverterClass::OpenPort(CString strPortName, DWORD dwBaud, int nReadInterval)
{
	m_This = this;
	m_Comport.SetCallBack(InverterCallBackEvent);
	m_Comport.m_b485Com = TRUE;
	//m_Comport.Open(_wtoi(strPortName.Mid(3, strPortName.GetLength() - 3)), dwBaud);
	m_Comport.OpenPort(NULL, strPortName, dwBaud, _wtoi(strPortName.Right(1)),2 , 0);
	m_strPortName.Format(_T("%s"), strPortName);
	m_dwBaud = dwBaud;
	//m_Comport.OpenPort(_T("\\\\.\\") + strPortName, dwBaud);//
	m_TimeOutCheck.SetCheckTime(INVERTER_WRITE_MS);
	//m_TimeOutCheck.StartTimer();
	m_WriteCommand = INVERTER_WRITE_CMD::NON_WRITE_CMD;
	m_ReadCommand = INVERTER_READ_CMD::NON_READ_CMD;
	m_bReadCommandEnd = TRUE;
	m_bThreadEnd = FALSE;
	m_pWriteThread = AfxBeginThread(ThreadWritePolling, this, THREAD_PRIORITY_NORMAL);
	SetThreadAffinityMask(m_pWriteThread, 1);
	if (m_pWriteThread != NULL)
	{
		m_pWriteThread->m_bAutoDelete = FALSE;
		m_pWriteThread->ResumeThread();
	}
}

void CInverterClass::Close()
{
	//m_Comport.Close();
	m_queWriteData.clear();
	m_bThreadEnd = TRUE;
	m_bReadCommandEnd = FALSE;
	BOOL rslt = TRUE;
	while (TRUE)
	{
		if (m_pWriteThread != NULL)
		{
			rslt = TRUE;
			DWORD dwRet = WaitForSingleObject(m_pWriteThread->m_hThread, 1000);

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
			if (m_pWriteThread)
				delete m_pWriteThread;

			m_pWriteThread = NULL;
			break;
		}
	}

	m_Comport.m_bThreadEnd = TRUE;
	m_Comport.ClosePort();
}

UINT CInverterClass::ThreadWritePolling(LPVOID pParam)
{
	CInverterClass* pdlg = (CInverterClass*)pParam;

	pdlg->WritePollingThread();

	return 0;
}

void CInverterClass::WritePollingThread()
{
	long lStart = (long)GetTickCount64();
	long lEnd = (long)GetTickCount64();
	double dTackTime = (double)(lEnd - lStart) / 1000.;
	while (TRUE)
	{
		if (m_bThreadEnd)
			break;

		if (m_Comport.m_bPortOpen)
		{
			if (m_queWriteData.size() > 0)
			{
				if (m_bReadCommandEnd)
				{
					if (WriteCommand(m_queWriteData[0].byData, m_queWriteData[0].nDataLenght))
					{
						m_bReadCommandEnd = FALSE;
						//m_TimeOutCheck.StartTimer();
						lStart = (long)GetTickCount64();
						m_bTimeOutError = FALSE;
					}
				}
				else
				{
					lEnd = (long)GetTickCount64();
					dTackTime = (double)(lEnd - lStart);
					//if (m_TimeOutCheck.IsTimeOver())
					if(dTackTime > INVERTER_WRITE_MS)
					{
						//m_Comport.m_bPortOpen = FALSE;
						//m_bTimeOutError = TRUE;
						//m_queWriteData.clear();
					}
				}
			}
			
			if (m_bThreadEnd)
				break;
		}

		if(m_bReConnect)
		{
			m_bReConnect = FALSE;
			if (m_bThreadEnd)
				break;

			m_Comport.ClosePort();
			//CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MC01_ON, FALSE);
			Delay(5000);
			if (m_bThreadEnd)
				break;
			CIOClass::Instance()->SetOutputValue(OUTPUT_INDEX_MC01_ON, TRUE);
			Delay(5000);
			m_Comport.OpenPort(NULL, m_strPortName, m_dwBaud, _wtoi(m_strPortName.Right(1)));
			m_WriteCommand = INVERTER_WRITE_CMD::NON_WRITE_CMD;
			m_ReadCommand = INVERTER_READ_CMD::NON_READ_CMD;
			m_queWriteData.clear();
			m_bReadCommandEnd = TRUE;
			//Delay(100);
		}

		if (m_bThreadEnd)
			break;

		Sleep(1);
	}
}
#endif