#pragma once

// CSCClientSocket ��� ����Դϴ�.

class CSCClientSocket : public CSocket
{
public:
	CSCClientSocket();
	virtual ~CSCClientSocket();
	void SetRecvWnd(CWnd * pWnd);
	CWnd * m_pWnd;
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


