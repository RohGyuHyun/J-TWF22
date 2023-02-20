#pragma once
#include "..\MilBase.h"
#include "Grabber.h"
#include "GrabChannel.h"
#include <mil.h>

class CMilDigitizer;

class MIL_DLLDECL CMatroxGrabber :public CGrabber
{
public:
	CMatroxGrabber(void);
	~CMatroxGrabber(void);
	int Init(void);
	CMilDigitizer *m_pMilDigitizer;
	CGrabChannel* NewChannel(TCHAR * szCamFile);
	void Close();
};
