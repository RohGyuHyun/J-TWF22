#pragma once
#ifdef _MULTICAM
#include "..\stdafx.h"
#include "atltypes.h"
#include <MultiCam.h>

class CMilBuffer;

class CEuresysChannel;

class MIL_DLLDECL CEuresysGrabber
{
public:
	CEuresysGrabber();
	~CEuresysGrabber();
	int Init();
	void Close();
	CSize m_ScrnSize[4];
	CEuresysChannel * NewChannel();
	// //Grabber���� �޾ƿ� ��ġ
	CMilBuffer *m_pMilBuffer;
};

#endif