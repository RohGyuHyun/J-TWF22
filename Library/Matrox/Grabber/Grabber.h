#pragma once
#include "..\stdafx.h"
#include "..\MilBase.h"
#include <mil.h>
#include <tchar.h>

class CMilBuffer;
class CMilSystem;
class CGrabChannel;

/*!
\brief 
* Grabber의 기초 Interface Class 
@author 전병근
*/
class MIL_DLLDECL CGrabber:public CMilBase
{
public:
	CGrabber(void);
	~CGrabber(void);
	CGrabber * Create(TCHAR* szGrabber=_T("NONE"));
	CString m_strGrabberName;
	static CGrabber *m_pInstance;
protected:
	CMilBuffer *m_pMilBuffer;
public:
	MIL_ID m_milIDSystem;
	int Alloc(void);
	virtual int Init(void);
	virtual CGrabChannel* NewChannel(TCHAR * szCamFile);
	static int m_nGrabber;
	CMilSystem *m_pMilSystem;
	virtual void Close();
};
