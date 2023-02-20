#include "StdAfx.h"
#include <Mil.h>
#include "MilContext.h"

CMilContext::CMilContext(void)
{
	m_milIDContext=M_NULL;
}

CMilContext::~CMilContext(void)
{
}

/*! @fn int CMilContext::GetIDContext(void)
*	@brief Class���� ����ϴ� ���� Context ID�� ��ȯ�Ѵ�
*	@return Context ID�� ��ȯ�Ѵ�.
*/
int CMilContext::GetIDContext(void)
{
	return m_milIDContext;
}


/*! @fn int CMilContext::RestoreContext(TCHAR * szLoadFile)
*	@brief Context�� ���Ϸκ��� �о�´�
*	@return 0�� �ƴϸ� Error
*/
int CMilContext::RestoreContext(TCHAR * szLoadFile)
{
	m_pMilSystem=CMilSystem::Instance();
	return MmodRestore(szLoadFile, m_pMilSystem->GetSystemID(), M_DEFAULT, &m_milIDContext);

}

/*! @fn int CMilContext::GetImage(CMilBuffer *pBuffer)
*	@brief Context�κ��� �̹����� �����´�
*   @n ��Ȯ�� ���ϸ� pBuffer�� �̹����� �׸��°��̴�.
*	@param pBuffer �̹����� �׸� Mil Buffer(Image �Ӽ��� �־�� ��)
*/
int CMilContext::GetImage(CMilBuffer *pBuffer)
{
	m_pMilSystem=CMilSystem::Instance();
	MmodDraw(M_DEFAULT, m_milIDContext, pBuffer->GetIDBuffer(), M_DRAW_IMAGE, M_DEFAULT, M_DEFAULT);
	return 0;
}

/*! @fn void CMilContext::SaveContext(TCHAR * szSaveFile)
*	@brief Context�� ���Ϸ� �����Ѵ�
*	@param szSaveFile ������ ���� �̸�(�������)
*/
void CMilContext::SaveContext(TCHAR * szSaveFile)
{
	MmodSave(szSaveFile, m_milIDContext, M_DEFAULT);
}

void CMilContext::SetMilSystem(CMilSystem * pMilSystem)
{
	m_pMilSystem=pMilSystem;
}

/*! @fn int CMilContext::Alloc(int MFType)
*	@brief ����Ҽ� �ֵ��� Alloc�Ѵ�
*/
int CMilContext::Alloc(int MFType)
{
	if(MFType==CMilContext::MFT_GEOMETRIC)
		return MmodAlloc(m_pMilSystem->GetSystemID(), M_GEOMETRIC, M_DEFAULT, &m_milIDContext);
	else
		return MmodAlloc(m_pMilSystem->GetSystemID(), M_GEOMETRIC_CONTROLLED, M_DEFAULT, &m_milIDContext);
}

int CMilContext::Control(int Index, int Type, int Value)
{
	MmodControl(m_milIDContext, Index, Type, Value);
	return 0;
}

int CMilContext::Close(void)
{
	if(m_milIDContext)MmodFree(m_milIDContext);
	m_milIDContext=M_NULL;
	return 0;
}

/*! @fn int CMilContext::PreProcess(void)
*	@brief Context�� ���� ���� ���⼭ �̷������
*   @n ���� ��� Context�� �Ϻλ���Ȥ�� �߰��� �̷������ �� PreProcess�� �Ͽ�����
*/
int CMilContext::PreProcess(void)
{
	MmodPreprocess(GetIDContext(), M_DEFAULT);
	return 0;
}

/*! @fn int CMilContext::GetModelNumber(void)
*	@brief Context���� ��� �ִ� ���� ������ ��´�.
*	@return Context���� �𵨰����� ��ȯ�Ѵ�.Ȥ Context�� �Ҵ���� �ʾ�����쿡�� -1�� ��ȯ
*/
int CMilContext::GetModelNumber(void)
{
	MIL_DOUBLE rv=0;
	if(!GetIDContext())return -1;
	MmodInquire(GetIDContext(), M_CONTEXT, M_NUMBER_MODELS, &rv);
	return (int)rv;
}

/*! @fn int CMilContext::DelContext(int Index)
*	@brief Context�� Index���� �����Ѵ�
*/
int CMilContext::DelModel(int Index)
{
	MmodDefine(GetIDContext(), M_DELETE, Index, M_DEFAULT, M_DEFAULT, M_DEFAULT, M_DEFAULT);
	return 0;
}


/*! @fn int CMilContext::DelAllContext(void)
*	@brief Context���� ��� ���� �����Ѵ�
*/
int CMilContext::DelAllModel(void)
{
	int nCount=GetModelNumber();

	if(nCount>0)
	{
		for(int i=0;i<nCount;i++)
		{
			DelModel(0);
		}
	}

	return 0;
}
