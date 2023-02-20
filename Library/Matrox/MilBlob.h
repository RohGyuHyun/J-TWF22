#pragma once

#include <mil.h>
#include "MilBase.h"

class CMilBuffer;
class CMilDisplay;

struct tWndBlobCondition;

/*!
\brief 
* Blob����� �޴µ� ���Ǵ� ����ü
*/
struct tBlobResultList
{
	int nCount;
	double *pCXs, *pCYs;
	double *pArea;
	double *pBoxLeft, *pBoxTop, *pBoxRight, *pBoxBottom;
	tWndBlobCondition *pWndBlobCdt;
};

/*!
\brief 
* Mil�� Blob����  �Լ����� Classȭ�Ͽ���
@author ������
*/
class MIL_DLLDECL CMilBlob:public CMilBase
{
public:
	CMilBlob(void);
	CMilBlob(CMilBuffer *pMilBuffer);
	~CMilBlob(void);
	void Caculate(void);
	int Control(void);
	void ReleaseResultMem(void);
	void Select(MIL_INT Operation, MIL_INT Feature, MIL_INT Condition, double CondLow, double CondHigh);
	int GetNumber(void);
	int GetResult(void);
	int SelectFeret(void);
	void SelectFeature(MIL_INT feature);
	int Reconstruct(void);
	int Fill(void);
	void Label(void);
	int AllocFeatureList(void);
	int AllocResult(void);
	void SetMilBuffer(CMilBuffer *pMilBuffer);
	void Draw(CMilDisplay *pMyDisplay, int iLabel);
	void Draw(CMilDisplay *pMilDisplay);
	MIL_ID m_featureList;
	MIL_ID m_milIDBuffer;
	MIL_ID m_milIDSystem;
	MIL_ID m_milResult;
	CMilBuffer *m_pMilBuffer;
	tBlobResultList m_ResultList;
	int Free(void);
	void SetBuffer(CMilBuffer * pMilBuffer);
};
