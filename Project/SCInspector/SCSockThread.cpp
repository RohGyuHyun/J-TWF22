// SCSockThread.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "SCInspector.h"
#include "SCSockThread.h"


// CSCSockThread

IMPLEMENT_DYNCREATE(CSCSockThread, CWinThread)

CSCSockThread::CSCSockThread()
{
}

CSCSockThread::~CSCSockThread()
{
}

BOOL CSCSockThread::InitInstance()
{
	// TODO:  ���⿡�� �� �����忡 ���� �ʱ�ȭ�� �����մϴ�.
	return TRUE;
}

int CSCSockThread::ExitInstance()
{
	// TODO:  ���⿡�� �� �����忡 ���� ������ �����մϴ�.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSCSockThread, CWinThread)
END_MESSAGE_MAP()


// CSCSockThread �޽��� ó�����Դϴ�.
