
// HistoCompo.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CHistoCompoApp:
// �� Ŭ������ ������ ���ؼ��� HistoCompo.cpp�� �����Ͻʽÿ�.
//

class CHistoCompoApp : public CWinApp
{
public:
	CHistoCompoApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CHistoCompoApp theApp;