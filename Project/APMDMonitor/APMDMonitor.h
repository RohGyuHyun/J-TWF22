
// APMDMonitor.h : APMDMonitor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.
#include "Repository.h"


// CAPMDMonitorApp:
// �� Ŭ������ ������ ���ؼ��� APMDMonitor.cpp�� �����Ͻʽÿ�.
//

class CAPMDMonitorApp : public CWinAppEx
{
public:
	CAPMDMonitorApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void InitEnvInstance();
};

extern CAPMDMonitorApp theApp;
