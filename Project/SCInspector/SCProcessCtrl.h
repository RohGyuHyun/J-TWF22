#pragma once
#include <UniIOPort.h>
class CCamDlg;

/** 
\brief 
I/O����̴� �˺��̾�������̴� ���� �۾� Process�� �����ϴ� Class�� Threadó���� �Ǿ� �־� backgroud�� �����̾� ����Ȥ�� WT���� ���, ���� �� ���� ��庰�� ������ �����صд�.@n
�˻籸�� ������ ������ Grab sequence�� �����Ѵ�.@n
\date 2017.08.31
\author �� ����
*/
class CSCProcessCtrl
{
public:
	enum tSCInspMachineType{MA_1, MA_NONE};
	enum tProcessMode{PROCESS_STOP, PROCESS_INSP};
	CSCProcessCtrl(void);
	~CSCProcessCtrl(void);
	virtual int CheckAndDo(void);
	virtual HANDLE MakeWaitableTimer(int setTime);
	HANDLE m_hWaitableTimer;
	static CSCProcessCtrl* Instance(tSCInspMachineType nType=MA_1);
	static UINT ThreadProc(LPVOID pParam);
	static CSCProcessCtrl *m_pInstance;
	int m_nCountOfCam;
	int m_Run;
	int InitThread(void);
	void StageProcess(int nStage, int *piStageStep);
	void ManStageProcess(int nStage, int * piStageStep);
	void AutoStageProcess(int nStage, int * piStageStep);
	int IsExitCdt(int nStage);
	CCamDlg **m_pCamDlg;
	int Stop();
	void StageProcessClear(int nStage);
	int DoInspect(int nStage, int nPos, BOOL bSave);
};
