#pragma once
#include <mil.h>

#define MERR_APPLICATION 
/*!
\brief 
* Mil�� Error Report�� ���� �߰��� Class
@author ������
*/
class CMilErr
{
public:
	CMilErr(void);
	CMilErr(int nErrCode)
	{
		m_LastError=nErrCode;
	}
	~CMilErr(void);

	int GetLastErrCode(void)
	{
		return 0;
	}

	CString GetLastErrString(void)
	{
		//switch(m_milLastErr)
		//{
		//case :
		//	return CString("Application is not initialize");
		//	break;
		//	case
		//}
		return CString();
	}

	int SetErrCode(MIL_ID MilID)
	{
		return 0;
	}
	MIL_ID m_LastError;
};
