#include "Grabber.h"
#include "GrabChannel.h"
#include "MatroxGrabber.h"
#include "MatroxChannel.h"
#include "..\MilSystem.h"

int CGrabber::m_nGrabber = 0;
CGrabber::CGrabber(void) 
	: m_pMilSystem(NULL)
{
}

CGrabber::~CGrabber(void)
{
}

int CGrabber::Init(void)
{
	if(m_strGrabberName != GB_NONE)
		m_nGrabber++;
	return 0;
}

CGrabber * CGrabber::Create(TCHAR *strGrabber)
{
	CGrabber *pInstance = NULL;

	m_strGrabberName = strGrabber;

	if(!m_strGrabberName.Compare(GB_NONE)) pInstance = new CGrabber;
	if(!m_strGrabberName.Compare(GB_SOLIOS)) pInstance = new CMatroxGrabber;

	if(pInstance)
		pInstance->m_pMilSystem = new CMilSystem;
	return pInstance;
}


int CGrabber::Alloc(void)
{
	return 0;
}

CGrabChannel* CGrabber::NewChannel(TCHAR * szCamFile)
{
	CGrabChannel* pGrabChannel = NULL;

	if(!m_strGrabberName.Compare(GB_NONE)) pGrabChannel = new CGrabChannel;
	if(!m_strGrabberName.Compare(GB_METER4M)) pGrabChannel = new CMatroxChannel;
	return pGrabChannel;
}

void CGrabber::Close()
{
}
