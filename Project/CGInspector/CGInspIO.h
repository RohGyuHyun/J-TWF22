#pragma once
#include <UniIOPort.h>
struct tOSubSeq
{
	long CapDone;
	long NG;
};

struct tOSeq
{
	tOSubSeq Pos[6];
	long UnitInspDone;
};

struct tOStage
{
	long VisionReady;
	long VisionNotReady;
	tOSeq Seq;
};

struct tIStage
{
	long InSensor;
	long TackInSensor;
	long TackOutSensor;
	long OutSensor;
};


class CCGInspIO :
	public CIOPort
{
public:
	CCGInspIO();
	~CCGInspIO();
	BOOL GetEMG(int *pErrStatgeNo);
	void LoadEnv();
	static CCGInspIO * Instance();
	static CCGInspIO *m_pInstance;
	static CIOPort *m_pBoard;
	static CString m_strBoardName;

//Input
	tIStage m_nConveyor;

//Output
	tOStage m_nOStage;
	int m_nOSpare0;
	int m_nOSpare31;

	void LoadPortNo();
	void SavePortNo();
	int Init();
	BOOL IsEMG(int nStage=-1);
	BOOL IsERR(int nStage=-1);
	BOOL GetErr(int* pErrStageNo);
	void Close();
	void SetAllOutputClear();
	BOOL GetMotionDone(int nStage, int *pErrStageNo);
	int GetConvOutSensor();
	int GetConvInSensor();
	int GetTackInSensor();
	int GetTackOutSensor();
};

