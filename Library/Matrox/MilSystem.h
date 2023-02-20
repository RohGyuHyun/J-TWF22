#pragma once
#include "MilClass.h"
#include "MilBase.h"
#include <vector>
#include ".\Grabber\Grabber.h"

class CGrabber;
class CMilDigitizer;
class CMilSystem;
using namespace std;
/*!
\brief 
* Mil�� System���� �Լ��� Classȭ��, �⺻������ 1���� �־�� �ϸ�, �߰��Ǵ°��� Grabber�� �����ϸ� ��.
@author ������
*/
class MIL_DLLDECL CMilSystem:public CMilBase
{
public:
	CMilSystem(void);
	CMilSystem(CMilApplication *pMilapplication);
	~CMilSystem(void);
	int Close(void);
	static CMilSystem * Instance(TCHAR* szGrabber);
	static CMilSystem * Instance(void);

	int Create(TCHAR *szGrabber=_T("NONE"));
	int Create(CMilApplication *pMilApplication);

	static CMilApplication *m_pMilApplication;
	MIL_ID m_milIDApplication;
	MIL_ID m_milIDSystem;
	static vector<CMilSystem *> m_vMilSystem;
	CMilSystem *m_pMilSystem;
	MIL_ID GetSystemID(void);
	void SetSystemID(MIL_ID idSystem);

	CGrabber *m_pGrabber;
	int GetDongleType(void);
	CString m_strGrabberName;
	CMilDigitizer* NewDigitizer(TCHAR *pCamFile);
};
