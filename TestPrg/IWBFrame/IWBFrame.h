
// IWBFrame.h : IWBFrame ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CIWBFrameApp:
// �� Ŭ������ ������ ���ؼ��� IWBFrame.cpp�� �����Ͻʽÿ�.
//

class CIWBFrameApp : public CWinApp
{
public:
	CIWBFrameApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CIWBFrameApp theApp;
