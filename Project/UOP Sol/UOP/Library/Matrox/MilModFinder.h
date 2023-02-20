#pragma once
#include <mil.h>
#include "MilBase.h"
#include "MilBuffer.h"
#include "MilResult.h"

class CMilContext;

/*!
\brief 
* Model Finder�� ����� �ޱ� ���� ����ü
@todo  CMilResult�� ���յǾ�� �ϴµ� ���������� �и��Ͽ� �۾��Ͽ���
@author ������
*/
struct tModResult{
	long NumResults;
	CSize ImageSize;
	CdblPoint Pos;
	double Angle;
	double Scale;
	double Score;
};


//* Alloc�� Close���� Context�� Result�� Alloc�� Free�� �����Ѵ�.
/*!
\brief 
* Mil�� Model Finder����  �Լ����� Classȭ�Ͽ���
@author ������
*/
class MIL_DLLDECL CMilModFinder:public CMilBase
{
public:
	enum tModTargetType{MOD_TARGET_RST, MOD_TARGET_CXT};
	CMilModFinder(void);
	~CMilModFinder(void);
	MIL_ID m_mIlIDSystem;
	CMilContext * m_pMilContext;
	CMilBuffer * m_pMilBuffer;
	CMilResult * m_pMilResult;
	int SetContext(CMilContext *pMilContext);
	int GetFeature(void);
	int GetFeature(CRect aoiRect);
	int DelAllFeature(int Index);
	int SetBuffer(CMilBuffer *pImage);
	int SetResult(CMilResult *pResult);
	int Define(long ltype, double dblParam1, double dblParam2, double dblParam3, double dblParam4, double dblParam5);
	int SetAOI(void);
	int SetAOI(CRect aoiRect);
	int Control(void);
	int PreProcess();
	int Find(tModResult *pResult);
	int Find(CMilResult *pResult);
	int DrawContext(void);
	int SetDrawOffset(CSize Offset=CSize(M_DEFAULT, M_DEFAULT));
	int GetDrawBoxOffset(CdblSize *pOffset);
	int DrawResult(void);
	int Alloc(void);
	int Control(long Index, long CtrlType, double CtrlValue, tModTargetType Target=MOD_TARGET_CXT);
	int SetSpeed(long lSpeed=M_VERY_HIGH);
	int SetSearchAngleRange(double dblScore=M_DEFAULT);
	int SetSearchScaleRange(double dblScore=M_DEFAULT);
	int SetAccptanceScore(double dblScore);
	int Close(void);
};
