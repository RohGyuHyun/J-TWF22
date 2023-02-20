// SCClientSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XFMS.h"
#include "SCClientSocket.h"


// CSCClientSocket

CSCClientSocket::CSCClientSocket()
	: m_pWnd(NULL)
{
}

CSCClientSocket::~CSCClientSocket()
{
}


// CSCClientSocket ��� �Լ�


void CSCClientSocket::SetRecvWnd(CWnd * pWnd)
{
	m_pWnd = pWnd;
}


void CSCClientSocket::OnReceive(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_pWnd->SendMessage(WM_CAM_REMOTE, (WPARAM)this);
	CSocket::OnReceive(nErrorCode);
}


void CSCClientSocket::OnClose(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_pWnd->PostMessage(WM_SOCK_LISTEN);
	CSocket::OnClose(nErrorCode);
}
