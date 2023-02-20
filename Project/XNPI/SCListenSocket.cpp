// SCListenSocket.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "XNPI.h"
#include "SCListenSocket.h"
#include "SCClientSocket.h"


// CSCListenSocket

CSCListenSocket::CSCListenSocket()
{
}

CSCListenSocket::~CSCListenSocket()
{
}


// CSCListenSocket ��� �Լ�


void CSCListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	CSCClientSocket *pClient = new CSCClientSocket;
	if (Accept((*pClient)))
	{
//		pClient->SetRecvWnd(m_p);
		m_lstClientSockList.AddTail(pClient);
	}
	else
	{
		delete pClient;
	}
	CAsyncSocket::OnAccept(nErrorCode);
}


void CSCListenSocket::CloseClientSock(CSCClientSocket * pClient)
{
	POSITION pos;
	pos = m_lstClientSockList.Find(pClient);
	if (pos != NULL)
	{
		if (pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();

		}
		m_lstClientSockList.RemoveAt(pos);
		delete pClient;
	}
}
