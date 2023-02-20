#pragma once
#include "TimeCheck.h"


#define MAX_CHAR					512
#define MOT_PROCESS_MAX_NUM							1
#define MOT_STATUS_MAX_NUM							3
#define MOT_STATUS_SUB_MAX_NUM						10000


#define SEQ_STATUS_STEP1			0
#define SEQ_STATUS_STEP2			1
#define SEQ_STATUS_STEP3			2
#define SEQ_STATUS_STEP4			3
#define SEQ_STATUS_READY			5
#define SEQ_STATUS_END				6

#define TIMEOUT_SAFE_INTERLOCK		10

//Motion Process Status///////////////////////////////////////////////////////
typedef struct MotionProcessStatus
{
	//Timeout
	double m_dTimeoutS;//���� �ð�
	double m_dTimeoutE;//���� �ð�(����)
	double m_dTimeout;//������ �ð�

	//Status����(���� �Ǵ� ��������� ����� ���ڿ�)
	char m_cNGStatus[MAX_CHAR];
	char m_cNGCode[MAX_CHAR];
	char m_cTypeStatus[MAX_CHAR];

	int m_iNGSeqIdx;

}typeMotionProcessSts;


//Motion Sequence///////////////////////////////////////////////////////
typedef struct MotionSeq
{
	//Sequence
	int m_iSubSeqIdx;
	int m_iNextSubSeqIdx;

	//Flag
	BOOL m_bAutoStart;
	BOOL m_bAlarm;

	//��ư�߽��� ���ۻ���
	BOOL m_bOpSts;

}typeMotionSeq;

class CMotionData
{
public:
	CMotionData(void);
	~CMotionData(void);

	//Motion process status
	typeMotionProcessSts	m_ProSts[MOT_STATUS_MAX_NUM][MOT_STATUS_SUB_MAX_NUM];

	//Motion sequence
	typeMotionSeq			m_Seq[MOT_STATUS_MAX_NUM];

	void ResetSeq();
};



//Motion flag///////////////////////////////////////////////////////
typedef struct MotionFlag
{
	BOOL m_bProcessThread;
	BOOL m_bAutoStart;
	BOOL m_bFirstStart;
	BOOL m_bAlarm;
	BOOL m_bAreaSensor;

}typeMotionFlag;



typedef struct SequenceFlag
{
	BOOL					m_bInitialized;
	BOOL					m_bByPass;
	int						m_nSeqIndex;		// 0:�����ġ   1:����#1   2:����#2		3:����#3
	int						m_nMotSubSeqIdx;
	
	long					m_lAxisPos[4];		//AXIS_X, AXIS_Y, AXIS_Z1, AXIS_Z2
	long					m_lAxisVel[MOT_STATUS_MAX_NUM];

}typeSequenceFlag;


class CSequenceModule
{
public:
	CSequenceModule(void);
	~CSequenceModule(void);

protected:
	HWND					m_hWndMain;

public:
	typeSequenceFlag		m_SeqFlag;
	typeMotionFlag			m_Flag;
	CMotionData				m_MDat[MOT_PROCESS_MAX_NUM];

	//CLogFile				*m_pMotLog;

	BOOL m_bswStart[2];
	BOOL m_bSeqEnd;
	//20190315 ngh
	BOOL m_bSeqProgress;

	int m_nSeqChannel;
	int m_nGrabStepIndex;
	int m_nMaxStepCnt;
	int m_nSeqIdx;
	int m_nDstDeqIdx;
	BOOL m_bReadyState;
	BOOL m_bInspecting;

	CTimerCheck m_TimeCheck;

	BOOL m_bInitSeq;

	int m_nPrc_Idx;
	int m_nSeq_Idx;
	int m_nSeq_Sub_Idx;

	deque<CString> m_strMainLogString;
	int m_nInSensingTime;
	int m_nOutSensingTime;
public:
	//Motion Sequence
	int MotionProcess(int prc_idx, int seq_idx);
	int StopModeOperation(int *prc_idx, int *seq_idx);
	int CheckTimeout(int prc_idx, int seq_idx, int seq_sub_idx, char* status_type);
	int CheckDelay(int prc_idx, int seq_idx, int delay);
	void SetSeqIdx(int prc_idx, int seq_idx, int seq_sub_idx);
	void SequenceLoop();
	void Open(HWND hWnd, int nChannel, CListCtrl* listLog = NULL);
	void Close();

	void Reset();
	BOOL Start();
	void Stop();

	void SetLamp(int nColor);//, BOOL bBuzz);

	void InitSeq();

	void SetSeqFirst(BOOL bFlag) { m_Flag.m_bFirstStart = bFlag; }

	//20190215 ngh
	BOOL GetSeqEnd(){return m_bSeqEnd;};
	void SetSeqEnd(BOOL isEnd){m_bSeqEnd = isEnd;};

	//20190315 ngh
	BOOL GetSeqProgress(){return m_bSeqProgress;};

	void SetSeqChannel(int nChannel){m_nSeqChannel = nChannel;};
	void SetGrabStepIndex(int nIndex){m_nGrabStepIndex = nIndex;};
	int GetGrabStepIndex(){return m_nGrabStepIndex;};
	void SetMaxStepCnt(int nCnt){m_nMaxStepCnt = nCnt;};
	BOOL GetAutoStart(){return m_Flag.m_bAutoStart;};
	BOOL GetReadyState(){return m_bReadyState;};
	BOOL GetInspecting(){return m_bInspecting;};

	void GetSeqIdx(int *prc_idx, int *seq_idx, int *seq_sub_idx);
	int GetSeqSubIdx(int prc_idx, int seq_idx);


	void SetInSensingTime(int nTime) { m_nInSensingTime = nTime; };
	void SetOutSensingTime(int nTime) { m_nOutSensingTime = nTime; };

	void WriteLog(CString strLog);
};