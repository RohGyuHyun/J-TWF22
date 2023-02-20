#include "StdAfx.h"
#include "ProcessLogicCtrl.h"
#include "Status.h"
#include "CGInspIO.h"
#include "CamDlg.h"
#include "TabDetailLogDlg.h"
#include "SCResult.h"
#include "Repository.h"

CProcessLogicCtrl* CProcessLogicCtrl::m_pInstance=NULL;

CProcessLogicCtrl::CProcessLogicCtrl(void)
: m_Run(0)
, m_nCountOfCam(0)
{
}

CProcessLogicCtrl::~CProcessLogicCtrl(void)
{
}

int CProcessLogicCtrl::CheckAndDo(void)
{
	static int iStep=0;
	static int InScan=FALSE;
	static int StageStep[4];

	if(CStatus::OPAUTO != CStatus::Instance()->GetOpMode())
	{
		iStep = 0;
		return 0;
	}

	if (iStep == 0)
	{
		for (int i = 0; i < 4; i++)
			StageStep[i] = 0;
		iStep = 1;
	}

	int nCountOfStage = CStatus::Instance()->m_nCountOfStage;
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;
	if (nCountOfCam == 0) return 0;
	m_nCountOfCam = nCountOfCam;
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();

	
	for (int i = 0; i < nCountOfStage; i++)
	{
//		StageProcess(i, &StageStep[i]);
	}
	return 0;
}

HANDLE CProcessLogicCtrl::MakeWaitableTimer(int setTime)
{
	if(m_hWaitableTimer==NULL)
		m_hWaitableTimer=CreateWaitableTimer(NULL, FALSE, NULL);

	LARGE_INTEGER myLI;
	myLI.QuadPart =-1 * setTime *10000;
	if(!SetWaitableTimer(m_hWaitableTimer, &myLI, 0,  NULL, NULL, 0))
	{
		TRACE("Waitable Timer Set Error");
		return NULL;
	}

	return m_hWaitableTimer;
}

CProcessLogicCtrl* CProcessLogicCtrl::Instance(tSCInspMachineType nType)
{
	if (m_pInstance)return m_pInstance;
	switch(nType)
	{
	case MA_1:
	if(!m_pInstance)m_pInstance=new CProcessLogicCtrl;
		break;
	default:
		if (!m_pInstance)m_pInstance = new CProcessLogicCtrl;
		break;
	}

	return m_pInstance;
}


UINT CProcessLogicCtrl::ThreadProc(LPVOID pParam)
{
		CProcessLogicCtrl *pProcessCtrl=(CProcessLogicCtrl *)pParam;

		while(pProcessCtrl->m_Run)
		{
			pProcessCtrl->CheckAndDo();
			Sleep(1);
		}
		return 0;
}

int CProcessLogicCtrl::InitThread(void)
{
	m_Run=TRUE;
//	SetConvMode(CONV_STOP);
	AfxBeginThread(ThreadProc, (LPVOID)this);
	return 0;
}


void CProcessLogicCtrl::ManStageProcess(int nStage, int * piStageStep)
{
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();

}


void CProcessLogicCtrl::AutoStageProcess(int nStage, int *piStageStep)
{
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();
	static int nProcPos[4] = { 0,0,0,0 };
	int nCountOfPos = CStatus::Instance()->m_nCountOfPos;

}


int CProcessLogicCtrl::IsExitCdt(int nStage)
{
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();
	return 0;
}


int CProcessLogicCtrl::Stop()
{
	m_Run = FALSE;
	return 0;
}



void CProcessLogicCtrl::StageProcessClear(int nStage)
{
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();

}

int CProcessLogicCtrl::DoInspect(int nStage, int nPos, BOOL bSave)
{
	CCGInspIO *pCGInspIO = CCGInspIO::Instance();
	
	int nCountOfCam = CStatus::Instance()->m_nCountOfCam;

	BOOL flagNG = FALSE;

	for (int i = 0; i < nCountOfCam; i++)
	{
		while (!m_pCamDlg[i].IsGrabDone());
		if (!(m_pCamDlg[i].Inspect()))
		{
			//Good
//			if(CStatus::Instance()->m_bSaveAllImage)
//				if (bSave)m_pCamDlg[i]->SaveImage(FALSE);
		}
		else
		{
			//NG
			if(bSave)m_pCamDlg[i].SaveImage();
			flagNG = TRUE;
		}
	}

	return 0;
}
