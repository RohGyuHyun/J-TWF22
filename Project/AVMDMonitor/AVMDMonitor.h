
// AVMDMonitor.h : AVMDMonitor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CAVMDMonitorApp:
// �� Ŭ������ ������ ���ؼ��� AVMDMonitor.cpp�� �����Ͻʽÿ�.
//

class CAVMDMonitorApp : public CWinAppEx
{
public:
	CAVMDMonitorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	void InitEnvInstance();
};

extern CAVMDMonitorApp theApp;
