//#include "StdAfx.h"
#include "MatroxGrabber.h"
#include "MatroxChannel.h"
#include <matrox.h>

CMatroxGrabber::CMatroxGrabber(void)
{
	m_pMilSystem = NULL;
}

CMatroxGrabber::~CMatroxGrabber(void)
{
}

int CMatroxGrabber::Init(void)
{
	int rv;
	switch (m_nGrabber)
	{
		//case 0:
		//	rv = MsysAlloc(M_SYSTEM_DEFAULT, M_DEFAULT, M_DEFAULT, &m_milIDSystem);
		//	break;
		default:
			rv = MsysAlloc(M_SYSTEM_SOLIOS, M_DEV0+m_nGrabber, M_DEFAULT, &m_milIDSystem);
		break;
	}
	
	if (!m_pMilSystem)m_pMilSystem = new CMilSystem;
	m_pMilSystem->SetSystemID(m_milIDSystem);
	CGrabber::Init();
	return rv;
}

CGrabChannel* CMatroxGrabber::NewChannel(TCHAR * szCamFile)
{
	CMatroxChannel *pChannel;
	pChannel= new CMatroxChannel;
	pChannel->m_milIDSystem = m_milIDSystem;
	pChannel->m_pMilSystem = m_pMilSystem;
	
	pChannel->m_strCameraFile = szCamFile;
	return pChannel;
}

void CMatroxGrabber::Close()
{
	MsysFree(m_milIDSystem);
}
