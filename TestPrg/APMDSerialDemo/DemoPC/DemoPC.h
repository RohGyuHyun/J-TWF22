
// DemoPC.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDemoPCApp:
// �� Ŭ������ ������ ���ؼ��� DemoPC.cpp�� �����Ͻʽÿ�.
//

class CDemoPCApp : public CWinApp
{
public:
	CDemoPCApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDemoPCApp theApp;