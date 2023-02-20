#pragma once
#ifdef MIL_USE
#include <Matrox.h>
class CRepository
{
public:
	CRepository();
	~CRepository();
	static CRepository * Instance();
	static CRepository *m_pInstance;
	CMilVLib m_MilLib;
};
#endif