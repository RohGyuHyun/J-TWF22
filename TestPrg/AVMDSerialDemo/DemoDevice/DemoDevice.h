
// DemoDevice.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CDemoDeviceApp:
// �� Ŭ������ ������ ���ؼ��� DemoDevice.cpp�� �����Ͻʽÿ�.
//

class CDemoDeviceApp : public CWinApp
{
public:
	CDemoDeviceApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CDemoDeviceApp theApp;