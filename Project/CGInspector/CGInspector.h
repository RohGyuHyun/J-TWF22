
// CGInspector.h : CGInspector ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CCGInspectorApp:
// �� Ŭ������ ������ ���ؼ��� CGInspector.cpp�� �����Ͻʽÿ�.
//

class CCGInspectorApp : public CWinApp
{
public:
	CCGInspectorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	BOOL m_bOnline;
	int InitVision();
	void InitEnvInstance();
};

extern CCGInspectorApp theApp;
