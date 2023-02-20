#pragma once
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
	CCamDlg **m_pCamDlg;
	int Stop();
	int DoInspect(int nStage, int nPos, BOOL bSave);
};
