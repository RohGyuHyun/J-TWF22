#pragma once
#include "PartBase.h"
#include <UniUtility.h>

class CCamPart : public CPartBase
{
	DECLARE_DYNAMIC(CCamPart)
public:
	CCamPart();
	~CCamPart();
	// ! 0..3������ ī�޶��ȣ
	int m_nCamNo;
	virtual int SaveParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);
	virtual int LoadParam(CXmlNode* pNode);//, BOOL bUseCurNode=FALSE);
};

