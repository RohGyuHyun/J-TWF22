#pragma once

#ifdef _UNIMOTION_DLL
#define UNIMOTION_DLLDECL __declspec(dllexport)
#else
#define UNIMOTION_DLLDECL __declspec(dllimport)

#ifndef _WIN64
#ifndef _DEBUG
#pragma comment(lib, "UniMotion32")
#else
#pragma comment(lib, "UniMotionD32")
#endif
#else
#ifndef _DEBUG
#pragma comment(lib, "UniMotion64")
#else
#pragma comment(lib, "UniMotionD64")
#endif
#endif

#endif


class CUniMotionBase
{
public:
	CUniMotionBase(void);
	~CUniMotionBase(void);
};
