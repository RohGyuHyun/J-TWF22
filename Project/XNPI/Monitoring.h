#pragma once

/**
\brief �ý��� ��ü������ ����͸��ϴ� ������ �ð� �ȴ�.Emergency���� ���� ���¸� �����ϰ� Alarm�� ���� ���� ��ġ�� ���Ѵ�.@n 
\date 2010.01.05
\author �� ����
*/
class CMonitoring
{
public:
	CMonitoring(void);
	~CMonitoring(void);
	static UINT ThreadMonitoring(LPVOID pParam);
	int Start(void);
	static CMonitoring* Instance(void);
	static CMonitoring* m_pInstance;
	static BOOL m_bRun;
	void Stop(void);
};
