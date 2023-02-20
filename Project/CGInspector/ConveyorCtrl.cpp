#include "StdAfx.h"
#include "ConveyorCtrl.h"
#include "Status.h"
#include "CGInspIO.h"
#include "MessageWnd.h"
#include "TabDetailLogDlg.h"
#include "MainFrm.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "CamDlg.h"
#include <UniMotion.h>


CConveyorCtrl* CConveyorCtrl::m_pConveyorCtrl=NULL;

CConveyorCtrl::CConveyorCtrl(void)
: m_modeConv(0)
, m_Run(0)
, m_pView(NULL)
{
}

CConveyorCtrl::~CConveyorCtrl(void)
{
}

int CConveyorCtrl::CheckAndDo(void)
{
	static int iStep=0;
	static int Dir=CONV_DIR_STOP;
	static BOOL bReset=TRUE;
	static BOOL bFirst = TRUE;

	//! Manual 상태이면 컨베이어를 정지시키고 리턴함
	if(CStatus::OPAUTO != CStatus::Instance()->GetOpMode())
	{
		if (bFirst)
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor First stop!"), RGB(50, 50, 50), FALSE);
			ConvGo(CONV_DIR_STOP);
			Dir = CONV_DIR_STOP;
			bFirst = FALSE;
		}
		if(Dir!=CONV_DIR_STOP){
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor stop!"), RGB(50, 50, 50), FALSE);
			ConvGo(CONV_DIR_STOP);
			Dir=CONV_DIR_STOP;
		}
		bReset = TRUE;
		return 0;
	}

	bFirst = TRUE;
	ConvAuto(bReset);
	bReset = FALSE;
	return 0;
}

int CConveyorCtrl::SetConvMode(int Mode)
{
	return m_modeConv=Mode;
}

int CConveyorCtrl::PrepareConv(BOOL bReset)
{
	static int iStep=0;
	static HANDLE hDuration=0;
	if (CStatus::OPAUTO != CStatus::Instance()->GetOpMode())
	{
		iStep = 0;
		return 0;
	}
	if (bReset)
	{
		iStep = 0;
	}

	switch(iStep)
	{
	case 0:
		if (CCGInspIO::Instance()->GetConvOutSensor() == CIOPort::PORTON) {
			iStep = 2;
		}
		else
		{
			ConvGo(CONV_DIR_FWD);
			hDuration = MakeWaitableTimer(10000);
			iStep = 1;
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor Prepare"), RGB(50, 50, 50), FALSE);
		}
		break;
	case 1:
		if (WaitForSingleObject(hDuration, 0) == WAIT_OBJECT_0)
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor stop for empty time"), RGB(50, 50, 50), FALSE);
			iStep = 2;
		}
		if (CCGInspIO::Instance()->GetConvOutSensor() == CIOPort::PORTON) {
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor stop for Out Detected"), RGB(50, 50, 50), FALSE);
			iStep = 2;
		}
		break;
	case 2:
		CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor stop!"), RGB(50, 50, 50), FALSE);
		ConvGo(CONV_DIR_STOP);
		ClearConvStatus();
		iStep = 3;
		break;
	case 3:
		break;
	}
	if (iStep == 3)return 1;
	else return 0;
}


void CConveyorCtrl::ConvAuto(BOOL bReset)
{
	static HANDLE hDuration = 0;
	static int iStep=0;
	static int iPrevStep = 0;
	static int iInSensor = 0;
	static int iOutSensor = 0;

	if (CStatus::OPAUTO != CStatus::Instance()->GetOpMode())
	{
		iStep = 0;
		return;
	}
	if (bReset)
	{
		iStep = 0;
	}

	if (CCGInspIO::Instance()->GetTackInSensor())iInSensor = GetTickCount();
	if (CCGInspIO::Instance()->GetTackOutSensor())iOutSensor = GetTickCount();

	CString strTime;
	strTime.Format(_T("Tack time:%d"), iOutSensor-iInSensor);

	switch (iStep)
	{
	case 0:
		if (PrepareConv(bReset))
		{
			iStep=1;
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor Ready"), RGB(50, 50, 50), FALSE);
		}
		break;
	case 1:
		m_pView->SetCameraOPMode(CStatus::OPAUTO);
		iStep = 2;
		break;
	case 2:
		if (CCGInspIO::Instance()->GetConvOutSensor() && (CStatus::Instance()->m_bActRevInsertMode))iStep = 3;//Reverse Mode
		CStatus::Instance()->ResetActRevInsertMode();
		if (CCGInspIO::Instance()->GetConvInSensor())
		{
			if (CCGInspIO::Instance()->GetConvOutSensor())
			{
				ConvGo(CONV_DIR_STOP);
				//MessageBox
				CMessageWnd::Instance()->SetMessageText(_T("There are Cover Glass in Output zone!!"));
			}
			else
			{
				CTabDetailLogDlg::Instance()->SetTextLog(_T("Detect board then Conveyor Go!"), RGB(50, 50, 50), FALSE);
				Sleep(1);
				ConvGo(CONV_DIR_FWD);
				iStep = 3;
			}
		}
		if (CCGInspIO::Instance()->GetConvOutSensor())iStep = 3;//Reverse Mode
		break;
	case 3://Rev 입력모드
		if (CCGInspIO::Instance()->GetConvOutSensor())
		{
			ConvGo(CONV_DIR_STOP);
			//! if RepConv모드일경 Conv를 후단입상태로 바꾼다. 아니면 멈춘다.
			if (CStatus::Instance()->m_bIsRepConv)
				CStatus::Instance()->SetActiveRevInsertMode();
			iStep = 4;
			if (iOutSensor > iInSensor)CTabDetailLogDlg::Instance()->Puts(strTime);
		}
		break;
	case 4:
		if (CStatus::Instance()->m_bIsRepConv)
			CStatus::Instance()->SetActiveRevInsertMode();
		if (CStatus::Instance()->IsRevInsertMode())
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Reverse Wait 1000"), RGB(50, 50, 50), FALSE);
			hDuration = MakeWaitableTimer(1000);
			iStep = 5;
		}
		break;
	case 5:
		if (WaitForSingleObject(hDuration, 0) == WAIT_OBJECT_0)
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor Reverse Insert Start!"), RGB(50, 50, 50), FALSE);
			ConvGo(CONV_DIR_BWD);
			iStep = 6;
		}
		break;
	case 6:
		if (CCGInspIO::Instance()->GetConvInSensor())
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Conveyor Reverse Stop Insensor"), RGB(50, 50, 50), FALSE);
			ConvGo(CONV_DIR_STOP);
			hDuration = MakeWaitableTimer(1000);
			iStep = 7;
		}
		break;
	case 7:
		if (WaitForSingleObject(hDuration, 0) == WAIT_OBJECT_0)
		{
			CTabDetailLogDlg::Instance()->SetTextLog(_T("Reverse Wait 1000"), RGB(50, 50, 50), FALSE);
			iStep = 2;
		}
		break;
	}
	iPrevStep = iStep;
}


int CConveyorCtrl::ConvGoVMode(int m_Dir)
{
	switch (m_Dir)
	{
	case CONV_DIR_BWD:
		for (int i = 0; i<4; i++)
			CMotionCtrl::Instance()->VeloMove(i, m_paramCvyMotion[0].vel, m_paramCvyMotion[0].acc);
		break;
	case CONV_DIR_FWD:
		for (int i = 0; i<4; i++)
			CMotionCtrl::Instance()->VeloMove(i, -m_paramCvyMotion[0].vel, m_paramCvyMotion[0].acc);
		break;
	case CONV_DIR_STOP:
		for (int i = 0; i<4; i++)
			CMotionCtrl::Instance()->MotionStop(i);
		break;
	}
	return 0;
}


int CConveyorCtrl::ConvGo(int modeDir)
{
	int nFrontAxis = CStatus::Instance()->m_nCvyrFrontAxis;
	int nEndAxis = CStatus::Instance()->m_nCvyrEndAxis;

	switch(modeDir)
	{
	case CONV_DIR_BWD:
		for(int i=0;i<4;i++)
			CMotionCtrl::Instance()->StartRelMove(i, m_paramCvyMotion[0].distance, 2*(m_paramCvyMotion[0].vel), m_paramCvyMotion[0].acc);
		break;
	case CONV_DIR_FWD:
		for (int i = 0; i<4; i++)
			CMotionCtrl::Instance()->StartRelMove(i, -m_paramCvyMotion[0].distance, m_paramCvyMotion[0].vel, m_paramCvyMotion[0].acc);
		break;
	case CONV_DIR_STOP:
		for (int i = 0; i < 4; i++)
		{
			CMotionCtrl::Instance()->MotionStop(i);
		}
		Sleep(100);
		for (int i = 0; i < 4; i++)
		{
			CMotionCtrl::Instance()->ClearStatus(i);
		}
		break;
	}
	return 0;
}

HANDLE CConveyorCtrl::MakeWaitableTimer(int setTime)
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

CConveyorCtrl* CConveyorCtrl::Instance(void)
{
	if(!m_pConveyorCtrl)m_pConveyorCtrl=new CConveyorCtrl;
	return m_pConveyorCtrl;
}

CConveyorCtrl* CConveyorCtrl::Instance(tCnvyorType nType)
{
	switch(nType)
	{
	case CONV_CGDEMO:
	if(!m_pConveyorCtrl)m_pConveyorCtrl=new CConveyorCtrl;
		break;
	case CONV_CGRETAIL:
		ASSERT(_T("\nCGRetail is not ready!!!"));
//	if(!m_pConveyorCtrl)m_pConveyorCtrl=new CCnvyor9800Ctrl;
		break;
	}

	return m_pConveyorCtrl;
}


int CConveyorCtrl::GetConvMode(void)
{
	return m_modeConv;
}

UINT CConveyorCtrl::ThreadConv(LPVOID pParam)
{
		CConveyorCtrl *pConvCtrl=(CConveyorCtrl *)pParam;

		while(pConvCtrl->m_Run)
		{
			pConvCtrl->CheckAndDo();
			Sleep(1);
		}
		pConvCtrl->ConvGo(CONV_DIR_STOP); 
		return 0;
}

int CConveyorCtrl::InitThread(void)
{
	m_Run=TRUE;
	SetConvMode(CONV_STOP);
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_pView = (CCGInspectorView *)pFrame->GetActiveView();
	AfxBeginThread(ThreadConv, (LPVOID)this);
	return 0;
}


int CConveyorCtrl::LoadCFG()
{
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, ProductName;

	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);
	m_paramCvyMotion = CStatus::Instance()->m_paramCvyMotion;

	ProductName = PRODUCT_NAME;
	MachineInputName = ProductName + _T("_INPUTSET");
	MachineOutputName = ProductName + _T("_OUTPUTSET");
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	// Front, End Conveyor Speed and Work End and Velocity, Max Speed parameter Loading
	m_paramCvyMotion[0].distance = pMyCfg->Get(2500000L, (TCHAR*)(LPCTSTR)MachineInputName,	_T("FrontConveyor-Distance-"));
	m_paramCvyMotion[0].vel = pMyCfg->Get(180000L, (TCHAR*)(LPCTSTR)MachineInputName,		_T("FrontConveyor-Velocity-"));
	m_paramCvyMotion[0].acc = pMyCfg->Get(20L, (TCHAR*)(LPCTSTR)MachineInputName,		_T("FrontConveyor-Accelerate-"));
	m_paramCvyMotion[1].distance = pMyCfg->Get(2500000L, (TCHAR*)(LPCTSTR)MachineInputName,	_T("EndConveyor-Distance-"));
	m_paramCvyMotion[1].vel = pMyCfg->Get(180000L, (TCHAR*)(LPCTSTR)MachineInputName,		_T("EndConveyor-Velocity-"));
	m_paramCvyMotion[1].acc = pMyCfg->Get(20L, (TCHAR*)(LPCTSTR)MachineInputName,		_T("EndConveyor-Accelerate-"));

	return 0;
}


int CConveyorCtrl::SaveCFG()
{
	CString MachineInputName, MachineOutputName, MachineInputLableName, MachineOutputLabelName, strProductName;
	CCfgFile* pMyCfg = CCfgFile::Instance();
	pMyCfg->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pMyCfg->Open();
	pMyCfg->SetDepth(2);

	strProductName = PRODUCT_NAME;
	MachineInputName.Format(_T("%s%s"), strProductName, _T("_INPUTSET"));
	MachineOutputName.Format(_T("%s%s"), strProductName, _T("_OUTPUTSET"));
	MachineInputLableName = MachineInputName + _T("_NAME");
	MachineOutputLabelName = MachineOutputName + _T("_NAME");

	//! Front, End Conveyor Speed and Work End and Velocity, Max Speed parameter Saving
	pMyCfg->Set(m_paramCvyMotion[0].distance, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Distance-"));
	pMyCfg->Set(m_paramCvyMotion[0].vel, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Velocity-"));
	pMyCfg->Set(m_paramCvyMotion[0].acc, (TCHAR*)(LPCTSTR)MachineInputName, _T("FrontConveyor-Accelerate-"));
	pMyCfg->Set(m_paramCvyMotion[1].distance, (TCHAR*)(LPCTSTR)MachineInputName, _T("EndConveyor-Distance-"));
	pMyCfg->Set(m_paramCvyMotion[1].vel, (TCHAR*)(LPCTSTR)MachineInputName, _T("EndConveyor-Velocity-"));
	pMyCfg->Set(m_paramCvyMotion[1].acc, (TCHAR*)(LPCTSTR)MachineInputName, _T("EndConveyor-Accelerate-"));

	pMyCfg->Flush();
	return 0;
}

int CConveyorCtrl::ClearConvStatus()
{
	for (int i = 0; i < 4; i++)
	{
		CMotionCtrl::Instance()->ClearStatus(i);
	}
	return 0;
}

int CConveyorCtrl::Stop()
{
	m_Run = FALSE;
	return 0;
}