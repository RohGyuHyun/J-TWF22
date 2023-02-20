#pragma once
#include "MilSystem.h"

/*!
\brief 
* Mil�� model finder�κ��� Context�κ��� ���� Classȭ�Ͽ���
@author ������
*/
class MIL_DLLDECL CMilContext
{
public:
	enum tMFtype{MFT_GEOMETRIC, MFT_GEOMETRIC_CONTROLLED};
	CMilContext(void);
	~CMilContext(void);
	MIL_ID m_milIDContext;
	CMilSystem *m_pMilSystem;
	int GetIDContext(void);
	void SaveContext(TCHAR * szSaveFile);
	int RestoreContext(TCHAR * szLoadFile);
	int GetImage(CMilBuffer *pBuffer);
	void SetMilSystem(CMilSystem * pMilSystem);
	int Alloc(int MFType);
	int Control(int Index=M_CONTEXT, int Type=M_SPEED, int Value=M_VERY_HIGH);
	int Close(void);
	int PreProcess(void);
	int GetModelNumber(void);
	int DelModel(int Index);
	int DelAllModel(void);
};
