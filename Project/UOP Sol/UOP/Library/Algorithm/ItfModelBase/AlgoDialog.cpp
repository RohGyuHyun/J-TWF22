// AlgoDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "AlgoDialog.h"
#include "StarMessage.h"


// CAlgoDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CAlgoDialog, CDialog)

CAlgoDialog::CAlgoDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
	, m_resID(0)
{
	
}

CAlgoDialog::~CAlgoDialog()
{
}

void CAlgoDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlgoDialog, CDialog)
	ON_MESSAGE(WM_SCRN_UPDATE, &CAlgoDialog::OnScrnUpdate)
END_MESSAGE_MAP()


// CAlgoDialog �޽��� ó�����Դϴ�.

void CAlgoDialog::SetResID(int resID)
{
	m_resID=resID;
}

int CAlgoDialog::GetParam(CAlgorithmBase * pAlgorithm)
{
	return 0;
}

int CAlgoDialog::SetParam(CAlgorithmBase * pAlgorithm)
{
	return 0;
}

/*!
	@fn CAlgorithmBase * CAlgoDialog::NewAlgoObject(void)
	@brief �˰����� ��ü�� �������� �ٶ� ����ϰ� �ȴ�. �˰����� �̸��� �𸣱⶧���� �ڵ������� �ʿ��ϴ�
*/
CAlgorithmBase * CAlgoDialog::NewAlgoObject(void)
{
	return NULL;
}
BOOL CAlgoDialog::IsChecked(void)
{
	return 0;
}

int CAlgoDialog::RegCaller(CParamsInterface * pParamsInterface)
{
	m_pParamsInterface = pParamsInterface;
	return 0;
}

/*!
	@fn int CAlgoDialog::CompareAlgoName(TCHAR * szAlgoName)
	@brief ���� Loading�Ҷ� �˰����̸��� �ִ��� Ȯ���� �� �ִ� ����� �������ش�
*/
int CAlgoDialog::CompareAlgoName(TCHAR * szAlgoName)
{
	return GetAlgoName().Compare(szAlgoName);
}

int CAlgoDialog::DoTrial(void)
{
	return 0;
}

int CAlgoDialog::DoCompoTrial(CRect inspRect, CInspResult * pResult)
{
	return 0;
}

int CAlgoDialog::Inspect(void)
{
	return 0;
}

/*!
	@fn void CAlgoDialog::SetInput(CMilBuffer * pInputImage)
	@brief �˻��ϰԵ� �̹����� �����ϰ� �ȴ�.
*/
void CAlgoDialog::SetInput(CMilBuffer * pInputImage)
{
	m_pInputImg = pInputImage;
}

/*!
	@fn CString CAlgoDialog::GetAlgoName(void)
	@brief �˰����� �̸��� ��ȯ�Ѵ�. �𵨷ε���� ����ϰ� �ȴ�.
*/
CString CAlgoDialog::GetAlgoName(void)
{
	return CString();
}

/*!
	@fn LRESULT CAlgoDialog::OnScrnUpdate(WPARAM wparam, LPARAM lparam)
	@brief ȭ�鰻���� ��û�Ǹ� �ķ����͵��� ���� ȭ������ Update���ش�
*/
LRESULT CAlgoDialog::OnScrnUpdate(WPARAM wparam, LPARAM lparam)
{
	UpdateData(FALSE);
	return 0;
}

/*!	@fn int CAlgoDialog::GetAlgoType(void)
	@brief �˰����� ���¸� ��ȯ�Ѵ�
*/
int CAlgoDialog::GetAlgoType(void)
{
	return 0;
}

void CAlgoDialog::SetCheck(bool bStatus)
{
}

int CAlgoDialog::DoAutoTeaching(void)
{
	return 0;
}

int CAlgoDialog::ReportResult(CInspResult * pInspResult)
{
	return 0;
}
