
// AGMDMonitor.h : AGMDMonitor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CAGMDMonitorApp:
// �� Ŭ������ ������ ���ؼ��� AGMDMonitor.cpp�� �����Ͻʽÿ�.
//

class CAGMDMonitorApp : public CWinAppEx
{
public:
	CAGMDMonitorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	void InitEnvInstance();
	DECLARE_MESSAGE_MAP()
};

extern CAGMDMonitorApp theApp;
