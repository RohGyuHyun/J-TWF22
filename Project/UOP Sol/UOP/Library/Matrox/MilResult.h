#pragma once
#include "MilBase.h"

/*!
\brief 
* Mil�� ��������� �޾� ���ϴ� ���·� �����Ͽ� �����ϴ� Classȭ��
@todo  ���� �ϼ��Ȱ��� �ƴ� �߰��۾��ʿ�
@author ������
*/
class MIL_DLLDECL CMilResult:public CMilBase
{
public:
	CMilResult(void);
	~CMilResult(void);
	MIL_ID m_milIDResult;
	int GetIDResult(void);
	int ModAlloc(void);
	int ModFree(void);
	int Close(void);
	BOOL m_fModAlloc;
};
