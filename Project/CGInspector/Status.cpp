#include "stdafx.h"
#include "SCAlgoBase.h"
#include "SCFidAlgo.h"
#include "Status.h"
#include "ModelData.h"
#include "SCFidParamDlg.h"
#include "CGInspIO.h"
#include "CGInspectorDoc.h"
#include "CGInspectorView.h"
#include "MainFrm.h"
#include "CamDlg.h"

CStatus * CStatus::m_pInstance = NULL;
CStatus::CStatus()
	: m_iSelCam(0)
	, m_strModelRepository(_T(""))
	, m_nCountOfCam(2)
	, m_pSCInspPacket(NULL)
	, m_pModelData(NULL)
	, m_nInspTypeToAdd(0)
	, m_ProductName(_T("SCInspector"))
	, m_opMode(OPSTOP)
	, m_bIsOffLine(FALSE)
	, m_strNGSaveFolder(_T(""))
	, m_MsgWndOpen(FALSE)
	, m_nCountOfStage(0)
	, m_nCountOfPos(6)
	, m_bSaveTestImage(FALSE)
	, m_bDrawCross(FALSE)
	, m_bUseDelOldImg(0)
	, m_nDaysDel(0)
	, m_bIsRepConv(FALSE)
	, m_bIsDataAqu(FALSE)
	, m_bActRevInsertMode(FALSE)
	, m_nCvyrFrontAxis(0)
	, m_nCvyrEndAxis(1)
	, m_strDataSaveFolder(_T(""))
	, m_bDrawDefect(TRUE)
{
	m_strWorkSpaceRuntime = _T("CGInspector.vrws");
}


CStatus::~CStatus()
{
}


CStatus * CStatus::Instance()
{
	if (!m_pInstance)m_pInstance = new CStatus;
	return m_pInstance;
}

void CStatus::SetOPMode(tOpMode opMode)
{
	m_opMode = opMode;
	switch (m_opMode)
	{
	case OPAUTO:
		CCGInspIO::Instance()->SetAllOutputClear();
		break;
	}
}

int CStatus::GetOpMode(void)
{
	return m_opMode;
}

void CStatus::SetSelCam(int nCam)
{
	m_iSelCam = nCam;
}


int CStatus::GetSelCamNo()
{
	return m_iSelCam;
}


CString CStatus::GetModelRepository()
{
	return m_strModelRepository;
}


void CStatus::SetModelRepository(TCHAR * szDataSaveFolder)
{
	m_strModelRepository = szDataSaveFolder;
}

CString CStatus::GetModelName()
{
	return m_strModelName;
}


void CStatus::SetModelName(TCHAR * szModelName)
{
	m_strModelName = szModelName;
}

int CStatus::SaveCFG()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->Open();
	pCfgFile->SetDepth(2);
	pCfgFile->Set(m_iBaudRate, _T("COMSET"), _T("BaudRate"));


	pCfgFile->Set(m_strModelRepository.GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	pCfgFile->Set(m_strModelName.GetBuffer(), _T("Environment"), _T("ModelName"));
	pCfgFile->Set(m_strNGSaveFolder.GetBuffer(), _T("Environment"), _T("NGSaveFolder"));

	pCfgFile->Set(m_nCvyrFrontAxis, _T("Motion"), _T("ConveyorFrontAxisNo"));
	pCfgFile->Set(m_nCvyrEndAxis, _T("Motion"), _T("ConveyorEndAxisNo"));

	pCfgFile->SetDepth(3);

	pCfgFile->Set(m_CamFov[0].dblCX, _T("CamSet"), _T("CamNo1"), _T("FovX"));
	pCfgFile->Set(m_CamFov[0].dblCY, _T("CamSet"), _T("CamNo1"), _T("FovY"));
	pCfgFile->Set(m_CamFov[1].dblCX, _T("CamSet"), _T("CamNo2"), _T("FovX"));
	pCfgFile->Set(m_CamFov[1].dblCY, _T("CamSet"), _T("CamNo2"), _T("FovY"));
	pCfgFile->Set(m_CamFov[2].dblCX, _T("CamSet"), _T("CamNo3"), _T("FovX"));
	pCfgFile->Set(m_CamFov[2].dblCY, _T("CamSet"), _T("CamNo3"), _T("FovY"));
	pCfgFile->Set(m_CamFov[3].dblCX, _T("CamSet"), _T("CamNo4"), _T("FovX"));
	pCfgFile->Set(m_CamFov[3].dblCY, _T("CamSet"), _T("CamNo4"), _T("FovY"));

	pCfgFile->Set(m_lCamExpTime[0], _T("CamSet"), _T("CamNo1"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[1], _T("CamSet"), _T("CamNo2"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[2], _T("CamSet"), _T("CamNo3"), _T("ExposeTime"));
	pCfgFile->Set(m_lCamExpTime[3], _T("CamSet"), _T("CamNo4"), _T("ExposeTime"));

	m_iCAM_HRES[0] = m_sizeTopCam.cx;
	m_iCAM_VRES[0] = m_sizeTopCam.cy;
	m_iCAM_HRES[1] = m_sizeBottomCam.cx;
	m_iCAM_VRES[1] = m_sizeBottomCam.cy;
	m_iCAM_HRES[2] = m_sizeSideLeftCam.cx;
	m_iCAM_VRES[2] = m_sizeSideLeftCam.cy;
	m_iCAM_HRES[3] = m_sizeSideRightCam.cx;
	m_iCAM_VRES[3] = m_sizeSideRightCam.cy;

	 pCfgFile->Set(m_sizeTopCam.cx, _T("CamSet"), _T("TopCam"), _T("CX"));
	 pCfgFile->Set(m_sizeTopCam.cy, _T("CamSet"), _T("TopCam"), _T("CY"));
	 pCfgFile->Set(m_sizeBottomCam.cx, _T("CamSet"), _T("BottomCam"), _T("CX"));
	 pCfgFile->Set(m_sizeBottomCam.cy, _T("CamSet"), _T("BottomCam"), _T("CY"));
	 pCfgFile->Set(m_sizeSideLeftCam.cx, _T("CamSet"), _T("SideCamLeft"), _T("CX"));
	 pCfgFile->Set(m_sizeSideLeftCam.cy, _T("CamSet"), _T("SideCamLeft"), _T("CY"));
	 pCfgFile->Set(m_sizeSideRightCam.cx, _T("CamSet"), _T("SideCamRight"), _T("CX"));
	 pCfgFile->Set(m_sizeSideRightCam.cy, _T("CamSet"), _T("SideCamRight"), _T("CY"));


	pCfgFile->Flush();
	return 0;
}


int CStatus::LoadCFG()
{
	CCfgFile *pCfgFile = CCfgFile::Instance();
	pCfgFile->SetPath(CAppInfo::GetAppDir() + CFG_FILE);
	pCfgFile->Open();
	pCfgFile->SetDepth(2);
	m_nCountOfCam = pCfgFile->Get(2L, _T("CamSet"), _T("nCountOfInstall"));
	m_nCountOfCamRemote = pCfgFile->Get(0L, _T("CamSet"), _T("nCountOfRemoteCam"));
	m_nCountOfStage = pCfgFile->Get(1L, _T("Stage"), _T("nCountOfInstall"));
	m_nCountOfPos = pCfgFile->Get(6L, _T("Stage"), _T("nCountOfPos"));

	m_nCvyrFrontAxis = pCfgFile->Get(0L, _T("Motion"), _T("ConveyorFrontAxisNo"));
	m_nCvyrEndAxis = pCfgFile->Get(1L, _T("Motion"), _T("ConveyorEndAxisNo"));

	m_bUseDelOldImg = pCfgFile->Get(0L, _T("DELOLDIMG"), _T("bIsUse"));
	m_nDaysDel = pCfgFile->Get(100L, _T("DELOLDIMG"), _T("nDaysToDel"));
	
	m_iBaudRate = pCfgFile->Get(9600L, _T("COMSET"), _T("BaudRate"));

	m_lIPAddr[0] = pCfgFile->Get(127L, _T("RemoteIP"), _T("Addr1"));
	m_lIPAddr[1] = pCfgFile->Get(0L, _T("RemoteIP"), _T("Addr2"));
	m_lIPAddr[2] = pCfgFile->Get(0L, _T("RemoteIP"), _T("Addr3"));
	m_lIPAddr[3] = pCfgFile->Get(1L, _T("RemoteIP"), _T("Addr4"));

	m_strModelRepository = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("DataSaveFolder"));
	m_strComportNo = pCfgFile->Get(_T("COM1"), _T("COMSET"), _T("ComportNo"));
	m_strNGSaveFolder = pCfgFile->Get(CAppInfo::GetAppDir().GetBuffer(), _T("Environment"), _T("NGSaveFolder"));
	m_strModelName = pCfgFile->Get(_T("SCInsp"), _T("Environment"), _T("ModelName"));
	pCfgFile->SetDepth(3);

	m_CamFov[0].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("TopCam"), _T("FovX"));
	m_CamFov[0].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("TopCam"), _T("FovY"));
	m_CamFov[1].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("BottomCam"), _T("FovX"));
	m_CamFov[1].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("BottomCam"), _T("FovY"));
	m_CamFov[2].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("SideCamLeft"), _T("FovX"));
	m_CamFov[2].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("SideCamLeft"), _T("FovY"));
	m_CamFov[3].dblCX = pCfgFile->Get(0.0, _T("CamSet"), _T("SideCamRight"), _T("FovX"));
	m_CamFov[3].dblCY = pCfgFile->Get(0.0, _T("CamSet"), _T("SideCamRight"), _T("FovY"));

	m_lCamExpTime[0] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo1"), _T("ExposeTime"));
	m_lCamExpTime[1] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo2"), _T("ExposeTime"));
	m_lCamExpTime[2] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo3"), _T("ExposeTime"));
	m_lCamExpTime[3] = pCfgFile->Get(2000L, _T("CamSet"), _T("CamNo4"), _T("ExposeTime"));

	m_iCAM_HRES[0] = m_sizeTopCam.cx		= pCfgFile->Get(8192L, _T("CamSet"), _T("TopCam"), _T("CX"));
	m_iCAM_VRES[0] = m_sizeTopCam.cy		= pCfgFile->Get(20000L, _T("CamSet"), _T("TopCam"), _T("CY"));
	m_iCAM_HRES[1] = m_sizeBottomCam.cx	= pCfgFile->Get(8192L, _T("CamSet"), _T("BottomCam"), _T("CX"));
	m_iCAM_VRES[1] = m_sizeBottomCam.cy	= pCfgFile->Get(18000L, _T("CamSet"), _T("BottomCam"), _T("CY"));
	m_iCAM_HRES[2] = m_sizeSideLeftCam.cx = pCfgFile->Get(2048L, _T("CamSet"), _T("SideCamLeft"), _T("CX"));
	m_iCAM_VRES[2] = m_sizeSideLeftCam.cy = pCfgFile->Get(18000L, _T("CamSet"), _T("SideCamLeft"), _T("CY"));
	m_iCAM_HRES[3] = m_sizeSideRightCam.cx = pCfgFile->Get(2048L, _T("CamSet"), _T("SideCamRight"), _T("CX"));
	m_iCAM_VRES[3] = m_sizeSideRightCam.cy = pCfgFile->Get(18000L, _T("CamSet"), _T("SideCamRight"), _T("CY"));

	return 0;
}

void CStatus::SetComportNo(TCHAR* szComportNo)
{
	m_strComportNo = szComportNo;
}

CString CStatus::GetComportNo()
{
	return m_strComportNo;
}

void CStatus::SetBaudRate(int iBaudRate)
{
	m_iBaudRate = iBaudRate;
}

int CStatus::GetBaudRate()
{
	return m_iBaudRate;
}

CSCInspPacket * CStatus::GetComm()
{
	return m_pSCInspPacket;
}


void CStatus::SetComm(CSCInspPacket * pSCInspPacket)
{
	m_pSCInspPacket = pSCInspPacket;
}


int CStatus::GetRVOption()
{
	return 0;
}


CModelData * CStatus::GetModelData()
{
	return m_pModelData;
}


void CStatus::SetModelData(CModelData * pModelData)
{
	m_pModelData = pModelData;
}


void CStatus::SetCurAddAlgoIndex(int nInspType)
{
	m_nInspTypeToAdd = nInspType;
}


int CStatus::GetCurAlgoToAddIndex()
{
	return m_nInspTypeToAdd;
}

CSCAlgoBase * CStatus::GetCurAlgoToAdd()
{
	CSCAlgoBase *pAlgo = NULL;
	pAlgo = GetAlgoInfo(m_nInspTypeToAdd);
	if (m_nInspTypeToAdd == INSPFID)
	{
		((CSCFidAlgo *)pAlgo)->m_nCamNo = GetSelCamNo();
	}
	return pAlgo;
}

CSCAlgoBase * CStatus::GetAlgoInfo(int nInspType, CXmlNode* pNode)
{
	CSCAlgoBase * pAlgo=NULL;
	switch (nInspType)
	{
	case INSPFID:
	{
		pAlgo = new CSCFidAlgo;
		CString strPathName = GetModelRepository() + _T("\\") + GetModelName();
		pAlgo->m_pParamDlg = CSCFidParamDlg::Instance();
		if (pNode)
		{
			((CSCFidAlgo *)pAlgo)->m_iMatRate = pNode->ReadLong(60, _T("FidScore"));
			((CSCFidAlgo *)pAlgo)->m_nCamNo = pNode->ReadLong(0, _T("CamNo"));
			((CSCFidAlgo *)pAlgo)->LoadFid(strPathName.GetBuffer(), ((CSCFidAlgo *)pAlgo)->m_nCamNo);
			((CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCX = pNode->ReadDouble(2.0, _T("SearchRangeFactorX"));
			((CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCY = pNode->ReadDouble(2.0, _T("SearchRangeFactorY"));
		}
	}
		break;
	case INSPHEDET:
		break;
	case INSPALIGN:
		break;
	case INSPHEIGHT:
		break;
	case INSPGAPGAGE:

		break;
	}
	if (!pAlgo)
		AfxMessageBox(_T("Undefined Algorithm!!"));
	else
		pAlgo->m_nInspType = nInspType;

	return pAlgo;
}

void CStatus::SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode)
{
	CString ModelFolder;
	switch (nInspType)
	{
	case INSPFID:
		{
			CString strPathName = GetModelRepository() + _T("\\") + GetModelName();
			pNode->SetLong(_T("FidScore"), ((CSCFidAlgo *)pAlgo)->m_iMatRate);
			pNode->SetLong(_T("CamNo"), ((CSCFidAlgo *)pAlgo)->m_nCamNo);

			pNode->SetDouble((_T("SearchRangeFactorX"), (CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCX);
			pNode->SetDouble((_T("SearchRangeFactorY"), (CSCFidAlgo *)pAlgo)->m_SearchRangeFactor.dblCY);

			if (((CSCFidAlgo *)pAlgo)->IsEmpty())AfxMessageBox(_T("You don't registrate the fiducial!!"));
			else
				((CSCFidAlgo *)pAlgo)->SaveFid(strPathName.GetBuffer(), ((CSCFidAlgo *)pAlgo)->m_nCamNo);
		}
		break;
	case INSPHEDET:
		break;
	case INSPALIGN:
		break;
	case INSPHEIGHT:
		break;
	case INSPGAPGAGE:
		break;

	}
}



int CStatus::GetCamNo()
{
	return 0;
}


CdblSize CStatus::GetPixelCalData(int iCamIndex)
{
	if( (m_CamFov[iCamIndex].dblCX == 0.0) || (m_CamFov[iCamIndex].dblCY ==0.0))
		return CdblSize(0.0, 0.0);
	CdblSize dblSize;
	dblSize.dblCX = m_CamFov[iCamIndex].dblCX / m_iCAM_HRES[iCamIndex];
	dblSize.dblCY = m_CamFov[iCamIndex].dblCY / m_iCAM_VRES[iCamIndex];
	return dblSize;
}



CString CStatus::GetRemoteIPString()
{
	CString strIP;
	strIP.Format(_T("%d.%d.%d.%d"), m_lIPAddr[0], m_lIPAddr[1], m_lIPAddr[2], m_lIPAddr[3]);
	return strIP;
}


void CStatus::SetRepConvMode(bool bIsRepConv)
{
	m_bIsRepConv = bIsRepConv;
}

BOOL CStatus::IsRepConv()
{
	return m_bIsRepConv;
}

BOOL CStatus::IsDataAquMode()
{
	return m_bIsDataAqu;
}


void CStatus::SetDataAquMode(bool bIsDataAqu)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCGInspectorView *pView = (CCGInspectorView *)pFrame->GetActiveView();
	m_bIsDataAqu = bIsDataAqu;
	for (int i = 0; i < 2; i++)
		pView->m_pCamDlg[i]->SetDataAquMode(bIsDataAqu);
}


void CStatus::SetActiveRevInsertMode()
{
	m_bActRevInsertMode = TRUE;
}


void CStatus::ResetActRevInsertMode()
{
	m_bActRevInsertMode = FALSE;
}

BOOL CStatus::IsOnline()
{
	return !m_bIsOffLine;
}

BOOL CStatus::IsOffLine()
{
	return m_bIsOffLine;
}

void CStatus::SetOnOffLine(bool bOnOffLine)
{
	m_bIsOffLine = bOnOffLine;
}

BOOL CStatus::IsRevInsertMode()
{
	return 	m_bActRevInsertMode;
}

/// 저장이미지의 Y길이값
int CStatus::GetCGLength()
{
	return m_sizeTopCam.cy;
}


void CStatus::SetCGLength(UINT nCGLength)
{
	m_sizeTopCam.cy		= nCGLength;
	m_sizeBottomCam.cy	= nCGLength;
	m_sizeSideLeftCam.cy = nCGLength;
	m_sizeSideRightCam = nCGLength;
}


BOOL CStatus::IsDrawDefect()
{
	return m_bDrawDefect;
}


void CStatus::SetDrawDefect(bool bDrawDefectMode)
{
	m_bDrawDefect = bDrawDefectMode;
}
