#include "StdAfx.h"
#include "IOPort.h"
#include "AjinIOPort.h"
#ifdef _MMC
#include "MMCIOPort.h"
#endif 
#ifdef _XMC
#include "CXMCIOPort.h"
#endif

CString MachineInputName, MachineOutputName;
CIOPort * CIOPort::m_pInstance=NULL;
CIOPort::CIOPort()
	: m_strBoardName(_T("Ajin"))
{
	TRACE(_T("Init IOPort IO\r\n"));

	for(int i=0; i<32; i++)
	{
		m_IOInput[i].pnNum = NULL;
		m_IOInput[i].strLabelName = _T("SPARE");
		m_IOOutput[i].pnNum = NULL;
		m_IOOutput[i].strLabelName = _T("SPARE");
	}

	
}

CIOPort::~CIOPort(void)
{
}

int CIOPort::SetPort(int port, long value)
{
	return 0;
}

int CIOPort::GetPort(int nPort, long * pValue)
{
	return 0;
}

int CIOPort::GetOutPort(int nPort, long * pValue)
{
	return 0;
}

int CIOPort::SetBit(int bitNo, int Value)
{
	return 0;
}

CIOPort * CIOPort::Instance(TCHAR * szBoardName)
{
	if (m_pInstance)return m_pInstance;
	CString strBoardName = szBoardName;
	if (strBoardName == _T("Ajin"))
	{
#ifdef _AJINCARD
		if (m_pInstance == NULL) m_pInstance = new CAjinIOPort;
#endif
	} else
	if (strBoardName == _T("MMC"))
	{
#ifdef _MMC
		if (m_pInstance == NULL) m_pInstance = new CMMCIOPort;
#endif
	} else
	if (strBoardName == _T("XMC"))
	{
#ifdef _XMC
		if (m_pInstance == NULL) m_pInstance = new CXMCIOPort;
#endif
	}else
	{
		if (m_pInstance == NULL) m_pInstance = new CIOPort;
	}

	m_pInstance->m_strBoardName = szBoardName;

	return m_pInstance;
}

//! return value 0:success -1:error
int CIOPort::Init(void)
{  
	return 0;
}

BOOL CIOPort::GetBit(int nBitNo)
{
	return 0;
}

BOOL CIOPort::GetOutBit(int nBitNo)
{
	return 0;
}

CString CIOPort::GetBoardName(void)
{
	return m_strBoardName;
}

void CIOPort::InitIOData(void)
{

}


void CIOPort::Close()
{
	return;
}
