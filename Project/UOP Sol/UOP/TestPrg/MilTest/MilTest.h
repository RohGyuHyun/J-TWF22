
// MilTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMilTestApp:
// �� Ŭ������ ������ ���ؼ��� MilTest.cpp�� �����Ͻʽÿ�.
//

class CMilTestApp : public CWinAppEx
{
public:
	CMilTestApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMilTestApp theApp;