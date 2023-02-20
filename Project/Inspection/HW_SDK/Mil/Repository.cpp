#include "pch.h"
#include "Repository.h"
#ifdef MIL_USE
CRepository * CRepository::m_pInstance = NULL;
CRepository::CRepository()
{
}


CRepository::~CRepository()
{
}


CRepository * CRepository::Instance()
{
	if (!m_pInstance)
	{
		m_pInstance = DBG_NEW CRepository;
	}
	return m_pInstance;
}
#endif