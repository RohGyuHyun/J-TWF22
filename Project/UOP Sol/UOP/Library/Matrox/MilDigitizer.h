#pragma once
#include <mil.h>
#include "MilClass.h"

#define METEOR_CREVIS_DCF _T("JMV-BS20A.dcf")

class CMilBuffer;
class CGrabChannel;
class CMilSystem;
/*!
\brief 
* Mil�� Migitizer����  �Լ����� Classȭ�Ͽ���
* Digitizer�� Channel������ �ش��ϴ� Class�̴�
@author ������
*/
class MIL_DLLDECL CMilDigitizer:public CMilBase
{
public:
	CMilDigitizer(void);
	~CMilDigitizer(void);

	int Create(CMilApplication * pMilApplication, CMilSystem *pMilSystem);
	int Alloc(void);
	int SetContinuous(void);
	int Inquire(void);
	int GetResolution(CSize& Rect);
	MIL_ID m_milIDApplication, m_milIDSystem, m_milIDDigitizer;
	CMilSystem *m_pMilSystem;
	int SetBuffer(CMilBuffer *pMilBuffer);
	CMilBuffer *m_pMilBuffer;///Grab�� �̹����� ���� MiBuffer
	int Close(void);
	int Grab(void);
	int StopGrab(void);
	int StopGrabContinuous(void); 
	int SetupTrigger(BOOL OnOff);
	int GrabTrigger(void);
	CMilDigitizer* m_pInstance;
	CString m_strCameraFile;
	CGrabChannel *m_pGrabChannel;
	CMilBuffer * GetGrabBuffer(void);
};
