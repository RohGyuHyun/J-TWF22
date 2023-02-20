#include "StdAfx.h"
#include "MilModFinder.h"
#include "MilSystem.h"

CMilModFinder::CMilModFinder(void):m_pMilContext(NULL)
{
}

CMilModFinder::~CMilModFinder(void)
{
	Close();
}


/*! @fn int CMilModFinder::SetContext(CMilContext *pMilContext)
	@brief Context�� �����Ѵ�
	@param pMilContext����� CMilContext
*/
int CMilModFinder::SetContext(CMilContext *pMilContext)
{
	m_pMilContext = pMilContext;
	return 0;
}

/*! @fn int CMilModFinder::DelAllFeature(int Index)
	@brief Context���� ��ϵ� ��� ���� �����
	@param pMilContext����� CMilContext
*/
int CMilModFinder::DelAllFeature(int Index)
{
	PreProcess();
	return 0;
}


/*! @fn int CMilModFinder::GetFeature(void)
	@brief �� ������ Parameter�� �ɼ��� �����Ѵ�.
*/
int CMilModFinder::GetFeature(void)
{
	Alloc();
	SetAOI();
	SetSpeed();
	PreProcess();
	return 0;
}

/*! @fn int CMilModFinder::GetFeature(CRect aoiRect)
	@brief aoiRect ���������� GetFeature�� ��
	@n SetBuffer���� ������ �̹��� �������� aoiRect�� �ǹ���
*/
int CMilModFinder::GetFeature(CRect aoiRect)
{
	Alloc();
	SetAOI(aoiRect);
	SetSpeed();
	PreProcess();
	return 0;
}

/*! @fn int CMilModFinder::SetBuffer(CMilBuffer *pImage)
	@brief ModelFinder�� ���� ���۸� �����Ѵ�
*/
int CMilModFinder::SetBuffer(CMilBuffer *pImage)
{
	m_pMilBuffer = pImage;
	return 0;
}


/*! @fn int CMilModFinder::SetResult(CMilResult *pResult)
	@brief ��������� CMilResult�� �����Ѵ�
*/
int CMilModFinder::SetResult(CMilResult *pResult)
{
	m_pMilResult = pResult;
	return 0;
}

/*! @fn int CMilModFinder::Define(long ltype, double dblParam1, double dblParam2, double dblParam3, double dblParam4, double dblParam5)
	@brief ModDefine�� �״�� ĸ��ȭ��
*/
int CMilModFinder::Define(long ltype, double dblParam1, double dblParam2, double dblParam3, double dblParam4, double dblParam5)
{
	MmodDefine(m_pMilContext->GetIDContext(), ltype, dblParam1, dblParam2, dblParam3, dblParam4, dblParam5);
	return 0;
}

/*! @fn int CMilModFinder::SetAOI(void)
	@brief ��ü���ۿ������� ModDefine��
*/
int CMilModFinder::SetAOI(void)
{
	tImageProperties prptImg;
	m_pMilBuffer->GetImageProperties(&prptImg);
	CRect imgRect(1,1, prptImg.ImageSize.cx-1, prptImg.ImageSize.cy-1);
	SetAOI(imgRect);
	return 0;
}

/*! @fn int CMilModFinder::SetAOI(CRect aoiRect)
	@brief aoiRect���ο����� ModDefine��
*/
int CMilModFinder::SetAOI(CRect aoiRect)
{
	MmodDefine(m_pMilContext->GetIDContext(), M_IMAGE, m_pMilBuffer->GetIDBuffer(), aoiRect.left, aoiRect.top, aoiRect.Width(), aoiRect.Height());
	return 0;
}

/*! @fn int CMilModFinder::PreProcess(void)
	@brief Contextȯ���� ����
*/
int CMilModFinder::PreProcess(void)
{
	MmodPreprocess(m_pMilContext->GetIDContext(), M_DEFAULT);
	return 0;
}

/*! @fn int CMilModFinder::Find(tModResult *pResult)
	@brief Contextȯ���� ����
*/
int CMilModFinder::Find(tModResult *pResult)
{
	MIL_ID *pID_RESULT;
	tImageProperties ImageProp;
	pID_RESULT=&m_pMilResult->m_milIDResult;
	MIL_DOUBLE posX, posY, Angle, Score, Scale;

	MmodFind(m_pMilContext->GetIDContext(), m_pMilBuffer->GetIDBuffer(), *pID_RESULT);
	m_pMilBuffer->GetImageProperties(&ImageProp);
	pResult->ImageSize=ImageProp.ImageSize;
	MmodGetResult(*pID_RESULT, M_DEFAULT, M_NUMBER+M_TYPE_MIL_INT, &pResult->NumResults);
	if(pResult->NumResults)
	{
		MmodGetResult(*pID_RESULT, 0, M_POSITION_X,&posX);
		MmodGetResult(*pID_RESULT, 0, M_POSITION_Y, &posY);
		MmodGetResult(*pID_RESULT, 0, M_ANGLE, &Angle);
		MmodGetResult(*pID_RESULT, 0, M_SCALE, &Scale);
		MmodGetResult(*pID_RESULT, 0, M_SCORE, &Score);
		pResult->Pos.dblPosX=posX;
		pResult->Pos.dblPosY=posY;
		pResult->Angle=Angle;
		pResult->Scale=Scale;
		pResult->Score=Score;

	}else
	{
		pResult->Pos.dblPosX = pResult->ImageSize.cx/2;
		pResult->Pos.dblPosY = pResult->ImageSize.cy/2;
		pResult->Angle = 0;
		pResult->Scale = 0;
		pResult->Score = 0;
		return -1;
	}

	return 0;
}

int CMilModFinder::Find(CMilResult *pResult)
{
	MmodFind(m_pMilContext->GetIDContext(), m_pMilBuffer->GetIDBuffer(), pResult->m_milIDResult);
	return 0;
}

int CMilModFinder::SetDrawOffset(CSize Offset)
{
	Control(M_DEFAULT,M_DRAW_RELATIVE_ORIGIN_X, Offset.cx, MOD_TARGET_RST);
	Control(M_DEFAULT,M_DRAW_RELATIVE_ORIGIN_Y, Offset.cy, MOD_TARGET_RST);

	return 0;
}

int CMilModFinder::GetDrawBoxOffset(CdblSize *pOffset)
{
	MIL_DOUBLE cx,cy;
	MmodInquire(m_pMilContext->GetIDContext(), M_DEFAULT,  M_BOX_OFFSET_X, &cx);
	MmodInquire(m_pMilContext->GetIDContext(), M_DEFAULT,  M_BOX_OFFSET_Y, &cy);
	pOffset->dblCX =cx;
	pOffset->dblCY =cy;

	return 0;
}

int CMilModFinder::DrawResult(void)
{
	MmodDraw(M_DEFAULT, m_pMilResult->GetIDResult(), m_pMilBuffer->GetDisplay()->GetIDOverlay(), M_DRAW_EDGE+M_DRAW_BOX+M_DRAW_POSITION, 0, M_DEFAULT);
	return 0;
}

int CMilModFinder::DrawContext(void)
{
	MmodDraw(M_DEFAULT, m_pMilContext->GetIDContext(), m_pMilBuffer->GetDisplay()->GetIDOverlay(), M_DRAW_BOX+M_DRAW_POSITION, 0, M_ORIGINAL);
	return 0;
}

int CMilModFinder::Alloc(void)
{
	if(!m_pMilResult->m_milIDResult)
		m_pMilResult->ModAlloc();

	if (!m_pMilContext->m_milIDContext)
		MmodAlloc(CMilSystem::Instance()->GetSystemID(), M_GEOMETRIC, M_DEFAULT, &(m_pMilContext->m_milIDContext));
	return 0; 
}

int CMilModFinder::Control(long Index, long CtrlType, double CtrlValue, tModTargetType Target)
{
	if(Target==MOD_TARGET_CXT)
	{
		MmodControl(m_pMilContext->GetIDContext(), Index, CtrlType, CtrlValue);
	}else
	{
		MmodControl(m_pMilResult->GetIDResult(), Index, CtrlType, CtrlValue);
	}
	return 0;
}

int CMilModFinder::SetSpeed(long lSpeed)
{
	double dblSpeed=lSpeed;
	Control(M_CONTEXT, M_SPEED, lSpeed);
	return 0;
}

int CMilModFinder::SetSearchAngleRange(double dblScore)
{
	Control(M_CONTEXT, M_SEARCH_ANGLE_RANGE, dblScore);
	return 0;
}

int CMilModFinder::SetSearchScaleRange(double dblScore)
{
	Control(M_CONTEXT, M_SEARCH_SCALE_RANGE, dblScore);
	return 0;
}

int CMilModFinder::SetAccptanceScore(double dblScore)
{
	MmodControl(m_pMilContext->GetIDContext(), M_ALL, M_ACCEPTANCE, dblScore);
	return 0;
}


int CMilModFinder::Close(void)
{
	if(m_pMilResult)m_pMilResult->ModFree();
	if(m_pMilContext)m_pMilContext->Close();
	return 0;
}
