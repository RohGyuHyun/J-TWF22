#pragma once



// CSCSockThread

class CSCSockThread : public CWinThread
{
	DECLARE_DYNCREATE(CSCSockThread)

protected:
	CSCSockThread();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CSCSockThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};


