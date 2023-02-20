#include "pch.h"
#include "CIOClass.h"
#include "uniioport.h"

CIOClass* CIOClass::m_pInstance = NULL;
CIOClass::CIOClass()
{
	int k = CIOPort::Instance(_T("XMC"))->Init();

	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		m_stInput[i].nIONum = -1;
		memset(m_stInput[i].strIOLabel, NULL, sizeof(wchar_t) * MAX_CHAR_LENG);
	}

	for (int i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		m_stOutput[i].nIONum = -1;
		memset(m_stOutput[i].strIOLabel, NULL, sizeof(wchar_t) * MAX_CHAR_LENG);
	}
}

CIOClass::~CIOClass()
{
	CIOPort::Instance()->Close();
}

CIOClass* CIOClass::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = DBG_NEW CIOClass;
	}
	return m_pInstance;
}

BOOL CIOClass::GetInputBit(int nIdx)
{
	return CIOPort::Instance()->GetBit(nIdx);
}

BOOL CIOClass::GetOutputBit(int nIdx)
{
	return CIOPort::Instance()->GetOutBit(nIdx);
}

void CIOClass::SetOutputBit(int nIdx, BOOL isOnOff)
{
	CIOPort::Instance()->SetBit(nIdx, isOnOff);
}

void CIOClass::SetIOPara(IO_PARA* Input, IO_PARA* Output)
{
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		m_stInput[i] = Input[i];
	}

	for (int i = 0; i < MAX_OUTPUT_NUM; i++)
	{
		m_stOutput[i] = Output[i];
	}
}

BOOL CIOClass::GetInputValue(Input_Enum nIdx)
{
	CString strLabel;
	int nRslt = -1;
	BOOL isRslt = TRUE;
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		strLabel.Format(_T("%s"), m_stInput[i].strIOLabel);
		if (!strLabel.Compare(INPUT_EMG) && nIdx == INPUT_INDEX_EMG)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
#ifdef CE_CONFIRM
			if (nRslt == 0)
				nRslt = 1;
			else if (nRslt == 1)
				nRslt = 0;
#endif
			break;
		}
		else if (!strLabel.Compare(INPUT_KEY_SW) && nIdx == INPUT_INDEX_KEY_SW)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
#ifdef CE_CONFIRM
			if (nRslt == 0)
				nRslt = 1;
			else if (nRslt == 1)
				nRslt = 0;
#endif
			break;
		}
#ifndef CE_CONFIRM
		else if (!strLabel.Compare(INPUT_DOOR) && nIdx == INPUT_INDEX_DOOR)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
#else
		else if (!strLabel.Compare(INPUT_DOOR1) && nIdx == INPUT_INDEX_DOOR1)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
		else if (!strLabel.Compare(INPUT_DOOR2) && nIdx == INPUT_INDEX_DOOR2)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
		else if (!strLabel.Compare(INPUT_DOOR3) && nIdx == INPUT_INDEX_DOOR3)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
	}
#endif
#ifndef INVERTER_COM
		else if (!strLabel.Compare(INPUT_INVERTER_ALARM) && nIdx == INPUT_INDEX_INVERTER_ALARM)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
#endif
#ifndef CE_CONFIRM
		else if (!strLabel.Compare(INPUT_TUBE_ALARM) && nIdx == INPUT_INDEX_TUBE_ALARM)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
#endif
		else if(!strLabel.Compare(INPUT_X_RAY_LAMP_ALARM) && nIdx == INPUT_INDEX_X_RAY_LAMP_ALM)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
#ifdef INVERTER_COM
		else if (!strLabel.Compare(INPUT_INPUT_SENSER) && nIdx == INPUT_INDEX_INPUT_SENSER)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
		else if (!strLabel.Compare(INPUT_OUTPUT_SENSER) && nIdx == INPUT_INDEX_OUTPUT_SENSER)
		{
			nRslt = GetInputBit(m_stInput[i].nIONum);
			break;
		}
#endif
	}

	if(nRslt != -1)
		isRslt = nRslt;

	strLabel.Empty();

	return isRslt;
}

int CIOClass::SetOutputValue(Output_Enum nIdx, BOOL isOnOff)
{
	CString strLabel;
	int nRslt = 0;
	BOOL isRslt = TRUE;
	for (int i = 0; i < MAX_INPUT_NUM; i++)
	{
		strLabel.Format(_T("%s"), m_stOutput[i].strIOLabel);
		if (!strLabel.Compare(OUTPUT_TOWER_LAMP_RED) && nIdx == OUTPUT_INDEX_TOWER_LAMP_RED)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
		else if (!strLabel.Compare(OUTPUT_TOWER_LAMP_YELLOW) && nIdx == OUTPUT_INDEX_TOWER_LAMP_YELLOW)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
		else if (!strLabel.Compare(OUTPUT_TOWER_LAMP_GREEN) && nIdx == OUTPUT_INDEX_TOWER_LAMP_GREEN)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
		else if (!strLabel.Compare(OUTPUT_TOWER_BUZZER1) && nIdx == OUTPUT_INDEX_TOWER_BUZZER1)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
#ifndef INVERTER_COM
		else if (!strLabel.Compare(OUTPUT_TOWER_BUZZER2) && nIdx == OUTPUT_INDEX_TOWER_BUZZER2)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
#endif
		else if (!strLabel.Compare(OUTPUT_MC01_ON) && nIdx == OUTPUT_INDEX_MC01_ON)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
		else if (!strLabel.Compare(OUTPUT_X_RAY_O) && nIdx == OUTPUT_INDEX_X_RAY_ON)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
#ifndef INVERTER_COM
		else if (!strLabel.Compare(OUTPUT_MOTOR_CW) && nIdx == OUTPUT_INDEX_MOTOR_CW)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
		else if (!strLabel.Compare(OUTPUT_MOTOR_CCW) && nIdx == OUTPUT_INDEX_MOTOR_CCW)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
#endif
		else if(!strLabel.Compare(OUTPUT_REJECTER) && nIdx == OUTPUT_INDEX_REJECTER_SOL)
		{
			SetOutputBit(m_stOutput[i].nIONum, isOnOff);
		}
	}

	strLabel.Empty();

	return nRslt;
}