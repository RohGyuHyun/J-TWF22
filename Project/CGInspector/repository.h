#pragma once
#include "ModelData.h"
#include "SCResult.h"
#include <Matrox.h>
class CRepository
{
public:
	CRepository();
	~CRepository();
	static CRepository * Instance();
	static CRepository *m_pInstance;
	CMilVLib m_MilLib;
//	CEuresysGrabber m_EuresysGrabber;
	CMatroxGrabber m_Grabber[2];
	int WriteInspResult(CSCResult *pResult);
	void WriteResultFile(CString strFileName, CSCResult *pResult);
};

