#pragma once
#include <UniIOPort.h>
#include "Defines.h"

class CCGInspectorView;


/** 
\brief 
컨베이어의 동작을 제어하는 Class로 Thread처리가 되어 있어 backgroud로 컨베이어 센서의 감시 및 각종 모드별로 동작을 정의해둔다.@n
검사구간 센서를 만나면 Grab sequence를 진행한다.@n
\date 2010.01.05
\author 전 병근
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
