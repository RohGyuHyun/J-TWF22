#pragma once
#include "afxcoll.h"
class CSCClientSocket;

// CSCListenSocket ��� ����Դϴ�.

class CSCListenSocket : public CAsyncSocket
{
public:
	CSCListenSocket();
	virtual ~CSCListenSocket();
	CPtrList m_lstClientSockList;
	virtual void OnAccept(int nErrorCode);
	void CloseClientSock(CSCClientSocket * pClient);
};


