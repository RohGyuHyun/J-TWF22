#pragma once
#include "..\Library\SerialLib\SerialMFC.h"	// 필수
#include "..\Library\SerialLib\SCInspPacket.h"	// 필수
#include <UniUtility.h>
#include "atltypes.h"
#include "Defines.h"
class CMilVLib;
class CModelData;
class CSCAlgoBase;
struct tInfoCamIng
{
	int nCamNo;
	CdblPoint posImage; //Mouse point가 가르키는 실제 이미지상의(Zoom적용되지않은) 좌표
	int GrayValue;      //Mouse point위치의 Gray값
};

class CStatus
{
public:
	enum tOpMode { OPAUTO, OPTEACHING, OPSTOP };
	enum tInspType {INSPFID, INSPHEDET, INSPALIGN, INSPHEIGHT, INSPGAPGAGE};
	CStatus();
	~CStatus();
	static CStatus * Instance();
	static CStatus *m_pInstance;
	void SetOPMode(tOpMode opMode);
	int GetOpMode(void);
	void SetSelCam(int nCam);
	tConvMotionParam m_paramCvyMotion[4];
	int m_iSelCam;
	int GetSelCamNo();
	CString GetModelRepository();
	void SetModelRepository(TCHAR * szDataSaveFolder);
	CString m_strModelRepository;
	CString m_strModelName;
	CString GetModelName();
	void SetModelName(TCHAR * szModelName);
	int SaveCFG();
	int LoadCFG();
	long m_nCountOfCam;
	CString GetComportNo();
	void SetBaudRate(int iBaudRate);
	int GetBaudRate();
	void SetComportNo(TCHAR* szComportNo);
	long m_iBaudRate;
	CString m_strComportNo;
	CSCInspPacket * GetComm();
	CSCInspPacket *m_pSCInspPacket;
	void SetComm(CSCInspPacket * pSCInspPacket);
	int GetRVOption();
	CModelData * GetModelData();
	CModelData *m_pModelData;
	void SetModelData(CModelData * pModelData);
	CdblSize m_CamFov[4];
	void SetCurAddAlgoIndex(int nInspType);
	int m_nInspTypeToAdd;
	int GetCurAlgoToAddIndex();
	CSCAlgoBase * GetCurAlgoToAdd();
	CSCAlgoBase * GetAlgoInfo(int nInspType, CXmlNode* pNode = NULL);
	void SetAlgoInfo(int nInspType, CSCAlgoBase *pAlgo, CXmlNode* pNode);
	long m_lCamExpTime[4];
	CString m_ProductName;
	tInfoCamIng m_infoCamMousePos;
	int m_opMode;
	CString m_strNGSaveFolder;
	BOOL m_MsgWndOpen;
	long m_nCountOfStage;
	int GetCamNo();
	long m_nCountOfPos;
	BOOL m_bSaveTestImage;
	BOOL m_bDrawCross;
	long m_bUseDelOldImg;
	long m_nDaysDel;
	CdblSize GetPixelCalData(int iCamIndex);
	long m_nCountOfCamRemote;
	long m_lIPAddr[4];
	CString GetRemoteIPString();
	BOOL IsRepConv();
	void SetRepConvMode(bool bIsRepConv);
	BOOL m_bIsRepConv;
	BOOL IsDataAquMode();
	BOOL m_bIsDataAqu;
	void SetDataAquMode(bool bIsDataAqu);
	BOOL IsOnline();
	BOOL IsOffLine();
	void SetOnOffLine(bool bOnOffLine);
	void SetActiveRevInsertMode();
	BOOL m_bActRevInsertMode;
	BOOL m_bIsOffLine;  //! OnLine:장비 OffLine:장비가 아닌 PC
	void ResetActRevInsertMode();
	long m_nCvyrFrontAxis;
	long m_nCvyrEndAxis;
	BOOL IsRevInsertMode();
	CSize m_sizeTopCam;
	CSize m_sizeBottomCam;
	CSize m_sizeSideLeftCam;
	CSize m_sizeSideRightCam;
	int GetCGLength();
	void SetCGLength(UINT nCGLength);
	int m_iCAM_HRES[4];
	int m_iCAM_VRES[4];
	CString m_strWorkSpaceRuntime = _T("RuntimeWS.vrws");
	CString m_strDataSaveFolder=_T("d:\\SaveImage");
	// ViDi Workspace name
	char *m_szWSName = "CGInspect";
	BOOL IsDrawDefect();
	void SetDrawDefect(bool bDrawDefectMode);
	// Defect를 그릴지 말지 설정
	BOOL m_bDrawDefect;
};

