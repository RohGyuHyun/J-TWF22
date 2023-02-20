#pragma once

#include <mil.h>

#ifdef _MATROX_DLL
#define MIL_DLLDECL __declspec(dllexport)
#else
#define MIL_DLLDECL __declspec(dllimport)

#ifndef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "Matrox32")
#else
#pragma comment(lib, "MatroxD32")
#endif
#else
#ifndef _DEBUG
#pragma comment(lib, "Matrox64")
#else
#pragma comment(lib, "MatroxD64")
#endif
#endif
#endif

#define GB_BLUEEYE	_T("BLUEEYE4A")
#define GB_BASLER	_T("BASLER")
#define GB_METER4M	_T("METEROR4M")
#define GB_SOLIOS	_T("SOLIOS")
#define GB_NONE		_T("NONE")
#define GB_NI		_T("NI")
#define GB_PAXSCAN	_T("PAXSCAN")
#define GB_XMARU	_T("XMARU")
#define GB_EURESYS  _T("EURESYS")

/*!
\brief
* 이미지의 Gray level windowing의 파라메터전달을 위해사용(CMilBuffer::WndLvlTranform)
*/
struct tWndLv
{
	long OrgStart, OrgEnd, DestStart, DestEnd;
};

/*!
\brief 
* 모든 Class의 Base Class로 공용의 자료활용에 사용됨
@author 전병근
*/
class MIL_DLLDECL CMilBase
{
public:
	CMilBase(void);
	~CMilBase(void);
	static MIL_ID m_milIDApplication;
};

#define WM_REQ_INSP WM_USER+200+1
