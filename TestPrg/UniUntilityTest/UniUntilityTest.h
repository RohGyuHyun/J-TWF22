
// UniUntilityTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CUniUntilityTestApp:
// �� Ŭ������ ������ ���ؼ��� UniUntilityTest.cpp�� �����Ͻʽÿ�.
//

class CUniUntilityTestApp : public CWinApp
{
public:
	CUniUntilityTestApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CUniUntilityTestApp theApp;