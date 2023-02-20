#pragma once
#ifdef _MULTICAM
#include "..\stdafx.h"
#include <MultiCam.h>
#include "..\MilBase.h"

class CMilBuffer;

class MIL_DLLDECL CEuresysChannel
{
public:
	CEuresysChannel();
	~CEuresysChannel();
	int m_nChannel;
	int Init();
	MCHANDLE m_Channel;
	int m_nExposeTime;
	int Grab();
	int GrabContinuous();
	int GrabStop();
	void Callback(PMCSIGNALINFO SigInfo);
//	int SetMilBuffer(CMilBuffer * pMilBufProc, CMilBuffer * pMilBufDisp = NULL);
	MIL_ID m_milIDApplication;
	MIL_ID m_milIDSystem;
	MIL_ID m_milIDBufProc;
	MIL_ID m_milIDBufDisp;
	BOOL	m_bIsGrabDone;
	PVOID m_pCurrent; //!�����ڿ��� NULL�� ������ ������ �����߻�!!!!
	void SetExpTime(int nExpTime);
	BOOL IsGrabDone();
	void Close();
};
#endif
