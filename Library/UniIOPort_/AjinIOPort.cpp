#include "StdAfx.h"
#ifdef _AJINCARD

#include "AjinIOPort.h"
#include <AXL.h>
#include <AXD.h>

CAjinIOPort::CAjinIOPort(void)
	: m_nInputModule(0)
	, m_nOutputModule(1)
{
}

CAjinIOPort::~CAjinIOPort(void)
{
}

int CAjinIOPort::SetPort(int nPort, long value)
{
	if (AXT_RT_SUCCESS == AxdoWriteOutport(m_nOutputModule, value)) return 0;
	else return -1;
	return 0;
}

int CAjinIOPort::GetPort(int nPort, long * pValue)
{
	if (AXT_RT_SUCCESS == AxdiReadInport(m_nInputModule, (DWORD *)pValue)) return 0;
	else return -1;
}

int CAjinIOPort::GetOutPort(int nPort, long * pValue)
{
	if (AXT_RT_SUCCESS == AxdoReadOutport(m_nOutputModule, (DWORD *)pValue)) return 0;
	else return -1;
}

int CAjinIOPort::SetBit(int bitNo, int Value)
{
	if (AXT_RT_SUCCESS == AxdoWriteOutportBit(m_nOutputModule, bitNo, Value)) return 0;
	else return -1;
}

BOOL CAjinIOPort::GetBit(int nBitNo)
{
	DWORD dwValue;
	if (AXT_RT_SUCCESS != AxdiReadInportBit(m_nInputModule, nBitNo, &dwValue)) TRACE(_T("Error Input"));
	if (dwValue)
		return TRUE;
	else
		return FALSE;
}

BOOL CAjinIOPort::GetOutBit(int nBitNo)
{
	DWORD dwValue;
	if (AXT_RT_SUCCESS != AxdoReadOutportBit(m_nOutputModule, nBitNo, &dwValue)) TRACE(_T("Get OutBit Error"));
	if (dwValue)
		return TRUE;
	else
		return FALSE;
}


//! return -1 error
int CAjinIOPort::Init()
{
	//IRQ�� �̱ۺ��忡���� ����(�� �ڵ�)
	if (AxlOpen(7) == AXT_RT_SUCCESS)TRACE(_T("���̺귯�� �ʱ�ȭ ����"));

	DWORD dwStatus;
	AxdInfoIsDIOModule(&dwStatus);
	if (dwStatus != STATUS_EXIST)
	{
		return -1;
	}
	
	//���� PCI-DB64R�� 0���� Input 1���� Output
	long lCount; 
	AxdInfoGetModuleCount(&lCount);//���ī��Ʈ
	if (lCount < 2)return -2; //��ǲ�ƿ�ǲ 2������ ��.
	AxdInfoGetInputCount(m_nInputModule, &lCount);//0������� �Է� ä�� ���� �������
//	if (lCount < 32)return -3; //0������� �Է� ä�� ������ 32�� �̸��̸� ��������
	AxdInfoGetOutputCount(m_nOutputModule, &lCount);
//	if (lCount < 32)return -4; //0������� ��� ä�� ������ 32�� �̸��̸� ��������

	return 0;
}


void CAjinIOPort::Close()
{
	if (AxlClose())TRACE("DIO card is closed");
}


int CAjinIOPort::SetOffset()
{
	for(int i=0;i<32;i++)
		AxdiLevelSetInportBit(m_nInputModule, i, 1);  //0������� i��°��Ʈ�� 1�̸� ���� ��ȣ High�϶� 1�� �Էµ���
	for (int i = 0; i<32; i++)
		AxdoLevelSetOutportBit(m_nOutputModule, i, 1);  //1������� i��°��Ʈ�� 1�̸� ���� ���� 1�� �� ��� High��
	return 0;
}

#endif