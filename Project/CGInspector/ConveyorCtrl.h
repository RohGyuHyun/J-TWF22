#pragma once
#include <UniIOPort.h>
#include "Defines.h"

class CCGInspectorView;


/** 
\brief 
�����̾��� ������ �����ϴ� Class�� Threadó���� �Ǿ� �־� backgroud�� �����̾� ������ ���� �� ���� ��庰�� ������ �����صд�.@n
�˻籸�� ������ ������ Grab sequence�� �����Ѵ�.@n
\date 2010.01.05
\author �� ����
*/
class CConveyorCtrl
{
public:
	enum tConvDir{CONV_DIR_FWD, CONV_DIR_BWD, CONV_DIR_STOP};
	enum tConvMode{CONV_DEMO, CONV_AUTO, CONV_STOP, CONV_PASS};
	enum tCnvyorType{CONV_CGDEMO, CONV_CGRETAIL};
	CConveyorCtrl(void);
	~CConveyorCtrl(void);
	tConvMotionParam *m_paramCvyMotion=NULL;
	virtual int CheckAndDo(void);
	int SetConvMode(int Mode);
	int PrepareConv(BOOL bReset);
	virtual int ConvGo(int modeDir);
	virtual HANDLE MakeWaitableTimer(int setTime);
	HANDLE m_hWaitableTimer;
	int m_modeConv;
	static CConveyorCtrl* Instance(void);
	static CConveyorCtrl* Instance(tCnvyorType nType);
	static CConveyorCtrl* m_pConveyorCtrl;
	int GetConvMode(void);
	static UINT ThreadConv(LPVOID pParam);
	int m_Run;
	int InitThread(void);
	int LoadCFG();
	int SaveCFG();
	void ConvAuto(BOOL bReset);
	int ConvGoVMode(int m_Dir);
	int ClearConvStatus();
	int Stop();
	CCGInspectorView *m_pView;
};
