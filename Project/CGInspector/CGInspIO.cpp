#include "stdafx.h"
#include "Defines.h"
#include "CGInspIO.h"

CCGInspIO * CCGInspIO::m_pInstance = NULL;
CString CCGInspIO::m_strBoardName = _T("MMC");
CIOPort * CCGInspIO::m_pBoard = NULL;
CCGInspIO::CCGInspIO()
{
}


CCGInspIO::~CCGInspIO()
{
}


BOOL CCGInspIO::GetEMG(int *pErrStageNo)
{
	BOOL rv = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (IsEMG(i))
		{
			*pErrStageNo = i;
			rv = TRUE;
			break;
		}
	}
	return rv;
}


BOOL CCGInspIO::GetErr(int* pErrStageNo)
{
	BOOL rv = FALSE;
	for (int i = 0; i < 3; i++)
	{
		if (IsERR(i))
		{
			*pErrStageNo = i;
			rv = TRUE;
			break;
		}
	}
	return rv;
}


void CCGInspIO::LoadEnv()
{
	m_pBoard = CIOPort::Instance();
	LoadPortNo();
	return;
}


CCGInspIO * CCGInspIO::Instance()
{
	if (m_pInstance)return m_pInstance;

	m_pInstance = new CCGInspIO;
	m_pInstance->m_pBoard = CIOPort::Instance(m_strBoardName.GetBuffer());

	return m_pInstance;
}

void CCGInspIO::LoadPortNo()
{
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, ProductName;

	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	ProductName = PRODUCT_NAME;
	MachineInputName = ProductName + _T("_INPUTSET");
	MachineOutputName = ProductName + _T("_OUTPUTSET");
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	// Input 
	m_nConveyor.InSensor = pMyCfg->Get(1L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIInSensor"));
	m_nConveyor.OutSensor = pMyCfg->Get(5L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIOutSensor"));
	m_nConveyor.TackInSensor = pMyCfg->Get(0L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nITackInSensor"));
	m_nConveyor.TackOutSensor = pMyCfg->Get(2L, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nITackOutSensor"));
}


void CCGInspIO::SavePortNo()
{
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, strProductName;
	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	strProductName = PRODUCT_NAME;
	MachineInputName.Format(_T("%s%s"), strProductName, _T("_INPUTSET"));
	MachineOutputName.Format(_T("%s%s"), strProductName, _T("_OUTPUTSET"));
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");


	//! Input port number setting
#pragma region Input port number setting
	pMyCfg->Set(m_nConveyor.InSensor, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIInSensor"));
	pMyCfg->Set(m_nConveyor.OutSensor, (TCHAR*)(LPCTSTR)MachineInputName, _T("m_nIOutSensor"));

#pragma endregion

	//! Ouput port number settting
#pragma region Output port number setting 
#pragma endregion

	pMyCfg->Flush();
}

int CCGInspIO::Init()
{
	LoadEnv();
	m_pBoard->Init();
	return 0;
}


BOOL CCGInspIO::IsEMG(int nStage)
{
	//if (nStage != -1)
	//	return m_pBoard->GetBit(m_nIStage[nStage].EMG);
	//else
	//{
	//	for (int i = 0; i < m_nCountOfStage; i++)
	//		if (m_pBoard->GetBit(m_nIStage[nStage].EMG))return TRUE;
	//}
	return FALSE;
}

BOOL CCGInspIO::IsERR(int nStage)
{
	//if (nStage != -1)
	//	return m_pBoard->GetBit(m_nIStage[nStage].ERR);
	//else
	//{
	//	for (int i = 0; i < m_nCountOfStage; i++)
	//		if (m_pBoard->GetBit(m_nIStage[nStage].ERR))return TRUE;
	//}
	return FALSE;
}


void CCGInspIO::Close()
{
	m_pBoard->Close();
}



void CCGInspIO::SetAllOutputClear()
{
	for(int i=0;i<31;i++)
		m_pBoard->SetBit(i, 0);
}



int CCGInspIO::GetConvOutSensor()
{
	return m_pBoard->GetBit(m_nConveyor.OutSensor);
}


int CCGInspIO::GetConvInSensor()
{
	return m_pBoard->GetBit(m_nConveyor.InSensor);
}


int CCGInspIO::GetTackInSensor()
{
	return m_pBoard->GetBit(m_nConveyor.TackInSensor);
}


int CCGInspIO::GetTackOutSensor()
{
	return m_pBoard->GetBit(m_nConveyor.TackOutSensor);
}
