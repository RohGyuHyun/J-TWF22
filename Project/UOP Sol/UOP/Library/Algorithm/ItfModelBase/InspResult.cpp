#include "StdAfx.h"
#include "InspResult.h"
#include "AlgorithmBase.h"

CListCtrl * CInspResult::m_pListResult = NULL;
CInspResult::CInspResult(void)
: m_iCount(0)
{
	m_pAlgorithm=NULL;
}

CInspResult::~CInspResult(void)
{
	ReleaseResults();
}

void CInspResult::ReleaseResults(void)
{
	if(!m_pAlgorithm)return;
	m_pAlgorithm->ReleaseResults(this);
	m_pAlgorithm=NULL;
}

void CInspResult::SetType(CAlgorithmBase *pAlgorithm)
{
	USES_CONVERSION;
	TCHAR* szTemp = A2W(pAlgorithm->GetRuntimeClass()->m_lpszClassName);
	m_strInspType=szTemp;
	m_pAlgorithm = pAlgorithm;
}

CString CInspResult::GetType(void)
{
	return m_strInspType;
}

/*! @fn CAlgorithmBase * CInspResult::GetAlgorithm(void)
	@brief ����� �޾ƿ� �˰��� 
	@n ���ο��� ���Ǿ�� �ϴ°����� �ܺο����� ����ϸ� �ȵ�
	@n �˰��򿡼� ��� ������ �ϰ����� ���� �������� �𸣴� ��Ȳ�̱� ������
*/
CAlgorithmBase * CInspResult::GetAlgorithm(void)
{
	return m_pAlgorithm;
}

int CInspResult::GetCount(void)
{
	return m_iCount;
}

void CInspResult::SetCount(int nCount)
{
	m_iCount=nCount;
}

void * CInspResult::GetResults(void)
{
	return m_pResult;
}

void CInspResult::Free(void)
{
	ReleaseResults();
}
